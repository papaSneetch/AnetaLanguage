#ifndef annetaBuilder
#define annetaBuilder

#include "llvmHeaders.h"
#include "codeGenContext.h"

#include <vector>
#include <utility>
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
AstExp(AstTypePtr& type):type(std::move(type)){}
AstExp(AstType* typeTmp){
type.reset(typeTmp);
}
AstExp() = default;
virtual llvm::Value* codeGen(genContext& context) {return nullptr;}
};

class AstCall : public AstExp
{
};

class AstName: public AstNode {
public:
std::string name;
AstName(const std::string& name):name(name){}
llvm::Value* codeGen(genContext& context);
};

class AstBlock: public AstNode {
public:
statementListPtr statements;
std::map<std::string,llvm::AllocaInst*> variableMap;
AstBlock(statementListPtr& statements): statements(std::move(statements)){}
AstBlock(statementList* statements): statements(statementListPtr(statements)){}
llvm::Value* codeGen(genContext& context);
llvm::Value* codeGenInFunction(genContext& context);
};

class AstIntType: public AstType
{
public:
llvm::IntegerType* typeOf(genContext& context);
AstIntType(){};
};

class AstStringType: public AstType
{
public:
llvm::Type* typeOf(genContext& context);
AstStringType(){};
};

class AstBoolType: public AstType
{
public:
llvm::IntegerType* typeOf(genContext& context);
AstBoolType(){};
};

class AstFloatType: public AstType
{
public:
llvm::Type* typeOf(genContext& context);
AstFloatType(){};
};

class AstConstant: public AstExp
{
public:
virtual ~AstConstant() = default;
AstConstant(AstTypePtr& type):AstExp(type){}
AstConstant(AstType* type):AstExp(type){}
virtual llvm::Constant* codeGen(genContext& context) = 0;
};

class AstIntValue: public AstConstant
{
public:
int value;
AstIntValue(int value):AstConstant(new AstIntType()),value(value){}
llvm::Constant* codeGen(genContext& context);
};

class AstStringValue: public AstConstant
{
public:
std::string value;
AstStringValue(std::string value):AstConstant(new AstStringType()),value(value){}
llvm::Constant* codeGen(genContext& context);
};

class AstBoolValue: public AstConstant
{
public:
bool value;
AstBoolValue(bool value):AstConstant(new AstBoolType()),value(value){}
llvm::Constant* codeGen(genContext& context);
};

class AstFloatValue: public AstConstant
{
public:
float value;
AstFloatValue(float value):AstConstant(new AstFloatType()),value(value){}
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
AstWhileLoop( AstBlock* whileBlock, AstExp* testCondition):whileBlock(whileBlock),testCondition(testCondition){}
llvm::Value* codeGen(genContext& context);
};

class AstBinOp: public AstExp
{
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
virtual llvm::Value* codeGen(genContext& context) = 0;
};

class AstVariableAsg: public AstAsg
{
public:
AstNamePtr variableName;
AstExpPtr rhs;
AstVariableAsg(AstNamePtr& variableName, AstExpPtr& rhs):variableName(std::move(variableName)),rhs(std::move(rhs)){}
AstVariableAsg( AstName* variableName, AstExp* rhs):variableName(AstNamePtr(variableName)),rhs(AstExpPtr(rhs)){}
llvm::Value* codeGen(genContext& context);
};

