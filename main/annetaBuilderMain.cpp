#define _POSIX_SOURCE
#include <unistd.h> 

#include <iostream>
#include <stdio.h>
#include "parser.tab.h"
#include "annetaBuilder.h"
#include "codeGenContext.h"

#include "llvm/Support/Host.h"

enum compilationTypes {llvmIR,bitcode,objectCode,exeCode};

int main(int argc, char *argv[])
{
bool userDefinedTarget = false;
compilationTypes compileTarget = exeCode;
std::string inputFileName;
std::string outputFileName;
std::string target = llvm::sys::getProcessTriple();
std::string cpu = "generic"  /* llvm::sys::getHostCPUName()*/; 	
target = llvm::sys::getProcessTriple();
std::string errorString;
for (int i = 1; i < argc; i++)
{
if (strcmp(argv[i],"-o") == 0)
{
i++;
if (argv[i] != nullptr)
{
outputFileName = argv[i];
continue;
}
}
else if (strcmp(argv[i],"-emit-llvm") == 0)
{
compileTarget = llvmIR;
continue;
}
else if (strcmp(argv[i],"-emit-bitcode") == 0)
{
compileTarget = bitcode;
continue;
}
else if (strcmp(argv[i],"-c") == 0)
{
compileTarget = objectCode;
continue;
}
else if (strcmp(argv[i],"-target") == 0)
{
i++;
if (argv[i] != nullptr)
{
target = argv[i];
userDefinedTarget = true;
continue;
}
}
else if (strcmp(argv[i],"-mcpu") == 0)
{
i++;
if (argv[i] != nullptr)
{
cpu= argv[i];
userDefinedTarget = true;
continue;
}
}
else if (argv[i] != nullptr)
{
stdin = fopen(argv[i],"r");
if (stdin)
{
inputFileName.assign(argv[i]);
continue;
}
else
{
std::cerr << "Input file doesn't exist: " << argv[i] << std::endl;
exit(1);
}
}
}

if (inputFileName.empty())
{
std::cerr << "No input file specified!" << std::endl;
exit(1);
}
if (userDefinedTarget)
{
currentContext.initializeNativeTargetList();
}
else
{
currentContext.initializeTargetList();
}

	yyparse();
	currentContext.initContext();
	currentContext.initializeTarget(target,errorString,cpu);
	currentContext.codeGen();
//	currentContext.createStart();

if (outputFileName.empty())
{
outputFileName = inputFileName;
std::size_t lastExt = outputFileName.find_last_of('.');
if (lastExt == std::string::npos || lastExt == 1)
{
lastExt = outputFileName.size();
}
outputFileName.resize(lastExt);
outputFileName.append(".");
if (compileTarget==objectCode)
{
outputFileName.append("o");
}
else if (compileTarget==llvmIR)
{
outputFileName.append("ll");
}
else if (compileTarget==exeCode)
{
outputFileName.append("exe");
}
else if (compileTarget==bitcode)
{
outputFileName.append("bc");
}
stdout = fopen(outputFileName.c_str(),"w");
}
if (compileTarget==objectCode)
{
currentContext.printObjectCode(outputFileName);
}
else if (compileTarget==llvmIR)
{
currentContext.printLLVMCode(outputFileName);
}
else if (compileTarget==exeCode)
{
currentContext.printExeCode(outputFileName);
}
else if (compileTarget==bitcode)
{
currentContext.printBitCode(outputFileName);
}
fclose(stdout);

return 0;
}
