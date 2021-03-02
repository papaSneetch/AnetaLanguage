CC = clang++ -Wall -I $(includeDir) -g
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
parserLib = parser.tab.cpp
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

annetaTypesSource = annetaTypes.cpp
annetaTypesInclude = annetaTypes.h
annetaTypesObject = annetaTypes.o
annetaTypesSourceLoc =  $(sourceDir)/$(annetaTypesSource)
annetaTypesIncludeLoc = $(includeDir)/$(annetaTypesInclude)
annetaTypesPrereq = $(annetaTypesSourceLoc) $(annetaTypesIncludeLoc)
annetaTypesObjectLoc = $(objectDir)/$(annetaTypesObject)
annetaTypesTarget = $(annetaTypesObjectLoc)

includes = $(parserIncludeLoc) $(lexIncludeLoc) $(annetaBuilderIncludeLoc) $(codeGenContextIncludeLoc)
objects = $(parserLexetObjectLoc) $(annetaBuilderObjectLoc) $(codeGenContextObjectLoc) $(annetaTypesObjectLoc)

annetaBuilderMain = annetaBuilderMain.cpp
annetaBuilderMainObject = annetaBuilderMain.o
annetaBuilderMainObjectLoc = $(objectDir)/$(annetaBuilderMainObject)
annetaBuilderMainBin = annetaBuilder.bin
annetaBuilderMainLoc = $(mainDir)/$(annetaBuilderMain)
annetaBuilderMainPrereq = $(includes) $(objects) $(llvmFlagsSourceLoc) $(annetaBuilderMainLoc)
annetaBuilderMainTargetLoc= $(binDir)/$(annetaBuilderMainBin)

targets = $(codeGenContextTarget) $(annetaBuilderTarget) $(parserTargets) $(lexerTargets) $(parserLexerTargets) $(annetaBuilderMainTargetLoc) $(parserBinTargets) $(lexerBinTargets) $(annetaTypesTarget)

all: $(targets)

annetaBuilder: $(annetaBuilderTarget)

lexer: $(lexerBinTargets)

codeGenContext: $(codeGenContextTarget)

annetaTypes: $(annetaTypesTarget)

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

$(annetaTypesTarget): $(annetaTypesPrereq)
	$(CC) -c $(annetaTypesSourceLoc) -o$(annetaTypesObjectLoc)

$(annetaBuilderMainTargetLoc): $(annetaBuilderMainPrereq)
	 $(CC) -c $(annetaBuilderMainLoc) -o $(annetaBuilderMainObjectLoc) 
	 $(CC) -o $(annetaBuilderMainTargetLoc) $(annetaBuilderMainObjectLoc) $(parserLexerObjectLoc) $(codeGenContextObjectLoc) $(annetaBuilderObjectLoc) $(annetaTypesObjectLoc) @$(llvmFlagsSourceLoc)

$(parserBinTargets): $(parserBinPrereq)
	 $(CC) -o$(parserBinLoc) $(parserMainLoc)  $(parserLexerObjectLoc) $(codeGenContextObjectLoc) $(annetaBuilderObjectLoc) $(annetaTypesObjectLoc) @$(llvmFlagsSourceLoc)

$(lexerBinTargets): $(lexerBinPrereq)
	$(CC) $(lexMainLoc) -o$(lexBinLoc) @$(llvmFlagsSourceLoc)

