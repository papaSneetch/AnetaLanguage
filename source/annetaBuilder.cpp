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
llvm::Value* generatedValue = (*rhs)[0]->codeGen(context);
context.Builder->CreateStore(generatedValue,var.alloca);
return generatedValue;
}

llvm::Value* AstArrayAsg::codeGen(genContext& context)
{
arrayInformation array= context.arrayLookUp(variableName->name);
if (array.type->getTypeName() == "pointer")
{
type = static_cast<const AstPointerType*>(array.type)->referType;
llvm::Value* zero = llvm::ConstantInt::get(*(context.IRContext), llvm::APInt(64, 0));
llvm::Value* indexPtr = context.Builder->CreateGEP(array.alloca,{zero,index->codeGen(context)},"getElementPtr");
llvm::Value* asignedValue = rhs->front()->codeGen(context);
context.Builder->CreateStore(asignedValue,indexPtr,false);
return asignedValue;
}
else
{
std::cerr << "Error: Not an array or pointer!" << std::endl;
exit(1);
}
}

llvm::Value* AstArrayStringAsg::codeGen(genContext& context)
{

arrayInformation array= context.arrayLookUp(variableName->name);
llvm::Value* zero = AstIntValue(0).codeGen(context);
int index = 0;
for (std::string::iterator it = str.begin(); it != str.end(); it++)
{
llvm::Value* indexPtr = context.Builder->CreateGEP(array.alloca,{zero,AstIntValue(index).codeGen(context)},"getElementPtr");
context.Builder->CreateStore(AstCharValue(*it).codeGen(context),indexPtr,false);
index++;
}
return context.Builder->CreateGEP(array.alloca,{zero,zero},"getElementPtr");
}


llvm::Value* AstArrayListAsg::codeGen(genContext& context)
{
arrayInformation array = context.arrayLookUp(variableName->name);
llvm::Value* zero = AstIntValue(0).codeGen(context);
int index = 0;
for (expressionList::iterator it = rhs->begin(); it != rhs->end(); it++)
{
llvm::Value* indexPtr = context.Builder->CreateGEP(array.alloca,{zero,AstIntValue(index).codeGen(context)},"getElementPtr");
context.Builder->CreateStore((*it)->codeGen(context),indexPtr,false);
index++;
}
return array.alloca;
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

llvm::Value* AstEql::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs->codeGen(context);
llvm::Value* rhsValue = rhs->codeGen(context);
type = new AstBoolType();
if (lhs->type == rhs->type)
{
if (lhs->type == &intType)
{
return context.Builder->CreateICmpEQ(lhsValue,rhsValue,"icmp_eq");
}
else if (lhs->type == &floatType)
{
return context.Builder->CreateFCmpOEQ(lhsValue,rhsValue,"fAddTmp");
}
}
std::cerr << "Error in AstEql:" << std::endl;
std::cerr<< "Type Mismatch!" << std::endl;
std::cerr<< "Not A Float or Int!" << std::endl;
exit(1);
}

llvm::Value* Astneq::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs->codeGen(context);
llvm::Value* rhsValue = rhs->codeGen(context);
type = new AstBoolType();
if (lhs->type == rhs->type)
{
if (lhs->type == &intType)
{
return context.Builder->CreateICmpNE(lhsValue,rhsValue,"icmp_ne");
}
else if (lhs->type == &floatType)
{
return context.Builder->CreateFCmpONE(lhsValue,rhsValue,"fAddTmp");
}
}
std::cerr << "Error in Astneq:" << std::endl;
std::cerr<< "Type Mismatch!" << std::endl;
std::cerr<< "Not A Float or Int!" << std::endl;
exit(1);
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
return nullptr;
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
llvm::Type* arrayType = llvm::ArrayType::get(variableType->typeOf(context),arraySize);
llvm::AllocaInst* arrayLocation = context.Builder->CreateAlloca(arrayType,AstIntValue(arraySize).codeGen(context),variableName->name);
context.pushArrayVariable(variableName->name,arrayLocation,variableType,arraySize);
return arrayLocation;
}


llvm::Value* AstArrayListDeclaration::codeGen(genContext& context)
{
llvm::Type* arrayType = llvm::ArrayType::get(variableType->typeOf(context),arraySize);
llvm::AllocaInst* arrayLocation = context.Builder->CreateAlloca(arrayType,AstIntValue(arraySize).codeGen(context),variableName->name);
context.pushArrayVariable(variableName->name,arrayLocation,variableType,arraySize);
llvm::Value* zero = AstIntValue(0).codeGen(context);
int index = 0;
for (expressionList::iterator it = list->begin(); it != list->end(); it++)
{
llvm::Value* indexPtr = context.Builder->CreateGEP(arrayLocation,{zero,AstIntValue(index).codeGen(context)},"getElementPtr");
context.Builder->CreateStore((*it)->codeGen(context),indexPtr,false);
index++;
}
return context.Builder->CreateGEP(arrayLocation,{zero,zero},"getElementPtr");
}

