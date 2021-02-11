#ifndef annetaBuilder
#define annetaBuilder

#include "llvmHeaders.h"
#include "codeGenContext.h"

#include <vector>
#include <map>
#include <string>
#include <memory>

class AstExp;
class AstVariableDeclaration;
class AstGlobalVariableDeclaration;
class AstArrayDeclaration;
class AstStat;
class AstType;
class AstName;
class AstConstant;
class AstIntValue;

typedef std::shared_ptr<AstNode> AstNodePtr;
typedef std::shared_ptr<AstStat> AstStatPtr;
typedef std::shared_ptr<AstType> AstTypePtr;
typedef std::shared_ptr<AstExp> AstExpPtr;
typedef std::shared_ptr<AstBlock> AstBlockPtr;
typedef std::shared_ptr<AstName> AstNamePtr;
typedef std::shared_ptr<AstConstant> AstConstantPtr;
typedef std::shared_ptr<AstIntValue> AstIntValuePtr;
typedef std::shared_ptr<AstVariableDeclaration> AstVariableDeclarationPtr;
typedef std::shared_ptr<AstGlobalVariableDeclaration> AstGlobalVariableDeclarationPtr;
typedef std::shared_ptr<AstArrayDeclaration> AstArrayDeclarationPtr;


typedef std::vector<AstExpPtr> expressionList;
typedef std::vector<AstVariableDeclaration> variableList;
typedef std::vector<AstStatPtr> statementList; 

typedef std::shared_ptr<statementList> statementListPtr;
typedef std::shared_ptr<expressionList> expressionListPtr;
typedef std::shared_ptr<variableList> variableListPtr;


class AstNode
{
public:
virtual ~AstNode() = default;
virtual llvm::Value* codeGen(genContext& context) = 0;
};

class AstType {
public:
virtual ~AstType() = default;
virtual llvm::Type* typeOf(genContext& context) {return nullptr;}
};

class AstStat: public AstNode {
public:
virtual ~AstStat() = default;
};

class AstExp: public AstNode {
public:
AstTypePtr type;
AstExp(AstTypePtr type):type(type){}
AstExp(AstType* type):type(AstTypePtr(type)){}
AstExp() = default;
virtual llvm::Value* codeGen(genContext& context) {return nullptr;}
};

class AstCall : public AstExp
{
};

class AstName: public AstNode {
public:
std::string name;
AstName(std::string name):name(name){}
llvm::Value* codeGen(genContext& context);
};

class AstBlock: public AstNode {
public:
statementListPtr statements;
std::map<std::string,llvm::AllocaInst*> variableMap;
AstBlock() {};
AstBlock(statementListPtr statements = std::make_shared<statementList>()): statements(statements){}
AstBlock(statementList* statements): statements(statementListPtr(statements)){}
llvm::Value* codeGen(genContext& context);
};

class AstIntType: public AstType
{
public:
llvm::Type* typeOf(genContext& context);
};
extern AstIntType intType;

class AstStringType: public AstType
{
public:
llvm::Type* typeOf(genContext& context);
};
extern AstStringType stringType;

class AstBoolType: public AstType
{
public:
llvm::Type* typeOf(genContext& context);
};
extern AstBoolType boolType;

class AstFloatType: public AstType
{
public:
llvm::Type* typeOf(genContext& context);
};
extern AstFloatType floatType;

class AstConstant: public AstExp
{
public:
virtual ~AstConstant() = default;
AstConstant(AstTypePtr type):AstExp(type){}
AstConstant(AstType* type):AstExp(AstTypePtr(type)){}
virtual llvm::Constant* codeGen(genContext& context) = 0;
};

class AstIntValue: public AstConstant
{
public:
int value;
AstIntValue(int value):AstConstant(std::make_shared<AstType>(intType)),value(value){}
llvm::Constant* codeGen(genContext& context);
};

class AstStringValue: public AstConstant
{
public:
std::string value;
AstStringValue(std::string value):AstConstant(std::make_shared<AstType>(stringType)),value(value){}
llvm::Constant* codeGen(genContext& context);
};

