all: myencrypt clean

# main: main.o myencrypt.o
# 	gcc -o myencrypt -L/usr/lib -lc++ main.o myencrypt.o; rm *.o
# main.o: main.cpp common.h myencrypt.h
# 	gcc -c main.cpp
# myencrypt.o: myencrypt.cpp common.h myencrypt.h
# 	gcc -c myencrypt.cpp

# myencrypt: main.o myencrypt.o
# 	gcc -o $@ -L/usr/lib -lc++ $^; rm *.o
# main.o: main.cpp common.h myencrypt.h
# 	gcc -c $<
# myencrypt.o: myencrypt.cpp common.h myencrypt.h
# 	gcc -c $< 

myencrypt: main.o myencrypt.o
	gcc -o $@ -L/usr/lib -lc++ $^
..c.o:
	gcc -c $<

clean: 
	rm *.o
