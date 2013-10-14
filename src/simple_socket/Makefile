######### Makefile ###########
all:clean lib server.out client.out clean

lib:my_read.c my_write.c
	gcc -c $^
server.out:server.c my_read.o my_write.o
	gcc $^ -o $@ -g
client.out:client.c my_read.o my_write.o
	gcc $^ -o $@ -g
clean:
	rm -rf *.o 
	