class AstArrayAsg: public AstAsg
{
public:
AstNamePtr variableName;
AstExpPtr rhs;
AstExpPtr index;
AstArrayAsg( AstNamePtr& variableName, AstExpPtr& rhs, AstExpPtr& index):variableName(std::move(variableName)),rhs(std::move(rhs)),index(std::move(index)){}
AstArrayAsg(AstName* variableNameTmp, AstExp* rhsTmp, AstExp* indexTmp)
{
variableName.reset(variableNameTmp);
rhs.reset(rhsTmp);
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
AstArrayListAsg(AstNamePtr& name, expressionListPtr& arrayValues, AstExpPtr& index):arrayValues(std::move(arrayValues)),variableName(std::move(name)),index(std::move(index)){}
AstArrayListAsg(AstNamePtr& name, expressionListPtr& arrayValues):arrayValues(std::move(arrayValues)),variableName(std::move(name)){
index.reset(new AstIntValue(0));
}
AstArrayListAsg(AstName* nameTmp, expressionList* arrayValuesTmp,AstExp* indexTmp = new AstIntValue(0)) {
arrayValues.reset(arrayValuesTmp);
variableName.reset(nameTmp);
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
AstTypePtr returnType;
AstNamePtr functionName;
AstBlockPtr block;
AstFunctionDeclaration(AstTypePtr& returnType, AstNamePtr& functionName, AstBlockPtr& block, variableListPtr& arguments): arguments(std::move(arguments)),returnType(std::move(returnType)), functionName(std::move(functionName)),block(std::move(block)){}
AstFunctionDeclaration( AstType* returnTypeTmp, AstName* functionNameTmp, AstBlock* blockTmp, variableList* argumentsTmp) {
arguments.reset(argumentsTmp);
returnType.reset(returnTypeTmp);
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
AstTypePtr variableType;
AstExpPtr initializer; 
AstVariableDeclaration( AstNamePtr& variableName, AstTypePtr& variableType): variableName(std::move(variableName)), variableType(std::move(variableType)) {}
AstVariableDeclaration( AstNamePtr& variableName, AstTypePtr& variableType, AstExpPtr& initializer):variableName(std::move(variableName)),variableType(std::move(variableType)),initializer(std::move(initializer))  {}
AstVariableDeclaration( AstName* variableNameTmp, AstType* variableTypeTmp) { 
variableName.reset(variableNameTmp);
variableType.reset(variableTypeTmp);
} 
AstVariableDeclaration( AstName* variableNameTmp, AstType* variableTypeTmp, AstExp* initializerTmp) {
variableName.reset(variableNameTmp);
variableType.reset(variableTypeTmp);
initializer.reset(initializerTmp);
}

llvm::AllocaInst* codeGen(genContext& context);
};

class AstGlobalVariableDeclaration : public AstStat
{
public:
AstNamePtr variableName;
AstTypePtr variableType;
AstConstantPtr initializer;
AstGlobalVariableDeclaration( AstNamePtr& variableName, AstTypePtr& variableType): variableName(std::move(variableName)), variableType(std::move(variableType)) {}
AstGlobalVariableDeclaration( AstName* variableNameTmp, AstType* variableTypeTmp) {
variableName.reset(variableNameTmp);
variableType.reset(variableTypeTmp);
}
AstGlobalVariableDeclaration( AstNamePtr& variableName, AstTypePtr& variableType, AstConstantPtr& initializer): variableName(std::move(variableName)), variableType(std::move(variableType)),initializer(std::move(initializer)) {}
AstGlobalVariableDeclaration( AstName* variableNameTmp, AstType* variableTypeTmp,AstConstant* initializerTmp) {
variableName.reset(variableNameTmp);
variableType.reset(variableTypeTmp);
initializer.reset(initializerTmp);
}
llvm::Value* codeGen(genContext& context);
};

class AstArrayDeclaration : public AstStat
{
public:
AstNamePtr variableName;
AstTypePtr variableType;
AstIntValuePtr arraySize;
expressionListPtr initializer; 
AstArrayDeclaration( AstNamePtr& variableName, AstTypePtr& variableType,  AstIntValuePtr& arraySize): variableName(std::move(variableName)), variableType(std::move(variableType)), arraySize(std::move(arraySize)) {}
AstArrayDeclaration( AstName* variableNameTmp, AstType* variableTypeTmp, AstIntValue* arraySizeTmp) {
variableName.reset(variableNameTmp);
variableType.reset(variableTypeTmp);
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
AstVariableCall( AstNamePtr& variableName):variableName(std::move(variableName)) {}
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
AstArrayCall( AstNamePtr& variableName, AstExpPtr& index):variableName(std::move(variableName)), index(std::move(index)) {}
AstArrayCall( AstName* variableNameTmp, AstExp* indexTmp) {
variableName.reset(variableNameTmp);
index.reset(indexTmp);
}
llvm::Value* codeGen(genContext& context);
};


#endif 
