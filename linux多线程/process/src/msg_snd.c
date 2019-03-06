#include <stdio.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef struct{
    long type;//消息类型
    int start;//消息数据本身
    int end;
}MSG;

int main(int argc, char* argv[]){

    if(argc < 2){
        printf("usage: %s key\n", argv[0]);
        exit(1);
    }

    key_t key = atoi(argv[1]);
    //key_t key = ftok(argv[1], 0);//一般第一个参数传一个路径
    printf("key: %d\n", key);

    //创建消息队列 
    int msg_id;
    if((msg_id = msgget(key, IPC_CREAT|IPC_EXCL|0777)) < 0){
        perror("msgget  error");
    }
    printf("msg id: %d\n", msg_id);

    //定义要发送的消息
    MSG m1 = {4, 4, 400};
    MSG m2 = {2, 2, 200};
    MSG m3 = {1, 1, 100};
    MSG m4 = {6, 6, 600};
    MSG m5 = {6, 60, 6000};

    //发送消息到消息队列(第二个参数是消息本身的长度) 
    if(msgsnd(msg_id, &m1, sizeof(MSG) - sizeof(long), IPC_NOWAIT) < 0){
        perror("msgsnd error");
    }
    if(msgsnd(msg_id, &m2, sizeof(MSG) - sizeof(long), IPC_NOWAIT) < 0){
        perror("msgsnd error");
    }
    if(msgsnd(msg_id, &m3, sizeof(MSG) - sizeof(long), IPC_NOWAIT) < 0){
        perror("msgsnd error");
    }
    if(msgsnd(msg_id, &m4, sizeof(MSG) - sizeof(long), IPC_NOWAIT) < 0){
        perror("msgsnd error");
    }
    if(msgsnd(msg_id, &m5, sizeof(MSG) - sizeof(long), IPC_NOWAIT) < 0){
        perror("msgsnd error");
    }

    //发送后获取消息队列中消息的总数
    struct msqid_ds ds;
    if(msgctl(msg_id, IPC_STAT, &ds) < 0){
        perror("msgclt error");
    }
    printf("msg total:%ld\n", ds.msg_qnum);

    exit(0);
}
