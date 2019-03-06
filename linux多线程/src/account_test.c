#include "../include/account.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char name[20];
    Account* account;
    double amt;
}OperArg;

//定义取款操作的线程运行函数
void* withdraw_fn(void* arg)
{
    OperArg* oper = (OperArg*)arg;
    double amt = withdraw(oper->account, oper->amt);

    printf("%8s(0x%lx) withdraw %f from account %d\n",
           oper->name, pthread_self(), amt, oper->account->code);
    

    return (void*)0;
}

//定义存款操作的线程运行函数
void* deposit_fn(void* arg)
{
    OperArg* oper = (OperArg*)arg;
    double amt = deposit(oper->account, oper->amt);

    printf("%8s(0x%lx) deposit %f to account %d\n",
           oper->name, pthread_self(), amt, oper->account->code);
    

    return (void*)0;
}

//定义查看银行账户的线程运行函数 
void* check_fn(void* arg)
{
    OperArg* oper = (OperArg*)arg;

    printf("%8s(0x%lx) check account %d, the balance is %f\n",
           oper->name, pthread_self(), oper->account->code, oper->account->balance);

    return (void*)0;
}

int  main(){
    
    int err;
    pthread_t boy, girl;
    Account* a = create_account(100001, 10000);

    OperArg o1, o2;
    strcpy(o1.name, "boy");
    o1.account = a;
    o1.amt = 10000;

    strcpy(o2.name, "girl");
    o2.account = a;
    o2.amt = 10000;

    //启动两个线程同时操作同一个银行账户
    if((err = pthread_create(&boy, NULL, withdraw_fn, (void*)&o1)) != 0){
        perror("pthread create error");
    }

    if((err = pthread_create(&girl, NULL, withdraw_fn, (void*)&o2)) != 0){
        perror("pthread create error");
    }

    pthread_join(boy, NULL);
    pthread_join(girl, NULL);

    printf("account balance: %f\n", get_balance(a));

    return 0;
}
