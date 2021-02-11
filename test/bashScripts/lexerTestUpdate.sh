 #!/bin/bash
 echo -n "Lexer Matching Input Test Updating:";
 inputFile="../lexerTestData/lexerTest.txt";
 outputFile="../lexerTestData/lexerTestResult.txt";
 ../../bin/lexer.bin $inputFile > $outputFile;
 echo " Complete";
