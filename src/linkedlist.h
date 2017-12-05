#include <stdint.h>

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct listNode {
    struct listNode *prev;
    struct listNode *next;
    void *ptr;
} listNode;

typedef struct linkedlist {
    uint64_t length;
    listNode *head; // 头指针
    listNode *tail; // 尾指针
} linkedlist;

/**
 *     将node从li中删除并释放掉node的空间
 *     @param li 进行操作的线性表
 *     @param node 要删除的节点
 */
void deleteNode(linkedlist *li, listNode *node);

/**
 *     从li中找到obj，节点存在返回节点，不存在返回NULL
 *     @param li 进行操作的线性表
 *     @param obj 需要被查找的obj
 *     @compare 比较函数，当obj1等于obj2返回0
 *     @return 返回查找到的节点
 */
listNode *getNode(linkedlist *li, void *obj,
                  int (*compare)(void *obj1, void *obj2));

/**
 *     创建一个节点，并将其插入linkedlist的尾部，节点的ptr指向obj
 *     @param li 进行操作的线性表
 *     @param obj ptr指向的对象
 * */  
void addNode(linkedlist *li, void *obj);

/**
 *     销毁linkedlist
 *     @param li 进行操作的linkedlist
*/
void destroyList(linkedlist *li);

/**
 *     malloc空间，创建一个新的linkedlist
 *     @return 返回linkedlist的指针
*/
linkedlist *newLinkedlist(void);
#endif