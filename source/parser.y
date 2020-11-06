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
	| progs prog

prog:
	  funcDecl { std::cout << "Syntax Object: stat." << std::endl; }
	| arrayDecl ';' {std::cout << "Syntax Object: stat. " << std::endl;}
	| varDecl ';' {std::cout << "Syntax Object: stat. " << std::endl;}

stats:
	  stats stat {
$1 -> statements.push_back($2);
std::cout << "Syntax Object: stats. " << std::endl;}
	| stat {
$$ = new AstBlock(); $$->statements.push_back($1);
std::cout << "Created New Block. Syntax Objcet: stats. " << std::endl;

stat:
	  varDecl ';' {std::cout << "Syntax Object: stat. " << std::endl;}
	| arrayDecl ';' {std::cout << "Syntax Object: stat. " << std::endl;}
	| funcCall ';' {std::cout << "Syntax Object: stat. " << std::endl;}
	| condStat {std::cout << "Syntax Object: stat. " << std::endl;}
	| exps ';' {std::cout << "Syntax Object: stat. " << std::endl;}
	| whileloop {std::cout << "Syntax Object: stat. " << std::endl;}
	| returnStat ';' {std::cout << "Syntax Object: exps. " << std::endl;} 

condStat:
 	  ifStat {std::cout << "Syntax Object: condStat. " << std::endl;}
	| ifStat elseIfStat {std::cout << "Syntax Object: condStat. \" << std::endl;}
	| ifStat elseStat {std::cout << "Syntax Object: condStat. " << std::endl;}

ifStat:
	  ifS '(' exps ')' block {
$$ = new
std::cout << "Syntax Object: ifStat. " << std::endl;
   
}

elseIfStat:
	  elseS ifStat elseStat {std::cout << "Syntax Object: elseIfStat. " << std::endl;}
    | elseS ifStat block elseIfStat {std::cout << "Syntax Object: elseIfStat. " << std::endl;}
	| elseS ifStat block  {std::cout << "Syntax Object: elseIfStat. " << std::endl;}

elseStat:
	  elseS '{' stats '}' {std::cout << "Syntax Object: elseStat. " << std::endl;}

varDecl:
	  varDefine  {std::cout << "Syntax Object: varDecl. " << std::endl;}
	| varDefine asg exps  {std::cout << "Syntax Object: varDecl. " << std::endl;}

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
	| type nameV '(' argDecl ')' block {std::cout << "Syntax Object: funcDef. " << std::endl;}
	| type nameV '(' ')' block {std::cout << "Syntax Object: funcDef. " << std::endl;}
 
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
      arrayDefine  {std::cout << "Syntax Object: arrayDecl. " << std::endl;}
	| arrayDefine asg exps  {std::cout << "Syntax Object: arrayDecl. " << std::endl;}

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
	  returnS exps

call:
	  varCall	   {std::cout << "Syntax Object: call. " << std::endl;}
	| funcCall {std::cout << "Syntax Object: call. " << std::endl;}
	| arrayCall {std::cout << "Syntax Object: call. " << std::endl;}

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
	  whileS '(' exps ')' block {
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
	| exp Or exps {
$$ = new AstOr($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp Xor exps {
$$ = new AstXor($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp And exps {
$$ = new AstAnd($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp leq exps {
$$ = new Astleq($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp geq exps {
$$ = new Astgeq($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp lt exps {
$$ = new Astlt($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp gt exps {
$$ = new Astgt($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp eql exps {
$$ = new Asteql($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp neq exps {
$$ = new Astneq($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp leftSh exps {
$$ = new AstLeftSh($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp rightSh exps {
$$ = new AstRightSh($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp add exps {
$$ = new AstAdd($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp sub exps {
$$ = new AstSub($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp mul exps {
$$ = new AstMul($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp Div exps {
$$ = new AstDiv($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp mod exps {
$$ = new AstMod($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| inc exp %prec preInc  {
std::cout << "Syntax Object: exp. " << std::endl;}
	| dec exp %prec preDec {std::cout << "Syntax Object: exps. " << std::endl;}
	| exp inc %prec postInc {std::cout << "Syntax Object: exps. " << std::endl;}
	| exp dec %prec postDec {std::cout << "Syntax Object: exps. " << std::endl;}
	| exp asg exps {
$$ = new AstAsg($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp aeg exps {
$$ = new AstAeg($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| exp meg exps {
$$ = new AstMeg($1,$2);
std::cout << "Syntax Object: exp. " << std::endl;}
	| call {std::cout << "Syntax Object: exp. " << std::endl;}
	| value {printf ("Syntax Object: exp. " << std::endl;}
	| '(' exp ')'  {std::cout << "Syntax Object: exps. " << std::endl;}

%%

