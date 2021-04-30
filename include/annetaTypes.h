#ifndef annetaTypes
#define annetaTypes

#include <vector>
#include <map>
#include <string>
#include <tuple> 

#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>

#include "codeGenContext.h"

class AstType {
public:
virtual llvm::Type* typeOf(genContext& context) const = 0;
virtual std::string getTypeName() const = 0;
virtual std::tuple<const AstType*,int> getBaseInfo(int depth) const;
virtual ~AstType(){}

};

class AstIntType: public AstType
{
public:
const std::string name= "int";
llvm::IntegerType* typeOf(genContext& context) const ;
std::string getTypeName() const;
};

class AstCharType: public AstType
{
public:
const std::string name= "char";
llvm::Type* typeOf(genContext& context) const;
std::string getTypeName() const;
};

class AstBoolType: public AstType
{
public:
const std::string name= "bool";
llvm::IntegerType* typeOf(genContext& context) const;
std::string getTypeName() const;
};

class AstFloatType: public AstType
{
public:
const std::string name= "float";
llvm::Type* typeOf(genContext& context) const;
std::string getTypeName() const;
};

class AstPointerType: public AstType
{
public:
const AstType* referType;
const std::string name= "pointer";
AstPointerType(const AstType* referType):referType(referType){}
llvm::Type* typeOf(genContext& context) const;
std::tuple<const AstType*,int> getBaseInfo(int depth) const;
std::string getTypeName() const;
};

class typeElement
{
private:
std::vector<const AstType*> typeChain;
int depthToNotDelete = 1;
public: 
typeElement(const AstType* base):typeChain(1,base){}
~typeElement();

int appendPointerType(const AstPointerType* pointerType);

const AstType* getBaseType();
const AstType* getExpandTypes(int depth);
};

class typeTable
{
private:
std::map<std::string,std::unique_ptr<typeElement>> typeMap;

public:
void createTypeElement(const AstType* baseType);
int appendPointerType(std::string baseTypeName,const AstPointerType* pointerType);

typeElement* getTypeElement(std::string typeName);

const AstType* getBaseType(std::string typeName);
const AstType* getExpandTypes(std::string typeName,int depth);

};

extern const AstIntType intType;
extern const AstFloatType floatType;
extern const AstBoolType boolType;
extern const AstCharType charType;

#endif 
