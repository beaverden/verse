all:
	yacc -d --verbose limbaj.ypp
	flex limbaj.l
	g++ -g -std=c++11 code.cpp y.tab.c lex.yy.c -o limbaj.out -ll -ly
	python test.py

clean:
	rm -f *.out *.o
