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
struct functionInformation;
struct functionInformation;
struct globalVariableInformation;

typedef std::unique_ptr<AstBlock> AstBlockPtr;
typedef std::unique_ptr<AstNode> AstNodePtr;

class genContext
{
private:
std::vector<AstBlockPtr> blockList;
std::map <std::string,functionInformation> functionMap;
std::map <std::string,globalVariableInformation> globalVariableMap;
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

int pushFunction(std::string name,llvm::Function* function,std::vector<const AstType*> types,const AstType* returnType); 

int pushGlobalVariable(std::string name,llvm::GlobalVariable* var,const AstType* type);

functionInformation functionLookUp(std::string name);
globalVariableInformation globalVariableLookUp(std::string name);

int pushAstNode(AstNode* node);
int pushAstNode(AstNodePtr& node);

variableInformation varLookUp (std::string name);

int codeGen();

int printCode();

genContext(llvm::LLVMContext& context, llvm::IRBuilder<>& builder ):IRContext(context),Builder(builder){}

AstBlockPtr& backBlock();
}; 

extern genContext currentContext;

#endif
