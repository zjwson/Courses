#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct
{
    int res;
    int is_wait;//用户给出用于判断的条件
    pthread_cond_t cond;//条件变量
    pthread_mutex_t mutex;//互斥锁
}Result;

//计算并将结果防止Result中的线程运行函数
void* set_fn(void* arg){

    int i = 1,  sum = 0;
    for(; i <= 100; ++i){
        sum += i;
    }
    Result* r = (Result*)arg;
    //将计算结果放置到Result的res中
    r->res = sum;

    //对两个线程共享的判断条件进行保护
    pthread_mutex_lock(&r->mutex);

    //判断获得结果的线程是否准备好
    while(!r->is_wait){
        pthread_mutex_unlock(&r->mutex);
        usleep(100);
        pthread_mutex_lock(&r->mutex);
    }
    pthread_mutex_unlock(&r->mutex);

    //唤醒 等待的线程
    pthread_cond_broadcast(&r->cond);

    return (void*)0;
}

void* get_fn(void* arg){

    Result* r = (Result*)arg;

    //对两个线程共享的判断条件进行保护（加锁）
    //两个 线程对判断条件操作是互斥的
    pthread_mutex_lock(&r->mutex);

    //代表获取结果的线程已准备好
    r->is_wait = 1;

    //获取结果的线程等待
    pthread_cond_wait(&r->cond, &r->mutex);

    //线程被唤醒后
    pthread_mutex_unlock(&r->mutex);

    //去获取计算的结果
    int res = r->res;
    printf("0x%lx get sum is %d\n", pthread_self(), res);

    return (void*)0;
}

int main(){

    Result r;
    r.is_wait = 0;
    pthread_cond_init(&r.cond, NULL);
    pthread_mutex_init(&r.mutex, NULL);

    int err;
    pthread_t cal, get;

    //启动获取结果的线程
    if((err = pthread_create(&get, NULL, get_fn, (void*)&r)) != 0){
        perror("pthread_create error");
    }

    //启动计算结果的线程
    if((err = pthread_create(&cal, NULL, set_fn, (void*)&r)) != 0){
        perror("pthread_create error");
    }

    pthread_join(get, NULL);
    pthread_join(cal, NULL);

    pthread_mutex_destroy(&r.mutex);
    pthread_cond_destroy(&r.cond);

    return 0;
}
