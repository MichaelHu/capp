all: doyacc dolex calc.out

doyacc:
	yacc -d -o calc.y.c calc.y

dolex:
	lex -o calc.lex.c calc.lex

calc.out: calc.lex.o calc.y.o
	gcc $^ -o $@ -g

..c.o:
	gcc -c $< -g

clean:
	rm -rf *.o 



