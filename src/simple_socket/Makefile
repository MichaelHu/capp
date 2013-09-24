######### Makefile ###########
all:clean lib server client clean

lib:my_read.c my_write.c
	gcc -c $^
server:server.c my_read.o my_write.o
	gcc $^ -o $@ -g
client:client.c my_read.o my_write.o
	gcc $^ -o $@ -g
clean:
	rm -rf *.o 
	
