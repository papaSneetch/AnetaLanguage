%{
/* C/C++ code to be copied */
#include <iostream>
%}
%option noyywrap

%%
if {std::cout << "Found an if token!" << std::endl;}
else {std::cout << "Found an else token" << std::endl;}
for {std::cout << "Found an for token" << std::endl;}
while {std::cout << "Found an while token" << std::endl;}
return {std::cout << "Found an return token" << std::endl;}

string {std::cout << "Found an string token" << std::endl;}
int {std::cout << "Found an int token" << std::endl;}
float {std::cout << "Found an float token" << std::endl;}
bool {std::cout << "Found an bool token" << std::endl;}

[ {std::cout << "Found an [ token" << std::endl;}
] {std::cout << "Found an ] token" << std::endl;}
{ {std::cout << "Found an { token" << std::endl;}
} {std::cout << "Found an } token" << std::endl;}
( {std::cout << "Found an ( token" << std::endl;}
) {std::cout << "Found an ) token" << std::endl;}
<< {std::cout << "Found an << token" << std::endl;}
>> {std::cout << "Found an >> token" << std::endl;}

+ {std::cout << "Found an + token" << std::endl;}
- {std::cout << "Found an - token" << std::endl;}
* {std::cout << "Found an * token" << std::endl;}
\ {std::cout << "Found an \ token" << std::endl;}
^ {std::cout << "Found an ^ token" << std::endl;}
% {std::cout << "Found an % token" << std::endl;}
++ {std::cout << "Found an ++ token" << std::endl;}
-- {std::cout << "Found an -- token" << std::endl;}

== {std::cout << "Found an == token" << std::endl;}
+= {std::cout << "Found an += token" << std::endl;}
-= {std::cout << "Found an -= token" << std::endl;}
= {std::cout << "Found an = token" << std::endl;}
&& {std::cout << "Found an && token" << std::endl;}
| {std::cout << "Found an | token" << std::endl;}
& {std::cout << "Found an & token" << std::endl;}
, {std::cout << "Found an , token" << std::endl;}
; {std::cout << "Found an ; token" << std::endl;}
! {std::cout << "Found an ! token" << std::endl;}

true {std::cout << "Found an true token" << std::endl;}
false {std::cout << "Found an false token" << std::endl;}

[0-9]+ {std::cout << "Found an integer token" << std::endl;}
[0-9]+\.[0-9]+ {std::cout << "Found an float token" << std::endl;}
[a-zA-Z][a-zA-Z0-9_]* {std::cout << "Found an name token" << std::endl;}
\".\" {std::cout << "Found an string token" << std::endl;}


%%

int main()
{
 yylex();
 return 0;
}

