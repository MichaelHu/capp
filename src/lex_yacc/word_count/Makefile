all: dolex word_count.out

dolex:
	lex -o word_count.c word_count.lex

word_count.out: word_count.o
	gcc $^ -o $@ -g

..c.o:
	gcc -c $< -g

clean:
	rm -rf *.o 