llvm::Value* AstArrayStringDeclaration::codeGen(genContext& context)
{
llvm::Type* arrayType = llvm::ArrayType::get(variableType->typeOf(context),arraySize);
llvm::AllocaInst* arrayLocation = context.Builder->CreateAlloca(arrayType,AstIntValue(arraySize).codeGen(context),variableName->name);
context.pushArrayVariable(variableName->name,arrayLocation,variableType,arraySize);
llvm::Value* zero = AstIntValue(0).codeGen(context);
int index = 0;
for (std::string::iterator it = string.begin(); it != string.end(); it++)
{
llvm::Value* indexPtr = context.Builder->CreateGEP(arrayLocation,{zero,AstIntValue(index).codeGen(context)},"getElementPtr");
context.Builder->CreateStore(AstCharValue(*it).codeGen(context),indexPtr,false);
index++;
}
return context.Builder->CreateGEP(arrayLocation,{zero,zero},"getElementPtr");
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
arrayInformation array = context.arrayLookUp(variableName->name);
if (array.alloca)
{
type = array.type;
llvm::Value* zero = llvm::ConstantInt::get(*(context.IRContext), llvm::APInt(64, 0));
return context.Builder->CreateGEP(context.Builder->CreateLoad(array.alloca,variableName->name),{zero,index->codeGen(context)},"getElementPtr");
}
else
{
std::cerr << "Couldn't find array: " << variableName->name << std::endl;
exit(1);
}
}


llvm::Value* AstArrayCallAddress::codeGen(genContext& context)
{
arrayInformation array = context.arrayLookUp(variableName->name);
if (array.alloca)
{
type = array.type;
return array.alloca;
}
else
{
std::cerr << "Couldn't find array: " << variableName->name << std::endl;
exit(1);
}
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
if (var.type->getTypeName() == "pointer")
{
type = static_cast<const AstPointerType*>(var.type)->referType;
llvm::Value* zero = llvm::ConstantInt::get(*(context.IRContext), llvm::APInt(64, 0));
llvm::Value* allocaLoc = context.Builder->CreateLoad(var.alloca,variableName->name);
llvm::Value* indexPtr = context.Builder->CreateGEP(allocaLoc,{zero,index->codeGen(context)},"getelementptr");
return context.Builder->CreateLoad(indexPtr,variableName->name);
}
else
{
std::cerr << "Error: Not an array or pointer!" << std::endl;
exit(1);
}
}

llvm::Value* AstDeref::codeGen(genContext& context)
{
return context.Builder->CreateLoad(type->typeOf(context),expNode->codeGen(context));
}

llvm::Value* AstFunctionDeclaration::codeGen(genContext& context)
{
// std::cerr << "Generating Function: " << functionName->name << std::endl;

std::vector<llvm::Type*> argTypes;
std::vector<const AstType*> types;
variableList::iterator it;
// std::cerr << "Generating Function Type List: " << functionName->name << std::endl;
for (it = arguments->begin(); it != arguments->end(); it++)
{
types.push_back(it->variableType);
argTypes.push_back(it->variableType->typeOf(context));
}
// std::cerr << "Generated Function Type List: " << functionName->name << std::endl;
llvm::FunctionType *ft = llvm::FunctionType::get(returnType->typeOf(context),argTypes,false);
llvm::Function* func = llvm::Function::Create(ft,llvm::Function::ExternalLinkage,functionName->name,context.CurModule.get());
llvm::BasicBlock *BB = llvm::BasicBlock::Create(*(context.IRContext),"entry",func);
// std::cerr << "Generated InsertPoint For: " << functionName->name << std::endl;
context.Builder->SetInsertPoint(BB);
context.pushBlock(block);
// std::cerr << "Generating Function Arguments: " << functionName->name << std::endl;

auto argPointer = func->arg_begin();
for (it = arguments->begin(); it!= arguments->end(); it++)
{
if (argPointer != func->arg_end())
{
std::string name = ((*it).variableName)->name;
// std::cerr << "Generating Function Argument Name: " << name << std::endl;
llvm::AllocaInst* alloca = context.Builder->CreateAlloca(it->variableType->typeOf(context),nullptr,name);
context.Builder->CreateStore(argPointer,alloca);
context.pushVariable(name,alloca,it->variableType);
argPointer++;
//std::cerr << "Generated Function Argument Name: " << name << std::endl;
}
}
// std::cerr << "Generated Function Arguments. Generating Function Body: " << functionName->name << std::endl;
context.backBlock()->codeGenInFunction(context);
// std::cerr << "Generated Function Body: " << functionName->name << std::endl;
context.popBlock();
context.pushFunction(functionName->name,func,types,returnType);
// std::cerr << "Generated: " << functionName->name << std::endl;
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
extern const AstCharType charType;
extern const AstBoolType boolType;
extern const AstFloatType floatType;
*/
