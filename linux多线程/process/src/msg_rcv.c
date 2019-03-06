#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/msg.h>

typedef struct{
    long type;
    int start;
    int end;
}MSG;

int main(int argc, char* argv[]){
    
    if(argc < 3){
        printf("usage: %s key type\n", argv[0]);
        exit(1);
    }

    key_t key = atoi(argv[1]);//消息队列对应键值
    long type = atol(argv[2]);//要获取的消息类型

    //获得指定的消息队列(注意：这里只是获取指定key对应的消息队列，并不是创建消息队列)
    int msg_id;
    if((msg_id = msgget(key, 0777)) < 0){
        perror("msgget error");
    }
    printf("msg id:%d\n", msg_id);

    //从消息队列 中获取指定类型的消息
    MSG m;
    if(msgrcv(msg_id, &m, sizeof(MSG) - sizeof(long), type, IPC_NOWAIT) < 0){
        perror("msgrcv error");
    }
    else{
        printf("type: %ld  start: %d  end: %d\n", m.type, m.start, m.end);
    }

    exit(0);
}
