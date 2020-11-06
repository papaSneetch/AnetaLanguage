#include "annetBuilder.h"

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
BasicBlock *ifBB = BasicBlock::Create(context.IRContext,"ifStats",ifBranchFunc);
BasicBlock *ElseBB = BasicBlock::Create(context.IRContext,"elseStats");
BasicBlock *MergeBB = BasicBlock::Create(context.IRContext,"merge");
llvm::Value* testResult = testCondition.codeGen(context.IRContext);
Builder.CreateCondBr(testResult,ifBB,ElseBB);
Builder.SetInsertPoint(ifBranchFunc);
ifBlock.codeGen(context.IRContext); //Generates the code in the if branch.
Builder.CreatBr(MergeBB);
MergeBB = Builder.GetInsertBlock(); //Updates branch in case of recursive function.
ifBranchFunc->getBasicBlockList().push_back(ElseBB); //Adds block to function.
Builder.SetInsertPoint(ElseBB);
elseBlock.codeGen(context.IRContext); //Generates the code for the else branch.
Builder.CreateBr(MergeBB);
MergeBB = Builder.GetInsertBlock();
ifBranchFunc->Builder.GetBasicBlockList().push_back(MergeBB);
Builder.SetInsertPoint(MergeBB);
}

AstWhileLoop::codeGen(genContext& context)
{
BasicBlock *loop = BasicBlock::Create(IRContext,"loop");
BasicBlock *endLoop = BasicBlock::Create(IRContext,"endLoop");
BasicBlock *condEval = BasicBlock::Create(IRContext,"condEval",function);
Builder.SetInsertPoint(condEval);
testCondition.codeGen(context.IRContext)
Builder.CreateCondBr(Condition_Statement_Register,loop,endLoop);
Builder.SetInsertPoint(loop);
whileBlock.codeGen(context.IRContext);
Builder.CreateBr(condEval);
Builder.SetInsertPoint(endLoop);
}

llvm::Value* AstOr::codeGen(genContext& context)
{
return Builder.CreateOr(lhs.codeGen(context),rhs.codeGen(context),"or");
}

llvm::Value* AstXor::codeGen(genContext& context)
{
return Builder.CreateXor(lhs.codeGen(context),rhs.codeGen(context),"xor");	
}

llvm::Value* AstAnd::codeGen(genContext& context)
{
return Builder.CreateAnd(lhs.codeGen(context),rhs.codeGen(context),"and");
}

llvm::Value* AstEql::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs.codeGen(context);
llvm::Value* rhsValue = rhs.codeGen(context);

if ((lhsValue.getType() == IntegerTyID) && (rhs.getType() == IntegerTyID))
{
return Builder.CreateICmpEQ(lhsValue(context),rhs.codeGen(context),"icmp_eq");
}
else if ((lhsValue.getType() == FloatTyId) && (rhs.getType() == FloatTyId))
{
return Builder.CreateFCmpOEQ(lhsValue(context),rhs.codeGen(context),"fAddTmp");
}
else
{
std::cout<<"Type Mismatch!" std::endl;
exit;
}
}

llvm::Value* Astneq::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs.codeGen(context);
llvm::Value* rhsValue = rhs.codeGen(context);

if ((lhsValue.getType() == IntegerTyID) && (rhs.getType() == IntegerTyID))
{
return Builder.CreateICmpNE(lhsValue(context),rhs.codeGen(context),"icmp_ne");
}
else if ((lhsValue.getType() == FloatTyId) && (rhs.getType() == FloatTyId))
{
return Builder.CreateFCmpONE(lhsValue(context),rhs.codeGen(context),"fAddTmp");
}
else
{
std::cout<<"Type Mismatch!" std::endl;
exit;
}
}

llvm::Value* AstLeftSh::codeGen(genContext& context)
{
return Builder.CreateShl(lhs.codeGen(context),rhs.codeGen(context),"shl");
}

llvm::Value* AstRightSh::codeGen(genContext& context)
{
return Builder.CreateLShr(lhs.codeGen(context),rhs.codeGen(context),"LShr");
}

llvm::Value* AstAsg::codeGen(genContext& context)
{
return Builder.CreateStore(lhs.codeGen(context),rhs.codeGen(context));
}

llvm::Value* Astleq::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs.codeGen(context);
llvm::Value* rhsValue = rhs.codeGen(context);

