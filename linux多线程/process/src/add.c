#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(){

    int x = 0, y = 0;
    if(read(STDIN_FILENO, &x, sizeof(int)) < 0){
        perror("read error");
    }
    if(read(STDIN_FILENO, &y, sizeof(int)) < 0){
        perror("read error");
    }

    int result = x + y;

    if(write(STDOUT_FILENO, &result, sizeof(int)) != sizeof(int)){
        perror("write  error");
    }

    return 0;
}
