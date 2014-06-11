all: dolex strings.out

dolex:
	lex -o strings.c strings.lex

strings.out: strings.o
	gcc $^ -o $@ -g

..c.o:
	gcc -c $< -g

clean:
	rm -rf *.o 



