all: parser

parser: src/parser.cpp src/ast.h
	g++ -o src/parser src/parser.cpp

clean:
	rm -f src/*.o src/parser
