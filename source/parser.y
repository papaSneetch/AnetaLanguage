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
%token aeq meq asg And Or
%token '[' ']' '{' '}'
%token ',' ';' '(' ')'
%token intV boolV floatV stringV nameV

%type <int_val> intV
%type <bool_val> boolV
%type <float_val> floatV

%left ','
%left Or 
%left Xor
%left And
%left eql neq
%left leq geq lt gt
%left leftSh rightSh
%left add sub
%left mul Div mod
%precedence inc dec

/* Grammar Definitions */
%%

stats:
	  stat stat

stat:
	  varDecl
	| arrayDecl
	| funcDecl
	| funcCall
	| condStat
	| asgStat
	| loop

condStat:
 	  ifStat
	| ifStat elseIfStat
	| ifStat elseStat

ifStat:
	  ifS '(' exps ')' '{' stats '}'

elseIfStat:
	  elseS ifS '(' exps ')' '{' stats '}' elseStat
    | elseS ifS '(' exps ')' '{' stats '}' elseIfStat
	| elseS ifS '(' exps ')' '{' stats '}'  

elseStat:
	  elseS '{' stats '}'

varDecl:
	  varDefine ';'
	| varDefine asg exps ';'

varDefine:
   	  type varNames

varNames:
	  varNames ',' nameV
	| nameV

type:
	  intD
	| floatD
	| boolD
	| stringD

funcDecl:
	  type nameV '(' argDecl ')'

argDecl:
	  type nameV ',' argDecl
	| type nameV

arrayDecl:
      arrayDefine ';'
	| arrayDefine asg listInits ';'

arrayDefine:
	  type arrayNames

arrayNames:
	  arrayNames ',' nameV '[' intV ']'
	| nameV '[' intV ']'

listInits:
	  listInits ',' listInit
	| listInit

listInit:
	  '{' exps '}'

funcCall:
	  nameV '(' exps ')' ';'

asgStat:
	  varCalls eql exps ';'

varCalls:
	  varCalls ',' arrayCall
	| varCalls ',' varCall
	| arrayCall
	| varCall

arrayCall:
	  nameV '[' intV ']'

varCall:
	  nameV

call:
	  varCall	  
	| funcCall
	| arrayCall

loop:
	  whileloop

whileloop:
	  whileS '(' exps ')' '{' stats '}'

exps:
      exps ',' exps
	| exps Or exps
	| exps Xor exps
	| exps And exps
	| exps leq exps
	| exps geq exps
	| exps lt exps
	| exps gt exps
	| exps eql exps
	| exps neq exps
	| exps leftSh exps
	| exps rightSh exps
	| exps add exps
	| exps sub exps
	| exps mul exps
	| exps Div exps
	| exps mod exps
	| inc exps
	| dec exps
	| call
	| '(' exps ')' 

%%

