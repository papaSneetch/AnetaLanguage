!/bin/bash 
cd $(dirname $0)
inputFile="../annetaBuilderTestData/test19"; 
outputFile="../annetaBuilderTestData/testResults.ll";
lldb -- ../../bin/annetaBuilder.bin -emit-llvm $inputFile -o $outputFile; 
