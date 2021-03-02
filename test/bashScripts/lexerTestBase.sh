#!/bin/bash 
cd $(dirname $0)
inputFile="../annetaBuilderTestData/test5"; 
outputFile="../annetaBuilderTestData/testResults.o";
../../bin/lexer.bin $inputFile; 
