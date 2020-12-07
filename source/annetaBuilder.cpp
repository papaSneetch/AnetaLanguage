#include "annetaBuilder.h"
#include <vector>
#include <iostream>
#include <stdlib.h>

llvm::IntegerType* AstIntType::typeOf(genContext& context)
{
return llvm::Type::getInt32Ty(context.IRContext);
} 

llvm::Type* AstStringType::typeOf(genContext& context)
{
	return nullptr;
}


llvm::Type* AstFloatType::typeOf(genContext& context)
{
return llvm::Type::getFloatTy(context.IRContext);
}

llvm::IntegerType* AstBoolType::typeOf(genContext& context)
{
return llvm::Type::getInt1Ty(context.IRContext);
}

llvm::Value* AstIfElseStat::codeGen(genContext& context)
{
llvm::BasicBlock *ifBB = llvm::BasicBlock::Create(context.IRContext,"ifBranch");
llvm::BasicBlock *elseBB = llvm::BasicBlock::Create(context.IRContext,"elseBranch");
llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(context.IRContext,"mergeBranch");
llvm::Value* testResult = testCondition->codeGen(context);
context.Builder.CreateCondBr(testResult,ifBB,elseBB);
context.Builder.SetInsertPoint(ifBB);
ifBlock->codeGen(context); //Generates the code in the if branch.
context.Builder.CreateBr(MergeBB);
context.Builder.SetInsertPoint(elseBB);
elseBlock->codeGen(context); //Generates the code for the else branch.
context.Builder.CreateBr(MergeBB);
context.Builder.SetInsertPoint(MergeBB);
return 0;
}

llvm::Value* AstWhileLoop::codeGen(genContext& context)
{
llvm::BasicBlock *loop = llvm::BasicBlock::Create(context.IRContext,"loop");
llvm::BasicBlock *endLoop = llvm::BasicBlock::Create(context.IRContext,"endLoop");
llvm::BasicBlock *condEval = llvm::BasicBlock::Create(context.IRContext,"condEval");
context.Builder.CreateBr(condEval);
context.Builder.SetInsertPoint(condEval);
llvm::Value* testResult = testCondition->codeGen(context);
context.Builder.CreateCondBr(testResult,loop,endLoop);
context.Builder.SetInsertPoint(loop);
whileBlock->codeGen(context);
context.Builder.CreateBr(condEval);
context.Builder.SetInsertPoint(endLoop);
return 0;
}

AstBinOp::AstBinOp(AstExp* lhsPtr,AstExp* rhsPtr)
{
lhs.reset(lhsPtr);
rhs.reset(rhsPtr);
if (typeid(lhs->type) == typeid(rhs->type))
{
type = std::move(lhs->type);
}
else
{
std::cout << "Type Mismatch" << std::endl;
exit(1);
}
}

AstBinOp::AstBinOp(AstExpPtr& lhs,AstExpPtr& rhs):
lhs(std::move(lhs)), rhs(std::move(rhs))
{
if (typeid(lhs->type) == typeid(rhs->type))
{
type = std::move(lhs->type);
}
else
{
std::cout << "Type Mismatch" << std::endl;
exit(1);
}
}

llvm::Value* AstOr::codeGen(genContext& context)
{
return context.Builder.CreateOr(lhs->codeGen(context),rhs->codeGen(context),"or");
}

llvm::Value* AstXor::codeGen(genContext& context)
{
return context.Builder.CreateXor(lhs->codeGen(context),rhs->codeGen(context),"xor");	
}

llvm::Value* AstAnd::codeGen(genContext& context)
{
return context.Builder.CreateAnd(lhs->codeGen(context),rhs->codeGen(context),"and");
}

llvm::Value* AstEql::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs->codeGen(context);
llvm::Value* rhsValue = rhs->codeGen(context);

