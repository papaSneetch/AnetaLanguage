%{
/* C/C++ code to be copied */
#include <stdio.h>
#include "parser.tab.h"

%}

%option noyywrap
%option bison-bridge

%{
/* Lex Statements/Rules */
%}

%%

if {
printf( "Found a if token." );
printf("\n"); 
return ifS;
}

else {
printf( "Found a else token." );
printf("\n");
return elseS;
}

while {
printf( "Found a while token." );
printf("\n"); 
return whileS;
}

return {
printf( "Found a return token." );
printf("\n"); 
return returnS;
}

string {
printf( "Found a string token." );
printf("\n"); 
return stringD;
}

int {
printf( "Found a int token." );
printf("\n"); 
return intD;
}

float {
printf( "Found a float token." );
printf("\n"); 
return floatD;
}

bool {
printf( "Found a bool token." );
printf("\n"); 
return boolD;
}

"[" {
printf( "Found a [ token." );
printf("\n"); 
return '[';
}

"]" {
printf( "Found a ] token." );
printf("\n"); 
return ']';
}

"{" {
printf( "Found a { token." );
printf("\n"); 
return '{';
}

"}" {
printf( "Found a } token." );
printf("\n"); 
return '}';
}

"(" {
printf( "Found a ( token." );
printf("\n"); 
return '(';
}

")" {
printf( "Found a ) token." );
printf("\n"); 
return ')';
}

"<<" {
printf( "Found a << token." );
printf("\n"); 
return leftSh;
}

">>" {
printf( "Found a >> token." );
printf("\n"); 
return rightSh;
}


"+" {
printf( "Found a + token." );
printf("\n"); 
return add;
}

"-" {
printf( "Found a - token." );
printf("\n"); 
return sub;
}

"*" {
printf( "Found a * token." );
printf("\n"); 
return mul;
}

"/" {
printf( "Found a / token." );
printf("\n"); 
return Div;
}

"^^" {
printf( " Found a ^^ token." );
printf("\n"); 
return pow;
}

"^" {
printf( "Found a ^ token." );
printf("\n"); 
return Xor;
}

"%" {
printf( "Found a %% token." );
printf("\n"); 
return mod;
}

"++" {
printf( "Found a ++ token." );
printf("\n"); 
return inc;
}

"--" {
printf( "Found a -- token." );
printf("\n"); 
return dec;
}

"==" {
printf( "Found a == token." );
printf("\n"); 
return eql;
}

"<=" {
printf( "Found a <= token." );
printf("\n"); 
return leq;
}

">=" {
printf( "Found a >= token." );
printf("\n"); 
return geq;
}

"<" {
printf( "Found a < token." );
printf("\n"); 
return lt;
}

">" {
printf( "Found a > token." );
printf("\n"); 
return gt;
}

"!=" {
printf( "Found a != token." );
printf("\n"); 
return neq;
}


"+=" {
printf( "Found a += token." );
printf("\n"); 
return aeq;
}

"-=" {
printf( "Found a -= token." );
printf("\n"); 
return meq;
}

"=" {
printf( "Found a = token." );
printf("\n"); 
return asg;
}

"&&" {
printf( "Found a && token." );
printf("\n"); 
return And;
}

"|" {
printf( "Found a | token." );
printf("\n"); 
return Or;
}

"," {
printf( "Found a , token." );
printf("\n"); 
return ',';
}

";" {
printf( "Found a ; token." );
printf("\n"); 
return ';';
}

true {
yylval-> bool_val = true;
printf( "Found a bool Value token." );
printf("\n"); 
return boolV;
}

false {
yylval-> bool_val = false;
printf( "Found a bool Value token." );
printf("\n"); 
return boolV;
}


[0-9]+ {
yylval->int_val = atoi(yytext);
printf( "Found a intValue token." );
printf("\n"); 
return intV;
}

[0-9]+\.[0-9]+ {
yylval->float_val = atof(yytext);
printf( "Found a floatValue token." );
printf("\n"); 
return floatV;
}

[a-zA-Z][a-zA-Z0-9_]* {
printf( "Found a name token." );
printf("\n"); 
return nameV;
}

\"[^\n"]+\"  {
printf( "Found a stringValue token." );
printf("\n"); 
return stringV;
}


%%
