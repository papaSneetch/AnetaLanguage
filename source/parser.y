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

%type <int_val> intV
%type <bool_val> boolV
%type <float_val> floatV
%type <name_val> nameV
%type <string_val> stringV


/* Grammar Definitions */
%%

stats:
	  stat stat
	| varDecl
	| arrayDecl
	| funcCall
	| condStat
	| asnStat
	| loop

condStat:
 	  ifStat
	| ifStat postIfStat

branch:
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
	| varDevine asg exps ';'

varDefine:
   	  type varNames

varNames:
	  varNames ',' varName

varName:
	  nameV 

arrayDecl:
      arrays ';'
	| arrays asg listInits ';'

arrays:
	  type arrayNames

arrayNames:
	  arrayNames ',' array '[' posInt ']'
	| array '[' posInt ']'

listInits:
	  listInits ',' listInit
	| listInit

listInit:
	  '{' exps '}'

funcCall:
	  funcId '(' exps ')' ';'

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
	  varID

loop:
	  whileloop

whileloop:
	  whileS '(' logexp ')' '{' stats '}'

exps:
      exp1 ',' exps
	| exp1

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
	  boolID
	| exp6


%%

int main()
{
 	yyparse();
	return 0;
}
