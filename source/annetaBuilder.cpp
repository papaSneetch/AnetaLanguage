
llvm::Value* AstOr::codeGen()
{
return Builder.CreateOr(lhs.codeGen(),rhs.codeGen(),"or");
}

llvm::Value* AstXor::codeGen()
{
return Builder.CreateXor(lhs,rhs,"xor");	
}

llvm::Value* AstAnd::codeGen()
{
return Builder.CreateAnd(lhs,rhs,"and");
}

llvm::Value* AstEql::codeGen()
{
if ((lhs.getType() == IntegerTyID) && (rhs.getType() == IntegerTyID))
{
return Builder.CreateICmpEQ(lhs.codeGen(),rhs.codeGen(),"icmp_eq");
}
else if ((lhs.getType() == FloatTyId) && (rhs.getType() == FloatTyId))
{
return Builder.CreateFCmpOEQ(lhs.codeGen(),rhs.codeGen(),"fAddTmp");
}
else
{
std::cout<<"Type Mismatch!" std::endl;
exit;
}
}

llvm::Value* Astneq::codeGen()
{
if ((lhs.getType() == IntegerTyID) && (rhs.getType() == IntegerTyID))
{
return Builder.CreateICmpNE(lhs.codeGen(),rhs.codeGen(),"icmp_ne");
}
else if ((lhs.getType() == FloatTyId) && (rhs.getType() == FloatTyId))
{
return Builder.CreateFCmpONE(lhs.codeGen(),rhs.codeGen(),"fAddTmp");
}
else
{
std::cout<<"Type Mismatch!" std::endl;
exit;
}
}

llvm::Value* AstLeftSh::codeGen()
{
return Builder.CreateShl(lhs.codeGen(),rhs.codeGen(),"shl");
}

llvm::Value* AstRightSh::codeGen()
{
return Builder.CreateLShr(lhs.codeGen(),rhs.codeGen(),"LShr");
}

llvm::Value* AstAsg::codeGen()
{
return Builder.CreateStore(lhs.codeGen(),rhs.codeGen());
}

