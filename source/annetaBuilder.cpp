#include "annetaBuilder.h"
#include "annetaTypes.h"

#include <llvm/IR/Value.h>
#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/Optional.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>

#include <vector>
#include <iostream>
#include <stdlib.h>

llvm::Value* AstIfElseStat::codeGen(genContext& context)
{
llvm::Function* curFunction = context.Builder->GetInsertBlock()->getParent();
llvm::BasicBlock *ifBB = llvm::BasicBlock::Create(*(context.IRContext),"ifBranch",curFunction);
llvm::BasicBlock *elseBB = llvm::BasicBlock::Create(*(context.IRContext),"elseBranch",curFunction);
llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(*(context.IRContext),"mergeBranch",curFunction);
llvm::Value* testResult = testCondition->codeGen(context);
context.Builder->CreateCondBr(testResult,ifBB,elseBB);
context.Builder->SetInsertPoint(ifBB);
context.pushBlock(ifBlock);
context.backBlock()->codeGenInFunction(context); //Generates the code in the if branch.
context.popBlock();
context.Builder->CreateBr(MergeBB);
context.Builder->SetInsertPoint(elseBB);
context.pushBlock(elseBlock);
context.backBlock()->codeGenInFunction(context); //Generates the code for the else branch.
context.popBlock();
context.Builder->CreateBr(MergeBB);
context.Builder->SetInsertPoint(MergeBB);
return 0;
}


llvm::Value* AstIfStat::codeGen(genContext& context)
{
llvm::Function* curFunction = context.Builder->GetInsertBlock()->getParent();
llvm::BasicBlock *ifBB = llvm::BasicBlock::Create(*(context.IRContext),"ifBranch",curFunction);
llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(*(context.IRContext),"mergeBranch",curFunction);
llvm::Value* testResult = testCondition->codeGen(context);
context.Builder->CreateCondBr(testResult,ifBB,MergeBB);
context.Builder->SetInsertPoint(ifBB);
context.pushBlock(ifBlock);
context.backBlock()->codeGenInFunction(context); //Generates the code in the if branch.
context.popBlock();
context.Builder->CreateBr(MergeBB);
context.Builder->SetInsertPoint(MergeBB);
return 0;
}

llvm::Value* AstWhileLoop::codeGen(genContext& context)
{
llvm::Function* curFunction = context.Builder->GetInsertBlock()->getParent();
llvm::BasicBlock *loop = llvm::BasicBlock::Create(*(context.IRContext),"loop",curFunction);
llvm::BasicBlock *endLoop = llvm::BasicBlock::Create(*(context.IRContext),"endLoop",curFunction);
llvm::BasicBlock *condEval = llvm::BasicBlock::Create(*(context.IRContext),"condEval",curFunction);
context.Builder->CreateBr(condEval);
context.Builder->SetInsertPoint(condEval);
llvm::Value* testResult = testCondition->codeGen(context);
context.Builder->CreateCondBr(testResult,loop,endLoop);
context.Builder->SetInsertPoint(loop);
context.pushBlock(whileBlock);
context.backBlock()->codeGenInFunction(context);
context.popBlock();
context.Builder->CreateBr(condEval);
context.Builder->SetInsertPoint(endLoop);
return 0;
}

AstBinOp::AstBinOp(AstExp* lhsPtr,AstExp* rhsPtr)
{
lhs.reset(lhsPtr);
rhs.reset(rhsPtr);
}

AstBinOp::AstBinOp(AstExpPtr& lhs,AstExpPtr& rhs):
lhs(std::move(lhs)), rhs(std::move(rhs)) {}

int AstBinOp::updateType()
{
auto lhsType = lhs->type;
auto rhsType = rhs->type;
if (typeid(*lhsType) == typeid(*rhsType))
{
type = lhsType;
return 0;
}
else
{
std::cerr<< "Error in AstBinOp:" << std::endl;
std::cerr<< "Type Mismatch" << std::endl;
std::cerr<< "Type 1: " << lhsType << std::endl;
std::cerr<< "Type 2: " << rhsType << std::endl;
exit(1);
}
}

llvm::Value* AstOr::codeGen(genContext& context)
{
auto lhsCodeGen = lhs->codeGen(context);
auto rhsCodeGen = rhs->codeGen(context);
updateType();
return context.Builder->CreateOr(lhsCodeGen,rhsCodeGen,"or");
}

