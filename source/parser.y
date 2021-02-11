%{
/* C/C++ code */

#include <iostream>
#include "lexer.yy.h"
#include <codeGenContext.h>

#define YYPARSE_PARAM yyscan_t scanner
#define YYLEX_PARAM scanner

void yyerror (char const *s)
{
  fprintf (stderr, "%s\n", s);
}
%}

%code requires {
#include "annetaBuilder.h"
}

/*Token Declarations */

%require "3.7.3"
%define api.pure

%union
{
int int_val;
bool bool_val;
float float_val;
char* string;
AstNode* node;
AstStat* statement;
AstExp* expression;
AstConstant* constant;
AstIntValue* intPtr;
AstBlock* blockPtr;
AstVariableDeclaration* varDecl;
AstGlobalVariableDeclaration* globalVarDecl;
AstArrayDeclaration* arrayDecl;
AstName* name;
AstType* type;
statementList* statements;
expressionList* expressions;
variableList* args;
}

%token ifS elseS whileS returnS
%token stringD intD floatD boolD %token leftSh rightSh add sub
%token mul Div exponent Xor mod inc dec
%token eql leq geq lt gt neq
%token aeg meg asg And Or
%token '[' ']' '{' '}'
%token ',' ';' '(' ')'
%token intV boolV floatV stringV nameV

%type <node> prog funcDecl 

%type <statements> stats 
%type <statement> stat condStat returnStat whileloop
%type <expression> call varCall arrayCall funcCall
%type <expression> incDecOption asgOptions exp arrayReferenceExp
%type <constant> value 
%type <intPtr> arrayReference

%type <varDecl> varDecl
%type<globalVarDecl> globalVarDecl

%type <arrayDecl> arrayDefine arrayDecl

%type <name> varNames arrayNames

%type <type> type

%type <blockPtr> block condElseBlock

%type <int_val> intV
%type <bool_val> boolV
%type <float_val> floatV
%type <string> stringV
%type <string> nameV

%type <expressions> exps
%type <args> argDecl funcArgs

%left ','
%right aeq meq asgOptionsPrec asg
%left Or 
%left Xor
%left And
%left eql neq
%left leq geq lt gt
%left leftSh rightSh
%left add sub
%left mul Div mod
%left postInc postDec
%right preInc preDec

/* Grammar Definitions */
%%

progs:
	  prog
{
currentContext.pushAstNode($1);
std::cout << "Syntax Object: prog. " << std::endl;
}
	| progs prog
{
currentContext.pushAstNode($2);
std::cout << "Syntax Object: prog. " << std::endl;
}

prog:
	  funcDecl {
$$ = $1; 
std::cout << "Syntax Object: stat." << std::endl; }
	| globalVarDecl ';' {
$$ = $1; 
std::cout << "Syntax Object: stat. " << std::endl;}

stats:
	  stats stat {
$$ = $1; $$->push_back(AstStatPtr($2));
std::cout << "Syntax Object: stats. " << std::endl;}
	| stat {
$$ = new statementList(); $$->push_back(AstStatPtr($1));
std::cout << "Created New Block. Syntax Objcet: stats. " << std::endl;}

stat:
	  varDecl ';' {
$$ = $1;
std::cout << "Syntax Object: stat. " << std::endl;}
	| arrayDecl ';' {
$$ = $1;
std::cout << "Syntax Object: stat. " << std::endl;}
	| condStat {
$$ = $1;
std::cout << "Syntax Object: stat. " << std::endl;}
	| exp ';' {
$$ = new AstExpStat($1);
std::cout << "Syntax Object: stat. " << std::endl;}
	| whileloop {
$$ = $1;
std::cout << "Syntax Object: stat. " << std::endl;}
	| returnStat ';' {
$$ = $1;
std::cout << "Syntax Object: exps. " << std::endl;} 


condStat:
	  ifS '(' exp ')' block condElseBlock{
$$ = new AstIfElseStat($5,$3,$6);
std::cout << "Syntax Object: ifStat. " << std::endl;
}

	| ifS '(' exp ')' block {
$$ = new AstIfElseStat($5,$3,nullptr);
std::cout << "Syntax Object: ifStat. " << std::endl;
}

