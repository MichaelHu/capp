/**
 * demo来自：http://www.cnblogs.com/skynet/archive/2010/10/30/1865267.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

int g_Flag = 0;

void *thread1(void *);
void *thread2(void *);

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int main(int argc, char **argv){

    printf("enter main\n");
    
    pthread_t tid1, tid2;
    int rc1 = 0, rc2 = 0;

    rc2 = pthread_create(&tid2, NULL, thread2, NULL);

    if(rc2 != 0){
        printf("%s: %d\n", __func__, strerror(rc2));
    }

    // 最后一个参数用于thread1调用pthread_join时使用
    rc1 = pthread_create(&tid1, NULL, thread1, &tid2);

    if(rc1 != 0){
        printf("%s: %d\n", __func__, strerror(rc1));
    }

    pthread_cond_wait(&cond, &mutex);

    printf("leave main\n");
    exit(0);
}

void *thread1(void *arg){

    printf("enter thread1\n");
    printf("this is thread1, g_Flag: %d, thread id is %u\n"
        ,g_Flag
        ,(unsigned int)pthread_self());

    pthread_mutex_lock(&mutex);

    if(g_Flag == 2){
        pthread_cond_signal(&cond);
    }

    g_Flag = 1;

    printf("this is thread1, g_Flag: %d, thread id is %u\n"
        ,g_Flag
        ,(unsigned int)pthread_self());

    pthread_mutex_unlock(&mutex);

    pthread_join(*(pthread_t *)arg, NULL);
    printf("leave thread1\n");
    pthread_exit(0);
}

void *thread2(void *arg){

    printf("enter thread2\n");
    printf("this is thread2, g_Flag: %d, thread id is %u\n"
        ,g_Flag
        ,(unsigned int)pthread_self());

    pthread_mutex_lock(&mutex);

    if(g_Flag == 1){
        pthread_cond_signal(&cond);
    }

    g_Flag = 2;

    printf("this is thread2, g_Flag: %d, thread id is %u\n"
        ,g_Flag
        ,(unsigned int)pthread_self());

    pthread_mutex_unlock(&mutex);

    printf("leave thread2\n");
    pthread_exit(0);
}