if ((lhs.getType() == IntegerTyID) && (rhs.getType() == IntegerTyID))
{
return Builder.CreateICmpSLE(lhsValue(context),rhs.codeGen(context),"icmp_ule");
}
else if ((lhsValue.getType() == FloatTyId) && (rhs.getType() == FloatTyId))
{
return Builder.CreateFCmpOLE(lhsValue(context),rhs.codeGen(context),"fAddTmp");
}
else
{
std::cout<<"Type Mismatch!" std::endl;
exit;
}

llvm::Value* Astgeq::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs.codeGen(context);
llvm::Value* rhsValue = rhs.codeGen(context);

if ((lhsValue.getType() == IntegerTyID) && (rhs.getType() == IntegerTyID))
{
return Builder.CreateICmpSGE(lhsValue(context),rhs.codeGen(context),"icmp_uge");
}
else if ((lhsValue.getType() == FloatTyId) && (rhs.getType() == FloatTyId))
{
return Builder.CreateFCmpOGE(lhsValue(context),rhs.codeGen(context),"fAddTmp");
}
else
{
std::cout<<"Type Mismatch!" std::endl;
exit;
}

llvm::Value* Astlt::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs.codeGen(context);
llvm::Value* rhsValue = rhs.codeGen(context);

if ((lhsValue.getType() == IntegerTyID) && (rhs.getType() == IntegerTyID))
{
return Builder.CreateICmpSLT(lhsValue(context),rhs.codeGen(context),"icmp_ult");
}
else if ((lhsValue.getType() == FloatTyId) && (rhs.getType() == FloatTyId))
{
return Builder.CreateFCmpOLT(lhsValue(context),rhs.codeGen(context),"fAddTmp");
}
else
{
std::cout<<"Type Mismatch!" std::endl;
exit;
}

llvm::Value* Astgt::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs.codeGen(context);
llvm::Value* rhsValue = rhs.codeGen(context);
if ((lhsValue.getType() == IntegerTyID) && (rhs.getType() == IntegerTyID))
{
return Builder.CreateICmpSGT(lhsValue(context),rhs.codeGen(context),"icmp_ugt");
}
else if ((lhsValue.getType() == FloatTyId) && (rhs.getType() == FloatTyId))
{
return Builder.CreateFCmpOGT(lhsValue(context),rhs.codeGen(context),"fAddTmp");
}
else
{
std::cout<<"Type Mismatch!" std::endl;
exit;
}

llvm::Value* AstAdd::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs.codeGen(context);
llvm::Value* rhsValue = rhs.codeGen(context);

if ((lhsValue.getType() == IntegerTyID) && (rhs.getType() == IntegerTyID))
{
return Builder.CreateAdd(lhsValue(context),rhs.codeGen(context),"addTmp");
}
else if ((lhsValue.getType() == FloatTyId) && (rhs.getType() == FloatTyId))
{
return Builder.CreateFAdd(lhsValue(context),rhs.codeGen(context),"fAddTmp");
}
else
{
std::cout<<"Type Mismatch!" std::endl;
exit;
}
}

llvm::Value* AstSub::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs.codeGen(context);
llvm::Value* rhsValue = rhs.codeGen(context);

if ((lhsValue.getType() == IntegerTyID) && (rhs.getType() == IntegerTyID))
{
return Builder.CreateSub(lhsValue(context),rhs.codeGen(context),"subTmp");
}
else if ((lhsValue.getType() == FloatTyId) && (rhs.getType() == FloatTyId))
{
return Builder.CreateFSub(lhsValue(context),rhs.codeGen(context),"fSubTmp");
}
else
{
std::cout<<"Type Mismatch!" std::endl;
exit;
}
}

llvm::Value* AstMul::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs.codeGen(context);
llvm::Value* rhsValue = rhs.codeGen(context);

if ((lhsValue.getType() == IntegerTyID) && (rhs.getType() == IntegerTyID))
{
return Builder.CreateMul(lhsValue(context),rhs.codeGen(context),"mulTmp");
}
else if ((lhsValue.getType() == FloatTyId) && (rhs.getType() == FloatTyId))
{
return Builder.CreateFMul(lhsValue(context),rhs.codeGen(context),"CreateFMul");
}
else
{
std::cout<<"Type Mismatch!" std::endl;
exit;
}
}