if (typeid(type) == typeid(AstIntType))
{
return context.Builder.CreateICmpEQ(lhsValue,rhsValue,"icmp_eq");
}
else if (typeid(type) == typeid(AstFloatType))
{
return context.Builder.CreateFCmpOEQ(lhsValue,rhsValue,"fAddTmp");
}
else
{
std::cout<< "Type Mismatch!" << std::endl;
exit(1);
}
}

llvm::Value* Astneq::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs->codeGen(context);
llvm::Value* rhsValue = rhs->codeGen(context);

if (typeid(type) == typeid(AstIntType))
{
return context.Builder.CreateICmpNE(lhsValue,rhsValue,"icmp_ne");
}
else if (typeid(type) == typeid(AstFloatType))
{
return context.Builder.CreateFCmpONE(lhsValue,rhsValue,"fAddTmp");
}
else
{
std::cout<< "Type Mismatch!" << std::endl;
exit(1);
}
}

llvm::Value* AstLeftSh::codeGen(genContext& context)
{
return context.Builder.CreateShl(lhs->codeGen(context),rhs->codeGen(context),"shl");
}

llvm::Value* AstRightSh::codeGen(genContext& context)
{
return context.Builder.CreateLShr(lhs->codeGen(context),rhs->codeGen(context),"LShr");
}

llvm::Value* AstVariableAsg::codeGen(genContext& context)
{
return context.Builder.CreateStore(rhs->codeGen(context),context.varLookUp(variableName->name));
}

llvm::Value* AstArrayAsg::codeGen(genContext& context)
{
llvm::Value* indexPtr = context.Builder.CreateGEP(context.varLookUp(variableName->name),index->codeGen(context),"getElementPtr");
context.Builder.CreateStore(rhs->codeGen(context),indexPtr,false);
return indexPtr;
}

llvm::Value* AstArrayListAsg::codeGen(genContext& context)
{
llvm::AllocaInst* arrayLocation = context.varLookUp(variableName->name);
llvm::Value* curIndex = index->codeGen(context);
llvm::Value* arraySize = AstIntValue(arrayLocation->getType()->getArrayNumElements()).codeGen(context);
llvm::Value* increment = AstIntValue(1).codeGen(context);
for (expressionList::iterator it = arrayValues->begin(); it != arrayValues->end(); it++)
{
llvm::Value* indexPtr = context.Builder.CreateGEP(arrayLocation,curIndex,"getElementPtr");
context.Builder.CreateStore((*it)->codeGen(context),indexPtr);
curIndex = context.Builder.CreateAdd(curIndex,increment);
curIndex = context.Builder.CreateMul(curIndex,context.Builder.CreateICmpSLE(curIndex,arraySize));
}
return arrayLocation;
}


llvm::Value* Astleq::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs->codeGen(context);
llvm::Value* rhsValue = rhs->codeGen(context);

if (typeid(type) == typeid(AstIntType))
{
return context.Builder.CreateICmpSLE(lhsValue,rhsValue,"icmp_ule");
}
else if (typeid(type) == typeid(AstFloatType))
{
return context.Builder.CreateFCmpOLE(lhsValue,rhsValue,"fAddTmp");
}
else
{
std::cout<< "Type Mismatch!" << std::endl;
exit(1);
}
}

llvm::Value* Astgeq::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs->codeGen(context);
llvm::Value* rhsValue = rhs->codeGen(context);

if (typeid(type) == typeid(AstIntType))
{
return context.Builder.CreateICmpSGE(lhsValue,rhsValue,"icmp_uge");
}
else if (typeid(type) == typeid(AstFloatType))
{
return context.Builder.CreateFCmpOGE(lhsValue,rhsValue,"fAddTmp");
}
else
{
std::cout<<"Type Mismatch!" << std::endl;
exit(1);
}
}

llvm::Value* Astlt::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs->codeGen(context);
llvm::Value* rhsValue = rhs->codeGen(context);