llvm::Value* AstXor::codeGen(genContext& context)
{
auto lhsCodeGen = lhs->codeGen(context);
auto rhsCodeGen = rhs->codeGen(context);
updateType();
return context.Builder->CreateXor(lhsCodeGen,rhsCodeGen,"xor");	
}

llvm::Value* AstAnd::codeGen(genContext& context)
{
auto lhsCodeGen = lhs->codeGen(context);
auto rhsCodeGen = rhs->codeGen(context);
updateType();
return context.Builder->CreateAnd(lhsCodeGen,rhsCodeGen,"and");
}

llvm::Value* AstEql::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs->codeGen(context);
llvm::Value* rhsValue = rhs->codeGen(context);
type = new AstBoolType();
if (typeid(*type) == typeid(AstIntType))
{
return context.Builder->CreateICmpEQ(lhsValue,rhsValue,"icmp_eq");
}
else if (typeid(*type) == typeid(AstFloatType))
{
return context.Builder->CreateFCmpOEQ(lhsValue,rhsValue,"fAddTmp");
}
else
{
std::cerr << "Error in AstEql:" << std::endl;
std::cerr<< "Type Mismatch!" << std::endl;
std::cerr<< "Not A Float or Int!" << std::endl;
exit(1);
}
}

llvm::Value* Astneq::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs->codeGen(context);
llvm::Value* rhsValue = rhs->codeGen(context);
type = new AstBoolType();
if (typeid(*type) == typeid(AstIntType))
{
return context.Builder->CreateICmpNE(lhsValue,rhsValue,"icmp_ne");
}
else if (typeid(*type) == typeid(AstFloatType))
{
return context.Builder->CreateFCmpONE(lhsValue,rhsValue,"fAddTmp");
}
else
{
std::cerr << "Error in Astneq:" << std::endl;
std::cerr<< "Type Mismatch!" << std::endl;
std::cerr<< "Not A Float or Int!" << std::endl;
exit(1);
}
}

llvm::Value* AstLeftSh::codeGen(genContext& context)
{
auto lhsCodeGen = lhs->codeGen(context);
auto rhsCodeGen = rhs->codeGen(context);
updateType();
return context.Builder->CreateShl(lhsCodeGen,rhsCodeGen,"shl");
}

llvm::Value* AstRightSh::codeGen(genContext& context)
{
auto lhsCodeGen = lhs->codeGen(context);
auto rhsCodeGen = rhs->codeGen(context);
updateType();
return context.Builder->CreateLShr(lhsCodeGen,rhsCodeGen,"LShr");
}

llvm::Value* AstVariableAsg::codeGen(genContext& context)
{
variableInformation var = context.varLookUp(variableName->name);
type = var.type;
return context.Builder->CreateStore((*rhs)[0]->codeGen(context),var.alloca);
}

llvm::Value* AstArrayAsg::codeGen(genContext& context)
{
variableInformation var = context.varLookUp(variableName->name);
type = var.type;
llvm::Value* zero = llvm::ConstantInt::get(*(context.IRContext), llvm::APInt(64, 0));
llvm::Value* indexPtr = context.Builder->CreateGEP(var.alloca,{zero,index->codeGen(context)},"getElementPtr");
context.Builder->CreateStore(rhs->codeGen(context),indexPtr,false);
return indexPtr;
}

llvm::Value* AstArrayListAsg::codeGen(genContext& context)
{
variableInformation var = context.varLookUp(variableName->name);
type = var.type;
llvm::Value* zero = llvm::ConstantInt::get(*(context.IRContext), llvm::APInt(64, 0)); 
llvm::Value* curIndex = index->codeGen(context);
llvm::Value* arraySize = AstIntValue(var.alloca->getType()->getArrayNumElements()).codeGen(context);
llvm::Value* increment = AstIntValue(1).codeGen(context);
for (expressionList::iterator it = arrayValues->begin(); it != arrayValues->end(); it++)
{
llvm::Value* indexPtr = context.Builder->CreateGEP(var.alloca,{zero, curIndex},"getElementPtr");
context.Builder->CreateStore((*it)->codeGen(context),indexPtr);
curIndex = context.Builder->CreateAdd(curIndex,increment);
curIndex = context.Builder->CreateMul(curIndex,context.Builder->CreateICmpSLE(curIndex,arraySize));
}
return var.alloca;
}


llvm::Value* Astleq::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs->codeGen(context);
llvm::Value* rhsValue = rhs->codeGen(context);

type = new AstBoolType();

