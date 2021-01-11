#!/bin/bash 
cd $(dirname $0)
inputFile="../annetaBuilderTestData/test1"; 
outputFile="../annetaBuilderTestData/testResults.o";
lldb -- ../../bin/annetaBuilder.bin $inputFile -o $outputFile; 
