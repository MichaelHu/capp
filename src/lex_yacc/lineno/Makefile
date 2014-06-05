all: dolex lineno.out

dolex:
	lex -o lineno.c lineno.lex

lineno.out: lineno.o
	gcc $^ -o $@ -g

..c.o:
	gcc -c $< -g

clean:
	rm -rf *.o 


