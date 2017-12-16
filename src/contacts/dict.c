#include "dict.h"
#include <malloc.h>
#include <stdint.h>
#include <string.h>

// rehash的步数
const uint32_t REHASH_STEP = 10;


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
 *     寻找key对应的dictEntry
 *     @param dt 进行查找的dict
 *     @param key 用于查找的key
 *     @param htIndex
 *          hashtable的index,决定在hashtable[0]还是在hashtable[1]中查找
 *     @param getStr 用于获取str的函数
 *     @return de 符合key的dictEntry, 如果不存在返回空的dictEntry或NULL
 */
dictEntry *_getDictEntry(dict *dt, void *key, int htIndex,
                         char *(getStr(void *obj))) {
    dictEntry *de = NULL;
    char *str = getStr(key);
    de = &dt->hashtable[htIndex].table[_dictHashFunction(
            dt->hashtable[htIndex].size, key, getStr)];
    while (de && de->key) {
        if (strcmp(str, getStr(de->key)) == 0) {
            return de;
        }
        de = de->next;
    }

    return de;
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


/********************************PUBLIC****************************************/
dict *newDict(void) {
    dict *dt = malloc(sizeof(dict));
    dt->rehashIndex = -1;
    dt->hashtable[0].size = DICT_INIT_SIZE;
    dt->hashtable[0].used = 0;
    dt->hashtable[0].table = malloc(sizeof(dictEntry*) * DICT_INIT_SIZE);
    dt->hashtable[1].table = NULL;

    // 初始化dictEntry
    for (int i = 0; i < DICT_INIT_SIZE; i++) {
        dt->hashtable[0].table[i].val = NULL;
        dt->hashtable[0].table[i].key = NULL;
        dt->hashtable[0].table[i].next = NULL;
    }

    return dt;
}

void rehashDict(dict *dt, char *(*getStr)(void *obj)) {
    dictEntry *cur = NULL;

    // 没进行rehash的话开始进行rehash
    if (dt->rehashIndex == -1) {
        if (dt->hashtable[1].table == NULL) {
            dt->hashtable[1].table =
                    malloc(sizeof(dictEntry) * dt->hashtable[1].size * 2);
            dt->hashtable[1].size = dt->hashtable[0].size * 2;
            dt->hashtable[1].used = 0;
        }
        dt->rehashIndex = 0;
    }

    // 已经rehash完成, 将rehashIndex重置为0, 将扩大后的table存入hashtable[0]
    if ((dt->rehashIndex >= dt->hashtable[0].size) ||
        (dt->hashtable[1].used == 0)) {

        // 释放hashtable[0], 将hashtable[1]存入hashtable[0]
        free(dt->hashtable[0].table);
        dt->hashtable[0].table = dt->hashtable[1].table;
        dt->hashtable[0].used = dt->hashtable[1].used;
        dt->hashtable[0].size = dt->hashtable[1].size;
        dt->hashtable[1].table = NULL;
        return;
    }

    // 进行REHASH_STEP次数的rehash
    uint32_t maxReshIndex = dt->rehashIndex + REHASH_STEP;
    while ((dt->rehashIndex < maxReshIndex) &&
           (dt->rehashIndex < dt->hashtable[1].size) &&
           (dt->hashtable[0].used > 0)) {
        cur = &dt->hashtable[0].table[dt->rehashIndex];

        // 将hashtable[0]中的数据逐渐转移到hashtable[1]中
        while (cur) {
            if (cur->key != NULL) {
                dt->hashtable[0].used--;
                dt->hashtable[1].used++;
                setDictEntry(dt, cur->key, cur->val, getStr);
            }

            cur = cur->next;
        }
        dt->rehashIndex++;
    }
}

double getDictRadio(dict *dt) {
    return ((dt->hashtable[0].used + 0.0) / dt->hashtable[0].size);
}

int isDictRehashing(dict *dt) {
    if (dt->rehashIndex == -1) {
        return 0;
    } else {
        return 1;
    }
}

void *getDictKey(dict *dt, void *key, char *(*getStr)(void *obj)) {
    dictEntry *de = NULL;

    // 如果正在进行rehash, 则在hashtable[1]中查找
    if (dt->rehashIndex > -1) {
        de = _getDictEntry(dt, key, 1, getStr);
        if (de) {
            return de->key;
        }
    }

    // 在hashtable[0]中查找
    de = _getDictEntry(dt, key, 0, getStr);
    if (de) {
        return de->key;
    }

    return NULL;
}

void *getDictVal(dict *dt, void *key, char *(*getStr)(void *obj)) {
    dictEntry *de = NULL;

    // 如果正在进行rehash, 则在hashtable[1]中查找
    if (dt->rehashIndex > -1) {
        de = _getDictEntry(dt, key, 1, getStr);
        if (de) {
            return de->val;
        }
    }

    // 在hashtable[0]中查找
    de = _getDictEntry(dt, key, 0, getStr);
    if (de) {
        return de->val;
    }

    return NULL;
}

void setDictEntry(dict *dt, void *key, void *val,
                  char *(*getStr)(void *obj)) {
    dictEntry *de = NULL;                
    uint32_t hash;
    if (dt->rehashIndex > -1) {
        hash = _dicthashFunction(dt->hashtable[0].size, key, getStr);
        if (hash >= dt->rehashIndex) {

        }
    }
}

#define DICT_TEST
#ifdef DICT_TEST

void testNewDict() {
    dict *dt = NULL;
    dt = newDict();
    int success = 1;
    if ((dt != NULL) && (dt->rehashIndex == -1) &&
        (dt->hashtable[0].size == DICT_INIT_SIZE) &&
        (dt->hashtable[0].used == 0) && (dt->hashtable[0].table != NULL) &&
        (dt->hashtable[1].table == NULL)) {
        for (int i = 0, size = dt->hashtable[0].size; i < size; i++) {
            if ((dt->hashtable[0].table[i].next != NULL) ||
                (dt->hashtable[0].table[i].key != NULL) ||
                (dt->hashtable[0].table[i].val != NULL)) {
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