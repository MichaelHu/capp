all: clean doyacc dolex markdown.out

doyacc:
	bison -d -omarkdown.y.c markdown.y

dolex:
	flex -omarkdown.lex.c markdown.lex

markdown.out: markdown.y.o markdown.lex.o htmltags.o tagstack.o tagstackcheck.o blocknode.o blocknodestack.o 
	gcc $^ -o $@ -g

..c.o:
	gcc -c $< -g

clean:
	rm -rf *.o 

