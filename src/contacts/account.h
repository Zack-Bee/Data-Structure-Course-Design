#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "dict.h"

typedef struct account {
    sds *password;
    dict *contacts;
    dict *groups;
} account;

/**
 *     创建一个account
 *     @return 返回创建的account
 */
account *newAccount();

/**
 *     销毁account
 *     @param act 进行销毁的account
 */
void destroyAccount(account *act);


#endif