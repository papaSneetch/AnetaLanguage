#include "llvmExternalFunction.h"

void createExternFunc(genContext& context)
{
createPuts(context);
}
void createPuts(genContext& context)
{
const AstType* returnType = intType;
std::vector<const AstType*> types{stringType};
std::vector<llvm::Type*> argTypes{types[0]->typeOf(context)};

llvm::FunctionType *ft = llvm::FunctionType::get(returnType->typeOf(context),argTypes,false);
llvm::Function* func = llvm::Function::Create(ft,llvm::Function::ExternalLinkage,"puts",context.CurModule.get());

context.pushFunction("puts",func,types,returnType);

std::cerr << "Generated: " << functionName->name << std::endl;
return func;
}
