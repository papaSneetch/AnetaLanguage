./bin/lexer.bin: ./gen/lexer.yy.cpp
	g++ -Wall -I/include -L/obj -o ./bin/lexer.bin ./gen/lexer.yy.cpp

./gen/lexer.yy.cpp: ./source/lexer.l
	flex -o ./gen/lexer.yy.cpp ./source/lexer.l