class AstBoolValue: public AstConstant
{
public:
bool value;
AstBoolValue(bool value):AstConstant(std::make_shared<AstType>(boolType)),value(value){}
llvm::Constant* codeGen(genContext& context);
};

class AstFloatValue: public AstConstant
{
public:
float value;
AstFloatValue(float value):AstConstant(std::make_shared<AstType>(floatType)),value(value){}
llvm::Constant* codeGen(genContext& context);
};
 
class AstIfElseStat: public AstStat
{
public:
AstBlockPtr ifBlock;
AstBlockPtr elseBlock;
AstExpPtr testCondition;
AstIfElseStat(AstBlockPtr ifBlock, AstExpPtr testCondition,AstBlockPtr elseBlock = AstBlockPtr(nullptr)):ifBlock(ifBlock),elseBlock(elseBlock),testCondition(testCondition){}
AstIfElseStat(AstBlock* ifBlock, AstExp* testCondition,AstBlock* elseBlock = nullptr):ifBlock(AstBlockPtr(ifBlock)),elseBlock(AstBlockPtr(elseBlock)),testCondition(AstExpPtr(testCondition)){}
llvm::Value* codeGen(genContext& context);
};

class AstWhileLoop: public AstStat
{
public:
AstBlockPtr whileBlock;
AstExpPtr testCondition;
AstWhileLoop(AstBlockPtr whileBlock,AstExpPtr testCondition):whileBlock(whileBlock),testCondition(testCondition){}
AstWhileLoop(AstBlock* whileBlock,AstExp* testCondition):whileBlock(AstBlockPtr(whileBlock)),testCondition(AstExpPtr(testCondition)){}
llvm::Value* codeGen(genContext& context);
};

class AstBinOp: public AstExp
{
public:
virtual ~AstBinOp() = default;
AstExpPtr lhs;
AstExpPtr rhs;
AstBinOp(AstExpPtr lhs, AstExpPtr rhs);
virtual llvm::Value* codeGen(genContext& context) = 0;
};

class AstOr: public AstBinOp
{
public:
AstOr(AstExpPtr lhs, AstExpPtr rhs):AstBinOp(lhs,rhs){} 
AstOr(AstExp* lhs, AstExp* rhs):AstBinOp(AstExpPtr(lhs),AstExpPtr(rhs)){} 
llvm::Value* codeGen(genContext& context);
};

class AstXor: public AstBinOp
{
public:
AstXor(AstExpPtr lhs, AstExpPtr rhs):AstBinOp(lhs,rhs){}
AstXor(AstExp* lhs, AstExp* rhs):AstBinOp(AstExpPtr(lhs),AstExpPtr(rhs)){} 

llvm::Value* codeGen(genContext& context);
};

class AstAnd: public AstBinOp
{
public:
AstAnd (AstExpPtr lhs, AstExpPtr rhs):AstBinOp(lhs,rhs){}
AstAnd(AstExp* lhs, AstExp* rhs):AstBinOp(AstExpPtr(lhs),AstExpPtr(rhs)){}
llvm::Value* codeGen(genContext& context);
};

class AstEql: public AstBinOp
{
public:
AstEql (AstExpPtr lhs, AstExpPtr rhs):AstBinOp(lhs,rhs){}
AstEql(AstExp* lhs, AstExp* rhs):AstBinOp(AstExpPtr(lhs),AstExpPtr(rhs)){}
llvm::Value* codeGen(genContext& context);
};

class Astleq: public AstBinOp
{
public:
Astleq (AstExpPtr lhs, AstExpPtr rhs):AstBinOp(lhs,rhs){}
Astleq(AstExp* lhs, AstExp* rhs):AstBinOp(AstExpPtr(lhs),AstExpPtr(rhs)){}
llvm::Value* codeGen(genContext& context);
};