if (lhs->type == rhs->type)
{
if (lhs->type == &intType)
{
return context.Builder->CreateICmpSLE(lhsValue,rhsValue,"icmp_ule");
}
else if (lhs->type == &floatType)
{
return context.Builder->CreateFCmpOLE(lhsValue,rhsValue,"fAddTmp");
}
else
{
std::cerr << "Error in Astleq:" << std::endl;
std::cerr<< "Type Mismatch!" << std::endl;
std::cerr<< "Not A Float or Int!" << std::endl;
exit(1);
}
}
else
{
std::cerr << "Error in Astleq:" << std::endl;
std::cerr<< "Type Mismatch!" << std::endl;
std::cerr << "Left hand doesn't match right hand side." << std::endl;
exit(1);
}
}

llvm::Value* Astgeq::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs->codeGen(context);
llvm::Value* rhsValue = rhs->codeGen(context);

type = new AstBoolType();


if (lhs->type == rhs->type)
{
if (lhs->type == &intType)
{
return context.Builder->CreateICmpSGE(lhsValue,rhsValue,"icmp_uge");
}
else if (lhs->type == &floatType)
{
return context.Builder->CreateFCmpOGE(lhsValue,rhsValue,"fAddTmp");
}
else
{
std::cerr << "Error in Astgeq:" << std::endl;
std::cerr<<"Type Mismatch!" << std::endl;
std::cerr<< "Not A Float or Int!" << std::endl;
exit(1);
}
}
else
{
std::cerr << "Error in Astleq:" << std::endl;
std::cerr<< "Type Mismatch!" << std::endl;
std::cerr << "Left hand doesn't match right hand side." << std::endl;
exit(1);
}
}

llvm::Value* Astlt::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs->codeGen(context);
llvm::Value* rhsValue = rhs->codeGen(context);

type = new AstBoolType();


if (lhs->type == rhs->type)
{
if (lhs->type == &intType)
{
return context.Builder->CreateICmpSLT(lhsValue,rhsValue,"icmp_ult");
}
else if (lhs->type == &intType)
{
return context.Builder->CreateFCmpOLT(lhsValue,rhsValue,"fAddTmp");
}
else
{
std::cerr << "Error in Astlt:" << std::endl;
std::cerr<<"Type Mismatch!" << std::endl;
std::cerr<< "Not A Float or Int!" << std::endl;
exit(1);
}
}
else
{
std::cerr << "Error in Astleq:" << std::endl;
std::cerr<< "Type Mismatch!" << std::endl;
std::cerr << "Left hand doesn't match right hand side." << std::endl;
exit(1);
}
}

llvm::Value* Astgt::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs->codeGen(context);
llvm::Value* rhsValue = rhs->codeGen(context);

type = new AstBoolType();

if (lhs->type == rhs->type)
{
if (lhs->type == &intType)
{
return context.Builder->CreateICmpSGT(lhsValue,rhsValue,"icmp_ugt");
}
else if (lhs->type == &floatType)
{
return context.Builder->CreateFCmpOGT(lhsValue,rhsValue,"fAddTmp");
}
else
{
std::cerr << "Error in Astgt:" << std::endl;
std::cerr<<"Type Mismatch!" << std::endl;
std::cerr<< "Not A Float or Int!" << std::endl;
exit(1);
}
}
else
{
std::cerr << "Error in Astleq:" << std::endl;
std::cerr<< "Type Mismatch!" << std::endl;
std::cerr << "Left hand doesn't match right hand side." << std::endl;
exit(1);
}
}

llvm::Value* AstAdd::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs->codeGen(context);
llvm::Value* rhsValue = rhs->codeGen(context);

updateType();

if (typeid(*type) == typeid(AstIntType))
{
return context.Builder->CreateAdd(lhsValue,rhsValue,"addTmp");
}
else if (typeid(*type) == typeid(AstFloatType))
{
return context.Builder->CreateFAdd(lhsValue,rhsValue,"fAddTmp");
}
else
{
std::cerr << "Error in AstAdd:" << std::endl;
std::cerr<< "Type Mismatch!"<< std::endl;
std::cerr<< "Not A Float or Int!" << std::endl;
std::cerr << "Type: " << typeid(*type).name() << std::endl;
exit(1);
}
}

llvm::Value* AstSub::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs->codeGen(context);
llvm::Value* rhsValue = rhs->codeGen(context);

updateType();

