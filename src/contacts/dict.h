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

    /** 销毁dict的函数指针 */
    void (*destroy)(dict *dt);
} dict;

/**
 *     创建一个dict
 *     @return 返回创建的字典的地址
 */
dict *newDict(void);

/**
 *     将dict进行expand
 *     @param dict 进行操作的dict
 */
void expandDict(dict *dt, char *(*getStr)(void *obj));

/**
 *     返回hashtable[0]的used/size的比
 *     @param dt 进行操作的dict
 *     @return 返回hashtable[0]的used/size的比
 */ 
double getDictRadio(dict *dt);

/**
 *     为key设置值
 *     @param dt 进行操作的dict
 *     @param key 设置key的值,如果已经存在对值进行更改
 *     @param val 设置val的值
 */
void setDictEntry(dict *dt, void *key, void *val);

/**
 *     删除dict的val
 *     @param dt 进行操作的dict
 *     @param key 进行查询的key
 */
void delDictEntry(dict *dt, void *key);

/**
 *     销毁dict
 *     @param dt 进行操作的dict
 */
void destroyDict(dict *dt);

/**
 *     使用function遍历dict
 *     @param dt 进行遍历的dict
 *     @param function 遍历进行使用的函数
*/
void traverseDict(dict *dt, void (*function)(void *param));

#endif    // DICT_H