class Astgeq: public AstBinOp
{
public:
Astgeq(AstExpPtr lhs, AstExpPtr rhs):AstBinOp(lhs,rhs){}
Astgeq(AstExp* lhs, AstExp* rhs):AstBinOp(AstExpPtr(lhs),AstExpPtr(rhs)){}
llvm::Value* codeGen(genContext& context);
};

class Astlt: public AstBinOp
{
public:
Astlt(AstExpPtr lhs, AstExpPtr rhs):AstBinOp(lhs,rhs){}
Astlt(AstExp* lhs, AstExp* rhs):AstBinOp(AstExpPtr(lhs),AstExpPtr(rhs)){}
llvm::Value* codeGen(genContext& context);
};

class Astgt: public AstBinOp
{
public:
Astgt(AstExpPtr lhs, AstExpPtr rhs):AstBinOp(lhs,rhs){}
Astgt(AstExp* lhs, AstExp* rhs):AstBinOp(AstExpPtr(lhs),AstExpPtr(rhs)){}
llvm::Value* codeGen(genContext& context);
};

class Astneq: public AstBinOp
{
public:
Astneq(AstExpPtr lhs, AstExpPtr rhs):AstBinOp(lhs,rhs){}
Astneq(AstExp* lhs, AstExp* rhs):AstBinOp(AstExpPtr(lhs),AstExpPtr(rhs)){}
llvm::Value* codeGen(genContext& context);
};

class AstLeftSh: public AstBinOp
{
public:
AstLeftSh(AstExpPtr lhs, AstExpPtr rhs):AstBinOp(lhs,rhs){}
AstLeftSh(AstExp* lhs, AstExp* rhs):AstBinOp(AstExpPtr(lhs),AstExpPtr(rhs)){}
llvm::Value* codeGen(genContext& context);
};

class AstRightSh: public AstBinOp
{
public:
AstRightSh(AstExpPtr lhs, AstExpPtr rhs):AstBinOp(lhs,rhs){}
AstRightSh(AstExp* lhs, AstExp* rhs):AstBinOp(AstExpPtr(lhs),AstExpPtr(rhs)){}
llvm::Value* codeGen(genContext& context);
};

class AstAdd: public AstBinOp
{
public:
AstAdd(AstExpPtr lhs, AstExpPtr rhs):AstBinOp(lhs,rhs){}
AstAdd(AstExp* lhs, AstExp* rhs):AstBinOp(AstExpPtr(lhs),AstExpPtr(rhs)){}
llvm::Value* codeGen(genContext& context);
};

class AstSub: public AstBinOp
{
public:
AstSub(AstExpPtr lhs, AstExpPtr rhs):AstBinOp(lhs,rhs){}
AstSub(AstExp* lhs, AstExp* rhs):AstBinOp(AstExpPtr(lhs),AstExpPtr(rhs)){}
llvm::Value* codeGen(genContext& context);
};

class AstMul: public AstBinOp
{
public:
AstMul(AstExpPtr lhs, AstExpPtr rhs):AstBinOp(lhs,rhs){}
AstMul(AstExp* lhs, AstExp* rhs):AstBinOp(AstExpPtr(lhs),AstExpPtr(rhs)){}
llvm::Value* codeGen(genContext& context);
};

class AstDiv: public AstBinOp
{
public:
AstDiv(AstExpPtr lhs, AstExpPtr rhs):AstBinOp(lhs,rhs){}
AstDiv(AstExp* lhs, AstExp* rhs):AstBinOp(AstExpPtr(lhs),AstExpPtr(rhs)){}
llvm::Value* codeGen(genContext& context);
};

class AstMod: public AstBinOp
{
public:
AstMod(AstExpPtr lhs, AstExpPtr rhs):AstBinOp(lhs,rhs){}
AstMod(AstExp* lhs, AstExp* rhs):AstBinOp(AstExpPtr(lhs),AstExpPtr(rhs)){}
llvm::Value* codeGen(genContext& context);
};

