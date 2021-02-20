#include <vector>
#include <map>
#include <string>

#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>


#ifndef annetaTypes
#define annetaTypes

class AstType {
public:
virtual llvm::Type* typeOf(const genContext& context) = 0;
virtual ~AstType() = 0;
};

class AstIntType: public AstType
{
public:
const std::string name= "int";
llvm::IntegerType* typeOf(const genContext& context) ;
};

class AstStringType: public AstType
{
public:
const std::string name= "string";
llvm::Type* typeOf(const genContext& context);
};

class AstCharType: public AstType
{
public:
const std::string name= "char";
llvm::Type* typeOf(const genContext& context);
};

class AstBoolType: public AstType
{
public:
const std::string name= "bool";
llvm::IntegerType* typeOf(const genContext& context);
};

class AstFloatType: public AstType
{
public:
const std::string name= "float";
llvm::Type* typeOf(const genContext& context);
};

class AstPointerType: public AstType
{
public:
const AstType* referType;
const std::string name= "pointer";
AstPointerType(const AstType* referType):referType(referType){}
llvm::Type* typeOf(const genContext& context);
};

extern const AstIntType intType;
extern const AstFloatType floatType;
extern const AstBoolType boolType;
extern const AstStringType stringType;
extern const AstStringType charType;

#endif 
