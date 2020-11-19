#include "annetaBuilder.h"
#include <vector>
#include <iostream>
#include <stdlib.h>

llvm::Type* AstIntType::typeOf(genContext& context)
{
return llvm::Type::getInt32Ty(context.IRContext); } 

llvm::Type* AstStringType::typeOf(genContext& context)
{
}


llvm::Type* AstBoolType::typeOf(genContext& context)
{
return llvm::Type::getFloatTy(context.IRContext);
}

llvm::Type* AstFloatType::typeOf(genContext& context)
{
return llvm::Type::getInt1Ty(context.IRContext);
}

llvm::Value* AstIfElseStat::codeGen(genContext& context)
{
llvm::BasicBlock *ifBB = llvm::BasicBlock::Create(context.IRContext,"ifBranch");
llvm::BasicBlock *elseBB = llvm::BasicBlock::Create(context.IRContext,"elseBranch");
llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(context.IRContext,"mergeBranch");
llvm::Value* testResult = testCondition.codeGen(context);
context.Builder.CreateCondBr(testResult,ifBB,elseBB);
context.Builder.SetInsertPoint(ifBB);
ifBlock.codeGen(context); //Generates the code in the if branch.
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
llvm::Value* testResult = testCondition.codeGen(context);
context.Builder.CreateCondBr(testResult,loop,endLoop);
context.Builder.SetInsertPoint(loop);
whileBlock.codeGen(context);
context.Builder.CreateBr(condEval);
context.Builder.SetInsertPoint(endLoop);
return 0;
}

AstBinOp::AstBinOp(AstExp& lhs, AstExp& rhs):
lhs(lhs), rhs(rhs)
{
if (typeid(lhs.type) == typeid(rhs.type))
{
type = lhs.type;
}
else
{
std::cout << "Type Mismatch" << std::endl;
exit(1);
}
}

llvm::Value* AstOr::codeGen(genContext& context)
{
return context.Builder.CreateOr(lhs.codeGen(context),rhs.codeGen(context),"or");
}

llvm::Value* AstXor::codeGen(genContext& context)
{
return context.Builder.CreateXor(lhs.codeGen(context),rhs.codeGen(context),"xor");	
}

llvm::Value* AstAnd::codeGen(genContext& context)
{
return context.Builder.CreateAnd(lhs.codeGen(context),rhs.codeGen(context),"and");
}

llvm::Value* AstEql::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs.codeGen(context);
llvm::Value* rhsValue = rhsValue;

if (typeid(type) == typeid(intType))
{
return context.Builder.CreateICmpEQ(lhsValue,rhsValue,"icmp_eq");
}
else if (typeid(type) == typeid(floatType))
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
llvm::Value* lhsValue = lhs.codeGen(context);
llvm::Value* rhsValue = rhsValue;

if (typeid(type) == typeid(intType))
{
return context.Builder.CreateICmpNE(lhsValue,rhsValue,"icmp_ne");
}
else if (typeid(type) == typeid(floatType))
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
return context.Builder.CreateShl(lhs.codeGen(context),rhs.codeGen(context),"shl");
}

llvm::Value* AstRightSh::codeGen(genContext& context)
{
return context.Builder.CreateLShr(lhs.codeGen(context),rhs.codeGen(context),"LShr");
}

llvm::Value* AstVariableAsg::codeGen(genContext& context)
{
return context.Builder.CreateStore(context.varLookUp(variableName.name),rhs.codeGen(context));
}

llvm::Value* AstArrayAsg::codeGen(genContext& context)
{
return context.Builder.CreateInsertValue(context.varLookUp(variableName.name),rhs.codeGen(context),index.codeGen(context),variableName.name);
}

llvm::Value* AstArrayListAsg::codeGen(genContext& context)
{
llvm::AllocaInst* arrayLocation = context.varLookUp(variableName.name);
int curIndex = index->value;
int arraySize = arrayLocation->getType()->getArrayNumElements();
for (expressionList::iterator it = arrayValues.begin(); it != arrayValues.end(); it++)
{
context.Builder.CreateInsertValue(arrayLocation,*it.codeGen(context),AstIntValue(curIndex).codeGen(context),variableName.name);
curIndex++;
if (curIndex > arraySize)
{
curIndex = 0;
}

}
return arrayLocation;
}


llvm::Value* Astleq::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs.codeGen(context);
llvm::Value* rhsValue = rhsValue;

if (typeid(type) == typeid(intType))
{
return context.Builder.CreateICmpSLE(lhsValue,rhsValue,"icmp_ule");
}
else if (typeid(type) == typeid(floatType))
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
llvm::Value* lhsValue = lhs.codeGen(context);
llvm::Value* rhsValue = rhsValue;

if (typeid(type) == typeid(intType))
{
return context.Builder.CreateICmpSGE(lhsValue,rhsValue,"icmp_uge");
}
else if (typeid(type) == typeid(floatType))
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
llvm::Value* lhsValue = lhs.codeGen(context);
llvm::Value* rhsValue = rhsValue;

