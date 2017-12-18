all:
	yacc -d --verbose limbaj.ypp
	flex limbaj.l
	g++ -std=c++11 y.tab.c lex.yy.c -o limbaj.out -ll -ly
	python test.py

clean:
	rm -f *.out *.o
