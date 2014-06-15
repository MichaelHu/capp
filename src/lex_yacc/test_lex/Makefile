all: dolex test_lex.out

dolex:
	lex -o test_lex.c test_lex.lex

test_lex.out: test_lex.o
	gcc $^ -o $@ -g

..c.o:
	gcc -c $< -g

clean:
	rm -rf *.o 



