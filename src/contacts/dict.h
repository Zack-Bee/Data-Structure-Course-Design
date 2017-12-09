#include <stdint.h>

#ifndef DICT_H
#define DICT_H

#define DICT_OK 1
#define DICT_ERR 0

// 哈希表节点
typedef struct dictEntry {
    void *key;
    void *val;

    // 指向下一个hash表节点，形成链表，用来解决冲突
    struct dictEntry *next;
} dictEntry;

// hash table
typedef struct dictht {
    dictEntry **table;    // 哈希表数组 
    unsigned long size;    // 哈希表大小
    unsigned long sizemask;    // 哈希表大小掩码，总是等于size-1，用于计算索引
    unsigned long used;    // 已经使用的节点的数量
} dictht;

typedef struct dict {
    void *privdata;    // 私有数据,用于使用定制函数
    dictht hashtable[2];    // 两个hash表，第二个用于rehash
    int rehashIndex;    // rehash的标志,当没有进行rehash时,值为-1(不过我不知道为什么是idx)
} dict;

#endif    // DICT_H