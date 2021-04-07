%{
/* C/C++ code to be copied */
#include <stdio.h>
#include <iostream>
#include "parser.tab.h"

const int maxStringSize = 4096;
static char string_buf [maxStringSize];  
static char* string_buf_ptr;
%}

%option noyywrap
%option bison-bridge

%x STRING
%x CHARSTRING
%x ESC

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

\" {
BEGIN(STRING);
stringPointer = string_buf;
yylval -> string = strdup(yytext);
return charsV;
}

\' {
BEGIN(CHARSTRING);
stringPointer = string_buf;
yylval -> string = strdup(yytext);
return charsV;
}

<STRING>{

[^\\\n\"]* {
*string_buf_ptr++ = yytext;
}

\" {
*string_buf_ptr = '\0' 
yylval -> string = strdup(string_buf);
return stringV;
}

\\a {
*string_buf_ptr = '\a' 
}

\\b {
*string_buf_ptr = '\b' 
}

\\f {
*string_buf_ptr = '\f'
}

\\n {
*string_buf_ptr = '\n'
}

\\r {
*string_buf_ptr = '\r'
}

\\t {
*string_buf_ptr = '\t'
}

\\v {
*string_buf_ptr = '\v'
}

\\\\  {
*string_buf_ptr = '\\'
}

\\\' {
*string_buf_ptr = '\''
}

\\\" {
*string_buf_ptr = '\"'
}

\\? {
*string_buf_ptr = '\?'
}


\\0 {
*string_buf_ptr = '\0'
}

}

<CHARSTRING>{

\' {
yylval -> string = strdup(string_buf);
return charsV;
}

[^\\\n\"]* {
*string_buf_ptr++ = yytext;
}

\\a {
*string_buf_ptr = '\a' 
}

\\b {
*string_buf_ptr = '\b' 
}

\\f {
*string_buf_ptr = '\f'
}

\\n {
*string_buf_ptr = '\n'
}

\\r {
*string_buf_ptr = '\r'
}

\\t {
*string_buf_ptr = '\t'
}

\\v {
*string_buf_ptr = '\v'
}

\\\\  {
*string_buf_ptr = '\\'
}

\\\' {
*string_buf_ptr = '\''
}

\\\" {
*string_buf_ptr = '\"'
}

\\? {
*string_buf_ptr = '\?'
}

\\0 {
*string_buf_ptr = '\0'
}

}


%%
