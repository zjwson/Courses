#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

//信号处理函数
void sig_handler(int signo){
    //检测是否是子线程捕获到信号 
    printf("pthread id in the sig_handler: 0x%lx\n", pthread_self());
    if(signo == SIGALRM){
        printf("timeout...\n");
    }
    alarm(2);
}

void* th_fn(void* arg){

    if(signal(SIGALRM, sig_handler) == SIG_ERR){
        perror("signal sigalrm error");
    }
    //在子线程设置定时器
    alarm(2);

    int i;
    for(i = 1; i <= 100; ++i){
        printf("thread(0x%lx) i: %d\n", pthread_self(), i);
        sleep(1);
    }

    return (void*)0;
}

void* th_fn1(void* arg){

    if(signal(SIGALRM, sig_handler) == SIG_ERR){
        perror("signal sigalrm error");
    }
    int i;
    for(i = 1; i <= 100; ++i){
        printf("thread(0x%lx) i: %d\n", pthread_self(), i);
        sleep(1);
    }

    return (void*)0;
}

int main(){

    int err;
    pthread_t th, th1;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if((err = pthread_create(&th, &attr, th_fn, (void*)0)) != 0){
        perror("pthread create error");
    }
    if((err = pthread_create(&th1, &attr, th_fn1, (void*)0)) != 0){
        perror("pthread create error");
    }

    //以下是主控线程运行部分，可对主控线程执行信号屏蔽函数
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGALRM);
    //对主控线程屏蔽SIGALRM信号
    pthread_sigmask(SIG_SETMASK, &set, NULL);

    //子线程以分离状态启动，主线程不能再调用pthread_join，否则出错
    while(1){
        printf("control thread(0x%lx) is running...\n", pthread_self());
        sleep(10);
    }
    printf("control thread over\n");

    return 0;
}
