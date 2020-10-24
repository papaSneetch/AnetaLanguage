#include <iostream>
extern "C"
{
#include <stdio.h>
#include "lexer.yy.h"
}

int main(int argc, char *argv[])
{
if (argc == 2)
		{
				stdin = fopen(argv[1],"r");
		}
		union YYSTYPE yylval;
		union YYSTYPE *yylvalPtr = &yylval;
		while (true)
		{
		    register int yylexResult = yylex(yylvalPtr);
			std::cout << yylexResult << std::endl;
			if (yylexResult == 0)
			{
				break;	
			}
		}

		return 0;
}
