#!/bin/bash 
cd $(dirname $0)
inputFile="../annetaBuilderTestData/test30"; 
outputFile="../annetaBuilderTestData/testResults.bin";
lldb -- ../../bin/annetaBuilder.bin $inputFile -o $outputFile; 
