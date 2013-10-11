## Simple HTTP server sample for sanos

In order to demonstrate programming under sanos I have made a simple web server demo. This sample is deliberately kept simple and lacks many important features you might want from a real web server. It is a single threaded server with only minimal error checking and it only handles the most basic aspects of the HTTP protocol. You can take a look at the httpd server implementation in sanos for a more scalable and robust HTTP server implementation.

However, I hope this little sample web sever with only 200 lines of code demonstrates some of the the basic programming style used for programming sanos server applications. As can be seen from the source code below, it is fairly standard C code. The code is very similar to other POSIX applications. In fact, it can compile under Linux without any changes.

## Compiling webserver.c under sanos using sanos SDK

The simplest way to try it out is to use the sanos SDK to compile webserver.c under sanos:

    /$ cd /usr/src
    /usr/src$ httpget www.jbox.dk /downloads/webserver.c webserver.c
    Received 5808 bytes in 130 ms (44 KB/s)
    /usr/src$ cc webserver.c
    /usr/src$ webserver &
    [process 18 started]
    HTTP server listening on port 80
    /usr/src$ _

## Compiling webserver.c for sanos using Visual Studio

You can use the Visual Studio wizard to create a new sanos project with the webserver:

* Create a new sanos project called 'webserver'.
* Set application type to user application (.exe).
* Select 'Use standard C library'.
* Select 'Create floppy disk boot image'.
* Select 'Enable network support'.

Add a new source file (webserver.c) to the project and paste the code below into it. Select the Release configuration and build the webserver and image projects. You should now have a webserver.flp boot image file in your release directory. This boot image contains a copy of sanos as well as your web server.

Copy this image to a floppy disk using the mkfloppy tool and boot the computer from the floppy. After booting sanos the webserver program executes. If you point your browser to the machine you can browser around in the sanos file system on the floppy. You now have a simple standalone web server on a floppy.

## Compiling webserver.c for sanos using on Linux / Mac

use Makefile:

    make
    