if (typeid(*type) == typeid(AstIntType))
{
return context.Builder->CreateSub(lhsValue,rhsValue,"subTmp");
}
else if (typeid(*type) == typeid(AstFloatType))
{
return context.Builder->CreateFSub(lhsValue,rhsValue,"fSubTmp");
}
else
{
std::cerr << "Error in AstSub:" << std::endl;
std::cerr<< "Type Mismatch!" << std::endl;
std::cerr<< "Not A Float or Int!" << std::endl;
exit(1);
}
}

llvm::Value* AstMul::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs->codeGen(context);
llvm::Value* rhsValue = rhs->codeGen(context);

updateType();

if (typeid(*type) == typeid(AstIntType))
{
return context.Builder->CreateMul(lhsValue,rhsValue,"mulTmp");
}
else if (typeid(*type) == typeid(AstFloatType))
{
return context.Builder->CreateFMul(lhsValue,rhsValue,"CreateFMul");
}
else
{
std::cerr << "Error in AstMul:" << std::endl;
std::cerr<< "Type Mismatch!" << std::endl;
std::cerr<< "Not A Float or Int!" << std::endl;
exit(1);
}
}

llvm::Value* AstDiv::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs->codeGen(context);
llvm::Value* rhsValue = rhs->codeGen(context);

updateType();

if (typeid(*type) == typeid(AstIntType))
{
return context.Builder->CreateSDiv(lhsValue,rhsValue,"uDivTmp");
}
else if (typeid(*type) == typeid(AstFloatType))
{
return context.Builder->CreateFDiv(lhsValue,rhsValue,"fDivTmp");
}
else
{
std::cerr << "Error in AstDiv:" << std::endl;
std::cerr<< "Type Mismatch!" << std::endl;
std::cerr<< "Not A Float or Int!" << std::endl;
exit(1);
}
}

llvm::Value* AstMod::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs->codeGen(context);
llvm::Value* rhsValue = rhs->codeGen(context);

updateType();

if (typeid(*type) == typeid(AstIntType))
{
return context.Builder->CreateSRem(lhsValue,rhsValue,"uRemTmp");
}
else if (typeid(*type) == typeid(AstFloatType))
{
return context.Builder->CreateFRem(lhsValue,rhsValue,"fRemTmp");
}
else
{
std::cerr << "Error in AstMod:" << std::endl;
std::cerr<< "Type Mismatch!" << std::endl;
std::cerr<< "Not A Float or Int!" << std::endl;
exit(1);
}
}

AstAsg::AstAsg(AstNamePtr& variableNameTmp, AstExpPtr& rhsTmp):variableName(std::move(variableNameTmp))
{
rhs.reset(new expressionList());
rhs->push_back(std::move(rhsTmp));
}

AstAsg::AstAsg(AstName* variableNameTmp, AstExp* rhsTmp)
{
variableName.reset(variableNameTmp);
rhs.reset(new expressionList());
rhs->emplace_back(rhsTmp);
}

AstAsg::AstAsg(AstNamePtr& variableNameTmp, expressionListPtr& arrayValuesTmp): variableName(std::move(variableNameTmp)),rhs(std::move(arrayValuesTmp)){}

AstAsg::AstAsg(AstName* variableNameTmp, expressionList* arrayValuesTmp)
{
variableName.reset(variableNameTmp);
rhs.reset(arrayValuesTmp);
}

llvm::Constant* AstIntValue::codeGen(genContext& context)
{
return llvm::ConstantInt::get(*(context.IRContext),llvm::APInt(32,(uint64_t)value));
}

llvm::Constant* AstCharValue::codeGen(genContext& context)
{
return llvm::ConstantInt::get(*(context.IRContext),llvm::APInt(8,(uint64_t)value));
}

/*AstStringValue::AstStringValue(char* string,unsigned int size):AstConstant(stringType)
{
for (int i = 0; i <= size; i++)
{
value.push_back(string[i]);
}
}*/

llvm::Constant* AstStringValue::codeGen(genContext& context)
{
/*std::vector<llvm::Constant*> stringValues;
for (std::vector<char>::iterator it = value.begin(); it != value.end(); ++it)
{
stringValues.push_back(llvm::ConstantInt::get(*(context.IRContext),llvm::APInt(8,(uint64_t)*it))
);
}
llvm::ArrayType* arrayType = llvm::ArrayType::get(llvm::Type::getInt8Ty(*(context.IRContext)),stringValues.size());
llvm::Constant* array = llvm::ConstantArray::get(arrayType,stringValues);
return llvm::ConstantExpr::getBitCast(array,llvm::Type::getInt8Ty(*(context.IRContext))->getPointerTo());*/
llvm::Constant* stringArray = llvm::ConstantDataArray::getString(*(context.IRContext),value,nullTerminated);
llvm::GlobalVariable* globalString = new llvm::GlobalVariable(*(context.CurModule),stringArray->getType(),true,llvm::GlobalVariable::InternalLinkage,stringArray);
return llvm::ConstantExpr::getBitCast(globalString,llvm::Type::getInt8Ty(*context.IRContext)->getPointerTo());
}

