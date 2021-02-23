#include "annetaTypes.h"

llvm::IntegerType* AstIntType::typeOf(genContext& context) const 
{
return llvm::Type::getInt32Ty(*(context.IRContext));
} 

llvm::Type* AstStringType::typeOf(genContext& context) const
{
return llvm::Type::getInt8Ty(*context.IRContext)->getPointerTo();
}

llvm::Type* AstCharType::typeOf(genContext& context) const
{
return llvm::Type::getInt8Ty(*context.IRContext);
}

llvm::Type* AstFloatType::typeOf(genContext& context) const
{
return llvm::Type::getFloatTy(*(context.IRContext));
}

llvm::IntegerType* AstBoolType::typeOf(genContext& context) const
{
return llvm::Type::getInt1Ty(*(context.IRContext));
}

llvm::Type* AstPointerType::typeOf(genContext& context) const
{
return referType->typeOf(context)->getPointerTo();
}

void typeTable::createTypeElement(std::string typeName,const AstType* baseType)
{
typeMap.insert(std::make_pair(typeName,std::make_unique<typeElement>(baseType)));
}

typeElement* typeTable::getTypeElement(std::string typeName)
{
return typeMap[typeName].get();
}

const AstType* typeTable::getBaseType(std::string typeName)
{
return typeMap[typeName]->getBaseType();
}

const AstType* typeTable::getExpandTypes(std::string typeName,int depth)
{
return typeMap[typeName]->getExpandTypes(depth);
}

const AstType* typeElement::getBaseType()
{
return typeChain.front();
}

const AstType* typeElement::getExpandTypes(int depth)
{
if (typeChain.size()>depth)
{
return typeChain[depth];
}
else
{
for (int i = typeChain.size()-1; i<depth; i++)
{
typeChain.push_back(new AstPointerType(typeChain[i]));
}
return typeChain.back();
}
}

typeElement::~typeElement()
{
for (std::vector<const AstType*>::iterator it = (typeChain.begin()++); it != typeChain.end(); it++)
{
delete const_cast<AstType*>(*it);
}
}

const AstIntType intType;
const AstStringType stringType;
const AstCharType charType;
const AstBoolType boolType;
const AstFloatType floatType;
