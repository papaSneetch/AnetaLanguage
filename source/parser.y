%{
/* C/C++ code */

#include <iostream>
#include "lexer.yy.h"
#include "llvmHeaders.h"

#define YYPARSE_PARAM yyscan_t scanner
#define YYLEX_PARAM scanner

void yyerror (char const *s)
{
  fprintf (stderr, "%s\n", s);
}
%}

/*Token Declarations */

%require "3.7.3"
%define api.pure

%union{
int int_val;
bool bool_val;
float float_val;
char* string;
}

%token <AstNode> ifS elseS whileS returnS
%token <AstNode> stringD intD floatD boolD
%token <AstNode> leftSh rightSh add sub
%token <AstNode> mul Div pow Xor mod inc dec
%token <AstNode> eql leq geq lt gt neq
%token <AstNode> aeg meg asg And Or
%token '[' ']' '{' '}'
%token ',' ';' '(' ')'
%token <AstNode> intV boolV floatV stringV nameV

%type <int_val> intV
%type <bool_val> boolV
%type <float_val> floatV
%type <string> stringV
%type <string> nameV


%left ','
%right aeq meq asg
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
currentContext.codeObjects.push($1);
std::cout << "Syntax Object: prog. " << std::endl;
}
	| progs prog
{
currentContext.codeObjects.push($2);
std::cout << "Syntax Object: prog. " << std::endl;
}

prog:
	  funcDecl {
$$ = $1; 
std::cout << "Syntax Object: stat." << std::endl; }
	| varDecl ';' {
$$ = $1; $1->globalBool = true;
std::cout << "Syntax Object: stat. " << std::endl;}

