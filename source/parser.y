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
%token eq leq geq lt gt neq
%token aeq meq asg and or
%token '[' ']' '{' '}'
%token ',' ';'

%type <int_val> intV
%type <bool_val> boolV
%type <float_val> floatV
%type <name_val> nameV
%type <string_val> stringV


/* Grammar Definitions */
%%
%%

int main()
{
 	yyparse();
	return 0;
}
