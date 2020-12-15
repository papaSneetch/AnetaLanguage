#!/bin/bash 
cd $(dirname $0)
inputFile="../annetaBuilderTestData/test3"; 
lldb -- ../../bin/annetaBuilder.bin $inputFile; 
