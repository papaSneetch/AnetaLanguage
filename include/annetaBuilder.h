#ifndef annetaBuilder
#define annetaBuilder

#include "llvmHeaders.h"
#include "codeGenContext.h"

#include <vector>
#include <utility>
#include <map>
#include <string>
#include <memory>

class AstType;
class AstExp;
class AstVariableDeclaration;
class AstGlobalVariableDeclaration;
class AstArrayDeclaration;
class AstStat;
class AstType;
class AstName;
class AstConstant;
class AstIntValue;

typedef std::unique_ptr<AstNode> AstNodePtr;
typedef std::unique_ptr<AstStat> AstStatPtr;
typedef std::unique_ptr<AstType> AstTypePtr;
typedef std::unique_ptr<AstExp> AstExpPtr;
typedef std::unique_ptr<AstBlock> AstBlockPtr;
typedef std::unique_ptr<AstName> AstNamePtr;
typedef std::unique_ptr<AstConstant> AstConstantPtr;
typedef std::unique_ptr<AstIntValue> AstIntValuePtr;
typedef std::unique_ptr<AstVariableDeclaration> AstVariableDeclarationPtr;
typedef std::unique_ptr<AstGlobalVariableDeclaration> AstGlobalVariableDeclarationPtr;
typedef std::unique_ptr<AstArrayDeclaration> AstArrayDeclarationPtr;

typedef std::vector<AstExpPtr> expressionList;
typedef std::vector<AstVariableDeclaration> variableList;
typedef std::vector<AstStatPtr> statementList; 

typedef std::unique_ptr<statementList> statementListPtr;
typedef std::unique_ptr<expressionList> expressionListPtr;
typedef std::unique_ptr<variableList> variableListPtr;

struct variableInformation;
struct functionInformation;
struct globalVariableInformation;


class AstNode
{
public:
virtual ~AstNode() = default;
virtual llvm::Value* codeGen(genContext& context) = 0;
};

class AstType {
public:
virtual llvm::Type* typeOf(genContext& context) const = 0;
};

struct variableInformation
{
llvm::AllocaInst* alloca;
const AstType* type;
};

class AstStat: public AstNode {
public:
virtual ~AstStat() = default;
};

class AstExp: public AstNode {
public:
const AstType* type;
AstExp(const AstType& type):type(&type){}
AstExp(const AstType* typeTmp):type(typeTmp){}
AstExp() = default;
virtual llvm::Value* codeGen(genContext& context) {return nullptr;}
};

class AstCall : public AstExp
{
};

class AstName: public AstNode {
public:
std::string name;
AstName(const std::string& nameTmp):name(nameTmp) {}
llvm::Value* codeGen(genContext& context);
};

