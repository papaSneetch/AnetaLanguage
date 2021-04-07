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
return llvm::Type::getDoubleTy(*(context.IRContext));
}

llvm::IntegerType* AstBoolType::typeOf(genContext& context) const
{
return llvm::Type::getInt1Ty(*(context.IRContext));
}

llvm::Type* AstPointerType::typeOf(genContext& context) const
{
return referType->typeOf(context)->getPointerTo();
}

std::string AstIntType::getTypeName() const
{
return "int";
}

std::string AstStringType::getTypeName() const
{
return "string";
}

std::string AstCharType::getTypeName() const 
{
return "char";
}

std::string AstBoolType::getTypeName() const
{
return "bool";
}

std::string AstFloatType::getTypeName() const
{
return "float";
}

std::string AstPointerType::getTypeName() const
{
return "pointer";
}

void typeTable::createTypeElement(const AstType* baseType)
{
typeMap.insert(std::make_pair(baseType->getTypeName(),std::make_unique<typeElement>(baseType)));
}


int typeTable::appendPointerType(std::string baseTypeName,const AstPointerType* pointerType)
{
return typeMap[baseTypeName]->appendPointerType(pointerType);
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

int typeElement::appendPointerType(const AstPointerType* pointerType)
{
if (typeChain.size()>depthToNotDelete)
{
return 1;
}
else
{
typeChain.push_back(pointerType);
depthToNotDelete++;
return 0;
}
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
for (std::vector<const AstType*>::iterator it = (typeChain.begin()+depthToNotDelete); it < typeChain.end(); it++)
{
delete const_cast<AstType*>(*it);
}
}

const AstIntType intType;
const AstCharType charType;
const AstBoolType boolType;
const AstFloatType floatType;
const AstPointerType stringType(&charType);
