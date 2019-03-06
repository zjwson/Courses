#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]){

    if(argc < 2){
        printf("usage:%s fifo\n", argv[0]);
        exit(1);
    }

    printf("open fifo read...\n");
    //打开命名管道
    int fd = open(argv[1], O_RDONLY);
    if(fd < 0){
        perror("open error");
        exit(1);
    }
    else{
        printf("open  file success: %d\n", fd);
    }

    //从命名管道中读取数据
    char buf[512];
    memset(buf, 0, sizeof(buf));
    while(read(fd, buf, sizeof(buf)) < 0){
        perror("read error");
    }
    printf("%s\n", buf);

    exit(0);
}