condElseBlock:
	  elseS condStat{
$$ = new AstBlock(new statementList); $$->statements->push_back(AstStatPtr($2));
std::cout << "Syntax Object: elseIfStat. " << std::endl;}
	| elseS block {
$$ = $2;
std::cout << "Syntax Object: elseIfStat. " << std::endl;};

globalVarDecl:
		type varNames{
$$ = new AstGlobalVariableDeclaration($2,$1);
std::cout << "Syntax Object: varDecl. " << std::endl;}
	  | type varNames asg value{
$$ = new AstGlobalVariableDeclaration($2,$1,$4); 
std::cout << "Syntax Object: varDecl. " << std::endl;}	 

varDecl:
	  type varNames{
$$ = new AstVariableDeclaration($2,$1);
std::cout << "Syntax Object: varDecl. " << std::endl;}
	| type varNames asg exp   {
$$ = new AstVariableDeclaration($2,$1,$4); 
std::cout << "Syntax Object: varDecl. " << std::endl;}

varNames:
	nameV {
$$ = new AstName(std::string($1));
std::cout << "Syntax Object: varNames. " << std::endl;}

type:
	  intD {
$$ = new AstIntType();
std::cout << "Syntax Object: type. " << std::endl;}
	| floatD {
$$ = new AstFloatType();
std::cout << "Syntax Object: type. " << std::endl;}
	| boolD {
$$ = new AstBoolType();
std::cout << "Syntax Object: type. " << std::endl;}
	| stringD {
$$ = new AstStringType();
std::cout << "Syntax Object: type. " << std::endl;}

funcDecl:
	  type varNames funcArgs block  {
$$ = new AstFunctionDeclaration($1,$2,$4,$3);
std::cout << "Syntax Object: funcDecl. " << std::endl;}

funcArgs:
	  '(' argDecl ')' {
$$ = $2;}
	|  '(' ')' {
$$ = new variableList();}
 
block:
	   '{' stats '}' {
$$ = new AstBlock($2);
std::cout << "Syntax Object: block. " << std::endl;}

argDecl:
	  argDecl ',' type varNames {
$$ = $1; $$->push_back(AstVariableDeclaration($4,$3));
std::cout << "Syntax Object: argDecl. " << std::endl;}
	| type varNames {
$$ = new variableList(); $$->push_back(AstVariableDeclaration($2,$1));
std::cout << "Syntax Object: argDecl. " << std::endl;}

arrayDecl:
      arrayDefine  {
$$ = $1;
std::cout << "Syntax Object: arrayDecl. " << std::endl;}
	| arrayDefine asg '{' exps '}'  {
$$ = $1; $$->initializer=expressionListPtr($4);
std::cout << "Syntax Object: arrayDecl. " << std::endl;}

arrayDefine:
	  type arrayNames arrayReference {
$$ = new AstArrayDeclaration($2,$1,$3);
std::cout << "Syntax Object: arrayDefine. " << std::endl;}

arrayReference:
	  '[' intV ']' {
$$ = new AstIntValue($2);}

arrayReferenceExp:
	  '[' exp ']' {
$$ = $2;}

arrayNames:
	nameV {
$$ = new AstName(std::string($1));
std::cout << "Syntax Object: arrayNames. " << std::endl;}

funcCall:
	  nameV '(' exps ')' {
$$ = new AstFunctionCall(new AstName($1),$3);
std::cout << "Syntax Object: listInit. " << std::endl;}

arrayCall:
	  nameV arrayReferenceExp {
$$ = new AstArrayCall(new AstName($1),$2);
std::cout << "Syntax Object: arrayCall. " << std::endl;}

varCall:
	  nameV {
$$ = new AstVariableCall(new AstName($1));
std::cout << "Syntax Object: varCall. " << std::endl;}

returnStat:
	  returnS exp {
$$ = new AstReturnStat($2);
}

call:
	  varCall	   {
$$ = $1;
std::cout << "Syntax Object: call. " << std::endl;}
	| funcCall {
$$ = $1;
std::cout << "Syntax Object: call. " << std::endl;}
	| arrayCall {
$$ = $1;
std::cout << "Syntax Object: call. " << std::endl;}

