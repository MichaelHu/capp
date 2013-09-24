#include <stdio.h>
#include <errno.h>

int my_write(int fd, void *buffer, int length){
    int bytes_left;
    int bytes_written;
    char *ptr;

	ptr = buffer;
    bytes_left = length;

    while(bytes_left > 0){
        bytes_written = write(fd, ptr, bytes_left);

        // 读错误
        if(bytes_written <= 0){
            if(errno == EINTR){
                bytes_written = 0;
            }
            else{
                return -1;
            }
        }

        bytes_left -= bytes_written;
        ptr += bytes_written;
    }

    return 0;
}
