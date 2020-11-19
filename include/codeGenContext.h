#ifndef codeGenContext
#define codeGenContext

#include "llvmHeaders.h"

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <utility>  

class AstNode;
class AstBlock;

class genContext
{
private:
std::vector<AstBlock*> blockList;

public:
std::queue<AstNode*> codeObjects;

llvm::LLVMContext IRContext;
llvm::IRBuilder<> Builder;
std::unique_ptr<llvm::Module> CurModule;

int initContext();

int pushBlock(AstBlock* block); 
int popBlock(AstBlock* block);

int pushVariable(std::string name, llvm::AllocaInst* varPointer);

llvm::AllocaInst* varLookUp (std::string name);
llvm::CallInst* functionLookUp (std::string name);

int printCode();

int genCode();

genContext();

AstBlock* backBlock();
} currentContext;

#endif
