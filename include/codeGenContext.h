#ifndef codegencontext
#define codegencontext

#include "llvmHeaders.h"

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <utility>  

class AstNode;
class AstBlock;
class AstType;
struct variableInformation;

typedef std::unique_ptr<AstBlock> AstBlockPtr;
typedef std::unique_ptr<AstNode> AstNodePtr;

class genContext
{
private:
std::vector<AstBlockPtr> blockList;
std::queue<AstNodePtr> codeObjects;

public:

llvm::LLVMContext& IRContext;
llvm::IRBuilder<>& Builder;
std::unique_ptr<llvm::Module> CurModule;

int initContext();

int pushBlock(AstBlockPtr& block); 
int pushBlock(AstBlock* blockPtr);
int popBlock();

int pushVariable(std::string name, llvm::AllocaInst* varPointer,const AstType* type);

int pushAstNode(AstNode* node);
int pushAstNode(AstNodePtr& node);

variableInformation varLookUp (std::string name);
llvm::Function* functionLookUp (std::string name);

int codeGen();

int printCode();

genContext(llvm::LLVMContext& context, llvm::IRBuilder<>& builder ):IRContext(context),Builder(builder){}

AstBlockPtr& backBlock();
}; 

extern genContext currentContext;

#endif