llvm::Constant* AstFloatValue::codeGen(genContext& context)
{
return llvm::ConstantFP::get(*(context.IRContext),llvm::APFloat(value));
}

llvm::Constant* AstBoolValue::codeGen(genContext& context)
{
return llvm::ConstantInt::get(*(context.IRContext),llvm::APInt(1,(uint64_t)value));
}


llvm::Value* AstGlobalVariableDeclaration::codeGen(genContext& context)
{
if (initializer)
{
llvm::GlobalVariable* globalVar = new llvm::GlobalVariable (*(context.CurModule),variableType->typeOf(context),false,llvm::GlobalValue::ExternalLinkage,initializer->codeGen(context),variableName->name); //The variableName can't be first or a bug in the the llvm::GlobalVariable will overwrite the queue pointer to the variable crashing the program.

context.pushGlobalVariable(variableName->name,globalVar,variableType);
return globalVar;
}
else
{
llvm::GlobalVariable* globalVar = new llvm::GlobalVariable (*(context.CurModule),variableType->typeOf(context),false,llvm::GlobalVariable::ExternalLinkage,nullptr,variableName->name); //The variableName can't be first or a bug in the the llvm::GlobalVariable will overwrite the queue pointer to the variable crashing the program.
context.pushGlobalVariable(variableName->name,globalVar,variableType);
return globalVar;
}
}

llvm::AllocaInst* AstVariableDeclaration::codeGen(genContext& context)
{
llvm::Type* varType = variableType->typeOf(context);
llvm::AllocaInst* alloca = context.Builder->CreateAlloca(varType,nullptr,variableName->name);
context.pushVariable(variableName->name,alloca,variableType);
if (initializer)
{
context.Builder->CreateStore(initializer->codeGen(context),alloca);
}
return alloca;
}

llvm::Value* AstArrayDeclaration::codeGen(genContext& context)
{
llvm::Type* arrayType = llvm::ArrayType::get(variableType->typeOf(context),arraySize->value);
llvm::AllocaInst* arrayLocation = context.Builder->CreateAlloca(arrayType,arraySize->codeGen(context),variableName->name);
context.pushVariable(variableName->name,arrayLocation,variableType);
if (initializer)
{
expressionList::iterator it;
unsigned int curIndex = 0;
unsigned int arraySizeMod = arraySize->value + 1;
llvm::Value* zero = llvm::ConstantInt::get(*(context.IRContext), llvm::APInt(64, 0));
for (it = initializer->begin(); it != initializer->end(); it++)
{
llvm::Value* offset = llvm::ConstantInt::get(*(context.IRContext),llvm::APInt(32,curIndex));
llvm::Value* pointer = context.Builder->CreateGEP(arrayLocation,{zero,offset},"getElementPtr");
context.Builder->CreateStore((*it)->codeGen(context),pointer);
//context.Builder->CreateInsertValue(arrayLocation,(*it)->codeGen(context),curIndex,"insertInArray");
curIndex = (curIndex + 1) % arraySizeMod;
}
}

return arrayLocation;
}

llvm::Value* AstFunctionCall::codeGen(genContext& context)
{
functionInformation functionValue = context.functionLookUp(functionName->name);
type = functionValue.returnType;
std::vector <llvm::Value*> argsV;
std::vector<const AstType*>::iterator functionTypesIt = functionValue.argumentTypes.begin();
for (expressionList::iterator it = args->begin(); it != args->end(); it++)
{
argsV.push_back((*it)->codeGen(context));
if (functionTypesIt != functionValue.argumentTypes.end())
{
if ((*it)->type->getTypeName() != (*functionTypesIt)->getTypeName())
{
std::cerr << "Function arguments doesn't match type!" << std::endl;
std::cerr << "Argument Type: " << (*it)->type->getTypeName() << std::endl;
std::cerr << "Expected Type: " << (*functionTypesIt)->getTypeName() << std::endl;
exit(1);
}
functionTypesIt++;
}
else if (functionValue.varArg == false)
{
std::cerr << "Too many arguments!" << std::endl;
}
}
return context.Builder->CreateCall(functionValue.function,argsV,"functionCall",nullptr);
}


