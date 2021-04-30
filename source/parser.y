%{
/* C/C++ code */

#include <iostream>
#include "lexer.yy.h"
#include <codeGenContext.h>

#define YYPARSE_PARAM yyscan_t scanner
#define YYLEX_PARAM scanner

void yyerror (char const *s)
{
  fprintf (stderr, "%s\n", s);
}
%}

%code requires {
#include "annetaBuilder.h"
}

/*Token Declarations */

%require "3.7.3"
%define api.pure

%union
{
int int_val;
bool bool_val;
double float_val;
char* string;
AstNode* node;
AstStat* statement;
AstConstant* constant;
AstExp* expression;
AstIntValue* intPtr;
AstBlock* blockPtr;
AstVariableDeclaration* varDecl;
AstGlobalVariableDeclaration* globalVarDecl;
AstArrayDeclaration* arrayDecl;
AstName* name;
const AstType* type;
statementList* statements;
expressionList* expressions;
variableList* args;
}

%token ifS elseS whileS returnS at
%token intD floatD boolD charD stringD
%token leftSh rightSh add sub
%token ast Div exponent Xor mod 
%token eql leq geq lt gt neq
%token aeg meg asg amp Or
%token '[' ']' '{' '}'
%token ',' ';' '(' ')'
%token intV boolV floatV charsV nameV stringV

%type <node> prog funcDecl 

%type <statements> stats 
%type <statement> stat condStat returnStat whileloop arrayDecl 
%type <expression> call varCall arrayCall funcCall 
%type <expression> exp arrayReferenceExp deref address arrayAsg asgOptions
%type <constant> value 

%type <varDecl> varDecl
%type<globalVarDecl> globalVarDecl

%type <name> varNames 

%type <type> type baseType

%type <blockPtr> block condElseBlock

%type <int_val> intV astChain 
%type <bool_val> boolV
%type <float_val> floatV
%type <string> charsV stringV
%type <string> nameV

%type <expressions> exps
%type <args> argDecl funcArgs

%left ','
%right asgOptionsPrec asg
%left Or 
%left Xor
%left amp
%left eql neq
%left leq geq lt gt
%left leftSh rightSh
%left add sub
%left ast Div mod
%right at derefPrec addressPrec preInc preDec
%left postInc postDec callPrec

/* Grammar Definitions */
%%

progs:
	  prog
{
currentContext.pushAstNode($1);
std::cerr << "Syntax Object: prog. " << std::endl;
}
	| progs prog
{
currentContext.pushAstNode($2);
std::cerr << "Syntax Object: prog. " << std::endl;
}

prog:
	  funcDecl {
$$ = $1; 
std::cerr << "Syntax Object: stat." << std::endl; }
	| globalVarDecl ';' {
$$ = $1; 
std::cerr << "Syntax Object: stat. " << std::endl;}

stats:
	  stats stat {
$$ = $1; $$->emplace_back($2);
std::cerr << "Syntax Object: stats. " << std::endl;}
	| stat {
$$ = new statementList(); $$->emplace_back($1);
std::cerr << "Created New Block. Syntax Objcet: stats. " << std::endl;}

stat:
	  varDecl ';' {
$$ = $1;
std::cerr << "Syntax Object: stat. " << std::endl;}
	| arrayDecl ';' {
$$ = $1;
std::cerr << "Syntax Object: stat. " << std::endl;}
	| condStat {
$$ = $1;
std::cerr << "Syntax Object: stat. " << std::endl;}
	| exp ';' {
$$ = new AstExpStat($1);
std::cerr << "Syntax Object: stat. " << std::endl;}
	| whileloop {
$$ = $1;
std::cerr << "Syntax Object: stat. " << std::endl;}
	| returnStat ';' {
$$ = $1;
std::cerr << "Syntax Object: exps. " << std::endl;} 
    | block {
$$ = $1;
std::cerr << "Syntax Object: Block. " << std::endl;}


