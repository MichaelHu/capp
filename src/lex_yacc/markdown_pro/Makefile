all: clean doyacc dolex markdown.out

doyacc:
	yacc -d -omarkdown.y.c markdown.y

dolex:
	lex -omarkdown.lex.c markdown.lex

markdown.out: markdown.y.o markdown.lex.o htmltags.o tagstack.o tagstackcheck.o blocknode.o blocknodestack.o 
	gcc $^ -o $@ -g

..c.o:
	gcc -c $< -g

clean:
	rm -rf *.o 

