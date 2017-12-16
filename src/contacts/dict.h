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

typedef struct dict {
    dictEntry *table;    // 哈希表数组 
    uint32_t size;    // 哈希表大小
    uint32_t used;    // 已经使用的节点的数量    
} dict;

/**
 *     创建一个dict
 *     @return 返回创建的字典的地址
 */
dict *newDict(void);

/**
 *     将dict进行expand
 *     @param dict 进行操作的dict
 *     @param (*getStr)(void *obj) 函数指针, 得到传入参数的字符串
 */
void expandDict(dict *dt, char *(*getStr)(void *obj));

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