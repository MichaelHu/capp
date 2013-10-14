all:simple_proc.out clean

simple_proc.out:args.o main.o
	gcc -o $@ -lc $^

..c.o:
	gcc -c $<

clean:
	rm -f *.o 
