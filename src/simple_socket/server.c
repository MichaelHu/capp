#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

int my_read(int fd, void *buffer, int length);

int main(int argc, char *argv[])
{
    int sockfd, new_fd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int sin_size, portnumber;
    char hello[]="Hello! Are You Fine?\n";
    char ip[60];

    char response_headers[] = "\
HTTP/1.1 200 OK\r\n\
Date: Sun, 22 Sep 2013 06:16:28 GMT\r\n\
Server: BWS/1.0\r\n\
Content-Length: 4920\r\n\
Content-Type: text/html;charset=utf-8\r\n\
Cache-Control: private\r\n\
BDPAGETYPE: 1\r\n\
BDUSERID: 47120610\r\n\
BDQID: 0xdcbdea28030c5278\r\n\
Set-Cookie: BDSVRTM=40; path=/\r\n\
Set-Cookie: H_PS_PSSID=3359_3381_2776_1425_2980; path=/; domain=.baidu.com\r\n\
Expires: Sun, 22 Sep 2013 06:16:28 GMT\r\n\
Content-Encoding: gzip\r\n\
Connection: Keep-Alive\r\n\
\
\r\n\
";

    char buffer[10240];
    int read_len;

    if(argc!=2)
    {
        fprintf(stderr,"Usage:%s portnumber\a\n",argv[0]);
        exit(1);
    }

    if((portnumber=atoi(argv[1]))<0)
    {
        fprintf(stderr,"Usage:%s portnumber\a\n",argv[0]);
        exit(1);
    }

    /* 服务器端开始建立socket描述符 */
    // if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
    if( -1 == ( sockfd = socket( PF_INET, SOCK_STREAM, 0) ) )
    {
        fprintf(stderr,"Socket error:%s\n\a",strerror(errno));
        exit(1);
    }

    /* 服务器端填充 sockaddr结构 */
    bzero(&server_addr,sizeof(struct sockaddr_in));
    server_addr.sin_family=PF_INET;
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    server_addr.sin_port=htons(portnumber);

    /* 捆绑sockfd描述符 */
    if(bind(sockfd
		,(struct sockaddr *)(&server_addr)
		,sizeof(struct sockaddr))==-1)
    {
        fprintf(stderr,"Bind error:%s\n\a",strerror(errno));
        exit(1);
    }

    /* 监听sockfd描述符 */
    if(listen( sockfd, 5 ) == -1)
    {
        fprintf(stderr,"Listen error:%s\n\a",strerror(errno));
        exit(1);
    }

    fprintf(stderr, "Listening Port: %d\n", portnumber);

    while(1)
    {
        /* 服务器阻塞,直到客户程序建立连接 */
        sin_size=sizeof(struct sockaddr_in);

        fprintf(stderr, "sin_size: %d\n", sin_size);

        if((new_fd = accept(
				sockfd
				,(struct sockaddr *)(&client_addr)
				,&sin_size
			))==-1)
        {
            fprintf(stderr,"Accept error:%s\n\a",strerror(errno));
            exit(1);
        }

        /*
         * inet_ntoa在64位OS下会出错，使用线程安全的inet_ntop
        fprintf(stderr
			,"Server get connection from %s\n"
        	,inet_ntoa(client_addr.sin_addr));
        */

        bzero(&ip, sizeof(ip));
        inet_ntop(PF_INET, (void *)&client_addr.sin_addr.s_addr, ip, 60);
        fprintf(stderr
			,"Server get connection from %s\n"
        	,ip);

        // if(write(new_fd,hello,strlen(hello))==-1)
        // {
        //     fprintf(stderr,"Write Error:%s\n",strerror(errno));
        //     exit(1);
        // }

        if(write(new_fd, response_headers, strlen(response_headers))==-1)
        {
            fprintf(stderr,"Write Error:%s\n",strerror(errno));
            exit(1);
        }


        bzero(buffer, 10240);
        read_len = my_read(new_fd, (void *)buffer, 10239);
        fprintf(stderr
			,"read len: %d\n"
        	,read_len);

        fprintf(stderr
            ,"read: %s\n"
            ,(char *)buffer);

        if(write(new_fd, response_headers, strlen(response_headers))==-1)
        {
            fprintf(stderr,"Write Error:%s\n",strerror(errno));
            exit(1);
        }

        /* 这个通讯已经结束 */
        close(new_fd);
        /* 循环下一个 */
    }

    close(sockfd);
    exit(0);
}

