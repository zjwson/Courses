#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "account.h"

int main(){

    //在共享内存中创建银行账户
    int shmid;
    if((shmid = shmget(IPC_PRIVATE, sizeof(Account), IPC_CREAT | IPC_EXCL | 0777)) < 0){
        perror("shmget error");
        exit(1);
    }

    //进行共享内存映射
    Account* a = (Account*)shmat(shmid, 0, 0);
    if(a == (Account*)-1){
        perror("shmat error");
        exit(1);
    }
    //初始化银行账户
    a->code = 100001;
    a->balance = 10000;

    //创建信号量集并初始化（1个信号量集/信号灯，初始值为 1）
    a->semid = I(1, 1);
    if(a->semid < 0){
        perror("I(1, 1) init error");
        exit(1);
    }
    printf("balance: %f\n", a->balance);

    pid_t pid;
    if((pid = fork()) < 0){
        perror("fork error");
        exit(1);
    }
    else if(pid > 0){
        //parent process执行取款操作
        double amt = withdraw(a, 10000);
        printf("pid %d withdraw %f from code %d\n", getpid(), amt, a->code);
        wait(0);

        //对共享内存的操作要在解除映射之前
        printf("balance: %f\n", a->balance);
        //销毁信号量 集
        D(a->semid);
        //解除共享内存的映射
        shmdt(a);
        //释放共享内存
        shmctl(shmid, IPC_RMID, NULL);
    }
    else{
        //子进程也执行取款操作
        double amt = withdraw(a, 10000);
        printf("pid %d withdraw %f from code %d\n", getpid(), amt, a->code);

        //解除共享内存映射
        shmdt(a);
    }

    return 0;
}
