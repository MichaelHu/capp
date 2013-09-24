all:simple_proc clean

simple_proc:args.o main.o
	gcc -o $@ -lc $^

..c.o:
	gcc -c $<

clean:
	rm -f *.o 
