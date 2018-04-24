all:
	yacc -d --verbose limbaj.ypp
	flex limbaj.l
	g++ -g -rdynamic -std=c++11 src/Execute.cpp src/AST.cpp y.tab.c lex.yy.c -o limbaj.out -ll -ly

clean:
	rm -f *.out *.o
