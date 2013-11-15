all: mddriver.out

mddriver.out: md5c.o mddriver.o 
	gcc $^ -o $@ -g

..c.o:
	gcc -c $< -g

clean:
	rm -rf *.o 