if (typeid(type) == typeid(AstIntType))
{
return context.Builder.CreateICmpSLT(lhsValue,rhsValue,"icmp_ult");
}
else if (typeid(type) == typeid(AstFloatType))
{
return context.Builder.CreateFCmpOLT(lhsValue,rhsValue,"fAddTmp");
}
else
{
std::cout<<"Type Mismatch!" << std::endl;
exit(1);
}
}

llvm::Value* Astgt::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs->codeGen(context);
llvm::Value* rhsValue = rhs->codeGen(context);
if (typeid(type) == typeid(AstIntType))
{
return context.Builder.CreateICmpSGT(lhsValue,rhsValue,"icmp_ugt");
}
else if (typeid(type) == typeid(AstFloatType))
{
return context.Builder.CreateFCmpOGT(lhsValue,rhsValue,"fAddTmp");
}
else
{
std::cout<<"Type Mismatch!" << std::endl;
exit(1);
}
}

llvm::Value* AstAdd::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs->codeGen(context);
llvm::Value* rhsValue = rhs->codeGen(context);;

if (typeid(type) == typeid(AstIntType))
{
return context.Builder.CreateAdd(lhsValue,rhsValue,"addTmp");
}
else if (typeid(type) == typeid(AstFloatType))
{
return context.Builder.CreateFAdd(lhsValue,rhsValue,"fAddTmp");
}
else
{
std::cout<< "Type Mismatch!"<< std::endl;
exit(1);
}
}

llvm::Value* AstSub::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs->codeGen(context);
llvm::Value* rhsValue = rhs->codeGen(context);

if (typeid(type) == typeid(AstIntType))
{
return context.Builder.CreateSub(lhsValue,rhsValue,"subTmp");
}
else if (typeid(type) == typeid(AstFloatType))
{
return context.Builder.CreateFSub(lhsValue,rhsValue,"fSubTmp");
}
else
{
std::cout<< "Type Mismatch!" << std::endl;
exit(1);
}
}

llvm::Value* AstMul::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs->codeGen(context);
llvm::Value* rhsValue = rhs->codeGen(context);

if (typeid(type) == typeid(AstIntType))
{
return context.Builder.CreateMul(lhsValue,rhsValue,"mulTmp");
}
else if (typeid(type) == typeid(AstFloatType))
{
return context.Builder.CreateFMul(lhsValue,rhsValue,"CreateFMul");
}
else
{
std::cout<< "Type Mismatch!" << std::endl;
exit(1);
}
}

llvm::Value* AstDiv::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs->codeGen(context);
llvm::Value* rhsValue = rhs->codeGen(context);

if (typeid(type) == typeid(AstIntType))
{
return context.Builder.CreateSDiv(lhsValue,rhsValue,"uDivTmp");
}
else if (typeid(type) == typeid(AstFloatType))
{
return context.Builder.CreateFDiv(lhsValue,rhsValue,"fDivTmp");
}
else
{
std::cout<< "Type Mismatch!" << std::endl;
exit(1);
}
}

llvm::Value* AstMod::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs->codeGen(context);
llvm::Value* rhsValue = rhs->codeGen(context);

if (typeid(type) == typeid(AstIntType))
{
return context.Builder.CreateSRem(lhsValue,rhsValue,"uRemTmp");
}
else if (typeid(type) == typeid(AstFloatType))
{
return context.Builder.CreateFRem(lhsValue,rhsValue,"fRemTmp");
}
else
{
std::cout<< "Type Mismatch!" << std::endl;
exit(1);
}
}

llvm::Constant* AstIntValue::codeGen(genContext& context)
{
return llvm::ConstantInt::get(context.IRContext,llvm::APInt(32,value));
}

llvm::Constant* AstFloatValue::codeGen(genContext& context)
{
return llvm::ConstantFP::get(context.IRContext,llvm::APFloat(value));
}

llvm::Constant* AstStringValue::codeGen(genContext& context)
{
return nullptr;
}


llvm::Constant* AstBoolValue::codeGen(genContext& context)
{
return llvm::ConstantInt::get(context.IRContext,llvm::APInt(1,value));
}


