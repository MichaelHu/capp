all: clean doyacc dolex markdown.out

doyacc:
	yacc -d -o markdown.y.c markdown.y

dolex:
	lex -o markdown.lex.c markdown.lex

markdown.out: markdown.lex.o markdown.y.o htmltags.o
	gcc $^ -o $@ -g

..c.o:
	gcc -c $< -g

clean:
	rm -rf *.o 

