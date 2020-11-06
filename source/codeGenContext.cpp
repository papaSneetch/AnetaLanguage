#include "codeGenContext.h"

int genContext::pushFunction(llvm::Function* function)
{
functionMap.push_back(function);
return 0;
}

int genContext::pushBlock(AstBlock* block)
{
blockList.push_back(block);
return 0;
}

int genContext::popBlock(AstBlock* block)
{
blockList.popBack(block);
}