condStat:
	  ifS '(' exp ')' block condElseBlock{
$$ = new AstIfElseStat($5,$3,$6);
std::cerr << "Syntax Object: ifStat. " << std::endl;
}

	| ifS '(' exp ')' block {
$$ = new AstIfStat($5,$3);
std::cerr << "Syntax Object: ifStat. " << std::endl;
}

condElseBlock:
	  elseS condStat{
$$ = new AstBlock(new statementList); $$->statements->emplace_back($2);
std::cerr << "Syntax Object: elseIfStat. " << std::endl;}
	| elseS block {
$$ = $2;
std::cerr << "Syntax Object: elseIfStat. " << std::endl;};

globalVarDecl:
		type varNames{
$$ = new AstGlobalVariableDeclaration($1,$2);
std::cerr << "Syntax Object: globalVarDecl. " << std::endl;}
	  | type varNames asg value{
$$ = new AstGlobalVariableDeclaration($1,$2,$4); 
std::cerr << "Syntax Object: globalVarDecl. " << std::endl;}	 

varDecl:
	  type varNames{
$$ = new AstVariableDeclaration($2,$1);
std::cerr << "Syntax Object: varDecl. " << std::endl;}
	| type varNames asg exp   {
$$ = new AstVariableDeclaration($2,$1,$4); 
std::cerr << "Syntax Object: varDecl. " << std::endl;} 

varNames:
	nameV {
$$ = new AstName(std::string($1));
std::cerr << "Syntax Object: varNames. " << std::endl;}

baseType:
	  intD {
$$ = &intType;
std::cerr << "Syntax Object: type. " << std::endl;}
	| floatD {
$$ = &floatType;
std::cerr << "Syntax Object: type. " << std::endl;}
	| boolD {
$$ = &boolType;
std::cerr << "Syntax Object: type. " << std::endl;}
	| charD {
$$ = &charType;
std::cerr << "Syntax Object: type. " << std::endl;}

    
type:
	baseType {
	$$ = $1;
}
	| baseType astChain {
$$=currentContext.types->getExpandTypes($1->getTypeName(),$2);
std::cerr << "Syntax Object: type. " << std::endl;}


astChain:
	  ast
{
$$ = 1;
}
	| astChain ast
{
$$ = ($1)++;
}

funcDecl:
	  type varNames funcArgs block  {
$$ = new AstFunctionDeclaration($1,$2,$4,$3);
std::cerr << "Syntax Object: funcDecl. " << std::endl;}

funcArgs:
	  '(' argDecl ')' {
$$ = $2;}
	|  '(' ')' {
$$ = new variableList();}
 
block:
	   '{' stats '}' {
$$ = new AstBlock($2);
std::cerr << "Syntax Object: block. " << std::endl;}

argDecl:
	  argDecl ',' type varNames {
$$ = $1; $$->emplace_back($4,$3);
std::cerr << "Syntax Object: argDecl. " << std::endl;}
	| type varNames {
$$ = new variableList(); $$->emplace_back($2,$1);
std::cerr << "Syntax Object: argDecl. " << std::endl;}

