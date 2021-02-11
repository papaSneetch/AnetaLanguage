CC = clang++ -Wall -I $(includeDir)
llvmLib = ../../Libaries/rootLibaries/llvm-11.0.0/lib

sourceDir = ./source
binDir = ./bin
includeDir = ./include
objectDir = ./obj
genDir = ./gen
dataDir = ./data
mainDir = ./main
depsDir = ./deps
testDir = ./test
confDir = ./conf
testScriptsDir = $(testDir)/bashScripts

lexSource = lexer.l
lexOutput = lexer.yy.h
lexBin = lexer.bin
lexMain = lexMain.cpp
lexTestScript = lexerTest.sh

bisonSource = parser.y
parserInclude = parser.tab.h
parserLib = parser.tab.c
parserObject = parser.tab.o
parserBin = parser.bin
parserMain = parserMain.cpp

llvmFlagsSource = llvmFlags.txt
llvmFlagsSourceLoc = $(confDir)/$(llvmFlagsSource)

bisonSourceLoc = $(sourceDir)/$(bisonSource)
parserIncludeLoc = $(includeDir)/$(parserInclude)
parserLibLoc = $(depsDir)/$(parserLib)

lexSourceLoc = $(sourceDir)/$(lexSource)
lexIncludeLoc = $(includeDir)/$(lexOutput)
lexMainLoc = $(mainDir)/$(lexMain)
lexBinLoc = $(binDir)/$(lexBin)
lexTestLoc = $(testScriptsDir)/$(lexTestScript)

parserLexerObject = parserLexer.o
parserLexerObjectLoc = $(objectDir)/$(parserLexerObject)
parserLexerIncludeLoc = $(lexIncludeLoc) $(parserIncludeLoc)

parserTargets = $(parserIncludeLoc) $(parserLibLoc)
parserPrereq = $(bisonSourceLoc)
parserMainLoc = $(mainDir)/$(parserMain)
parserBinLoc = $(binDir)/$(parserBin)

lexerTargets = $(lexIncludeLoc)
lexerPrereq = $(lexSourceLoc)

parserLexerTargets = $(parserLexerObjectLoc)
parserLexerPrereq = $(parserTargets) $(lexerTargets)

lexerBinTargets = $(lexBinLoc)
lexerBinPrereq = $(parserLexerObjectLoc) $(lexIncludeLoc) $(lexMainLoc) $(llvmFlagsSourceLoc)

parserBinTargets = $(parserBinLoc)
parserBinPrereq = $(parserLexerObjectLoc) $(parserIncludeLoc) $(parserMainLoc) $(llvmFlagsSourceLoc)

annetaBuilderSource = annetaBuilder.cpp
annetaBuilderInclude = annetaBuilder.h
annetaBuilderObject = annetaBuilder.o
annetaBuilderSourceLoc = $(sourceDir)/$(annetaBuilderSource)
annetaBuilderIncludeLoc = $(includeDir)/$(annetaBuilderInclude)
annetaBuilderPrereq = $(annetaBuilderSourceLoc) $(annetaBuilderIncludeLoc)
annetaBuilderObjectLoc = $(objectDir)/$(annetaBuilderObject)
annetaBuilderTarget = $(annetaBuilderObjectLoc)

codeGenContextSource = codeGenContext.cpp
codeGenContextInclude = codeGenContext.h
codeGenContextObject = codeGenContext.o
codeGenContextSourceLoc = $(sourceDir)/$(codeGenContextSource)
codeGenContextIncludeLoc = $(includeDir)/$(codeGenContextInclude)
codeGenContextPrereq = $(codeGenContextSourceLoc) $(codeGenContextIncludeLoc)
codeGenContextObjectLoc = $(objectDir)/$(codeGenContextObject)
codeGenContextTarget = $(codeGenContextObjectLoc)

includes = $(parserIncludeLoc) $(lexIncludeLoc) $(annetaBuilderIncludeLoc) $(codeGenContextIncludeLoc)
objects = $(parserLexetObjectLoc) $(annetaBuilderObjectLoc) $(codeGenContextObjectLoc)

annetaBuilderMain = annetaBuilderMain.cpp
annetaBuilderMainObject = annetaBuilderMain.o
annetaBuilderMainObjectLoc = $(objectDir)/$(annetaBuilderMainObject)
annetaBuilderMainBin = annetaBuilder.bin
annetaBuilderMainLoc = $(mainDir)/$(annetaBuilderMain)
annetaBuilderMainPrereq = $(includes) $(objects) $(llvmFlagsSourceLoc)
annetaBuilderMainTargetLoc= $(binDir)/$(annetaBuilderMainBin)



targets = $(codeGenContextTarget) $(annetaBuilderTarget) $(parserTargets) $(lexerTargets) $(parserLexerTargets) $(annetaBuilderMainTargetLoc) $(parserBinTargets) $(lexerBinTargets) 

all: $(targets)

annetaBuilder: $(annetaBuilderTarget)

codeGenContext: $(codeGenContextTarget)

$(codeGenContextTarget): $(codeGenContextPrereq)
	$(CC) -c $(codeGenContextSourceLoc) -o$(codeGenContextObjectLoc)

$(annetaBuilderTarget): $(annetaBuilderPrereq)
	$(CC) -c $(annetaBuilderSourceLoc) -o$(annetaBuilderObjectLoc)

$(parserTargets): $(parserPrereq) 
	bison --defines=$(parserIncludeLoc) -Wcounterexamples -Wconflicts-sr -Wother  $(bisonSourceLoc) -o$(parserLibLoc) 

$(lexerTargets): $(lexerPrereq) 
	flex -o$(lexIncludeLoc) $(lexSourceLoc) 

$(parserLexerTargets): $(parserLexerPrereq)
	$(CC) -c $(parserLibLoc) -o$(parserLexerObjectLoc)

$(annetaBuilderMainTargetLoc): $(annetaBuilderMainPrereq)
	 $(CC) -c $(annetaBuilderMainLoc) -o $(annetaBuilderMainObjectLoc) 
	 $(CC) -o $(annetaBuilderMainTargetLoc) $(annetaBuilderMainObjectLoc) $(parserLexerObjectLoc) $(codeGenContextObjectLoc) $(annetaBuilderObjectLoc) @$(llvmFlagsSourceLoc)

$(parserBinTargets): $(parserBinPrereq)
	 $(CC) $(parserMainLoc)  $(parserLexerObjectLoc) $(codeGenContextObjectLoc) $(annetaBuilderObjectLoc) -o$(parserBinLoc) @$(llvmFlagsSourceLoc)

$(lexerBinTargets): $(lexerBinPrereq)
	$(CC) $(lexMainLoc) -o$(lexBinLoc) @$(llvmFlagsSourceLoc)



