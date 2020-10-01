%{
/* C/C++ code */
#include <iostream>
#include <string>
%}

/*Token Declarations */

%union{
int int_val;
bool bool_val;
float float_val;
string* name_val;
string* string_val;
}

%token ifS elseS whileS returnS
%token stringD intD floatD boolD
%token leftSh rightSh add sub
%token mul div pow xor mod inc dec
%token eql leq geq lt gt neq
%token aeq meq asg and or
%token '[' ']' '{' '}'
%token ',' ';' '(' ')'
%token intV boolV floatV stringV
%token intID boolID floatID stringID funcID arrayID

%type <int_val> intV
%type <bool_val> boolV
%type <float_val> floatV
%type <string_val> stringV
%type <name_val> intID boolID floatID stringID arrayID


/* Grammar Definitions */
%%

stats:
	  stat stat

stat:
	  varDecl
	| arrayDecl
	| funcCall
	| condStat
	| asgStat
	| loop

condStat:
 	  ifStat
	| ifStat postIfStat

postIfStat:
	  elseIfStat
	| elseStat

ifStat:
	  ifS '(' logexp ')' '{' stats '}'

elseIfStat:
	  elseS ifS '(' logexp ')' '{' stats '}'

elseStat:
	  elseS '(' logexp ')' '{' stats '}'

varDecl:
	  varDefine ';'
	| varDefine asg exps ';'

varDefine:
   	  type varNames

varNames:
	  varNames ',' varName

varName:
	  intID
	| floatID
	| boolID
	| stringID

type:
	  intD
	| floatD
	| boolD
	| stringD
arrayDecl:
      arrays ';'
	| arrays asg listInits ';'

arrays:
	  type arrayIDs

arrayIDs:
	  arrayIDs ',' array '[' posInt ']'
	| arrayID '[' posInt ']'

listInits:
	  listInits ',' listInit
	| listInit

listInit:
	  '{' exps '}'

funcCall:
	  funcID '(' exps ')' ';'

asgStat:
	  varCalls eql exps ';'

varCalls:
	  varCalls ',' arrayCall
	| varCalls ',' varCall
	| arrayCall
	| varCall

arrayCall:
	  arrayID '[' posInt ']'

varCall:
	  intID
	| floatID
	| boolID
	| stringID
	| arrayID 

shift:
	  leftSh 
	| rightSh

loop:
	  whileloop

whileloop:
	  whileS '(' logexp ')' '{' stats '}'

exps:
      exp0 ',' exps
	| exp0

exp0:
	  exp1
	| logexp1

exp1:
	  exp1 or exp2
	| exp2

exp2:
	  exp2 xor exp3
	| exp3

exp3:
	  exp3 and exp4
	| exp4

exp4:
	  exp4 logcmp exp5
	| exp5

exp5:
	  exp5 cmp exp6
	| exp6

exp6:
	  exp6 shift exp7
	| exp7

exp7:
	  exp7 addsub exp8
	| exp8

exp8:
	  exp8 muldivmod exp9
	| exp9

exp9:
	  unary exp10
	| exp10

exp10:
	  exp10 pow exp11
	| exp11

exp11:
	  varCall
	| exp12	

exp12:
	  '(' exp1 ')' 

logexp:
	  logexp1
	 

logexp1:
	  logexp1 or logexp2
	| logexp2

logexp2:
	  logexp2 xor logexp3
	| logexp3

logexp3:
	  logexp3 and logexp4
	| logexp4

logexp4:
	  logexp4 eql logexp5
	| logexp5

logexp5:
	  boolID

unary:
	  inc
	| dec

addsub:
	  add
	| sub

muldivmod:
	  mul
	| div
	| mod

cmp:
      eql
	| neq

logcmp:
	  leq
	| geq
	| lt
	| gt

posInt:
	  intV
	| intID

array:


%%

int main()
{
 	yyparse();
	return 0;
}
