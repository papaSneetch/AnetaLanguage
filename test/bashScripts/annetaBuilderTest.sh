#!/bin/bash 
cd $(dirname $0)
echo -n "AnnetaBuilder Matching Input Test: "; 
inputFile="../annetaBuilderTestData/test1"; 
../../bin/annetaBuilder.bin $inputFile;
