#include "annetaTypes.h"

llvm::IntegerType* AstIntType::typeOf(const genContext& context) 
{
return llvm::Type::getInt32Ty(*(context.IRContext));
} 

llvm::Type* AstStringType::typeOf(const genContext& context)
{
return llvm::Type::getInt8Ty(*context.IRContext)->getPointerTo();
}

llvm::Type* AstCharType::typeOf(const genContext& context) 
{
return llvm::Type::getInt8Ty(*context.IRContext);
}

llvm::Type* AstFloatType::typeOf(const genContext& context) 
{
return llvm::Type::getFloatTy(*(context.IRContext));
}

llvm::IntegerType* AstBoolType::typeOf(const genContext& context) 
{
return llvm::Type::getInt1Ty(*(context.IRContext));
}


llvm::Type* AstPointerType::typeOf(const genContext& context)
{
return referType->typeOf(context)->getPointerTo();
}

const AstIntType intType;
const AstStringType stringType;
const AstCharType stringType;
const AstBoolType boolType;
const AstFloatType floatType;
