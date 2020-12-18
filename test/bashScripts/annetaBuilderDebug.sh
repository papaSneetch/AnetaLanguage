#!/bin/bash 
cd $(dirname $0)
inputFile="../annetaBuilderTestData/test4"; 
lldb -- ../../bin/annetaBuilder.bin $inputFile; 
