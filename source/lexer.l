%{
/* C/C++ code to be copied */
#include <iostream>
%}
%option noyywrap

%{
/* Lex Statements/Rules */
%}

%%
if {std::cout << "Found a if token!" << std::endl;}
else {std::cout << "Found a else token" << std::endl;}
for {std::cout << "Found a for token" << std::endl;}
while {std::cout << "Found a while token" << std::endl;}
return {std::cout << "Found a return token" << std::endl;}

string {std::cout << "Found a string token" << std::endl;}
int {std::cout << "Found a int token" << std::endl;}
float {std::cout << "Found a float token" << std::endl;}
bool {std::cout << "Found a bool token" << std::endl;}

[ {std::cout << "Found a [ token" << std::endl;}
] {std::cout << "Found a ] token" << std::endl;}
{ {std::cout << "Found a { token" << std::endl;}
} {std::cout << "Found a } token" << std::endl;}
( {std::cout << "Found a ( token" << std::endl;}
) {std::cout << "Found a ) token" << std::endl;}
<< {std::cout << "Found a << token" << std::endl;}
>> {std::cout << "Found a >> token" << std::endl;}

+ {std::cout << "Found a + token" << std::endl;}
- {std::cout << "Found a - token" << std::endl;}
* {std::cout << "Found a * token" << std::endl;}
\ {std::cout << "Found a \ token" << std::endl;}
^ {std::cout << "Found a ^ token" << std::endl;}
% {std::cout << "Found a % token" << std::endl;}
++ {std::cout << "Found a ++ token" << std::endl;}
-- {std::cout << "Found a -- token" << std::endl;}

== {std::cout << "Found a == token" << std::endl;}
+= {std::cout << "Found a += token" << std::endl;}
-= {std::cout << "Found a -= token" << std::endl;}
= {std::cout << "Found a = token" << std::endl;}
&& {std::cout << "Found a && token" << std::endl;}
| {std::cout << "Found a | token" << std::endl;}
& {std::cout << "Found a & token" << std::endl;}
, {std::cout << "Found a , token" << std::endl;}
; {std::cout << "Found a ; token" << std::endl;}
! {std::cout << "Found a ! token" << std::endl;}

true {std::cout << "Found a true token" << std::endl;}
false {std::cout << "Found a false token" << std::endl;}

[0-9]+ {std::cout << "Found a integer token" << std::endl;}
[0-9]+\.[0-9]+ {std::cout << "Found a float token" << std::endl;}
[a-zA-Z][a-zA-Z0-9_]* {std::cout << "Found a name token" << std::endl;}
\".\" {std::cout << "Found a string token" << std::endl;}


%%

int main()
{
 yylex();
 return 0;
}

