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

    /** 销毁list的函数的指针 */
    void (*destroy)(list *li);
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
 *     设置节点的key, val, 如果节点中相应的key不存在, 则创建新的节点插入.
 *     如果存在的话, 将原来的的val销毁
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
 *     根据key返回相应listNode中的val
 *     @param li 进行查询的list
 *     @param key 用于查询的key
 *     @return 返回查询到的val, 如果没有结果, 返回NULL
*/
void *getListVal(list *li, char *key);

/**
 *     使用function对list进行遍历
 *     @param li 进行遍历的list
 *     @param function 遍历使用的函数, 其中param是listNode的val
 */
void traverseList(list *li, void (*function)(void *param));

/**
 *     销毁linkedlist
 *     @param li 进行操作的linkedlist
 */
void destroyList(list *li);

#endif // LINKEDLIST_H