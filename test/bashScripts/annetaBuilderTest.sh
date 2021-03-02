#!/bin/bash 
cd $(dirname $0)
echo -n "AnnetaBuilder Matching Input Test: "; 
inputFile="../annetaBuilderTestData/test5";
outputFile="../annetaBuilderTestData/testResults.o";
../../bin/annetaBuilder.bin $inputFile -o $outputFile; 
