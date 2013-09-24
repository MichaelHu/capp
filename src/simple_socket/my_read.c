#include <stdio.h>
#include <errno.h>

int my_read(int fd, void *buffer, int length){
    int bytes_left;
    int bytes_read;
    char *ptr;

	ptr = buffer;
    bytes_left = length;

    while(bytes_left > 0){
        bytes_read = read(fd, ptr, bytes_left);

        // 读错误
        if(bytes_read < 0){
            if(errno == EINTR){
                bytes_read = 0;
            }
            else{
                return -1;
            }
        }
        // 读取至末尾
        else if(bytes_read == 0){
            break;
        }

        bytes_left -= bytes_read;
        ptr += bytes_read;
    }

    return (length - bytes_left);
}
