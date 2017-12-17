#include <stdint.h>

#ifndef LIST_H
#define LIST_H

typedef struct listNode {
    struct listNode *prev;
    struct listNode *next;
    sds *key;
    void *val;
} listNode;

typedef struct list {

    /** 线性表的长度 */
    uint32_t length;

    /** 头指针 */
    listNode *head; 

    /** 尾指针 */
    listNode *tail; 
} list;

/**
 *     malloc空间，创建一个新的linkedlist
 *     @return 返回linkedlist的指针
 */
list *newList(void);

/**
 *     得到list的长度
 *     @param li 进行操作的list
 *     @return list的长度
*/
uint32_t getListLength(list *li);

/**
 *     得到listHead
 *     @param li 进行操作的list
 *     @return 如果listHead存在的话, 返回, 否则返回NULL
*/
listNode *getListHead(list *li);

/**
 *     设置节点的key, val, 如果节点中相应的key不存在, 则创建新的节点插入
 *     @param li 进行操作的list
 *     @param key 进行查询的key
 *     @param val 设置的val
 *     @return 如果新建了节点加入, 则返回1, 否则返回0
 */ 
uint32_t setListNode(list *li, char *key, void *val);

/**
 *     根据key删除节点
 *     @param li 进行操作的list
 *     @param key 进行查询的key
 *     @return 如果删除了节点, 则返回1, 否则返回0
 */ 
uint32_t delListNode(list *li, char *key);

/**
 *     销毁linkedlist
 *     @param li 进行操作的linkedlist
 */
void destroyList(list *li);

#endif    // LINKEDLIST_H