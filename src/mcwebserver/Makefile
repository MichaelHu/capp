all:webserv.out clean

webserv.out: webserv.o helper.o servreq.o reqhead.o reqbody.o resphead.o response.o resource.o ../md5/md5c.o outputcache.o
	gcc $^ -o $@ -g

..c.o:
	gcc -c $< -g

clean:
	rm -rf *.o 


# webserv.out: webserv.o helper.o servreq.o reqhead.o resphead.o resource.o
# 	gcc -o webserv.out webserv.o helper.o servreq.o reqhead.o resphead.o resource.o -Wall -g; rm *.o
# 
# webserv.o: webserv.c helper.h servreq.h
# 	gcc -o webserv.o webserv.c -c -ansi -pedantic -Wall -g
# 
# helper.o: helper.c helper.h
# 	gcc -o helper.o helper.c -c -ansi -pedantic -Wall -g
# 
# servreq.o: servreq.c servreq.h helper.h
# 	gcc -o servreq.o servreq.c -c -ansi -pedantic -Wall -g
# 
# reqhead.o: reqhead.c reqhead.h servreq.h helper.h
# 	gcc -o reqhead.o reqhead.c -c -ansi -pedantic -Wall -g
# 
# resphead.o: resphead.c resphead.h helper.h
# 	gcc -o resphead.o resphead.c -c -ansi -pedantic -Wall -g
# 
# resource.o: resource.c resource.h
# 	gcc -o resource.o resource.c -c -ansi -pedantic -Wall -g
# 

