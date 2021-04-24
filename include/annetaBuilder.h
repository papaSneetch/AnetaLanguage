#ifndef annetaBuilder
#define annetaBuilder

#include "codeGenContext.h"
#include "annetaTypes.h"

#include <vector>
#include <utility>
#include <map>
#include <string>
#include <memory>

class AstNode;
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

typedef std::unique_ptr<AstName> AstNamePtr;
typedef std::unique_ptr<AstNode> AstNodePtr;
typedef std::unique_ptr<AstStat> AstStatPtr;
typedef std::unique_ptr<AstType> AstTypePtr;
typedef std::unique_ptr<AstExp> AstExpPtr;
typedef std::unique_ptr<AstBlock> AstBlockPtr;
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
struct arrayInformation;
struct functionInformation;
struct globalVariableInformation;


class AstNode
{
public:
virtual ~AstNode() = default;
virtual llvm::Value* codeGen(genContext& context) = 0;
};

struct variableInformation
{
llvm::AllocaInst* alloca;
const AstType* type;
};


struct arrayInformation 
{
llvm::AllocaInst* alloca;
const AstType* type;
int arraySize;
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

class AstBlock: public AstStat {
public:
statementListPtr statements;
std::map<std::string,variableInformation> variableMap;
std::map<std::string,arrayInformation> arrayMap;
AstBlock(statementListPtr& statements): statements(std::move(statements)){}
AstBlock(statementList* statements): statements(statementListPtr(statements)){}
llvm::Value* codeGen(genContext& context);
llvm::Value* codeGenInFunction(genContext& context);
};

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


class AstCharValue: public AstConstant
{
public:
char value;
AstCharValue(char value):AstConstant(charType),value(value){}
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
double value;
AstFloatValue(double value):AstConstant(floatType),value(value){}
llvm::Constant* codeGen(genContext& context);
};
 
class AstIfElseStat: public AstStat
{
public:
AstBlockPtr ifBlock;
AstBlockPtr elseBlock;
AstExpPtr testCondition;
AstIfElseStat( AstBlockPtr& ifBlock, AstExpPtr& testCondition, AstBlockPtr& elseBlock):ifBlock(std::move(ifBlock)),elseBlock(std::move(elseBlock)),testCondition(std::move(testCondition)){}
AstIfElseStat( AstBlock* ifBlockTmp, AstExp* testConditionTmp, AstBlock* elseBlockTmp = nullptr) {
ifBlock.reset(ifBlockTmp);
elseBlock.reset(elseBlockTmp);
testCondition.reset(testConditionTmp);
}
llvm::Value* codeGen(genContext& context);
};

class AstIfStat: public AstStat
{
public:
AstBlockPtr ifBlock;
AstExpPtr testCondition;
AstIfStat( AstBlockPtr& ifBlock, AstExpPtr& testCondition):ifBlock(std::move(ifBlock)),testCondition(std::move(testCondition)){}
AstIfStat( AstBlock* ifBlockTmp, AstExp* testConditionTmp) {
ifBlock.reset(ifBlockTmp);
testCondition.reset(testConditionTmp);
}
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
AstExpPtr index;
AstArrayAsg(AstNamePtr& variableName, AstExpPtr& rhs, AstExpPtr& index):AstAsg(variableName,rhs),index(std::move(index)){}
AstArrayAsg(AstName* variableNameTmp, AstExp* rhsTmp, AstExp* indexTmp):AstAsg(variableNameTmp,rhsTmp)
{
index.reset(indexTmp);
}
llvm::Value* codeGen(genContext& context);
};


class AstArrayStringAsg: public AstExp
{
public:
AstNamePtr variableName;
std::string str;
bool nullTerm; 
AstArrayStringAsg(AstNamePtr& variableNameTmp,char* strTmp,bool nullTermTmp):AstExp(charType),variableName(std::move(variableNameTmp)),str(std::string(strTmp)),nullTerm(nullTermTmp){};
AstArrayStringAsg(AstName* variableNameTmp, char* strTmp,bool nullTermTmp):AstExp(charType),str(std::string(strTmp)),nullTerm(nullTermTmp){
variableName.reset(variableNameTmp);
}
llvm::Value* codeGen(genContext& context);
};


class AstArrayListAsg: public AstAsg
{
public:
AstArrayListAsg(AstNamePtr& variableNameTmp,expressionListPtr& listEle):AstAsg(variableNameTmp,listEle){}
AstArrayListAsg(AstName* variableNameTmp, expressionList* listEle):AstAsg(variableNameTmp,listEle){}
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

class AstDeref: public AstExp
{
public:
AstExpPtr expNode;
AstDeref(AstExpPtr& expNodePtr):AstExp((expNodePtr->type->getTypeName() == "pointer")?expNodePtr->type:nullptr),expNode(std::move(expNodePtr)){
if (type==nullptr)
{
std::cerr << "Error: can't dereference a non pointer."<<std::endl;
exit(1);
}
}
AstDeref(AstExp* expNodePtr):AstExp((expNodePtr->type->getTypeName() == "pointer")?expNodePtr->type:nullptr){
expNode.reset(expNodePtr);
std::cerr << "Error: can't dereference a non pointer."<<std::endl;
exit(1);
}
llvm::Value* codeGen(genContext& context);
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
const AstType* variableType; 
AstNamePtr variableName; //The variableName can't be first or a bug in the the llvm::GlobalVariable will overwrite the queue pointer to the variable crashing the program.
AstConstantPtr initializer;
AstGlobalVariableDeclaration(const AstType& variableType,AstNamePtr& variableName): variableType(&variableType),variableName(std::move(variableName)){}
AstGlobalVariableDeclaration(const AstType* variableTypeTmp, AstName* variableNameTmp):variableType(variableTypeTmp) {
variableName.reset(variableNameTmp);
}
AstGlobalVariableDeclaration(const AstType& variableType, AstNamePtr& variableName, AstConstantPtr& initializer): variableType(&variableType),variableName(std::move(variableName)),initializer(std::move(initializer)) {}
AstGlobalVariableDeclaration(const AstType* variableTypeTmp, AstName* variableNameTmp, AstConstant* initializerTmp):variableType(variableTypeTmp) {
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
int arraySize;

AstArrayDeclaration(AstNamePtr& variableName,const AstType& variableTypeTmp, int size): variableName(std::move(variableName)), variableType(&variableTypeTmp), arraySize(size) {}
AstArrayDeclaration(AstName* variableNameTmp, const AstType* variableTypeTmp, int size):variableType(variableTypeTmp), arraySize(size) {
variableName.reset(variableNameTmp);
}
llvm::Value* codeGen(genContext& context);
};


class AstArrayListDeclaration: public AstStat
{
public:
AstNamePtr variableName;
expressionListPtr list;
const AstType* variableType;
int arraySize;
AstArrayListDeclaration(AstNamePtr& variableName,expressionListPtr& listEle,const AstType& variableTypeTmp, int size):variableName(std::move(variableName)),list(std::move(listEle)),variableType(&variableTypeTmp), arraySize(size){}
AstArrayListDeclaration(AstName* variableNameTmp, expressionList* listEle,const AstType* variableTypeTmp,int size):variableType(variableTypeTmp), arraySize(size){
variableName.reset(variableNameTmp);
list.reset(listEle);
}
llvm::Value* codeGen(genContext& context);
};


class AstArrayStringDeclaration: public AstStat
{
public:
AstNamePtr variableName;
std::string string;
const AstType* variableType;
int arraySize;
bool nullTerminate;
AstArrayStringDeclaration(AstNamePtr& variableName,char* charPtr,const AstType& variableTypeTmp, int size,bool nullTerminate):variableName(std::move(variableName)),string(std::string(charPtr)),variableType(&variableTypeTmp), arraySize(size),nullTerminate(nullTerminate){}
AstArrayStringDeclaration(AstName* variableNameTmp, char* charPtr,const AstType* variableTypeTmp, int size, bool nullTerminate):string(std::string(charPtr)),variableType(variableTypeTmp), arraySize(size),nullTerminate(nullTerminate){
variableName.reset(variableNameTmp);
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

class AstVariableAddress: public AstCall
{
AstNamePtr variableName;
public:
AstVariableAddress(AstNamePtr& variableNameTmp):variableName(std::move(variableNameTmp)) {}
AstVariableAddress( AstName* variableNameTmp) {
variableName.reset(variableNameTmp);
}
llvm::Value* codeGen(genContext& context);
};


class AstArrayAddress : public AstCall
{
public:
AstNamePtr variableName;
AstExpPtr index;
AstArrayAddress( AstNamePtr& variableName, AstExpPtr& index):variableName(std::move(variableName)), index(std::move(index)) {}
AstArrayAddress( AstName* variableNameTmp, AstExp* indexTmp) {
variableName.reset(variableNameTmp);
index.reset(indexTmp);
}
llvm::Value* codeGen(genContext& context);
};


class AstArrayCallAddress : public AstCall
{
public:
AstNamePtr variableName;
AstExpPtr index;
AstArrayCallAddress( AstNamePtr& variableName):variableName(std::move(variableName)) {}
AstArrayCallAddress( AstName* variableNameTmp) {
variableName.reset(variableNameTmp);
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
bool varArg;
};

struct globalVariableInformation
{
llvm::GlobalVariable* alloca;
const AstType* type;
};

#endif 
