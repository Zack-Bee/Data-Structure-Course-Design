#include <stdint.h>

#ifndef DICT_H
#define DICT_H

#define DICT_OK 1
#define DICT_ERR 0
#define DICT_INIT_SIZE 32

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
    uint64_t size;    // 哈希表大小
    uint64_t sizemask;    // 哈希表大小掩码，总是等于size-1，用于计算索引
    uint64_t used;    // 已经使用的节点的数量
} dictht;

typedef struct dict {
    dictht hashtable[2];    // 两个hash表，第二个用于rehash
    uint64_t rehashIndex;    // rehash的标志,当没有进行rehash时,值为-1(不过我不知道为什么是idx)
} dict;

/**
 *     创建一个dict
 *     @return 返回创建的字典的地址
 */
dict *newDict(void);

/**
 *     将dict进行rehash
 *     @param dict 进行操作的dict
 *     @param (*getStr)(void *obj) 函数指针, 得到传入参数的字符串
 */
void rehashDict(dict *dt, char *(*getStr)(void *obj));

/**
 *     根据key在dict中返回对应的key
 *     @param dt 进行查询的dict
 *     @param key 查询根据的key
 *     @param (*getStr)(void *obj) 函数指针, 得到传入参数的字符串
 *     @return 返回查询到的key
 */
void *getDictKey(dict *dt, void *key, char *(*getStr)(void *obj));

/**
 *     返回hashtable[0]的used/size的比
 *     @param dt 进行操作的dict
 *     @return返回hashtable[0]的used/size的比
 */ 
double getDictRadio(dict *dt);

/**
 *     判断dict是否正在进行rehash
 *     @param dt 进行判断的dict
 *     @return 1或0, 1代表正在rehash, 0代表没有rehash
 */
int isDictRehashing(dict *dt); 

/**
 *     根据key在dict中找到对应值
 *     @param dt 进行查询的dict
 *     @param key 查询根据的key
 *     @param (*getStr)(void *obj) 函数指针, 得到传入参数的字符串
 *     @return 返回查询到的val
 */
void *getDictVal(dict *dt, void *key, char *(*getStr)(void *obj));

/**
 *     为key设置值
 *     @param dt 进行操作的dict
 *     @param key 设置key的值,如果已经存在对值进行更改
 *     @param (*getStr)(void *obj) 函数指针, 得到传入参数的字符串
 */
void setDictEntry(dict *dt, void *key, void *val, char *(*getStr)(void *obj));

/**
 *     删除dict的val
 *     @param dt 进行操作的dict
 *     @param key 进行查询的key
 *     @param (*getStr)(void *obj) 函数指针, 得到传入参数的字符串
 */
void delDictEntry(dict *dt, void *key, char *(*getStr)(void *obj));

/**
 *     销毁dict
 *     @param dt 进行操作的dict
*/
void destroyDict(dict *dt);

#endif    // DICT_H