llvm::Value* Astleq::codeGen()
{
if ((lhs.getType() == IntegerTyID) && (rhs.getType() == IntegerTyID))
{
return Builder.CreateICmpSLE(lhs.codeGen(),rhs.codeGen(),"icmp_ule");
}
else if ((lhs.getType() == FloatTyId) && (rhs.getType() == FloatTyId))
{
return Builder.CreateFCmpOLE(lhs.codeGen(),rhs.codeGen(),"fAddTmp");
}
else
{
std::cout<<"Type Mismatch!" std::endl;
exit;
}

llvm::Value* Astgeq::codeGen()
{
if ((lhs.getType() == IntegerTyID) && (rhs.getType() == IntegerTyID))
{
return Builder.CreateICmpSGE(lhs.codeGen(),rhs.codeGen(),"icmp_uge");
}
else if ((lhs.getType() == FloatTyId) && (rhs.getType() == FloatTyId))
{
return Builder.CreateFCmpOGE(lhs.codeGen(),rhs.codeGen(),"fAddTmp");
}
else
{
std::cout<<"Type Mismatch!" std::endl;
exit;
}

llvm::Value* Astlt::codeGen()
{
if ((lhs.getType() == IntegerTyID) && (rhs.getType() == IntegerTyID))
{
return Builder.CreateICmpSLT(lhs.codeGen(),rhs.codeGen(),"icmp_ult");
}
else if ((lhs.getType() == FloatTyId) && (rhs.getType() == FloatTyId))
{
return Builder.CreateFCmpOLT(lhs.codeGen(),rhs.codeGen(),"fAddTmp");
}
else
{
std::cout<<"Type Mismatch!" std::endl;
exit;
}

llvm::Value* Astgt::codeGen()
{
if ((lhs.getType() == IntegerTyID) && (rhs.getType() == IntegerTyID))
{
return Builder.CreateICmpSGT(lhs.codeGen(),rhs.codeGen(),"icmp_ugt");
}
else if ((lhs.getType() == FloatTyId) && (rhs.getType() == FloatTyId))
{
return Builder.CreateFCmpOGT(lhs.codeGen(),rhs.codeGen(),"fAddTmp");
}
else
{
std::cout<<"Type Mismatch!" std::endl;
exit;
}

llvm::Value* AstAdd::codeGen()
{
if ((lhs.getType() == IntegerTyID) && (rhs.getType() == IntegerTyID))
{
return Builder.CreateAdd(lhs.codeGen(),rhs.codeGen(),"addTmp");
}
else if ((lhs.getType() == FloatTyId) && (rhs.getType() == FloatTyId))
{
return Builder.CreateFAdd(lhs.codeGen(),rhs.codeGen(),"fAddTmp");
}
else
{
std::cout<<"Type Mismatch!" std::endl;
exit;
}
}

llvm::Value* AstSub::codeGen()
{
if ((lhs.getType() == IntegerTyID) && (rhs.getType() == IntegerTyID))
{
return Builder.CreateSub(lhs.codeGen(),rhs.codeGen(),"subTmp");
}
else if ((lhs.getType() == FloatTyId) && (rhs.getType() == FloatTyId))
{
return Builder.CreateFSub(lhs.codeGen(),rhs.codeGen(),"fSubTmp");
}
else
{
std::cout<<"Type Mismatch!" std::endl;
exit;
}
}

llvm::Value* AstMul::codeGen()
{
if ((lhs.getType() == IntegerTyID) && (rhs.getType() == IntegerTyID))
{
return Builder.CreateMul(lhs.codeGen(),rhs.codeGen(),"mulTmp");
}
else if ((lhs.getType() == FloatTyId) && (rhs.getType() == FloatTyId))
{
return Builder.CreateFMul(lhs.codeGen(),rhs.codeGen(),"CreateFMul");
}
else
{
std::cout<<"Type Mismatch!" std::endl;
exit;
}
}

llvm::Value* AstDiv::codeGen()
{
if ((lhs.getType() == IntegerTyID) && (rhs.getType() == IntegerTyID))
{
return Builder.CreateSDiv(lhs.codeGen(),rhs.codeGen(),"uDivTmp");
}
else if ((lhs.getType() == FloatTyId) && (rhs.getType() == FloatTyId))
{
return Builder.CreateFDiv(lhs.codeGen(),rhs.codeGen(),"fDivTmp");
}
else
{
std::cout<<"Type Mismatch!" std::endl;
exit;
}
}

llvm::Value* AstMod::codeGen()
{
if ((lhs.getType() == IntegerTyID) && (rhs.getType() == IntegerTyID))
{
return Builder.CreateSRem(lhs.codeGen(),rhs.codeGen(),"uRemTmp");
}
else if ((lhs.getType() == FloatTyId) && (rhs.getType() == FloatTyId))
{
return Builder.CreateFRem(lhs.codeGen(),rhs.codeGen(),"fRemTmp");
}
else
{
std::cout<<"Type Mismatch!" std::endl;
exit;
}
}

llvm::Value* AstAdd::codeGen()
{
if ((lhs.getType() == IntegerTyID) && (rhs.getType() == IntegerTyID))
{
return Builder.Create(lhs.codeGen(),rhs.codeGen(),);
}
else if ((lhs.getType() == FloatTyId) && (rhs.getType() == FloatTyId))
{
return Builder.CreateFAdd(lhs.codeGen(),rhs.codeGen(),);
}
else
{
std::cout<<"Type Mismatch!" std::endl;
exit;
}
}

llvm::Value* AstInt::codeGen()
{
return ConstantInt::get(IRContext,APInt(32,val)
}

llvm::Value* AstFloat::codeGen()
{
return ConstantFP::get(IRContext,APFloat(val));
}

llvm::Value* AstBool::codeGen()
{
return ConstantInt::get(IRContext,APInt(1,val));
}

llvm::Value* AstVariableDeclaration::codeGen()
{

};

llvm::Value* AstFunctionDeclaration::codeGen()
{

}

llvm::Value* AstVariableCall::codeGen()
{

}

llvm::Value* AstFunctionCall::codeGen()
{
vector<const Type*> argTypes;
variableList::const_iterator it;
for (it = arguments.begin(); it != arguments.end(); it++)
{
argTypes.push_back((typeof(**it).type));
}
FunctionType *ft = FunctionType::get(typeof(returnType),argTypes,false);
Function* func = Function::Create(ft,Function::ExternalLinkage,functionName,CurModule.get());
context.pushblock(block);
for (it=arguments.begin(); it!= arguments.end(); it++)
{
(**it).codeGen();
}
block.genCode();
context.popblock(block);
return func;
}

llvm::Value* AstBlock::codeGen()
{
statementList::const_iterator it;
Value* genResult;
for (it = statements.begin();it != statements.end(); it++)
{
genResult = (**it).codeGen();
}
return genResult;
}
