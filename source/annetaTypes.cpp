llvm::IntegerType* AstIntType::typeOf(genContext& context) const
{
return llvm::Type::getInt32Ty(*(context.IRContext));
} 

llvm::Type* AstStringType::typeOf(genContext& context) const
{
return llvm::Type::getInt8Ty(*context.IRContext)->getPointerTo();
}

llvm::Type* AstStringType::typeOf(genContext& context) const
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


void typeTable::createTypeElement(std::string typeName,AstType* baseType)
{
typeMap.insert(std::make_pair(typeName,typeElement(baseType)));
}

typeElement* typeTable::getTypeElement(std::string typeName)
{
return typeMap[typeName];
}

const AstType* typeTable::getBaseType(std::string typeName)
{
return typeMap[typeName].getBaseType();
}

const AstType* typeTable::getExpandTypes(std::string typeName,int depth)
{
return typeMap[typeName].getExpandTypes(depth);
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
for (int i = pointerTypes.size()-1; i<depth; i++)
{
typeChain.push_back(new AstPointerType(typeChain[i]));
}
return typeChain.back();
}
}

typeElement::~typeElement()
{
for (std::vector<AstType*>::iterator it = ++typeChain.begin(); it != typeChain.end(); it++)
{
delete *it;
}
}

const AstIntType intType;
const AstStringType stringType;
const AstCharType stringType;
const AstBoolType boolType;
const AstFloatType floatType;