stats:
	  stats stat {
$1 -> statements.push_back($2);
std::cout << "Syntax Object: stats. " << std::endl;}
	| stat {
$$ = new AstBlock(); $$->statements.push_back($1);
std::cout << "Created New Block. Syntax Objcet: stats. " << std::endl;

stat:
	  varDecl ';' {
$$ = $1;
std::cout << "Syntax Object: stat. " << std::endl;}
	| arrayDecl ';' {
$$ = $1;
std::cout << "Syntax Object: stat. " << std::endl;}
	| funcCall ';' {
$$ = $1;
std::cout << "Syntax Object: stat. " << std::endl;}
	| condStat {
$$ = $1;
std::cout << "Syntax Object: stat. " << std::endl;}
	| exps ';' {
$$ = $1;
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
$$ = new AstBlock(); $$->statements.push_back($1);
std::cout << "Syntax Object: elseIfStat. " << std::endl;}
	| elseS block {
$$ = $2;
std::cout << "Syntax Object: elseIfStat. " << std::endl};

varDecl:
	  varDefine  {
$$ = $1;
std::cout << "Syntax Object: varDecl. " << std::endl;}
	| varDefine asg exps  {
$$ = $1; $$->initializer=$3;
std::cout << "Syntax Object: varDecl. " << std::endl;}

varDefine:
   	  type varNames {
for (
$$ = new AstVariableDeclaration($2,$1);
std::cout << "Syntax Object: varDefine. " << std::endl;}

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
	  type nameV '(' argDecl ')' block ';' {
$$ = new AstFunctionDeclaration($1,$2,,$6);
std::cout << "Syntax Object: funcDecl. " << std::endl;}
	| type nameV '(' ')' block ';' {
$$ = new AstFunctionDeclaration($1,$2,variableList(),$6);
std::cout << "Syntax Object: funcDecl. " << std::endl;}

funcDef:
	| type nameV '(' argDecl ')' block {
$$ = new AstFunctionDeclaration($1,$2,$4,$6);
std::cout << "Syntax Object: funcDef. " << std::endl;}
	| type nameV '(' ')' block {
$$ = new AstFunctionDeclaration($1,$2,nullptr,$5);
std::cout << "Syntax Object: funcDef. " << std::endl;}
 
block:
	   '{' stats '}' {
$$ = $2;
std::cout << "Syntax Object: block. " << std::endl;}

argDecl:
	  argDecl ',' type nameV {
$1->push_back(AstVariableDeclaration($4,$3);
std::cout << "Syntax Object: argDecl. " << std::endl;}
	| type nameV {
$$ = new variableList(); $$->push_back($2,$1);
std::cout << "Syntax Object: argDecl. " << std::endl;}

arrayDecl:
      arrayDefine  {
$$ = $1;
std::cout << "Syntax Object: arrayDecl. " << std::endl;}
	| arrayDefine asg exps  {
$$ = $1; $1->initializer=$3;
std::cout << "Syntax Object: arrayDecl. " << std::endl;}

arrayDefine:
	  type arrayNames '[' intV ']' {
$$ = new AstArrayDeclaration($2,$1,$3);
std::cout << "Syntax Object: arrayDefine. " << std::endl;}

arrayNames:
	nameV {
$$ = new AstName(std::string($1));
std::cout << "Syntax Object: arrayNames. " << std::endl;}

funcCall:
	  nameV '(' exps ')' {
$$ = new AstFunctionCall($1,$3);
std::cout << "Syntax Object: listInit. " << std::endl;}

arrayCall:
	  nameV '[' intV ']' {
$$ = new AstArrayCall(std::string($1,$2),
std::cout << "Syntax Object: arrayCall. " << std::endl;}

varCall:
	  nameV {
$$ = AstVariableCall(std::string($1));
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
$$ = new AstIntValue(std::stoi($1);
std::cout << "Syntax Object: value. " << std::endl;
}
	| boolV {
$$ = new AstBoolValue($1 == "true");
$$ = new AstBoolValue()
std::cout << "Syntax Object: value. " << std::endl;
}
	| floatV {
$$ = new AstFloatValue(std::stof($1);
std::cout << "Syntax Object: value. " << std::endl;
}
	| stringV {
$$ = new AstStringValue(std::string($1));
std::cout << "Syntax Object: value. " << std::endl;

}

whileloop:
	  whileS '(' exp ')' block {
$$ = new AstWhileLoop($5,$3);
std::cout << "Syntax Object: whileloop. " << std::endl;}

exps:
      exps ',' exp {
$1 -> push_back($3);
std::cout << "Syntax Object: exps. " << std::endl;}
	| exp {
$$ = new expressionList(); $$->push_back($1);
std::cout << "Syntax Object: exps. " << std::endl;}

exp:
	| exp Or exp {
$$ = new AstOr($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp Xor exp {
$$ = new AstXor($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp And exp {
$$ = new AstAnd($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp leq exp {
$$ = new Astleq($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp geq exp {
$$ = new Astgeq($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp lt exp {
$$ = new Astlt($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp gt exp {
$$ = new Astgt($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp eql exp {
$$ = new Asteql($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp neq exp {
$$ = new Astneq($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp leftSh exp {
$$ = new AstLeftSh($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp rightSh exp {
$$ = new AstRightSh($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp add exp {
$$ = new AstAdd($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp sub exp {
$$ = new AstSub($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp mul exp {
$$ = new AstMul($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp Div exp {
$$ = new AstDiv($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp mod exp {
$$ = new AstMod($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| inc exp %prec preInc  {
std::cout << "Syntax Object: exp. " << std::endl;}
	| dec exp %prec preDec {std::cout << "Syntax Object: exps. " << std::endl;}
	| exp inc %prec postInc {std::cout << "Syntax Object: exps. " << std::endl;}
	| exp dec %prec postDec {std::cout << "Syntax Object: exps. " << std::endl;}
	| asgOptions
$$ = $1
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp aeg exp {
$$ = new AstAeg($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp meg exp {
$$ = new AstMeg($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| call {std::cout << "Syntax Object: exp. " << std::endl;}
	| value {printf ("Syntax Object: exp. " << std::endl;}
	| '(' exp ')'  {std::cout << "Syntax Object: exps. " << std::endl;}

asgOptions:
	  nameV asg exp {
$$ = new AstVariableAsg($1,$3);
std::cout << "Syntax Object: asgOptions." << std::endl;
}
	| nameV '[' intV ']' asg exp {
$$ = new AstArrayAsg($1,$6,$3);
std::cout << "Syntax Object: asgOptions." << std::endl;
}

	| nameV '[' intV ']' asg exps {
$$ = new AstArrayListAsg($1,$6,$3);
std::cout << "Syntax Object: asgOptions." << std::endl;
}

	| nameV '[' ']' asg exps {
$$ = new AstArrayListAsg($1,$5,AstIntValue(0));
std::cout << "Syntax Object: asgOptions." << std::endl;
}

%%