value:
	  intV {
$$ = new AstIntValue($1);
std::cout << "Syntax Object: value. " << std::endl;
}
	| boolV {
$$ = new AstBoolValue($1);
std::cout << "Syntax Object: value. " << std::endl;
}
	| floatV {
$$ = new AstFloatValue($1);
std::cout << "Syntax Object: value. " << std::endl;
}
	| stringV {
$$ = new AstStringValue($1);
std::cout << "Syntax Object: value. " << std::endl;

}

whileloop:
	  whileS '(' exp ')' block {
$$ = new AstWhileLoop($5,$3);
std::cout << "Syntax Object: whileloop. " << std::endl;}

exps:
      exps ',' exp {
$$ = $1; $$ -> push_back(AstExpPtr($3));
std::cout << "Syntax Object: exps. " << std::endl;}
	| exp {
$$ = new expressionList(); $$->push_back(AstExpPtr($1));
std::cout << "Syntax Object: exps. " << std::endl;}

exp:
	  exp Or exp {
$$ = new AstOr($1,$3);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp Xor exp {
$$ = new AstXor($1,$3);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp And exp {
$$ = new AstAnd($1,$3);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp leq exp {
$$ = new Astleq($1,$3);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp geq exp {
$$ = new Astgeq($1,$3);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp lt exp {
$$ = new Astlt($1,$3);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp gt exp {
$$ = new Astgt($1,$3);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp eql exp {
$$ = new AstEql($1,$3);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp neq exp {
$$ = new Astneq($1,$3);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp leftSh exp {
$$ = new AstLeftSh($1,$3);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp rightSh exp {
$$ = new AstRightSh($1,$3);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp add exp {
$$ = new AstAdd($1,$3);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp sub exp {
$$ = new AstSub($1,$3);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp mul exp {
$$ = new AstMul($1,$3);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp Div exp {
$$ = new AstDiv($1,$3);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp mod exp {
$$ = new AstMod($1,$3);
std::cout << "Syntax Object: exp. " << std::endl;}
	| inc incDecOption {std::cout << "Syntax Object: exp. " << std::endl;} %prec preInc
	| dec incDecOption {std::cout << "Syntax Object: exps. " << std::endl;} %prec preDec
	| incDecOption inc {std::cout << "Syntax Object: exps. " << std::endl;} %prec postInc
	| incDecOption dec {std::cout << "Syntax Object: exps. " << std::endl;} %prec postDec 
	| asgOptions {
$$ = $1;
std::cout << "Syntax Object: exp. " << std::endl;}
%prec asgOptionsPrec
	| call {std::cout << "Syntax Object: exp. " << std::endl;}
	| value {std::cout << "Syntax Object: exp. " << std::endl;}
	| '(' exp ')'  {std::cout << "Syntax Object: exps. " << std::endl;}

asgOptions:
	  nameV asg exp {
$$ = new AstVariableAsg(new AstName($1),$3);
std::cout << "Syntax Object: asgOptions." << std::endl;
}
	| nameV arrayReferenceExp asg exp {
$$ = new AstArrayAsg(new AstName($1),$4,$2);
std::cout << "Syntax Object: asgOptions." << std::endl;
}

	| nameV arrayReferenceExp asg '{' exps '}' {
$$ = new AstArrayListAsg(new AstName($1),$5,$2);
std::cout << "Syntax Object: asgOptions." << std::endl;
}

	| nameV '[' ']' asg '{' exps '}' {
$$ = new AstArrayListAsg(new AstName($1),$6,new AstIntValue(0));
std::cout << "Syntax Object: asgOptions." << std::endl;
}

incDecOption:
	  call {
$$ = $1;
std::cout << "Syntax Object: incDecOption. " << std::endl;}

	| value {
$$ = $1;
std::cout << "Syntax Object: incDecOption. " << std::endl;}
	| '(' exp ')' {
$$ = $2;
std::cout << "Syntax Object: incDecOption. " << std::endl;
}

%%

