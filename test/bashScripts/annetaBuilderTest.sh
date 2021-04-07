#!/bin/bash 
cd $(dirname $0)
echo -n "AnnetaBuilder Matching Input Test: "; 
inputFile="../annetaBuilderTestData/test21";
outputFile="../annetaBuilderTestData/testResults.ll";
../../bin/annetaBuilder.bin -emit-llvm $inputFile -o $outputFile; 
