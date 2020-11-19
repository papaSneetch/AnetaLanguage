#ifndef annetaBuilder
#define annetaBuilder

#include "llvmHeaders.h"
#include "codeGenContext.h"

#include <vector>
#include <map>
#include <string>

class AstExp;
class AstVariableDeclaration;
class AstStat;

typedef std::vector<AstExp> expressionList;
typedef std::vector<AstVariableDeclaration> variableList;
typedef std::vector<AstStat*> statementList; 

class AstNode
{
public:
virtual ~AstNode();
virtual llvm::Value* codeGen(genContext& context);
};

class AstType: public AstNode {
public:
virtual llvm::Type* typeOf(genContext& context);
};

class AstStat: public AstNode {
};

class AstExp: public AstNode {
public:
AstType type;
AstExp(AstType &type):type(type){}
AstExp();
virtual llvm::Value* codeGen(genContext& context);
};

class AstCall : public AstExp
{
};

class AstName : public AstNode {
public:
std::string& name;
AstName(std::string& name):name(name){}
};

class AstBlock: public AstNode {
public:
statementList statements;
std::map<std::string,llvm::AllocaInst*> variableMap;
AstBlock() {};
AstBlock(statementList& statements): statements(statements){}
llvm::Value* codeGen(genContext& context);
};

class AstIntType: public AstType
{
public:
llvm::Type* typeOf(genContext& context);
}intType;

class AstStringType: public AstType
{
public:
llvm::Type* typeOf(genContext& context);
}stringType;

class AstBoolType: public AstType
{
public:
llvm::Type* typeOf(genContext& context);
}boolType;

class AstFloatType: public AstType
{
public:
llvm::Type* typeOf(genContext& context);
}floatType;

class AstConstant: public AstExp
{
public:
AstConstant(AstType &type):AstExp(type){}
};

class AstIntValue: public AstConstant
{
public:
int value;
AstIntValue(int value):value(value),AstConstant(intType){}
llvm::Constant* codeGen(genContext& context);
};

class AstStringValue: public AstConstant
{
public:
std::string value;
AstStringValue(std::string value):value(value),AstConstant(stringType){}
llvm::Constant* codeGen(genContext& context);
};

class AstBoolValue: public AstConstant
{
public:
bool value;
AstBoolValue(bool value):value(value),AstConstant(boolType){}
llvm::Constant* codeGen(genContext& context);
};

class AstFloatValue: public AstConstant
{
public:
float value;
AstFloatValue(float value):value(value),AstConstant(floatType){}
llvm::Constant* codeGen(genContext& context);
};
 
class AstIfElseStat: public AstStat
{
public:
AstBlock& ifBlock;
AstBlock* elseBlock;
AstExp& testCondition;
AstIfElseStat(AstBlock& ifBlock, AstExp& testCondition,AstBlock* elseBlock = nullptr):ifBlock(ifBlock),elseBlock(elseBlock),testCondition(testCondition){}
llvm::Value* codeGen(genContext& context);
};

class AstWhileLoop: public AstStat
{
public:
AstBlock& whileBlock;
AstExp& testCondition;
AstWhileLoop(AstBlock& whileBlock,AstExp& testCondition):whileBlock(whileBlock),testCondition(testCondition){}
llvm::Value* codeGen(genContext& context);
};

class AstBinOp: public AstExp
{
public:
AstExp& lhs;
AstExp& rhs;
AstBinOp(AstExp& lhs, AstExp& rhs);
virtual llvm::Value* codeGen(genContext& context);
};

class AstOr: public AstBinOp
{
public:
AstOr(AstExp& lhs, AstExp& rhs):AstBinOp(lhs,rhs){} 
llvm::Value* codeGen(genContext& context);
};

