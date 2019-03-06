#ifndef __ACCOUNT_H_
#define __ACCOUNT_H_

typedef struct
{
    int code;
    double balance;
}Account;

//创建账户 
extern Account* create_account(int code,  double  balance);
//销毁账户
extern void  destroy_account(Account* a);
//取款
extern double withdraw(Account* a, double  amt);
//存款
extern double deposit(Account* a, double amt);
//查看账户余额
extern double get_balance(Account* a);

#endif
