#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char* cmd1[3] = {"/bin/cat", "/etc/passwd", NULL};
char* cmd2[3] = {"/bin/grep", "root", NULL};

int main(){

    int fd[2];
    if(pipe(fd) < 0){
        perror("pipe error");
        exit(1);
    }
    int i = 0;
    pid_t pid;
    for(; i < 2; ++i){
        pid = fork();
        if(pid < 0){
            perror("fork error");
            exit(1);
        }
        else if(pid == 0){
            //子进程处理
            if(i == 0){
                //第一个子进程负责往管道写入数据
                

                break;
            }
            if(i == 1){
                //第二个子进程，负责从管道读取数据
                

                break;
            }
        }
        else{
            //父进程
            if(i == 1){
                //父进程要等到子进程全部创建完成才去回收
                
                wait(0);
                wait(0);
            }
        }
    }


    return 0
}
