#include <stdio.h>
/**
 * 省略若干头文件
 * */

int main(){

    /*
     * 省略 初始化、bind  listen
     * */

    int connfd = accept();
    if(connfd < 0){
        //
    }
    else{
        int pipefd[2];
        assert(ret != -1);
        ret = pipe(pipefd);//创建管道
        //将connfd上流入的客户数据定向到管道中
        ret = splice(connfd, NULL, pipefd[1], NULL, 32768,
                     SPLICE_F_MORE | SPLICE_F_MOVE);
        assert(ret != -1);
        //将管道的输出重定向到connfd客户连接文件描述符
        ret = splice(pipefd[0], NULL, connfd, NULL, 32768,
                     SPLICE_F_MORE | SPLICE_F_MOVE);
        assert(ret != -1);
        close(connfd);
    }

    return 0;
}