if (typeid(type) == typeid(intType))
{
return context.Builder.CreateICmpSLT(lhsValue,rhsValue,"icmp_ult");
}
else if (typeid(type) == typeid(floatType))
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
llvm::Value* lhsValue = lhs.codeGen(context);
llvm::Value* rhsValue = rhsValue;
if (typeid(type) == typeid(intType))
{
return context.Builder.CreateICmpSGT(lhsValue,rhsValue,"icmp_ugt");
}
else if (typeid(type) == typeid(floatType))
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
llvm::Value* lhsValue = lhs.codeGen(context);
llvm::Value* rhsValue = rhsValue;

if (typeid(type) == typeid(intType))
{
return context.Builder.CreateAdd(lhsValue,rhsValue,"addTmp");
}
else if (typeid(type) == typeid(floatType))
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
llvm::Value* lhsValue = lhs.codeGen(context);
llvm::Value* rhsValue = rhsValue;

if (typeid(type) == typeid(intType))
{
return context.Builder.CreateSub(lhsValue,rhsValue,"subTmp");
}
else if (typeid(type) == typeid(floatType))
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
llvm::Value* lhsValue = lhs.codeGen(context);
llvm::Value* rhsValue = rhsValue;

if (typeid(type) == typeid(intType))
{
return context.Builder.CreateMul(lhsValue,rhsValue,"mulTmp");
}
else if (typeid(type) == typeid(floatType))
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
llvm::Value* lhsValue = lhs.codeGen(context);
llvm::Value* rhsValue = rhsValue;

if (typeid(type) == typeid(intType))
{
return context.Builder.CreateSDiv(lhsValue,rhsValue,"uDivTmp");
}
else if (typeid(type) == typeid(floatType))
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
llvm::Value* lhsValue = lhs.codeGen(context);
llvm::Value* rhsValue = rhsValue;

if (typeid(type) == typeid(intType))
{
return context.Builder.CreateSRem(lhsValue,rhsValue,"uRemTmp");
}
else if (typeid(type) == typeid(floatType))
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

llvm::Constant* AstBoolValue::codeGen(genContext& context)
{
return llvm::ConstantInt::get(context.IRContext,llvm::APInt(1,value));
}

llvm::Constant* AstVariableDeclaration::codeGen(genContext& context)
{
if (globalBool)
{
return llvm::GlobalVariable((*context.CurModule),variableType.typeOf(context),false,llvm::GlobalVariable::ExternalLinkage,initializer->codeGen(context),variableName.name);
}
else
{
llvm::Type* varType = variableType.typeOf(context);
return context.Builder.CreateAlloca(varType,nullptr,variableName.name);
}
}

llvm::Value* AstArrayDeclaration::codeGen(genContext& context)
{
llvm::Type* arrayType = llvm::ArrayType::get(variableType.typeOf(context),arraySize.value);
llvm::Value* arrayLocation = context.Builder.CreateAlloca(arrayType,nullptr,variableName.name);

if (initializer)
{
expressionList::const_iterator it;
signed int curIndex = 0;
for (it = arguments.begin(); it != arguments.end(); it++)
{
context.Builder.CreateInsertValue(arrayLocation,**it.codeGen(context),AstIntValue(curIndex).codeGen(context),variableName.name);
curIndex++;
if (curIndex > arraySize.value)
{
curIndex = 0;
}

}
}

return arrayLocation
}

llvm::Value* AstFunctionCall::codeGen(genContext& context)
{
llvm::Function* function = CurModule->getFunction(functionName);
return context.Builder.CreateCall(function,args,"functionCall");
}

llvm::Value* AstVariableCall::codeGen(genContext& context)
{
return context.Builder.CreateLoad(context.varLookUp(variableName),variableName); 
}

llvm::Value* AstArrayCall::codeGen(genContext& context)
{
return context.Builder.CreateExtractValue(context.varLookUp(variableName,index,variableName);
}

llvm::Value* AstFunctionDeclaration::codeGen(genContext& context)
{
std::vector<AstType*> argTypes;
variableList::const_iterator it;
for (it = arguments->begin(); it != arguments->end(); it++)
{
argTypes.push_back((**it).typeOf(context)));
}
llvm::FunctionType *ft = llvm::FunctionType::get(returnType.typeOf(context),argTypes,false);
llvm::Function* func = llvm::Function::Create(ft,llvm::Function::ExternalLinkage,functionName,context.CurModule.get());
context.pushBlock(&block);
for (it=arguments->begin(); it!= arguments->end(); it++)
{
(**it).codeGen(context);
}
block.codeGen(context);
context.popBlock(&block);
return func;
}

llvm::Value* AstReturnStat::codeGen(genContext& context)
{
return context.Builder.CreateRet(returnValue.codeGen(context));
}

llvm::Value* AstBlock::codeGen(genContext& context)
{
context.pushBlock(this);
statementList::const_iterator it;
llvm::Value* genResult;
for (it = statements.begin();it != statements.end(); it++)
{
genResult = (**it).codeGen(context);
}
return genResult;
context.popBlock(this);
}


