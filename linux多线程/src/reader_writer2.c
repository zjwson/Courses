#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

typedef struct{
    
    int value;
    
    pthread_cond_t rc;
    pthread_mutex_t rm;
    int r_wait;

    pthread_cond_t wc;
    pthread_mutex_t wm;
    int w_wait;
    int w_ready;
}Storage;

#define N 5

void set_data(Storage* s, int value){
    s->value = value;
}

int get_data(Storage* s){
    return s->value;
}

void* set_fn(void* arg){
    
    Storage* s = (Storage*)arg;

    int i = 1;
    for(; i <= 100; ++i){
        set_data(s, i+100);

        printf("0x%lx(%-3d) write data: %d\n", pthread_self(), i, i+100);

        //写好后，检查读端是否已经准备好
        pthread_mutex_lock(&s->rm);
        while(s->r_wait < N){
            pthread_mutex_unlock(&s->rm);
            sleep(1);
            pthread_mutex_lock(&s->rm);
        }
        s->r_wait = 0;
        pthread_mutex_unlock(&s->rm);
        //当所有读者线程都准备好后，再去唤醒
        pthread_cond_broadcast(&s->rc);

        /*
         * 写端阻塞等待，等待读端读完后唤醒
         * 等所有读端都读完后，再去写
         * */

        pthread_mutex_lock(&s->wm);
        s->w_ready = 1;
        pthread_cond_wait(&s->wc, &s->wm);
        pthread_mutex_unlock(&s->wm);
        s->w_wait = 0;        
    }

    return (void*)0;
}

void* get_fn(void* arg){

    Storage* s = (Storage*)arg;

    int i = 1;
    for(; i <= 100; ++i){
        pthread_mutex_lock(&s->rm);
        s->r_wait++;
        pthread_cond_wait(&s->rc, &s->rm);
        pthread_mutex_unlock(&s->rm);
        
        int value = get_data(s);
        printf("0x%lx(%-3d) read data: %d\n", pthread_self(), i, value);

        //读完数据后阻塞等待写端准备好
        pthread_mutex_lock(&s->wm);
        s->w_wait++;
        while(!s->w_ready || s->w_wait != N){//等待 写端准备好
            pthread_mutex_unlock(&s->wm);
            sleep(1);
            pthread_mutex_lock(&s->wm);
        }

        s->w_wait = 0;
        pthread_mutex_unlock(&s->wm);
        pthread_cond_broadcast(&s->wc);
    }

    return (void*)0;
}

int main(){

    int err;
    pthread_t writer;
    pthread_t reader[N];
    Storage s;
    s.r_wait = 0;
    s.w_wait = 0;

    pthread_cond_init(&s.rc, NULL);
    pthread_cond_init(&s.wc, NULL);
    pthread_mutex_init(&s.rm, NULL);
    pthread_mutex_init(&s.wm, NULL);


    if((err = pthread_create(&writer, NULL, set_fn, (void*)&s)) < 0){
        perror("pthread_create error");
    }
    for(int i = 0; i < N; ++i){
        if((err = pthread_create(&reader[i], NULL, get_fn, (void*)&s)) < 0){
            perror("pthread_create error");
        }
        
    }


    pthread_join(writer, NULL);
    for(int i = 0; i < N; ++i){   
        pthread_join(reader[i], NULL);
    }

    pthread_cond_destroy(&s.rc);
    pthread_cond_destroy(&s.wc);
    pthread_mutex_destroy(&s.rm);
    pthread_mutex_destroy(&s.wm);

    return 0;
}