llvm::Value* AstDiv::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs.codeGen(context);
llvm::Value* rhsValue = rhs.codeGen(context);

if ((lhsValue.getType() == IntegerTyID) && (rhs.getType() == IntegerTyID))
{
return Builder.CreateSDiv(lhsValue(context),rhs.codeGen(context),"uDivTmp");
}
else if ((lhsValue.getType() == FloatTyId) && (rhs.getType() == FloatTyId))
{
return Builder.CreateFDiv(lhsValue(context),rhs.codeGen(context),"fDivTmp");
}
else
{
std::cout<<"Type Mismatch!" std::endl;
exit;
}
}

llvm::Value* AstMod::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs.codeGen(context);
llvm::Value* rhsValue = rhs.codeGen(context);

if ((lhsValue.getType() == IntegerTyID) && (rhs.getType() == IntegerTyID))
{
return Builder.CreateSRem(lhsValue(context),rhs.codeGen(context),"uRemTmp");
}
else if ((lhsValue.getType() == FloatTyId) && (rhs.getType() == FloatTyId))
{
return Builder.CreateFRem(lhsValue(context),rhs.codeGen(context),"fRemTmp");
}
else
{
std::cout<<"Type Mismatch!" std::endl;
exit;
}
}

llvm::Value* AstAdd::codeGen(genContext& context)
{
llvm::Value* lhsValue = lhs.codeGen(context);
llvm::Value* rhsValue = rhs.codeGen(context);

if ((lhsValue.getType() == IntegerTyID) && (rhs.getType() == IntegerTyID))
{
return Builder.Create(lhsValue(context),rhs.codeGen(context));
}
else if ((lhsValue.getType() == FloatTyId) && (rhs.getType() == FloatTyId))
{
return Builder.CreateFAdd(lhsValue(context),rhs.codeGen(context),);
}
else
{
std::cout<<"Type Mismatch!" std::endl;
exit;
}
}

llvm::Value* AstIntValue::codeGen(genContext& context)
{
return ConstantInt::get(IRContext,APInt(32,val)
}

llvm::Value* AstFloatValue::codeGen(genContext& context)
{
return ConstantFP::get(IRContext,APFloat(val));
}

llvm::Value* AstBoolValue::codeGen(genContext& context)
{
return ConstantInt::get(IRContext,APInt(1,val));
}

llvm::Value* AstVariableDeclaration::codeGen(genContext& context)
{
return Builder.CreateAlloca(variableType.typeOf(),"varDecl");
}

llvm::Value* AstArrayDeclaration::codeGen(genContext& context)
{
return Builder.CreateAlloca(variableType.typeOf(),arraySize.codeGen(context),"arrayDecl");
}

llvm::Value* AstFunctionCall::codeGen(genContext& context)
{
return Builder.CreateCall(functionLookUp(functionName),args,"functionCall");
}

llvm::Value* AstVariableCall::codeGen(genContext& context)
{
return Builder.CreateLoad(context.varLookUp(variableName),"varCall"); 
}

llvm::Value* AstArrayCall::codeGen(genContext& context)
{
Value* variable = context.varLookUp(variableName);
Value* arrayPointer = llvm::ConstantFolder::CreateGetElementPtr(variable*.typeOf(),variable,index);
return Builder.CreateLoad(arrayPointer,"arrayCall");
}

llvm::Value* AstFunctionDeclaration::codeGen(genContext& context)
{
vector<const Type*> argTypes;
variableList::const_iterator it;
for (it = arguments.begin(); it != arguments.end(); it++)
{
argTypes.push_back((typeof(**it).type));
}
FunctionType *ft = FunctionType::get(returnType.typeOf(),argTypes,false);
Function* func = Function::Create(ft,Function::ExternalLinkage,functionName,CurModule.get());
context.pushblock(block);
for (it=arguments.begin(); it!= arguments.end(); it++)
{
(**it).codeGen(context);
}
block.genCode(context);
context.popblock(block);
return func;
}

llvm::Value* AstBlock::codeGen(genContext& context)
{
statementList::const_iterator it;
Value* genResult;
for (it = statements.begin();it != statements.end(); it++)
{
genResult = (**it).codeGen(context);
}
return genResult;
}
