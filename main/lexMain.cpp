extern "C"
{
#include "lexer.yy.h"
}

int main()
{
		union YYSTYPE yylval;
		union YYSTYPE *yylvalPtr = &yylval;
		while (yylex(yylvalPtr))
		{
			continue;
		}

		return 0;
}
