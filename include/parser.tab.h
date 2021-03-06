/* A Bison parser, made by GNU Bison 3.7.3.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_INCLUDE_PARSER_TAB_H_INCLUDED
# define YY_YY_INCLUDE_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 17 "./source/parser.y"

#include "annetaBuilder.h"

#line 53 "./include/parser.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    ifS = 258,                     /* ifS  */
    elseS = 259,                   /* elseS  */
    whileS = 260,                  /* whileS  */
    returnS = 261,                 /* returnS  */
    intD = 262,                    /* intD  */
    floatD = 263,                  /* floatD  */
    boolD = 264,                   /* boolD  */
    charD = 265,                   /* charD  */
    stringD = 266,                 /* stringD  */
    leftSh = 267,                  /* leftSh  */
    rightSh = 268,                 /* rightSh  */
    add = 269,                     /* add  */
    sub = 270,                     /* sub  */
    ast = 271,                     /* ast  */
    Div = 272,                     /* Div  */
    exponent = 273,                /* exponent  */
    Xor = 274,                     /* Xor  */
    mod = 275,                     /* mod  */
    inc = 276,                     /* inc  */
    dec = 277,                     /* dec  */
    eql = 278,                     /* eql  */
    leq = 279,                     /* leq  */
    geq = 280,                     /* geq  */
    lt = 281,                      /* lt  */
    gt = 282,                      /* gt  */
    neq = 283,                     /* neq  */
    aeg = 284,                     /* aeg  */
    meg = 285,                     /* meg  */
    asg = 286,                     /* asg  */
    amp = 287,                     /* amp  */
    Or = 288,                      /* Or  */
    intV = 289,                    /* intV  */
    boolV = 290,                   /* boolV  */
    floatV = 291,                  /* floatV  */
    charsV = 292,                  /* charsV  */
    nameV = 293,                   /* nameV  */
    aeq = 294,                     /* aeq  */
    meq = 295,                     /* meq  */
    asgOptionsPrec = 296,          /* asgOptionsPrec  */
    postInc = 297,                 /* postInc  */
    postDec = 298,                 /* postDec  */
    preInc = 299,                  /* preInc  */
    preDec = 300                   /* preDec  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 27 "./source/parser.y"

int int_val;
bool bool_val;
float float_val;
char* string;
AstNode* node;
AstStat* statement;
AstExp* expression;
AstConstant* constant;
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

#line 136 "./include/parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (void);

#endif /* !YY_YY_INCLUDE_PARSER_TAB_H_INCLUDED  */
