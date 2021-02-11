%{
/* C/C++ code */

#include <stdio.h>
#include <stdbool.h>
#include "lexer.yy.h"

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
}

%token ifS elseS whileS returnS
%token stringD intD floatD boolD
%token leftSh rightSh add sub
%token mul Div pow Xor mod inc dec
%token eql leq geq lt gt neq
%token aeg meg asg And Or
%token '[' ']' '{' '}'
%token ',' ';' '(' ')'
%token intV boolV floatV stringV nameV

%type <int_val> intV
%type <bool_val> boolV
%type <float_val> floatV

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

stats:
	  stats stat {printf("Syntax Object: stats. \n");}
	| stat

stat:
	  varDecl ';' {printf("Syntax Object: stat. \n");}
	| arrayDecl ';' {printf("Syntax Object: stat. \n");}
	| funcDecl {printf("Syntax Object: stat. \n");}
	| funcCall ';' {printf("Syntax Object: stat. \n");}
	| condStat {printf("Syntax Object: stat. \n");}
	| exps ';' {printf("Syntax Object: stat. \n");}
	| whileloop {printf("Syntax Object: stat. \n");}
	| returnStat ';' {printf("Syntax Object: exps. \n");} 

condStat:
 	  ifStat {printf("Syntax Object: condStat. \n");}
	| ifStat elseIfStat {printf("Syntax Object: condStat. \n");}
	| ifStat elseStat {printf("Syntax Object: condStat. \n");}

ifStat:
	  ifS '(' exps ')' '{' stats '}' {printf("Syntax Object: ifStat. \n");}

elseIfStat:
	  elseS ifS '(' exps ')' '{' stats '}' elseStat {printf("Syntax Object: elseIfStat. \n");}
    | elseS ifS '(' exps ')' '{' stats '}' elseIfStat {printf("Syntax Object: elseIfStat. \n");}
	| elseS ifS '(' exps ')' '{' stats '}'   {printf("Syntax Object: elseIfStat. \n");}

elseStat:
	  elseS '{' stats '}' {printf("Syntax Object: elseStat. \n");}

varDecl:
	  varDefine  {printf("Syntax Object: varDecl. \n");}
	| varDefine asg exps  {printf("Syntax Object: varDecl. \n");}

varDefine:
   	  type varNames {printf("Syntax Object: varDefine. \n");}

varNames:
	  varNames ',' nameV {printf("Syntax Object: varNames. \n");}
	| nameV {printf("Syntax Object: varNames. \n");}

type:
	  intD {printf("Syntax Object: type. \n");}
	| floatD {printf("Syntax Object: type. \n");}
	| boolD {printf("Syntax Object: type. \n");}
	| stringD {printf("Syntax Object: type. \n");}

funcDecl:
	  type nameV '(' argDecl ')' ';' {printf("Syntax Object: funcDecl. \n");}
	| type nameV '(' ')' ';' {printf("Syntax Object: funcDecl. \n");}
	| funcDef {printf("Syntax Object: funcDecl. \n");} 

funcDef:
	| type nameV '(' argDecl ')' '{' stats '}' {printf("Syntax Object: funcDef. \n");}
	| type nameV '(' ')' '{' stats '}' {printf("Syntax Object: funcDef. \n");}
 

argDecl:
	  type nameV ',' argDecl {printf("Syntax Object: argDecl. \n");}
	| type nameV {printf("Syntax Object: argDecl. \n");}

arrayDecl:
      arrayDefine  {printf("Syntax Object: arrayDecl. \n");}
	| arrayDefine asg exps  {printf("Syntax Object: arrayDecl. \n");}

arrayDefine:
	  type arrayNames '[' intV ']' {printf("Syntax Object: arrayDefine. \n");}

arrayNames:
	  arrayNames ',' nameV {printf("Syntax Object: arrayNames. \n");}
	| nameV {printf("Syntax Object: arrayNames. \n");}

funcCall:
	  nameV '(' exps ')' {printf("Syntax Object: listInit. \n");}

varCalls:
	  varCalls ',' arrayCall {printf("Syntax Object: varCalls. \n");}
	| varCalls ',' varCall {printf("Syntax Object: varCalls. \n");}
	| arrayCall {printf("Syntax Object: varCalls. \n");}
	| varCall {printf("Syntax Object: varCall. \n");}

arrayCall:
	  nameV '[' intV ']' {printf("Syntax Object: arrayCall. \n");}

varCall:
	  nameV {printf("Syntax Object: varCall. \n");}

returnStat:
	  returnS exps

call:
	  varCall	   {printf("Syntax Object: call. \n");}
	| funcCall {printf("Syntax Object: call. \n");}
	| arrayCall {printf("Syntax Object: call. \n");}

value:
	  intV {printf("Syntax Object: value. \n");}
	| boolV {printf("Syntax Object: value. \n");}
	| floatV {printf("Syntax Object: value. \n");}
	| stringV {printf("Syntax Object: value. \n");}


whileloop:
	  whileS '(' exps ')' '{' stats '}' {printf("Syntax Object: whileloop. \n");}

exps:
      exps ',' exps {printf("Syntax Object: exps. \n");}
	| exps Or exps {printf("Syntax Object: exps. \n");}
	| exps Xor exps {printf("Syntax Object: exps. \n");}
	| exps And exps {printf("Syntax Object: exps. \n");}
	| exps leq exps {printf("Syntax Object: exps. \n");}
	| exps geq exps {printf("Syntax Object: exps. \n");}
	| exps lt exps {printf("Syntax Object: exps. \n");}
	| exps gt exps {printf("Syntax Object: exps. \n");}
	| exps eql exps {printf("Syntax Object: exps. \n");}
	| exps neq exps {printf("Syntax Object: exps. \n");}
	| exps leftSh exps {printf("Syntax Object: exps. \n");}
	| exps rightSh exps {printf("Syntax Object: exps. \n");}
	| exps add exps {printf("Syntax Object: exps. \n");}
	| exps sub exps {printf("Syntax Object: exps. \n");}
	| exps mul exps {printf("Syntax Object: exps. \n");}
	| exps Div exps {printf("Syntax Object: exps. \n");}
	| exps mod exps {printf("Syntax Object: exps. \n");}
	| inc exps %prec preInc  {printf("Syntax Object: exps. \n");}
	| dec exps %prec preDec {printf("Syntax Object: exps. \n");}
	| exps inc %prec postInc {printf("Syntax Object: exps. \n");}
	| exps dec %prec postDec {printf("Syntax Object: exps. \n");}
	| exps asg exps {printf("Syntax Object: exps. \n");}
	| exps aeg exps {printf("Syntax Object: exps. \n");}
	| exps meg exps {printf("Syntax Object: exps. \n");}
	| call {printf("Syntax Object: exps. \n");}
	| value {printf ("Syntax Object: exps. \n");}
	| '(' exps ')'  {printf("Syntax Object: exps. \n");}

%%

