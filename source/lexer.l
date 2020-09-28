%{
/* C/C++ code to be copied */
#include <iostream>
%}
%option noyywrap

%{
/* Lex Statements/Rules */
%}

%%

if {
std::cout << "Found a if token." << std::endl;
return ifS;
}

else {
std::cout << "Found a else token." << std::endl;
return elseS;
}

while {
std::cout << "Found a while token." << std::endl;
return whileS;
}

return {
std::cout << "Found a return token." << std::endl;
return returnS;
}


string {
std::cout << "Found a string token." << std::endl;
return stringD;
}

int {
std::cout << "Found a int token." << std::endl;
return intD;
}

float {
std::cout << "Found a float token." << std::endl;
return floatD;
}

bool {
std::cout << "Found a bool token." << std::endl;
return boolD;
}


"[" {
std::cout << "Found a [ token." << std::endl;
return '[';
}

"]" {
std::cout << "Found a ] token." << std::endl;
return ']';
}

"{" {
std::cout << "Found a { token." << std::endl;
return '{';
}

"}" {
std::cout << "Found a } token." << std::endl;
return '}';
}

"(" {
std::cout << "Found a ( token." << std::endl;
return '(';
}

")" {
std::cout << "Found a ) token." << std::endl;
return ')';
}

"<<" {
std::cout << "Found a << token." << std::endl;
return leftSh;
}

">>" {
std::cout << "Found a >> token." << std::endl;
return rightSh;
}


"+" {
std::cout << "Found a + token." << std::endl;
return add;
}

"-" {
std::cout << "Found a - token." << std::endl;
return sub
}

"*" {
std::cout << "Found a * token." << std::endl;
return mul;
}

"/" {
std::cout << "Found a / token." << std::endl;
return div;
}

"^^" {
std::cout << " Found a ^^ token." << std::endl;
return pow;

"^" {
std::cout << "Found a ^ token." << std::endl;
return xor;
}

"%" {
std::cout << "Found a % token." << std::endl;
return mod;
}

"++" {
std::cout << "Found a ++ token." << std::endl;
return inc;
}

"--" {
std::cout << "Found a -- token." << std::endl;
return dec;
}


"==" {
std::cout << "Found a == token." << std::endl;
return eq;
}

"<=" {
std::cout << "Found a <= token." << std::endl;
return leq;
}

">=" {
std::cout << "Found a >= token." << std::endl;
return geq;
}

"<" {
std::cout << "Found a < token." << std::endl;
return lt;
}

">" {
std::cout << "Found a > token." << std::endl;
return gt;
}

"!=" {
std::cout << "Found a != token." << std::endl;
return neq;
}


"+=" {
std::cout << "Found a += token." << std::endl;
return aeq;
}

"-=" {
std::cout << "Found a -= token." << std::endl;
return meq;
}

"=" {
std::cout << "Found a = token." << std::endl;
return asg;
}

"&&" {
std::cout << "Found a && token." << std::endl;
return and;
}

"|" {
std::cout << "Found a | token." << std::endl;
return or;
}

"," {
std::cout << "Found a , token." << std::endl;
return ',';
}

";" {
std::cout << "Found a ; token." << std::endl;
return ';';
}

"!" {
std::cout << "Found a ! token." << std::endl;
return nop;
}


true {
std::cout << "Found a bool Value token." << std::endl;
return boolV;
}

false {
std::cout << "Found a bool Value token." << std::endl;
return boolV;
}


[0-9]+ {
std::cout << "Found a intValue token." << std::endl;
return intV;
}

[0-9]+\.[0-9]+ {
std::cout << "Found a floatValue token." << std::endl;
return floatV;
}

[a-zA-Z][a-zA-Z0-9_]* {
std::cout << "Found a name token." << std::endl;
return nameV;
}

\"[^\n"]+\"  {
std::cout << "Found a stringValue token." << std::endl;
return stringV;
}


%%

int main()
{
 yylex();
 return 0;
}

