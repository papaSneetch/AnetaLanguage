%{
/* C/C++ code to be copied */
#include <stdio.h>
#include <iostream>
#include "parser.tab.h"
%}

%option noyywrap
%option bison-bridge

%{
/* Lex Statements/Rules */
%}

%%

if {
return ifS;
}

else {
return elseS;
}

while {
return whileS;
}

return {
return returnS;
}

string {
return stringD;
}

int {
return intD;
}

float {
return floatD;
}

bool {
return boolD;
}

char {
return charD;
}

"[" {
return '[';
}

"]" {
return ']';
}

"{" {
return '{';
}

"}" {
return '}';
}

"(" {
return '(';
}

")" {
return ')';
}

"<<" {
return leftSh;
}

">>" {
return rightSh;
}


"+" {
return add;
}

"-" {
return sub;
}

"*" {
return ast;
}

"/" {
return Div;
}

"^^" {
return exponent;
}

"^" {
return Xor;
}

"%" {
return mod;
}

"++" {
return inc;
}

"--" {
return dec;
}

"==" {
return eql;
}

"<=" {
return leq;
}

">=" {
return geq;
}

"<" {
return lt;
}

">" {
return gt;
}

"!=" {
return neq;
}


"+=" {
return aeg;
}

"-=" {
return meg;
}

"=" {
return asg;
}

"&" {
return amp;
}

"|" {
return Or;
}

"," {
return ',';
}

";" {
return ';';
}

true {
yylval-> bool_val = true;
return boolV;
}

false {
yylval-> bool_val = false;
return boolV;
}


[0-9]+ {
yylval->int_val = atoi(yytext);
return intV;
}

[0-9]+\.[0-9]+ {
yylval->float_val = atof(yytext);
return floatV;
}

[a-zA-Z][a-zA-Z0-9_]* {
yylval -> string = strdup(yytext);
return nameV;
}

\"[^\n"]+\"  {
yylval -> string = strdup(yytext);
return charsV;
}

\'[^\n"]+\'  {
yylval -> string = strdup(yytext);
return charsV;
}

%%