arrayDecl:
	  type varNames '[' intV ']' {
$$ = new AstArrayDeclaration($2,$1,$4);
std::cerr << "Syntax Object: arrayDecl. " << std::endl;}
	| type varNames '[' ']' asg '{' exps '}'{
$$ = new AstArrayListDeclaration($2,$7,$1,$7->size());
std::cerr << "Syntax Object: arrayDecl. " << std::endl;}
    | type varNames '[' ']' asg charsV {
if ($1->getTypeName() == "char")
{
$$ = new AstArrayStringDeclaration($2,$6,strlen($6),false);
delete []$6;
std::cerr << "Syntax Object: arrayDecl. " << std::endl;
}
else
{
delete []$6;
std::cerr << "Array needs to have char type for char array. " << std::endl;
exit(1);
}
}
	| type varNames '[' ']' asg stringV {
if ($1->getTypeName() == "char") {
$$ = new AstArrayStringDeclaration($2,$6,strlen($6)+1,true);
delete []$6;
std::cerr << "Syntax Object: arrayDecl. " << std::endl;
}
else
{
delete []$6;
std::cerr << "Array needs to have char type for char array. " << std::endl;
exit(1);
}
}
	| type varNames '[' intV ']' asg '{' exps '}'{
if ( $4 < $8->size())
{
std::cerr << "Array size is too small." << std::endl;
exit(1);
}
else 
{
$$ = new AstArrayListDeclaration($2,$8,$1,$4);
std::cerr << "Syntax Object: arrayDecl. " << std::endl;}
}

    | type varNames '[' intV ']' asg charsV {
if ($4 < strlen($7))
{
std::cerr << "Array size is too small." << std::endl;
delete []$7;
exit(1);
}
else if ($1->getTypeName() == "char")
{
$$ = new AstArrayStringDeclaration($2,$7,$4,false);
std::cerr << "Syntax Object: arrayDecl. " << std::endl;
delete []$7;
}
else
{
delete []$7;
std::cerr << "Array needs to have char type for char array. " << std::endl;
exit(1);
}
}
	| type varNames '[' intV ']' asg stringV {
if ($4 < strlen($7)+1)
{
std::cerr << "Array size is too small." << std::endl;
delete []$7;
exit(1);
}
else if ($1->getTypeName() == "char")
{
$$ = new AstArrayStringDeclaration($2,$7,$4,true);
std::cerr << "Syntax Object: arrayDecl. " << std::endl;
delete []$7;
}
else
{
delete []$7;
std::cerr << "Array needs to have char type for char array. " << std::endl;
exit(1);
}
}

arrayAsg:
	  nameV '[' ']' asg charsV {
$$ = new AstArrayStringAsg(new AstName($1),$5,false);
delete []$5;
}
	| nameV '[' ']' asg stringV {
$$ = new AstArrayStringAsg(new AstName($1),$5,true);
delete []$5;
}
	| nameV arrayReferenceExp asg exp {
$$ = new AstArrayAsg(new AstName($1),$4,$2);
}

arrayReferenceExp:
	  '[' exp ']' {
$$ = $2;}

funcCall:
	  nameV '(' exps ')' {
$$ = new AstFunctionCall(new AstName($1),$3);
std::cerr << "Syntax Object: listInit. " << std::endl;}
   | nameV '('')' {
$$ = new AstFunctionCall(new AstName($1),new expressionList());
std::cerr << "Syntax Object: listInit. " << std::endl;}

arrayCall:
	  nameV arrayReferenceExp {
$$ = new AstArrayCall(new AstName($1),$2);
std::cerr << "Syntax Object: arrayCall. " << std::endl;}

varCall:
	  nameV {
$$ = new AstVariableCall(new AstName($1));
std::cerr << "Syntax Object: varCall. " << std::endl;}

returnStat:
	  returnS exp {
$$ = new AstReturnStat($2);
}

call:
	  varCall {
$$ = $1;
std::cerr << "Syntax Object: call. " << std::endl;}
	| funcCall {
$$ = $1;
std::cerr << "Syntax Object: call. " << std::endl;}
	| arrayCall {
$$ = $1;
std::cerr << "Syntax Object: call. " << std::endl;}

value:
	  intV {
$$ = new AstIntValue($1);
std::cerr << "Syntax Object: value. " << std::endl;
}
	| boolV {
$$ = new AstBoolValue($1);
std::cerr << "Syntax Object: value. " << std::endl;
}
	| floatV {
$$ = new AstFloatValue($1);
std::cerr << "Syntax Object: value. " << std::endl;
}

whileloop:
	  whileS '(' exp ')' block {
$$ = new AstWhileLoop($5,$3);
std::cerr << "Syntax Object: whileloop. " << std::endl;}

