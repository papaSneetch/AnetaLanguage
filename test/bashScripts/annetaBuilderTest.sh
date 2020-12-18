#!/bin/bash 
cd $(dirname $0)
echo -n "AnnetaBuilder Matching Input Test: "; 
inputFile="../annetaBuilderTestData/test4"; 
../../bin/annetaBuilder.bin $inputFile;
