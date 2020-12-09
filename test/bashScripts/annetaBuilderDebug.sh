#!/bin/bash 
cd $(dirname $0)
inputFile="../annetaBuilderTestData/test2"; 
lldb -- ../../bin/annetaBuilder.bin $inputFile; 
