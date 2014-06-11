all: dolex jsstring.out

dolex:
	lex -o jsstring.c jsstring.lex

jsstring.out: jsstring.o
	gcc $^ -o $@ -g

..c.o:
	gcc -c $< -g

clean:
	rm -rf *.o 



