#include <iostream>
#include <stdio.h>
#include "parser.tab.h"
#include "annetaBuilder.h"
#include "codeGenContext.h"

int main(int argc, char *argv[])
{
if (argc >= 2)
	{
			stdin = fopen(argv[1],"r");
	}
if (argc >= 3)
	{
		stdout = fopen(argv[2],"w");
	}	

	yyparse();
	currentContext.initContext();
	currentContext.codeGen();
	currentContext.printCode();
	return 0;
}
