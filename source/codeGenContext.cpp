#include "codeGenContext.h"

genContext::genContext()
{
Builder = llvm::IRBuilder(IRContext);
}

int genContext::pushBlock(AstBlock* block)
{
blockList.push_back(block);
return 0;
}

int genContext::popBlock(AstBlock* block)
{
blockList.popBack(block);
return 0;
}

int genContext::pushVariable(std::string name, llvm::AllocaInst* varPointer)
{
blockList.back().variableMap.insert(std::make_pair(name,varPointer));
}

llvm::AllocaInst* genContext::varLookUp (std::string name)
{
for (std::vector<AstBlock*>::reverse_iterator it = blockList.rbegin(); it != blockList.rend(); ++it)
{
std::map<std::string,llvm::AllocaInst*>::iterator mapIt = it -> variableMap.find(name);
if (mapIt != (it -> variableMap.end()))
{
return *mapIt;
}
else
{
		std::cout << "Couldn't find variable: " name << std::endl;
}
}
}

llvm::CallInst* genContext::functionLookUp (std::string name)
{
return CurModule.getFunction(name);
}


int genContext::printCode()
{
CurModule->print(errs(),nullptr);
}

int genContext::genCode()
{
while (!codeObjects.empty())
{
codeObjects.front() -> codeGen(*this);
codeObjects.pop();
}
return 0;
}

genContext::AstBlock* backBlock()
{
return blockList.back();
}

