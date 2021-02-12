#include <vector>
#include <map>
#include <string>

#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>

class AstType {
public:
virtual llvm::Type* typeOf(genContext& context) const = 0;
};

class AstIntType: public AstType
{
public:
llvm::IntegerType* typeOf(genContext& context) const;
};

class AstStringType: public AstType
{
public:
llvm::Type* typeOf(genContext& context) const;
};

class AstCharType: public AstType
{
public:
llvm::Type* typeOf(genContext& context) const;
};

class AstBoolType: public AstType
{
public:
llvm::IntegerType* typeOf(genContext& context) const;
};

class AstFloatType: public AstType
{
public:
llvm::Type* typeOf(genContext& context) const;
};

class AstPointerType: public AstType
{
private:
AstType* referType;
public:
AstPointerType(AstType* baseType):baseType(baseType){}
llvm::Type* typeOf(genContext& context) const;
}

extern const AstIntType intType;
extern const AstFloatType floatType;
extern const AstBoolType boolType;
extern const AstStringType stringType;
extern const AstStringType charType;

class typeTable
{
private:
std::map<std::string,typeElement> typeMap;

public:
void createTypeElement(std::string typeName,AstType* baseType);
typeElement* getTypeElement(std::string typeName);

const AstType* getBaseType(std::string typeName);
const AstType* getExpandTypes(std::string typeName,int depth);
}

class typeElement
{
private:
std::vector<const AstType*> typeChain;

public: 
const AstType* getBaseType();
const AstType* getExpandTypes(int depth);

typeElement(AstType* baseType):baseType(baseType){}
~typeElement();
}
