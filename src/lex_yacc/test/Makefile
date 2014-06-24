all: clean doyacc dolex test.out

doyacc:
	yacc -r all -d -o test.y.c test.y

dolex:
	lex -o test.lex.c test.lex

test.out: test.lex.o test.y.o
	gcc $^ -o $@ -g

..c.o:
	gcc -c $< -g

clean:
	rm -rf *.o 