class AstAsg: public AstExp
{
public:
virtual llvm::Value* codeGen(genContext& context) = 0;
};

class AstVariableAsg: public AstAsg
{
public:
AstNamePtr variableName;
AstExpPtr rhs;
AstVariableAsg(AstNamePtr variableName, AstExpPtr rhs):variableName(variableName),rhs(rhs){}
AstVariableAsg(AstName* variableName, AstExp* rhs):variableName(AstNamePtr(variableName)),rhs(AstExpPtr(rhs)){}
llvm::Value* codeGen(genContext& context);
};

class AstArrayAsg: public AstAsg
{
public:
AstNamePtr variableName;
AstExpPtr rhs;
AstExpPtr index;
AstArrayAsg(AstNamePtr variableName, AstExpPtr rhs, AstExpPtr index):variableName(variableName),rhs(rhs),index(index){}
AstArrayAsg(AstName* variableName, AstExp* rhs, AstExp* index):variableName(AstNamePtr(variableName)),rhs(AstExpPtr(rhs)),index(AstExpPtr(index)){}
llvm::Value* codeGen(genContext& context);
};

class AstArrayListAsg: public AstAsg
{
public:
expressionListPtr arrayValues;
AstNamePtr variableName;
AstExpPtr index;
AstArrayListAsg(AstNamePtr name,expressionListPtr arrayValues,AstExpPtr index = std::make_shared<AstExp>(AstIntValue(0))):arrayValues(arrayValues),variableName(name),index(index){}
AstArrayListAsg(AstName* name,expressionList* arrayValues,AstExp* index = new AstIntValue(0)):arrayValues(expressionListPtr(arrayValues)),variableName(AstNamePtr(name)),index(AstExpPtr(index)){}
llvm::Value* codeGen(genContext& context);
};

class AstAeg: public AstBinOp
{
public:
AstAeg(AstExpPtr lhs, AstExpPtr rhs):AstBinOp(lhs,rhs){}
AstAeg(AstExp* lhs, AstExp* rhs):AstBinOp(AstExpPtr(lhs),AstExpPtr(rhs)){}

llvm::Value* codeGen(genContext& context);
};

class AstMeg: public AstBinOp
{
public:
AstMeg (AstExpPtr lhs, AstExpPtr rhs):AstBinOp(lhs,rhs){}
AstMeg(AstExp* lhs, AstExp* rhs):AstBinOp(AstExpPtr(lhs),AstExpPtr(rhs)){}
llvm::Value* codeGen(genContext& context);
};

class AstUnaryOp: public AstExp
{
public:
AstExpPtr expNode;
AstUnaryOp(AstExpPtr expNodePtr):AstExp(expNodePtr->type),expNode(expNodePtr){}
virtual llvm::Value* codeGen(genContext& context);
};

class AstFunctionDeclaration : public AstStat //Note: The function declaration needs to ensure the vector memory contigentcy remains. Functions can only be defined once. May need to investigate std::move.
{
public:
variableListPtr arguments;
AstTypePtr returnType;
AstNamePtr functionName;
AstBlockPtr block;
AstFunctionDeclaration(AstTypePtr returnType, AstNamePtr functionName, AstBlockPtr block, variableListPtr arguments = variableListPtr(nullptr)): arguments(arguments),returnType(returnType), functionName(functionName),block(block){}
AstFunctionDeclaration(AstType* returnType, AstName* functionName, AstBlock* block, variableList* arguments = nullptr): arguments(variableListPtr(arguments)),returnType(AstTypePtr(returnType)), functionName(AstNamePtr(functionName)),block(AstBlockPtr(block)){}
llvm::Value* codeGen(genContext& context);
};

class AstReturnStat: public AstStat
{
public:
AstExpPtr returnValue;
AstReturnStat(AstExpPtr returnValue):returnValue(returnValue){}
AstReturnStat(AstExp* returnValue):returnValue(AstExpPtr(returnValue)){}
llvm::Value* codeGen(genContext& context);
};

