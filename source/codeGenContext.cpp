#include "codeGenContext.h"
#include "annetaBuilder.h"

int genContext::pushBlock(AstBlockPtr block)
{
blockList.push_back(block);
return 0;
}

int genContext::popBlock(AstBlockPtr block)
{
blockList.pop_back();
return 0;
}

int genContext::pushVariable(std::string name, llvm::AllocaInst* varPointer)
{
blockList.back()->variableMap.insert(std::make_pair(name,varPointer));
return 0;
}

int genContext::pushAstNode(AstNode* node)
{
codeObjects.push(AstNodePtr(node));
return 0;
}

int genContext::pushAstNode(AstNodePtr node)
{
codeObjects.push(node);
return 0;
}

int genContext::varPush(std::string name, llvm::AllocaInst* alloca)
{
blockList.back()->variableMap.insert(std::make_pair(name,alloca));
return 0;
}

llvm::AllocaInst* genContext::varLookUp (std::string name)
{
for (std::vector<AstBlockPtr>::reverse_iterator it = blockList.rbegin(); it != blockList.rend(); ++it)
{
std::map<std::string,llvm::AllocaInst*>::iterator mapIt = (*it) -> variableMap.find(name);
if (mapIt != ((*it) -> variableMap.end()))
{
return mapIt->second;
}
}
std::cout << "Couldn't find variable: ";
std::cout << name << std::endl;
exit(1);
}

llvm::Function* genContext::functionLookUp (std::string name)
{
return CurModule->getFunction(name);
}


int genContext::printCode()
{
CurModule->print(llvm::errs(),nullptr);
return 0;
}

int genContext::codeGen()
{
while (!codeObjects.empty())
{
codeObjects.front() -> codeGen(*this);
codeObjects.pop();
}
return 0;
}

AstBlockPtr genContext::backBlock()
{
return blockList.back();
}

namespace genContextNamespace
{
llvm::LLVMContext context;
llvm::IRBuilder<> builder(context);
}

genContext currentContext(genContextNamespace::context,genContextNamespace::builder);

