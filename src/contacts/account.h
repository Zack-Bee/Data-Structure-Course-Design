#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "dict.h"

/** 账号结构 */
typedef struct account {

    /** 账号的密码 */
    sds *password;

    /** 联系人字典 */ 
    dict *contacts;

    /** 联系人群组 */ 
    dict *groups;

    /** 销毁函数的指针 */
    void (*destroy)(struct account *act);
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

/**
 *     设置account的password
 *     @param act 进行修改的account
 *     @param password 设置的password
 */
void setAccountPassword(account *act, char *password);

/**
 *     设置account的contacts
 *     @param act 进行设置的account
 *     @param name 联系人的名字
 *     @param phone 联系人的电话
 */
void setAccountContacts(account *act, char *name, char *phone);

/**
 *     设置account中的群组成员的名字和电话号码
 *     @param act 进行操作的account
 *     @param group 群组的名字
 *     @param name 联系人的名字
 *     @param phone 联系人的电话
 */
void setAccountGroupsMember(account *act, char *group, char *name, char *phone);

/**
 *     删除account的联系人中的成员
 *     @param act 进行删除的account
 *     @param name 联系人的名字
*/
void delAccountContacts(account *act, char *name);

/**
 *     删除account的群组中的联系人
 *     @param act 进行删除的账号
 *     @param group 进行删除的群组名
 *     @param name 要从群组中删除的联系人的名字
 */
void delAccountGroupsMember(account *act, char *group, char *name);

/**
 *     删除account的群组
 *     @param act 进行删除操作的account
 *     @param group 要删除的群组的名字
*/
void delAccountGroups(account *act, char *group);

void getAccountAll(account *act, sds *s);
#endif