class AstVariableDeclaration : public AstStat
{
public:
AstNamePtr variableName;
AstTypePtr variableType;
AstExpPtr initializer = nullptr;
AstVariableDeclaration(AstNamePtr variableName, AstTypePtr variableType): variableName(variableName), variableType(variableType) {}
AstVariableDeclaration(AstNamePtr variableName, AstTypePtr variableType,AstExpPtr initializer): variableName(variableName), variableType(variableType),initializer(initializer) {}
AstVariableDeclaration(AstName* variableName, AstType* variableType): variableName(AstNamePtr(variableName)), variableType(AstTypePtr(variableType)) {}
AstVariableDeclaration(AstName* variableName, AstType* variableType,AstExp* initializer): variableName(AstNamePtr(variableName)), variableType(AstTypePtr(variableType)),initializer(AstExpPtr(initializer)) {}

llvm::Value* codeGen(genContext& context);
};

class AstGlobalVariableDeclaration : public AstStat
{
public:
AstNamePtr variableName;
AstTypePtr variableType;
AstConstantPtr initializer = nullptr;
AstGlobalVariableDeclaration(AstNamePtr variableName, AstTypePtr variableType): variableName(variableName), variableType(variableType) {}
AstGlobalVariableDeclaration(AstName* variableName, AstType* variableType): variableName(AstNamePtr(variableName)), variableType(AstTypePtr(variableType)) {}
AstGlobalVariableDeclaration(AstNamePtr variableName, AstTypePtr variableType,AstConstantPtr initializer): variableName(variableName), variableType(variableType),initializer(initializer) {}
AstGlobalVariableDeclaration(AstName* variableName, AstType* variableType,AstConstant* initializer): variableName(AstNamePtr(variableName)), variableType(AstTypePtr(variableType)),initializer(AstConstantPtr(initializer)) {}
llvm::Value* codeGen(genContext& context);
};

class AstArrayDeclaration : public AstStat
{
public:
AstNamePtr variableName;
AstTypePtr variableType;
AstIntValuePtr arraySize;
expressionListPtr initializer = expressionListPtr(nullptr);
AstArrayDeclaration(AstNamePtr variableName, AstTypePtr variableType, AstIntValuePtr arraySize): variableName(variableName), variableType(variableType), arraySize(arraySize) {}
AstArrayDeclaration(AstName* variableName, AstType* variableType, AstIntValue* arraySize): variableName(AstNamePtr(variableName)), variableType(AstTypePtr(variableType)), arraySize(AstIntValuePtr(arraySize)) {}
llvm::Value* codeGen(genContext& context);
};

class AstExpStat: public AstStat
{
public:
AstExpPtr expression;
AstExpStat(AstExpPtr expression):expression(expression){}
AstExpStat(AstExp* expression):expression(AstExpPtr(expression)){}
llvm::Value* codeGen(genContext& context);
};

class AstFunctionCall: public AstCall
{
public:
AstNamePtr functionName;
expressionListPtr args;
AstFunctionCall(AstNamePtr functionName, expressionListPtr args): functionName(functionName),args(args) {}
AstFunctionCall(AstName* functionName, expressionList* args): functionName(AstNamePtr(functionName)),args(expressionListPtr(args)) {}
llvm::Value* codeGen(genContext& context);
};

class AstVariableCall: public AstCall
{
public:
AstNamePtr variableName;
AstVariableCall(AstNamePtr variableName):variableName(variableName) {}
AstVariableCall(AstName* variableName):variableName(AstNamePtr(variableName)) {}
llvm::Value* codeGen(genContext& context);
};

class AstArrayCall : public AstCall
{
public:
AstNamePtr variableName;
AstExpPtr index;
AstArrayCall(AstNamePtr variableName,AstExpPtr index):variableName(variableName), index(index) {}
AstArrayCall(AstName* variableName,AstExp* index):variableName(AstNamePtr(variableName)), index(AstExpPtr(index)) {}
llvm::Value* codeGen(genContext& context);
};

#endif 
