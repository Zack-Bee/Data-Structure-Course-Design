#include "dict.h"
#include <malloc.h>
#include <stdint.h>
#include <string.h>


/********************************PRIVATE***************************************/
// BKDR Hash Function
uint32_t _BKDRHash(char *str) {
    uint32_t seed = 131; // 31 131 1313 13131 131313 etc..
    uint32_t hash = 0;

    while (*str) {
        hash = hash * seed + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

uint32_t _dictHashFunction(uint32_t size, void *obj,
                           char *(getStr(void *obj))) {
    return (_BKDRHash(getStr(obj)) % size);
}

/**
 *     创建一个新的dictEntry
 *     @param key 需要绑定的key
 *     @param val 需要绑定的val
 *     @return 返回生成的dictEntry
 */
dictEntry *_newDictEntry(void *key, void *val) {
    dictEntry *de = malloc(sizeof(dictEntry));
    de->key = key;
    de->val = val;
    de->next = NULL;
    return de;
}

/**
 *     寻找key对应的dictEntry
 *     @param ht 进行查找的hashtable
 *     @param key 用于查找的key
 *     @param size hashtable的大小
 *     @param getStr 用于获取str的函数
 *     @return de 符合key的dictEntry, 如果不存在返回空的dictEntry或NULL
 */
dictEntry *_getTableEntry(dictEntry ht[], void *key, uint32_t size,
                          char *(*getStr)(void *obj)) {
    dictEntry *de = NULL;
    char *str = getStr(key);
    de = &ht[_dictHashFunction(size, key, getStr)];
    while (de) {
        if (strcmp(str, getStr(de->key)) == 0) {
            return de;
        }
        de = de->next;
    }

    return NULL;
}


void _setTableEntry(dictEntry ht[], void *key, void *val, uint32_t size,
                    char *(*getStr)(void *obj)) {
    dictEntry *de = NULL;
    char *str = getStr(key);
    uint32_t hash = _dictHashFunction(size, key, getStr);
    de = &ht[hash];

    // 如果存在的话, 直接进行修改
    while (de && de->key) {
        if (strcmp(str, getStr(de->key)) == 0) {
            de->val = val;
            de->key = key;
            return;
        }
        de = de->next;
    }

    // 不存在的话, 创建dictEntry插入表头
    de = _newDictEntry(key, val);
    de->next = ht[hash].next;
    ht[hash].next = de;
}


/********************************PUBLIC****************************************/
dict *newDict(void) {
    dict *dt = malloc(sizeof(dict));
    dt->size = DICT_INIT_SIZE;
    dt->used = 0;
    dt->table = malloc(sizeof(dictEntry) * DICT_INIT_SIZE);

    // 初始化dictEntry
    for (int i = 0; i < DICT_INIT_SIZE; i++) {
        dt->table[i].val = NULL;
        dt->table[i].key = NULL;
        dt->table[i].next = NULL;
    }

    return dt;
}

void expandDict(dict *dt, char *(*getStr)(void *obj)) {
    dictEntry *newTable = malloc(sizeof(dictEntry) * dt->size * 2);
    dictEntry *cur = NULL;
    uint32_t hash;

    for (uint32_t i = 0; i < dt->size; i++) {
        cur = &dt->table[i];
        while (cur) {
            if (cur->key != NULL) {
                _setTableEntry(newTable, cur->key, cur->val, dt->size * 2,
                               getStr);
            }
            cur = cur->next;
        }
    }
    dt->size *= 2;
    free(dt->table);
    dt->table = newTable;
}

double getDictRadio(dict *dt) { return ((dt->used + 0.0) / dt->size); }

void *getDictKey(dict *dt, void *key, char *(*getStr)(void *obj)) {
    dictEntry *de = NULL;
    de = _getTableEntry(dt->table, key, dt->size, getStr);    
    if (de) {
        return de->key;
    }

    return NULL;
}

void *getDictVal(dict *dt, void *key, char *(*getStr)(void *obj)) {
    dictEntry *de = NULL;
    de = _getTableEntry(dt->table, key, dt->size, getStr);    
    if (de) {
        return de->val;
    }

    return NULL;
}

void setDictEntry(dict *dt, void *key, void *val, char *(*getStr)(void *obj)) {
    dictEntry *de = NULL;
    uint32_t hash;
    
}

#define DICT_TEST
#ifdef DICT_TEST

void testNewDict() {
    dict *dt = NULL;
    dt = newDict();
    int success = 1;
    if ((dt != NULL) && (dt->rehashIndex == -1) &&
        (dt->size == DICT_INIT_SIZE) && (dt->used == 0) &&
        (dt->table != NULL) && (dt->table == NULL)) {
        for (int i = 0, size = dt->size; i < size; i++) {
            if ((dt->table[i].next != NULL) || (dt->table[i].key != NULL) ||
                (dt->table[i].val != NULL)) {
                success = 0;
                break;
            }
        }
    } else {
        success = 0;
    }

    if (success == 1) {
        printf("newDict success\n");
    } else {
        printf("newDict fail\n");
    }
}
int main() {

    // test newDict()
    testNewDict();
}

#endif