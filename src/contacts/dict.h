#include <stdint.h>
#include "sds.h"
#include "list.h"

#ifndef DICT_H
#define DICT_H

#define DICT_INIT_SIZE 32


/**
 * 字典结构, 每一个hashtable的节点第一个为空
 */
typedef struct dict {

    /** 线性表数组, hashtable */
    list **table;

    /** 哈希表大小 */
    uint32_t size;

    /** 已经使用的节点的数量 */ 
    uint32_t used;
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
dictEntry *delDictEntry(dict *dt, void *key, char *(*getStr)(void *obj));

/**
 *     销毁dict
 *     @param dt 进行操作的dict
 */
void destroyDict(dict *dt);

#endif    // DICT_H