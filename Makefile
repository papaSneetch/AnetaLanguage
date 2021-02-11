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

targets = $(parserTargets) $(lexerTargets) $(parserLexerTargets) $(lexerBinTargets) $(parserBinTargets) 

all: $(targets)

$(parserTargets): $(parserPrereq) 
	bison --defines=$(parserIncludeLoc) $(bisonSourceLoc) -o$(parserLibLoc) 

$(lexerTargets): $(lexerPrereq) 
	flex -o$(lexIncludeLoc) $(lexSourceLoc) 

$(parserLexerTargets): $(parserLexerPrereq)
	gcc -Wall -I$(includeDir) -L$(depsDir) -c $(parserLibLoc) -o$(parserLexerObjectLoc)

$(lexerBinTargets): $(lexerBinPrereq)
	g++ -Wall -I$(includeDir) $(lexMainLoc) -o$(lexBinLoc)
	$(lexTestLoc)

$(parserBinTargets): $(parserBinPrereq)
	g++ -Wall -I$(includeDir) $(parserMainLoc) $(parserLexerObjectLoc) -o$(parserBinLoc)

