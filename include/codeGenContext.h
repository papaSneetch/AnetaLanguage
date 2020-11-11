#include "llvmHeaders.h"

#include <string>
#include <map>
#include <vector>
#include <queue>

#ifndef codeGenContext
#define codeGenContext

class genContext
{
private:
std::vector<AstBlock*> blockList;
std::map <std::string,llvm::Function*> functionMap;
std::map<std::string,llvm::AllocaInst*> globalVariableMap

public:
std::queue<AstNode*> codeObjects;

LLVMContext IRContext;
IRBuilder<> Builder(IRContext);
std::unique_ptr<Module> CurModule;

int initContext();

int pushFunction(llvm::Function* function);
int pushBlock(AstBlock* block); 
int popBlock(AstBlock* block);

int addVariable(std::string name, llvm::AllocaInst* varPointer);

llvm::AllocaInst* varLookUp (std::string name);
llvm::CallInst* functionLookUp (std::string name);

int printCode();

int genCode();

AstBlock* backBlock();
} currentContext;

#endif