class AstBlock: public AstNode {
public:
statementListPtr statements;
std::map<std::string,variableInformation> variableMap;
AstBlock(statementListPtr& statements): statements(std::move(statements)){}
AstBlock(statementList* statements): statements(statementListPtr(statements)){}
llvm::Value* codeGen(genContext& context);
llvm::Value* codeGenInFunction(genContext& context);
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


extern const AstIntType intType;
extern const AstFloatType floatType;
extern const AstBoolType boolType;
extern const AstStringType stringType;

class AstConstant: public AstExp
{
public:
virtual ~AstConstant() = default;
AstConstant(const AstType& type):AstExp(type){}
AstConstant(const AstType* type):AstExp(type){}
virtual llvm::Constant* codeGen(genContext& context) = 0;
};

class AstIntValue: public AstConstant
{
public:
int value;
AstIntValue(int value):AstConstant(intType),value(value){}
llvm::Constant* codeGen(genContext& context);
};

class AstStringValue: public AstConstant
{
public:
std::string value;
AstStringValue(std::string value):AstConstant(stringType),value(value){}
llvm::Constant* codeGen(genContext& context);
};

class AstBoolValue: public AstConstant
{
public:
bool value;
AstBoolValue(bool value):AstConstant(boolType),value(value){}
llvm::Constant* codeGen(genContext& context);
};

class AstFloatValue: public AstConstant
{
public:
float value;
AstFloatValue(float value):AstConstant(floatType),value(value){}
llvm::Constant* codeGen(genContext& context);
};
 
class AstIfElseStat: public AstStat
{
public:
AstBlockPtr ifBlock;
AstBlockPtr elseBlock;
AstExpPtr testCondition;
AstIfElseStat( AstBlockPtr& ifBlock, AstExpPtr& testCondition, AstBlockPtr& elseBlock):ifBlock(std::move(ifBlock)),elseBlock(std::move(elseBlock)),testCondition(std::move(testCondition)){}
AstIfElseStat( AstBlockPtr& ifBlock, AstExpPtr& testCondition):ifBlock(std::move(ifBlock)),testCondition(std::move(testCondition)){}
AstIfElseStat( AstBlock* ifBlock, AstExp* testCondition, AstBlock* elseBlock = nullptr):ifBlock(ifBlock),elseBlock(elseBlock),testCondition(testCondition){}
llvm::Value* codeGen(genContext& context);
};

class AstWhileLoop: public AstStat
{
public:
AstBlockPtr whileBlock;
AstExpPtr testCondition;
AstWhileLoop( AstBlockPtr& whileBlock, AstExpPtr& testCondition):whileBlock(std::move(whileBlock)),testCondition(std::move(testCondition)){}
AstWhileLoop( AstBlock* whileBlockTmp, AstExp* testConditionTmp){
whileBlock.reset(whileBlockTmp);
testCondition.reset(testConditionTmp);
}
llvm::Value* codeGen(genContext& context);
};

class AstBinOp: public AstExp
{
protected:
int updateType();
public:
virtual ~AstBinOp() = default;
AstExpPtr lhs;
AstExpPtr rhs;
AstBinOp(AstExp* lhsPtr, AstExp* rhsPtr);
AstBinOp(AstExpPtr& lhs, AstExpPtr& rhs);
virtual llvm::Value* codeGen(genContext& context) = 0;
};

class AstOr: public AstBinOp
{
public:
AstOr(AstExpPtr& lhs, AstExpPtr& rhs):AstBinOp(lhs,rhs){} 
AstOr(AstExp* lhs, AstExp* rhs):AstBinOp(lhs,rhs){} 
llvm::Value* codeGen(genContext& context);
};

class AstXor: public AstBinOp
{
public:
AstXor( AstExpPtr& lhs, AstExpPtr& rhs):AstBinOp(lhs,rhs){}
AstXor( AstExp* lhs, AstExp* rhs):AstBinOp(lhs,rhs){} 

llvm::Value* codeGen(genContext& context);
};

class AstAnd: public AstBinOp
{
public:
AstAnd ( AstExpPtr& lhs, AstExpPtr& rhs):AstBinOp(lhs,rhs){}
AstAnd( AstExp* lhs, AstExp* rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstEql: public AstBinOp
{
public:
AstEql ( AstExpPtr& lhs, AstExpPtr& rhs):AstBinOp(lhs,rhs){}
AstEql( AstExp* lhs, AstExp* rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class Astleq: public AstBinOp
{
public:
Astleq ( AstExpPtr& lhs, AstExpPtr& rhs):AstBinOp(lhs,rhs){}
Astleq( AstExp* lhs, AstExp* rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class Astgeq: public AstBinOp
{
public:
Astgeq( AstExpPtr& lhs, AstExpPtr& rhs):AstBinOp(lhs,rhs){}
Astgeq( AstExp* lhs, AstExp* rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class Astlt: public AstBinOp
{
public:
Astlt( AstExpPtr& lhs,  AstExpPtr& rhs):AstBinOp(lhs,rhs){}
Astlt( AstExp* lhs, AstExp* rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class Astgt: public AstBinOp
{
public:
Astgt( AstExpPtr& lhs, AstExpPtr& rhs):AstBinOp(lhs,rhs){}
Astgt( AstExp* lhs, AstExp* rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class Astneq: public AstBinOp
{
public:
Astneq( AstExpPtr& lhs,  AstExpPtr& rhs):AstBinOp(lhs,rhs){}
Astneq( AstExp* lhs, AstExp* rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstLeftSh: public AstBinOp
{
public:
AstLeftSh( AstExpPtr& lhs, AstExpPtr& rhs):AstBinOp(lhs,rhs){}
AstLeftSh( AstExp* lhs, AstExp* rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstRightSh: public AstBinOp
{
public:
AstRightSh( AstExpPtr& lhs, AstExpPtr& rhs):AstBinOp(lhs,rhs){}
AstRightSh( AstExp* lhs, AstExp* rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstAdd: public AstBinOp
{
public:
AstAdd( AstExpPtr& lhs, AstExpPtr& rhs):AstBinOp(lhs,rhs){}
AstAdd( AstExp* lhs, AstExp* rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstSub: public AstBinOp
{
public:
AstSub( AstExpPtr& lhs, AstExpPtr& rhs):AstBinOp(lhs,rhs){}
AstSub( AstExp* lhs, AstExp* rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstMul: public AstBinOp
{
public:
AstMul( AstExpPtr& lhs, AstExpPtr& rhs):AstBinOp(lhs,rhs){}
AstMul( AstExp* lhs, AstExp* rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstDiv: public AstBinOp
{
public:
AstDiv( AstExpPtr& lhs, AstExpPtr& rhs):AstBinOp(lhs,rhs){}
AstDiv( AstExp* lhs, AstExp* rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstMod: public AstBinOp
{
public:
AstMod( AstExpPtr& lhs, AstExpPtr& rhs):AstBinOp(lhs,rhs){}
AstMod( AstExp* lhs, AstExp* rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstAsg: public AstExp
{
public:
AstNamePtr variableName;
expressionListPtr rhs;
AstAsg(AstNamePtr& variableNameTmp, AstExpPtr& rhsTmp);
AstAsg(AstName* variableNameTmp, AstExp* rhsTmp);
AstAsg(AstNamePtr& variableNameTmp, expressionListPtr& arrayValuesTmp);
AstAsg(AstName* variableNameTmp, expressionList* arrayValuesTmp);
virtual llvm::Value* codeGen(genContext& context) = 0;
};

class AstVariableAsg: public AstAsg
{
public:
AstVariableAsg(AstNamePtr& variableName, AstExpPtr& rhs):AstAsg(variableName,rhs){}
AstVariableAsg( AstName* variableName, AstExp* rhs):AstAsg(variableName,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstArrayAsg: public AstAsg
{
public:
AstNamePtr variableName;
AstExpPtr rhs;
AstExpPtr index;
AstArrayAsg(AstNamePtr& variableName, AstExpPtr& rhs, AstExpPtr& index):AstAsg(variableName,rhs),index(std::move(index)){}
AstArrayAsg(AstName* variableNameTmp, AstExp* rhsTmp, AstExp* indexTmp):AstAsg(variableNameTmp,rhsTmp)
{
index.reset(indexTmp);
}
llvm::Value* codeGen(genContext& context);
};

class AstArrayListAsg: public AstAsg
{
public:
expressionListPtr arrayValues;
AstNamePtr variableName;
AstExpPtr index;
AstArrayListAsg(AstNamePtr& name, expressionListPtr& arrayValues, AstExpPtr& index):AstAsg(name,arrayValues),index(std::move(index)){}
AstArrayListAsg(AstNamePtr& name, expressionListPtr& arrayValues):AstAsg(name,arrayValues){
index.reset(new AstIntValue(0));
}
AstArrayListAsg(AstName* nameTmp, expressionList* arrayValuesTmp,AstExp* indexTmp = new AstIntValue(0)):AstAsg(nameTmp,arrayValuesTmp) {
index.reset(indexTmp);
}
llvm::Value* codeGen(genContext& context);
};

class AstAeg: public AstBinOp
{
public:
AstAeg( AstExpPtr& lhs, AstExpPtr& rhs):AstBinOp(lhs,rhs){}
AstAeg( AstExp* lhs, AstExp* rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstMeg: public AstBinOp
{
public:
AstMeg ( AstExpPtr& lhs, AstExpPtr& rhs):AstBinOp(lhs,rhs){}
AstMeg( AstExp* lhs, AstExp* rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstUnaryOp: public AstExp
{
public:
AstExpPtr expNode;
AstUnaryOp(AstExpPtr& expNodePtr):AstExp(expNodePtr->type),expNode(std::move(expNodePtr)){}
AstUnaryOp(AstExp* expNodePtr):AstExp(expNodePtr->type) {expNode.reset(expNodePtr);} 
virtual llvm::Value* codeGen(genContext& context);
};

class AstFunctionDeclaration : public AstStat //Note: The function declaration needs to ensure the vector memory contigentcy remains. Functions can only be defined once. May need to investigate std::move.
{
public:
variableListPtr arguments;
const AstType* returnType;
AstNamePtr functionName;
AstBlockPtr block;
AstFunctionDeclaration(const AstType& returnType, AstNamePtr& functionName, AstBlockPtr& block, variableListPtr& arguments): arguments(std::move(arguments)),returnType(&returnType), functionName(std::move(functionName)),block(std::move(block)){}
AstFunctionDeclaration(const AstType* returnTypeTmp, AstName* functionNameTmp, AstBlock* blockTmp, variableList* argumentsTmp):returnType(returnTypeTmp) {
arguments.reset(argumentsTmp);
functionName.reset(functionNameTmp);
block.reset(blockTmp);
}
llvm::Value* codeGen(genContext& context);
};

class AstReturnStat: public AstStat
{
public:
AstExpPtr returnValue;
AstReturnStat(AstExpPtr& returnValue):returnValue(std::move(returnValue)){}
AstReturnStat( AstExp* returnValueTmp) {
returnValue.reset(returnValueTmp);
}
llvm::Value* codeGen(genContext& context);
};

class AstVariableDeclaration : public AstStat
{
public:
AstNamePtr variableName;
const AstType* variableType;
AstExpPtr initializer; 
AstVariableDeclaration( AstNamePtr& variableName,const AstType& variableType): variableName(std::move(variableName)), variableType(&variableType) {}
AstVariableDeclaration( AstNamePtr& variableName,const AstType& variableType, AstExpPtr& initializer):variableName(std::move(variableName)),variableType(&variableType),initializer(std::move(initializer))  {}
AstVariableDeclaration( AstName* variableNameTmp,const AstType* variableTypeTmp):variableType(variableTypeTmp) { 
variableName.reset(variableNameTmp);
} 
AstVariableDeclaration( AstName* variableNameTmp,const AstType* variableTypeTmp, AstExp* initializerTmp):variableType(variableTypeTmp) {
variableName.reset(variableNameTmp);
initializer.reset(initializerTmp);
}

llvm::AllocaInst* codeGen(genContext& context);
};

class AstGlobalVariableDeclaration : public AstStat
{
public:
AstNamePtr variableName;
const AstType* variableType;
AstConstantPtr initializer;
AstGlobalVariableDeclaration( AstNamePtr& variableName, const AstType& variableType): variableName(std::move(variableName)), variableType(&variableType) {}
AstGlobalVariableDeclaration( AstName* variableNameTmp, const AstType* variableTypeTmp):variableType(variableTypeTmp) {
variableName.reset(variableNameTmp);
}
AstGlobalVariableDeclaration( AstNamePtr& variableName, const AstType& variableType, AstConstantPtr& initializer): variableName(std::move(variableName)), variableType(&variableType),initializer(std::move(initializer)) {}
AstGlobalVariableDeclaration( AstName* variableNameTmp, const AstType* variableTypeTmp,AstConstant* initializerTmp):variableType(variableTypeTmp) {
variableName.reset(variableNameTmp);
initializer.reset(initializerTmp);
}
llvm::Value* codeGen(genContext& context);
};

class AstArrayDeclaration : public AstStat
{
public:
AstNamePtr variableName;
const AstType* variableType;
AstIntValuePtr arraySize;
expressionListPtr initializer; 
AstArrayDeclaration( AstNamePtr& variableName,const AstType& variableType,  AstIntValuePtr& arraySize): variableName(std::move(variableName)), variableType(&variableType), arraySize(std::move(arraySize)) {}
AstArrayDeclaration(AstName* variableNameTmp, const AstType* variableTypeTmp, AstIntValue* arraySizeTmp):variableType(variableTypeTmp) {
variableName.reset(variableNameTmp);
arraySize.reset(arraySizeTmp);
}
llvm::Value* codeGen(genContext& context);
};

class AstExpStat: public AstStat
{
public:
AstExpPtr expression;
AstExpStat(AstExpPtr& expression):expression(std::move(expression)){}
AstExpStat(AstExp* expressionTmp) {
expression.reset(expressionTmp);
}
llvm::Value* codeGen(genContext& context);
};

class AstFunctionCall: public AstCall
{
public:
AstNamePtr functionName;
expressionListPtr args;
AstFunctionCall( AstNamePtr& functionName, expressionListPtr& args): functionName(std::move(functionName)),args(std::move(args)) {}
AstFunctionCall( AstName* functionNameTmp, expressionList* argsTmp) {
functionName.reset(functionNameTmp);
args.reset(argsTmp);
}
llvm::Value* codeGen(genContext& context);
};

class AstVariableCall: public AstCall
{
public:
AstNamePtr variableName;
AstVariableCall( AstNamePtr& variableNameTmp):variableName(std::move(variableNameTmp)) {}
AstVariableCall( AstName* variableNameTmp) {
variableName.reset(variableNameTmp);
}
llvm::Value* codeGen(genContext& context);
};

class AstArrayCall : public AstCall
{
public:
AstNamePtr variableName;
AstExpPtr index;
llvm::AllocaInst* alloca;
AstArrayCall( AstNamePtr& variableName, AstExpPtr& index):variableName(std::move(variableName)), index(std::move(index)) {}
AstArrayCall( AstName* variableNameTmp, AstExp* indexTmp) {
variableName.reset(variableNameTmp);
index.reset(indexTmp);
}
llvm::Value* codeGen(genContext& context);
};

struct functionInformation
{
llvm::Function* function;
std::vector<const AstType*> argumentTypes;
const AstType* returnType;
};

struct globalVariableInformation
{
llvm::GlobalVariable* globalVariablePtr;
const AstType* type;
};

#endif 