class AstXor: public AstBinOp
{
public:
AstXor(AstExp& lhs, AstExp& rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstAnd: public AstBinOp
{
public:
AstAnd (AstExp& lhs, AstExp& rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstEql: public AstBinOp
{
public:
AstEql (AstExp& lhs, AstExp& rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class Astleq: public AstBinOp
{
public:
Astleq (AstExp& lhs, AstExp& rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class Astgeq: public AstBinOp
{
public:
Astgeq(AstExp& lhs, AstExp& rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class Astlt: public AstBinOp
{
public:
Astlt(AstExp& lhs, AstExp& rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class Astgt: public AstBinOp
{
public:
Astgt(AstExp& lhs, AstExp& rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class Asteql: public AstBinOp
{
public:
Asteql(AstExp& lhs, AstExp& rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class Astneq: public AstBinOp
{
public:
Astneq(AstExp& lhs, AstExp& rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstLeftSh: public AstBinOp
{
public:
AstLeftSh(AstExp& lhs, AstExp& rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstRightSh: public AstBinOp
{
public:
AstRightSh(AstExp& lhs, AstExp& rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstAdd: public AstBinOp
{
public:
AstAdd(AstExp& lhs, AstExp& rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstSub: public AstBinOp
{
public:
AstSub(AstExp& lhs, AstExp& rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstMul: public AstBinOp
{
public:
AstMul(AstExp& lhs, AstExp& rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstDiv: public AstBinOp
{
public:
AstDiv(AstExp& lhs, AstExp& rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstMod: public AstBinOp
{
public:
AstMod(AstExp& lhs, AstExp& rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstAsg: public AstExp
{
public:
virtual llvm::Value* codeGen(genContext& context);
AstAsg();
};

class AstVariableAsg: public AstAsg
{
public:
AstName& variableName;
AstExp& rhs;
AstVariableAsg(AstName& variableName, AstExp& rhs):variableName(variableName),rhs(rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstArrayAsg: public AstAsg
{
public:
AstName& variableName;
AstExp& rhs;
AstIntValue& index;
AstArrayAsg(AstName& variableName, AstExp& rhs, AstIntValue& index):variableName(variableName),rhs(rhs),index(index){}
llvm::Value* codeGen(genContext& context);
};

class AstArrayListAsg: public AstAsg
{
public:
expressionList& arrayValues;
AstName& variableName;
AstIntValue* index;
AstArrayListAsg(AstName& name,expressionList& arrayValues,AstIntValue* index = new AstIntValue(0)):variableName(name),arrayValues(arrayValues), index(index){}
llvm::Value* codeGen(genContext& context);
};

class AstAeg: public AstBinOp
{
public:
AstAeg(AstExp& lhs, AstExp& rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstMeg: public AstBinOp
{
public:
AstMeg (AstExp& lhs, AstExp& rhs):AstBinOp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstUnaryOp: public AstExp
{
public:
AstExp& expNode;
AstUnaryOp(AstExp& expNode) :
expNode(expNode)
{
type = expNode.type;
}
virtual llvm::Value* codeGen(genContext& context);
};

class AstFunctionDeclaration : public AstStat //Note: The function declaration needs to ensure the vector memory contigentcy remains. Functions can only be defined once. May need to investigate std::move.
{
public:
variableList* arguments;
AstType& returnType;
AstName& functionName;
AstBlock& block;
AstFunctionDeclaration(AstType& returnType, AstName& functionName, variableList* arguments, AstBlock& block): returnType(returnType), functionName(functionName), arguments(arguments), block(block){}
llvm::Value* codeGen(genContext& context);
};

class AstReturnStat: public AstStat
{
public:
AstExp& returnValue;
AstReturnStat(AstExp& returnValue):returnValue(returnValue){}
llvm::Value* codeGen(genContext& context);
};

class AstVariableDeclaration : public AstStat
{
public:
AstName& variableName;
AstType& variableType;
AstConstant* initializer = nullptr;
bool globalBool = false;
AstVariableDeclaration(AstName& variableName, AstType& variableType): variableName(variableName), variableType(variableType) {}
llvm::Value* codeGen(genContext& context);
};


class AstArrayDeclaration : public AstStat
{
public:
AstName& variableName;
AstType& variableType;
AstIntValue& arraySize;
expressionList* initializer = nullptr;
AstArrayDeclaration(AstName& variableName, AstType& variableType, AstIntValue& arraySize): variableName(variableName), variableType(variableType), arraySize(arraySize) {}
llvm::Value* codeGen(genContext& context);
};

class AstFunctionCall: public AstCall
{
public:
AstName& functionName;
expressionList args;
AstFunctionCall(AstName& functionName, expressionList& args): functionName(functionName),args(args) {}
llvm::Value* codeGen(genContext& context);
};

class AstVariableCall: public AstCall
{
public:
std::string& variableName;
AstVariableCall(std::string& variableName):variableName(variableName) {}
llvm::Value* codeGen(genContext& context);
};

class AstArrayCall : public AstCall
{
public:
std::string& variableName;
AstIntValue& index;
AstArrayCall(std::string& variableName,AstIntValue& index):variableName(variableName), index(index) {}
llvm::Value* codeGen(genContext& context);
};

#endif 
