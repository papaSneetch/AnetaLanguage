CC = g++ -w -fmax-errors=5 -I$(includeDir)


sourceDir = ./source
binDir = ./bin
includeDir = ./include
objectDir = ./obj
genDir = ./gen
dataDir = ./data
mainDir = ./main
depsDir = ./deps
testDir = ./test
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
lexerBinPrereq = $(parserLexerObjectLoc) $(lexIncludeLoc) $(lexMainLoc)

parserBinTargets = $(parserBinLoc)
parserBinPrereq = $(parserLexerObjectLoc) $(parserIncludeLoc) $(parserMainLoc)

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

includes = $(parserIncludeLoc) $(lexIncludeLoc) $(annetaBuilderIncludeLoc) $(codeGenContextInclude)
objects = $(parserLexetObjectLoc) $(annetaBuilderObjectLoc) $(codeGenContextObject)

annetaBuilderMain = annetaBuilderMain.cpp
annetaBuilderMainBin = annetaBuilder.bin
annetaBuilderMainLoc = $(mainDir)/$(annetaBuilderMain)
annetaBuilderMainPrereq = $(includes) $(objects)
annetaBuilderMainTargetLoc= $(binDir)/$(annetaBuilderMainBin)

targets = $(parserTargets) $(lexerTargets) $(parserLexerTargets) $(lexerBinTargets) $(annetaBuilderTarget) $(codeGenContextTarget) $(annetaBuilderMainTargetLoc)

all: $(targets)

annetaBuilder: $(annetaBuilderTarget)

codeGenContext: $(codeGenContextTarget)

$(parserTargets): $(parserPrereq) 
	bison --defines=$(parserIncludeLoc) -Wcounterexamples $(bisonSourceLoc) -o$(parserLibLoc) 

$(lexerTargets): $(lexerPrereq) 
	flex -o$(lexIncludeLoc) $(lexSourceLoc) 

$(parserLexerTargets): $(parserLexerPrereq)
	$(CC) -L$(depsDir) -c $(parserLibLoc) -o$(parserLexerObjectLoc)

$(lexerBinTargets): $(lexerBinPrereq)
	$(CC) $(lexMainLoc) -o$(lexBinLoc)

$(parserBinTargets): $(parserBinPrereq)
	$(CC) $(parserMainLoc) $(parserLexerObjectLoc) -o$(parserBinLoc)

$(annetaBuilderTarget): $(annetaBuilderPrereq)
	$(CC) -c $(annetaBuilderSourceLoc) -o$(annetaBuilderObjectLoc)

$(codeGenContextTarget): $(codeGenContextPrereq)
	$(CC) -c$(codeGenContextSourceLoc) -o$(codeGenContextObjectLoc

$(annetaBuilderMainTargetLoc): $(annetaBuilderMainPrereq)
	$(CC) $(annetaBuilderMainLoc) $(objects) -o$(annetaBuilderMainTargetLoc)


