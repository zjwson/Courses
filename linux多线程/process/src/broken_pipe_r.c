#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

/*
 * 不完整管道：读取一个写端已经关闭的管道
 * */

int main(){

    int fd[2];
    if(pipe(fd) < 0){
        perror("pipe error");
        exit(1);
    }
    pid_t pid;
    if((pid = fork()) < 0){
        perror("fork error");
        exit(1);
    }
    else if(pid == 0){
        //child process
        //write data
        close(fd[0]);
        char *s = "1234";
        write(fd[1], s, strlen(s));
        close(fd[1]);        
    }
    else{
        //parent process
        //read data
        sleep(2);
        close(fd[1]);
        while(1){
            char c;
            if(read(fd[0], &c, 1) == 0){
                printf("\nwrite end of pipe closed!\n");
                break;
            }
            printf("%c", c);
        }
        close(fd[0]);
        wait(0);
    }

    return 0;
}