llvm::Value* AstGlobalVariableDeclaration::codeGen(genContext& context)
{
if (initializer)
{
llvm::GlobalVariable globalVar ((*context.CurModule),variableType->typeOf(context),false,llvm::GlobalVariable::ExternalLinkage,initializer->codeGen(context),variableName->name);
}
else
{
llvm::GlobalVariable globalVar ((*context.CurModule),variableType->typeOf(context),false,llvm::GlobalVariable::ExternalLinkage,nullptr,variableName->name);

}
return nullptr;
}

llvm::Value* AstVariableDeclaration::codeGen(genContext& context)
{
llvm::Type* varType = variableType->typeOf(context);
llvm::AllocaInst* alloca = context.Builder.CreateAlloca(varType,nullptr,variableName->name);
context.varPush(variableName->name,alloca);
if (initializer)
{
context.Builder.CreateStore(initializer->codeGen(context),alloca);
}
return alloca;
}

llvm::Value* AstArrayDeclaration::codeGen(genContext& context)
{
llvm::Type* arrayType = llvm::ArrayType::get(variableType->typeOf(context),arraySize->value);
llvm::Value* arrayLocation = context.Builder.CreateAlloca(arrayType,nullptr,variableName->name);

if (initializer)
{
expressionList::iterator it;
int curIndex = 0;
for (it = initializer->begin(); it != initializer->end(); it++)
{
context.Builder.CreateInsertValue(arrayLocation,(*it)->codeGen(context),curIndex,variableName->name);
curIndex++;
if (curIndex > arraySize->value)
{
curIndex = 0;
}

}
}

return arrayLocation;
}

llvm::Value* AstFunctionCall::codeGen(genContext& context)
{
llvm::Function* function = context.CurModule->getFunction(functionName->name);
std::vector <llvm::Value*> argsV;
for (expressionList::iterator it = args->begin(); it != args->end(); it++)
{
argsV.push_back((*it)->codeGen(context));
}
return context.Builder.CreateCall(function,argsV,"functionCall",nullptr);
}

llvm::Value* AstVariableCall::codeGen(genContext& context)
{
return context.Builder.CreateLoad(context.varLookUp(variableName->name),variableName->name); 
}

llvm::Value* AstArrayCall::codeGen(genContext& context)
{
llvm::Value* indexPtr = context.Builder.CreateGEP(context.varLookUp(variableName->name),index->codeGen(context),"getElementPtr");
return context.Builder.CreateLoad(indexPtr,variableName->name);
}

llvm::Value* AstFunctionDeclaration::codeGen(genContext& context)
{
std::vector<llvm::Type*> argTypes;
variableList::iterator it;
for (it = arguments->begin(); it != arguments->end(); it++)
{
argTypes.push_back(it->variableType->typeOf(context));
}
llvm::FunctionType *ft = llvm::FunctionType::get(returnType->typeOf(context),argTypes,false);
llvm::Function* func = llvm::Function::Create(ft,llvm::Function::ExternalLinkage,functionName->name,context.CurModule.get());
llvm::BasicBlock *BB = llvm::BasicBlock::Create(context.IRContext,"entry",func);
context.Builder.SetInsertPoint(BB);
for (it = arguments->begin(); it!= arguments->end(); it++)
{
it->codeGen(context);
}
block->codeGen(context);
context.popBlock();
return func;
}

llvm::Value* AstReturnStat::codeGen(genContext& context)
{
return context.Builder.CreateRet(returnValue->codeGen(context));
}

llvm::Value* AstName::codeGen(genContext& context)
{
return nullptr;
}

llvm::Value* AstBlock::codeGen(genContext& context)
{
context.pushBlock(this);
llvm::Value* genResult;
statementList::iterator it;
for (it = statements->begin();it != statements->end(); it++)
{
genResult = (*it)->codeGen(context);
}
context.popBlock();
return genResult;
}

llvm::Value* AstExpStat::codeGen(genContext& context)
{
return expression->codeGen(context);
}

