## Simple HTTP server sample for sanos

In order to demonstrate programming under sanos I have made a simple web server demo. This sample is deliberately kept simple and lacks many important features you might want from a real web server. It is a single threaded server with only minimal error checking and it only handles the most basic aspects of the HTTP protocol. You can take a look at the httpd server implementation in sanos for a more scalable and robust HTTP server implementation.

However, I hope this little sample web sever with only 200 lines of code demonstrates some of the the basic programming style used for programming sanos server applications. As can be seen from the source code below, it is fairly standard C code. The code is very similar to other POSIX applications. In fact, it can compile under Linux without any changes.

## Compiling webserver.c under sanos using sanos SDK
