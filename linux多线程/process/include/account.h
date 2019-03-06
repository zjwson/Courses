#ifndef __ACCOUNT_H_
#define __ACCOUNT_H_

typedef struct
{
    int code;
    double balance;
    int semid;//在共享资源上绑定一个信号量集
}Account;

//取款
extern double withdraw(Account* a, double  amt);
//存款
extern double deposit(Account* a, double amt);
//查看账户余额
extern double get_balance(Account* a);

#endif