exps:
      exps ',' exp {
$$ = $1; $$ -> emplace_back($3);
std::cerr << "Syntax Object: exp. " << std::endl;}
	| exp {
$$ = new expressionList(); $$->emplace_back($1);
std::cerr << "Syntax Object: exp. " << std::endl;}

exp:
	  exp Or exp {
$$ = new AstOr($1,$3);
std::cerr << "Syntax Object: exp. " << std::endl;}
	| exp Xor exp {
$$ = new AstXor($1,$3);
std::cerr << "Syntax Object: exp. " << std::endl;}
	| exp amp exp {
$$ = new AstAnd($1,$3);
std::cerr << "Syntax Object: exp. " << std::endl;}
	| exp leq exp {
$$ = new Astleq($1,$3);
std::cerr << "Syntax Object: exp. " << std::endl;}
	| exp geq exp {
$$ = new Astgeq($1,$3);
std::cerr << "Syntax Object: exp. " << std::endl;}
	| exp lt exp {
$$ = new Astlt($1,$3);
std::cerr << "Syntax Object: exp. " << std::endl;}
	| exp gt exp {
$$ = new Astgt($1,$3);
std::cerr << "Syntax Object: exp. " << std::endl;}
	| exp eql exp {
$$ = new AstEql($1,$3);
std::cerr << "Syntax Object: exp. " << std::endl;}
	| exp neq exp {
$$ = new Astneq($1,$3);
std::cerr << "Syntax Object: exp. " << std::endl;}
	| exp leftSh exp {
$$ = new AstLeftSh($1,$3);
std::cerr << "Syntax Object: exp. " << std::endl;}
	| exp rightSh exp {
$$ = new AstRightSh($1,$3);
std::cerr << "Syntax Object: exp. " << std::endl;}
	| exp add exp {
$$ = new AstAdd($1,$3);
std::cerr << "Syntax Object: exp. " << std::endl;}
	| exp sub exp {
$$ = new AstSub($1,$3);
std::cerr << "Syntax Object: exp. " << std::endl;}
	| exp ast exp {
$$ = new AstMul($1,$3);
std::cerr << "Syntax Object: exp. " << std::endl;}
	| exp Div exp {
$$ = new AstDiv($1,$3);
std::cerr << "Syntax Object: exp. " << std::endl;}
	| exp mod exp {
$$ = new AstMod($1,$3);
std::cerr << "Syntax Object: exp. " << std::endl;}
    | deref {
$$ = $1;
std::cerr << "Syntax Object: exps. " << std::endl;} %prec derefPrec
   | address {
$$ = $1;
std::cerr << "Syntax Object: exps. " << std::endl;} %prec addressPrec
	| call {
$$ = $1;
std::cerr << "Syntax Object: exp. " << std::endl;}
	| value {
$$ = $1;
std::cerr << "Syntax Object: exp. " << std::endl;}
	| '(' exp ')'  {
$$ = $2;
std::cerr << "Syntax Object: exp. " << std::endl;}
    | asgOptions {
$$ = $1;
std::cerr << "Syntax Object: exp. " << std::endl;} %prec asgOptionsPrec

asgOptions:
	  nameV asg exp {
$$ = new AstVariableAsg(new AstName($1),$3);
std::cerr << "Syntax Object: asgOptions." << std::endl;
}
	| arrayAsg {
$$ = $1;
std::cerr << "Syntax Object: asgOptions." << std::endl;
}

deref:
	  ast varCall {
$$ = new AstDeref($2);
std::cerr << "Syntax Object: Dereference. " << std::endl;
}
	| ast arrayCall {
$$ = new AstDeref($2);
std::cerr << "Syntax Object: Dereference. " << std::endl;
}

address:
	  at nameV {
$$ = new AstVariableAddress(new AstName($2));
}
	| at nameV arrayReferenceExp {
$$ = new AstArrayAddress(new AstName($2),$3);
}
   | nameV '['']' {
$$ = new AstArrayAddress(new AstName($1),new AstIntValue(0));
}

%%

