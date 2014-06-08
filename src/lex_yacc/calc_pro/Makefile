all: doyacc dolex calc_pro.out

doyacc:
	yacc -d -o calc_pro.y.c calc_pro.y

dolex:
	lex -o calc_pro.lex.c calc_pro.lex

calc_pro.out: calc_pro.lex.o calc_pro.y.o interpreter.o
	gcc $^ -o $@ -g

..c.o:
	gcc -c $< -g

clean:
	rm -rf *.o 

