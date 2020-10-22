#include "llvm/IR/LLVMContext.h" 
#include "llvm/IR/IRBuilder.h"
#include <list>
#include <string>
		
static LLVMContext IRContext;
static IRBuilder<> Builder(IRContext);
static std::unique_ptr<Module> CurModule;
std::map <std::string,llvm::AllocaInst*> variableMap;
std::list<std::list<std::string>> variableScope;
std::list<std::string> listOfVariables;
std::map <std::string,int> functionMap;

auto addExp(auto register1, auto register2)
{
if (!float)
{
return Builder.CreateAdd(register1,register2,"addTmp");
}
if (float)
{
return Builder.CreataFAdd(register1,register2, "fAddTmp");
}
}

auto subExp(auto register1, auto register2)
{
if (!float)
{
return Builder.CreateSub(register1,register2,"subTmp");
}
if (float)
{
return Builder.CreateFSub(register1,register2,"fSubTmp");
}
}

auto mulExp(auto register1, auto register2)
{
if (!float)
{
return Builder.CreateMul(register1,register2,"mulTmp");
}
if (float)
{
return Builder.CreateFMul(register1,register2,"fMulTmp");
}
}

auto divExp(auto register1, auto register2)
{
if (!float)
{
return Builder.CreateSDiv(register1,register2,"sDivTmp");
}
if (float)
{
return Builder.CreateFDiv(register1,register2,"fDivTmp");
}
}

auto incExp(auto register1)
{
ConstantData* num;
if (int)
{
num = ConstantInt::get(IRContext,APInt(4,1));
}
if (float)
{
num = ConstantFP::get(IRContext,APFloat(1.0f));
}
 return Builder.CreateAdd(register1,num,"inc");
}

auto decExp(auto register1)
{
ConstantData* num;
if (int)
{
num = ConstantInt::get(IRContext,APInt(4,1));
}
if (float)
{
num = ConstantFP::get(IRContext,APFloat(1.0f));
}return Builder.CreateSub(register1,num,"dec");
}

auto leftSh(auto register1, auto register2)
{
return Builder.CreateShl(register1,register2,"shl");
}

auto rightSh(auto register1, auto register2)
{
return Builder.CreateLShr(register1,register2,"LShr");
}

auto andExp(auto register1,auto register2)
{
return Builder.CreateAnd(register1,register2,"and");
}

auto orExp(auto register1,auto register2)
{
return Builder.CreateXor(register1,register2,"xor");
}

auto xorExp(auto register1, auto register2)
{
return Builder.CreateOr(register1,register2,"or");
}

auto ltCmp(auto register1, auto register2)
{
if (signed)
{
return Builder.CreateICmpSLT(register1,register2,"icmp_slt");
}
if (unsigned)
{
return Builder.CreateICmpULT(register1,register2,"icmp_ult");
}
}

auto gtCmp(auto register1, auto register2)
{
if (signed)
{
return Builder.CreateICmpSGT(register1,register2,"icmp_sgt");
}
if (unsigned)
{
return Builder.CreateICmpUGT(register1,register2,"icmp_ugt");
}
}

auto leqCmp(auto register1, auto register2)
{
if (signed)
{
return Builder.CreateICmpSLE(register1,register2,"icmp_sle");
}
if (unsigned)
{
return Builder.CreateICmpULE(register1,register2,"icmp_ule");
}
}

auto geqCmp(auto register1, auto register2)
{
if (signed)
{
return Builder.CreateICmpSGE(register1,register2,"icmp_sge");
}
if (unsigned)
{
return CreateICmpUGE(register1,register2,"icmp_uge");
}
}

auto eqlCmp(auto register1, auto register2)
{
return Builder.CreateICmpEQ(register1,register2,"icmp_eq");
}

auto neqCmp(auto register1, auto register2)
{
return Builder.CreateICmpNE(register1,register2,"icmp_ne");
}

auto whileLoop()
{

}

auto ifStat()
{

}

auto varDefine()
{
Builder.CreateAlloca(
}

auto varCall()
{

}

auto asgStat()
{

}

auto FuncDecl()
{

}

auto FuncCall()
{

}

}
