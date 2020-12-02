#!/bin/bash 
cd $(dirname $0)
inputFile="../annetaBuilderTestData/test1"; 
lldb -- ../../bin/annetaBuilder.bin $inputFile; 
