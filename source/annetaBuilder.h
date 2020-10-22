#include <vector>
#include <map>
#include <string>

class genContext;
typedef std::vector<AstStat> statementList; 
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
};

class AstName : public AstNode {
public:
std::string& name;
AstName(std::string& name):name(name){}
};

class AstBlock : public AstNode {
public:
statementList statements;
AstBlock() {};
llvm::Value* codeGen(genContext& context);
};

class AstBinOp: public AstExp
{
public:
AstExp& lhs;
AstExp& rhs;
NBinaryOperator(AstExp& lhs, AstExp& rhs) :
lhs(lhs), rhs(rhs) { }
virtual llvm::Value* codeGen(genContext& context);
};

class AstOr: public AstExp
{
AstOr(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){} 
llvm::Value* codeGen(genContext& context);
};

class AstXor: public AstExp
{
AstXor(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstAnd: public AstExp
{
AstAnd (AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class Astleq: public AstExp
{
Astleq (AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class Astgeq: public AstExp
{
Astgeq(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class Astlt: public AstExp
{
Astlt(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class Astgt: public AstExp
{
Astgt(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class Asteql: public AstExp
{
Asteql(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class Astneq: public AstExp
{
Astneq(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstLeftSh: public AstExp
{
AstLeftSh(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstRightSh: public AstExp
{
AstRightSh(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstAdd: public AstExp
{
AstAdd(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstSub: public AstExp
{
AstSub(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstMul: public AstExp
{
AstMul(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstDiv: public AstExp
{
AstDiv(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstMod: public AstExp
{
AstMod(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstAsg: public AstExp
{
AstAsg(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstAeg: public AstExp
{
AstAeg(AstExp& lhs, AstExp& rhs):AstExp(lhs,rhs){}
llvm::Value* codeGen(genContext& context);
};

class AstMeg: public AstExp
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

class AstInt : public AstExp
{
public:
int val;
AstInt(int val): val(val) {}
llvm::Value* codeGen(genContext& context);
};

class AstFloat : public AstExp
{
public:
float val;
AstFloat(float val): val(val) {}
llvm::Value* codeGen(genContext& context);
};

class AstBool : public AstExp
{
public:
bool val;
AstBool(bool val): val(val) {}
llvm::Value* codeGen(genContext& context);
};

class AstString : public AstExp
{
public:
std::string val;
AstString(std::string val): val(val){}
llvm::Value* codeGen(genContext& context);
};



class AstFunctionDeclaration : public AstStat //Note: The function declaration needs to ensure the vector memory contigentcy remains. Functions can only be defined once. May need to investigate std::move.
{
public:
variableList arguments;
AstType& returnType;
AstName& functionName;
AstBlock& block;
AstFunctionDeclartion(AstType& returnType,AstName& functionName, variableList& arguments, AstBlock& block): returnType(returnType), functionName(functionName), arguments(arguments), block(block) {}
llvm::Value* codeGen(genContext& context);
};

class AstVariableDeclaration : public AstStat
{
public:
AstName& variableName;
AstType& variableType;
AstVariableDeclaration(AstName& variableName, AstType& variableType): variableName(variableName), variableType(variableType) {}
llvm::Value* codeGen(genContext& context);
};

class functionCall: public AstCall
{
AstName& functionName;
expressionList args;
functionCall(AstName& functionName, expressionList& args): functionName(functionName),args(args) {}
llvm::Value* codeGen(genContext& context);
};

class variableCall: public AstCall
{
std::string& variableName;
variableCall(std::string& variableName):variableName(variableName) {}
llvm::Value* codeGen(genContext& context);
}


