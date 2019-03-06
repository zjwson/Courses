#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct
{
    char name[20];//名字
    int time;//睡眠时间
    int start;//起点
    int end;//终点
}RaceArg;

void* th_fn(void *arg){
    RaceArg *r = (RaceArg*)arg;
    int i = r->start;
    for(; i <= r->end; ++i){
        printf("%s(%lx) running %d\n", r->name, pthread_self(), i);
        usleep(r->time);
    }

    return (void*)0;
}

int main(){

    int err;
    pthread_t rabbit, turtle;

    RaceArg r_a = {"rabbit", (int)(drand48()*100000000), 20, 50};
    RaceArg t_a = {"turtle", (int)(drand48()*100000000), 10, 60};

    if((err = pthread_create(&rabbit, NULL, th_fn, (void*)&r_a)) != 0){
        perror("pthread_create error");
    }
    if((err = pthread_create(&turtle, NULL,th_fn, (void*)&t_a)) != 0){
        perror("pthread_create error");
    }

    pthread_join(rabbit, NULL);
    pthread_join(turtle, NULL);

    return 0;
}
