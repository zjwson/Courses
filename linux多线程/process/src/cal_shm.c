#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "tell.h"

typedef struct{
    int age;
    char* name;
    char sex;
}Person;

int main(){

    //创建共享内存(注意：共享内存创建好后，所有进程是共享的)
    int shmid;
    if((shmid = shmget(IPC_PRIVATE, 1024, IPC_CREAT | IPC_EXCL | 0777)) < 0){
        perror("shmget error");
        exit(1);
    }

    pid_t pid;
    init();//初始化管道
    if((pid = fork()) < 0){
        perror("fork error");
        exit(1);
    }
    else  if(pid > 0){
        //parent process
        Person* pi = (Person*)shmat(shmid, 0, 0);
        pi->age = 18;
        pi->name = "zhaijunwei";
        pi->sex = 'F';
       /* int *pi = (int*)shmat(shmid, 0, 0);
        if(pi == (int*)-1){
            perror("shmat error");
            exit(1);
        }
        //往共享内存中写入数据（通过操作映射的地址即可）
        *pi = 100;
        *(pi + 1) = 200;
        */
        //操作完毕解除共享内存的映射
        shmdt(pi);
        //通知子进程去读取数据
        notify_pipe();
        wait(0);
        destroy_pipe();
    }
    else{//child process
        //子进程阻塞，等待父进程先往共享内存中写入数据
        wait_pipe();
       //子进程去共享内存读取数据
       //子进程进行共享内存的映射
/*       int* pi = (int*)shmat(shmid, 0, 0);
       if(pi == (int*)-1){
           perror("shmat error");
           exit(1);
       }
       printf("start: %d, end: %d\n", *pi, *(pi+1));
       */
       
       Person* pi = (Person*)shmat(shmid, 0, 0) ;
       printf("name: %s age: %d sex: %c\n", pi->name, pi->age, pi->sex);

       //读取完毕后解除映射
       shmdt(pi);
       //删除共享内存
       shmctl(shmid, IPC_RMID, NULL);

       destroy_pipe();
    }

    return 0;
}
