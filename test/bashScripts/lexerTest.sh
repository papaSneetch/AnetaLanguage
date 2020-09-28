#!/bin/bash
echo -n "Lexer Matching Input Test: "
inputFile="../lexerTestData/lexerTest.txt";
outputFile="../lexerTestData/lexerTestResult.txt";
testResult=$(mktemp /tmp/testResult-XXXXX.txt);
../../bin/lexer.bin < $inputFile > $testResult;
dif=$(diff -q $testResult $outputFile &);
if [ -n "$dif" ]; 
then
		echo "Failed.";
		echo "Input Test File: $inputFile";
		echo "Output Test File: $outputFile";
		echo "Input Test Results Location: $testResult";
else
		echo "Passed.";
fi