llvm::Value* AstVariableAddress::codeGen(genContext& context)
{
variableInformation var = context.varLookUp(variableName->name);
if (var.alloca)
{
	return var.alloca;
}
else
{
globalVariableInformation globalVar = context.globalVariableLookUp(variableName->name);
if (globalVar.alloca)
{
	return var.alloca;
}
else
{
std::cerr << "Couldn't find variable: " << variableName->name << std::endl;
exit(1);
}
}
}


llvm::Value* AstArrayAddress::codeGen(genContext& context)
{
return context.varLookUp(variableName->name).alloca;
}

llvm::Value* AstVariableCall::codeGen(genContext& context)
{
variableInformation var = context.varLookUp(variableName->name);
if (var.alloca)
{
type = var.type;
return context.Builder->CreateLoad(var.alloca,variableName->name);
}
else
{
globalVariableInformation globalVar = context.globalVariableLookUp(variableName->name);
if (globalVar.alloca)
{
type = globalVar.type;
return context.Builder->CreateLoad(globalVar.alloca,variableName->name);
}
else
{
std::cerr << "Couldn't find variable: " << variableName->name << std::endl;
exit(1);
}
}
}

llvm::Value* AstArrayCall::codeGen(genContext& context)
{
variableInformation var = context.varLookUp(variableName->name);
llvm::AllocaInst* alloca = var.alloca;
type = var.type;
llvm::Value* zero = llvm::ConstantInt::get(*(context.IRContext), llvm::APInt(64, 0));
llvm::Value* indexPtr = context.Builder->CreateGEP(alloca,{zero,index->codeGen(context)},"getElementPtr");
return context.Builder->CreateLoad(indexPtr,variableName->name);
}


llvm::Value* AstDeref::codeGen(genContext& context)
{
return context.Builder->CreateLoad(type->typeOf(context),expNode->codeGen(context));
}

llvm::Value* AstFunctionDeclaration::codeGen(genContext& context)
{
std::vector<llvm::Type*> argTypes;
std::vector<const AstType*> types;
variableList::iterator it;
for (it = arguments->begin(); it != arguments->end(); it++)
{
types.push_back(it->variableType);
argTypes.push_back(it->variableType->typeOf(context));
}
llvm::FunctionType *ft = llvm::FunctionType::get(returnType->typeOf(context),argTypes,false);
llvm::Function* func = llvm::Function::Create(ft,llvm::Function::ExternalLinkage,functionName->name,context.CurModule.get());
llvm::BasicBlock *BB = llvm::BasicBlock::Create(*(context.IRContext),"entry",func);
context.Builder->SetInsertPoint(BB);
context.pushBlock(block);
for (it = arguments->begin(); it!= arguments->end(); it++)
{
std::string name = ((*it).variableName)->name;
llvm::AllocaInst* alloca = it->codeGen(context);
context.pushVariable(name,alloca,returnType);
}
context.backBlock()->codeGenInFunction(context);
context.popBlock();
context.pushFunction(functionName->name,func,types,returnType);
std::cerr << "Generated: " << functionName->name << std::endl;
return func;
}

llvm::Value* AstReturnStat::codeGen(genContext& context)
{
return context.Builder->CreateRet(returnValue->codeGen(context));
}

llvm::Value* AstName::codeGen(genContext& context)
{
return nullptr;
}

llvm::Value* AstBlock::codeGen(genContext& context)
{
context.pushBlock(static_cast<AstBlock*>(context.popAndGetAstNode()));
llvm::Value* genResult;
statementList::iterator it;
for (it = statements->begin();it != statements->end(); it++)
{
genResult = (*it)->codeGen(context);
}
context.popBlock();
return genResult;
}

llvm::Value* AstBlock::codeGenInFunction(genContext& context)
{
llvm::Value* genResult;
statementList::iterator it;
for (it = statements->begin();it != statements->end(); it++)
{
genResult = (*it)->codeGen(context);
}
return genResult;
}

llvm::Value* AstExpStat::codeGen(genContext& context)
{
return expression->codeGen(context);
}

/*
extern const AstIntType intType;
extern const AstStringType stringType;
extern const AstCharType charType;
extern const AstBoolType boolType;
extern const AstFloatType floatType;
*/
