#ifndef codegencontext
#define codegencontext

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <utility>  

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>

#include "llvm/Target/TargetMachine.h"
#include "llvm/Support/TargetSelect.h"

#include "llvm/IR/LegacyPassManager.h"

class AstNode;
class AstBlock;
class AstType;
class typeTable;
struct variableInformation;
struct functionInformation;
struct functionInformation;
struct globalVariableInformation;

typedef std::unique_ptr<AstBlock> AstBlockPtr;
typedef std::unique_ptr<AstNode> AstNodePtr;

class genContext
{
private:
std::queue<AstNodePtr> codeObjects;
std::vector<AstBlockPtr> blockList;
std::map <std::string,functionInformation> functionMap;
std::map <std::string,globalVariableInformation> globalVariableMap;
const llvm::Target* target;
llvm::TargetMachine* targetMachine;
llvm::legacy::PassManager pass;

void initLibaryFunctions();

public:
std::unique_ptr<llvm::LLVMContext> IRContext;
std::unique_ptr<llvm::IRBuilder<>> Builder;
std::unique_ptr<llvm::Module> CurModule;

std::unique_ptr<typeTable> types;

void initContext();
void initPrimativeTypes();
void createStart();

void pushBlock(AstBlockPtr& block); 
void pushBlock(AstBlock* blockPtr);
void popBlock();

void pushVariable(std::string name, llvm::AllocaInst* varPointer,const AstType* type);

void pushFunction(std::string name,llvm::Function* function,std::vector<const AstType*> types,const AstType* returnType); 

void pushGlobalVariable(std::string name,llvm::GlobalVariable* var,const AstType* type);

void initializeTarget(const std::string& triple, std::string& errorString,llvm::StringRef CPU);

void initializeTargetList();

void initializeNativeTargetList();

void setStartingFunction(const std::string functionName);

functionInformation functionLookUp(std::string name);
globalVariableInformation globalVariableLookUp(std::string name);

void pushAstNode(AstNode* node);
void pushAstNode(AstNodePtr& node);
AstNode* popAndGetAstNode();

variableInformation varLookUp (std::string name);

void codeGen();

void printBitCode(std::string outputFileName);
void printObjectCode(std::string outputFileName);
void printExeCode(std::string outputFileName);
void printLLVMCode(std::string outputFileName);

AstBlockPtr& backBlock();
}; 

extern genContext currentContext;

#endif
