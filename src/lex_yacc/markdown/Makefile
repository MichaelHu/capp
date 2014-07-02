all: clean doyacc dolex markdown.out

doyacc:
	yacc -r all -d -o markdown.y.c markdown.y

dolex:
	lex -o markdown.lex.c markdown.lex

markdown.out: markdown.y.o markdown.lex.o htmltags.o tagstack.o tagstackcheck.o 
	gcc $^ -o $@ -g

..c.o:
	gcc -c $< -g

clean:
	rm -rf *.o 

