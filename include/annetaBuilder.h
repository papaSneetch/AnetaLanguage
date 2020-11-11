#include "llvmHeaders.h"

#include <vector>
#include <map>
#include <string>

#ifndef annetaBuilder
#define annetaBuilder

typedef std::vector<AstStat*> statementList; 
typedef std::vector<AstExp> expressionList;
typedef std::vector<AstVariableDeclaration> variableList


class AstNode
{
public:
virtual ~AstNode(){};
virtual llvm::Value* gen(genContext& context){};
};

class AstExp : public AstNode {
};

class AstCall : public AstExp
{
}

class AstStat : public AstNode {
};

class AstType : public AstNode {
virtual llvm::Type* typeOf(genContext& context);
};

class AstName : public AstNode {
public:
std::string& name;
AstName(std::string& name):name(name){}
};

class AstBlock: public AstNode {
public:
statementList statements;
std::map<std::string,llvm::AllocaInst*> variableMap
AstBlock() {};
AstBlock(statementList& statements): statements(statements){};
llvm::Value* codeGen(genContext& context);
};

class AstIntType: public AstType
{
llvm::Type* typeOf(genContext& context);
};

class AstStringType: public AstType
{
llvm::Type* typeOf(genContext& context);
};

class AstBoolType: public AstType
{
llvm::Type* typeOf(genContext& context);
};

class AstFloatType: public AstType
{
llvm::Type* typeOf(genContext& context);
};

class AstConstant: public AstExp
{
};

class AstIntValue: public AstConstant
{
signed int value;
AstIntValue(signed int value}:value(value){}
llvm::Value* codeGen(genContext& context);
};

class AstStringValue: public AstConstant
{
std::string value;
AstStringValue(std::string value):value(value){}
llvm::Value* codeGen(genContext& context);
};

class AstBoolValue: public AstConstant
{
bool value;
AstBoolValue(bool value):value(value){}
llvm::Value* codeGen(genContext& context);
};

class AstFloatValue: public AstConstant
{
float value;
AstFloatValue(float value):value(value){}
llvm::Value* codeGen(genContext& context);
};
 $$->
class AstArrayListAsg: public AstConstant
{
expressionList& arrayValues;
AstName& variableName;
AstIntValue& index;
AstArrayValue(AstName& name,expressionList& arrayValue,AstIntValue& index = AstIntValue(0)):name(name),arrayValue(arrayValue), index(index){}
llvm::Value* codeGen(genContext& context);
}

class AstIfElseStat: public AstStat
{
public:
AstBlock& ifBlock;
AstBlock* elseBlock;
AstExp& testCondition;
AstIfElseStat(AstBlock& ifBlock, AstExp& testCondition,AstBlock* elseBlock = nullptr):ifBlock(ifBlock),elseBlock(elseBlock),returnBlock(returnBlock),testCondition(testCondition){}
llvm::Value* codeGen(genContext& context);
}

class AstWhileLoop: public AstStat
{
AstBlock& whileBlock;
AstExp& testCondition;
AstWhileLoop(AstBlock& whileBlock,AstExp& testCondition):whileBlock(whileBlock),testCondition(testCondition){}
llvm::Value* codeGen(genContext& context);
}

class AstBinOp: public AstExp
{
public:
AstExp& lhs;
AstExp& rhs;
NBinaryOperator(AstExp& lhs, AstExp& rhs) :
lhs(lhs), rhs(rhs) { }
virtual llvm::Value* codeGen(genContext& context);
};

class AstOr: public AstBinOp
{
AstOr(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){} 
llvm::Value* codeGen(genContext& context);
};

class AstXor: public AstBinOp
{
AstXor(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstAnd: public AstBinOp
{
AstAnd (AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class Astleq: public AstBinOp
{
Astleq (AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class Astgeq: public AstBinOp
{
Astgeq(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class Astlt: public AstBinOp
{
Astlt(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class Astgt: public AstBinOp
{
Astgt(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class Asteql: public AstBinOp
{
Asteql(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class Astneq: public AstBinOp
{
Astneq(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstLeftSh: public AstBinOp
{
AstLeftSh(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstRightSh: public AstBinOp
{
AstRightSh(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstAdd: public AstBinOp
{
AstAdd(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstSub: public AstBinOp
{
AstSub(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstMul: public AstBinOp
{
AstMul(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstDiv: public AstBinOp
{
AstDiv(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstMod: public AstBinOp
{
AstMod(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstAsg: public AstBinOp
{
};

class AstVariableAsg: public AstAsg
{
AstName& variableName;
AstExp& rhs;
AstVariableAsg(AstName& variableName, AstExp& rhs):variableName(variableName),rhs(rhs){}
llvm::Value* codeGen(genContext& context);
}

class AstArrayAsg: public AstAsg
{
AstName& variableName;
AstExp& rhs;
AstIntValue& index;
AstArrayAsg(AstName& variableName, AstExp& rhs, AstIntValue& index):variableName(variableName),rhs(rhs),index(index){}
llvm::Value* codeGen(genContext& context);
}

class AstAeg: public AstBinOp
{
AstAeg(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstMeg: public AstBinOp
{
(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstUnaryOp: public AstExp
{
public:
AstExp& expNode;
NBinaryOperator(AstExp& expNode) :
expNode(expNode) { }
virtual llvm::Value* codeGen(genContext& context);
}	

class AstFunctionDeclaration : public AstStat //Note: The function declaration needs to ensure the vector memory contigentcy remains. Functions can only be defined once. May need to investigate std::move.
{
public:
variableList* arguments;
AstType& returnType;
AstName& functionName;
AstBlock& block;
AstFunctionDeclartion(AstType& returnType,AstName& functionName, variableList* arguments, AstBlock& block): returnType(returnType), functionName(functionName), arguments(arguments), block(block) {}
llvm::Value* codeGen(genContext& context);
};

class AstReturnStat: public AstStat
{
public:
AstExp& returnValue;
AstReturnStat(AstExp& returnValue):returnValue(returnValue){}
llvm::Value* codeGen(genContext& context);
}

class AstVariableDeclaration : public AstStat
{
public:
AstName& variableName;
AstType& variableType;
AstExp* initializer = nullptr;
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
AstVariableDeclaration(AstName& variableName, AstType& variableType, Value& arraySize): variableName(variableName), variableType(variableType), arraySize(arraySize) {}
llvm::Value* codeGen(genContext& context);
}

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
}

class AstArrayCall : public AstCall
{
public:
std::string& variableName;
AstIntValue& index;
AstVariableCall(std::string& variableName,Value& index):variableName(variableName), index(index) {}
llvm::Value* codeGen(genContext& context);
};

#endif annetaBuilder
