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
    dictEntry *table;    // 哈希表数组 
    unsigned long size;    // 哈希表大小
    unsigned long sizemask;    // 哈希表大小掩码，总是等于size-1，用于计算索引
    unsigned long used;    // 已经使用的节点的数量
} dictht;

typedef struct dict {
    void *privdata;    // 私有数据,用于使用定制函数
    dictht hashtable[2];    // 两个hash表，第二个用于rehash
    int rehashIndex;    // rehash的标志,当没有进行rehash时,值为-1(不过我不知道为什么是idx)
} dict;

/**
 *     判断dict是否正在进行rehash
 *     @param dt 进行操作的dict
 *     @return 1表示正在rehash, 0表示没有进行rehash
 */
int isDictRehashing(dict *dt);

/**
 *     创建一个dict
 *     @return 返回创建的字典的地址
 */
dict *newDict(void);

/**
 *     将dict进行rehash
 *     @param dict 进行操作的dict
 */
void rehashDict(dict *dt);

/**
 *     将val, key存入字典, 如果已经存在则不进行操作
 *     @param dt 进行操作的dict
 *     @param key dict的键
 *     @param val dict的值
 */
void addDictEntry(dict *dt, void *key, void *val);

/**
 *     根据key在dict中找到对应值
 *     @param dt 进行查询的dict
 *     @param key 查询根据的key
 *     @return 返回查询到的key
 */
void *getDictVal(dict *dt, void *key);

/**
 *     为key设置值
 *     @param dt 进行操作的dict
 *     @param key 设置key的值,如果已经存在对值进行更改
 *     @param val 设置的值
 */
void setDictVal(dict *dt, void *key, void *val);

/**
 *     删除dict的val,如果存在多个则删除后加入的
 *     @param dt 进行操作的dict
 *     @param key 进行查询的key
 */
void delDictEntry(dict *dt, void *key);

/**
 *     销毁dict
 *     @param dt 进行操作的dict
*/
void destroyDict(dict *dt);
#endif    // DICT_H