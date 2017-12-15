#include "dict.h"
#include <malloc.h>
#include <stdint.h>

#define true 1
#define false 0

// rehash的步数
const int REHASH_STEP = 10;

// BKDR Hash Function
unsigned int _BKDRHash(char *str) {
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;

    while (*str) {
        hash = hash * seed + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

unsigned int _dictHashFunction(unsigned int size, void *obj,
                               char *(getStr(void *obj))) {
    return (_BKDRHash(getStr(obj)) % size);
}

dict *newDict(void) {
    dict *dt = malloc(sizeof(dict));
    dt->rehashIndex = -1;
    dt->hashtable[0].size = DICT_INIT_SIZE;
    dt->hashtable[0].sizemask = DICT_INIT_SIZE - 1;
    dt->hashtable[0].used = 0;
    dt->hashtable[0].table = malloc(sizeof(dictEntry) * DICT_INIT_SIZE);
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
            dt->hashtable->size = dt->hashtable[0].size * 2;
            dt->hashtable->sizemask = dt->hashtable[1].size - 1;
            dt->hashtable->used = 0;
        }
        dt->rehashIndex = 0;
    }

    unsigned long maxReshIndex = dt->rehashIndex + REHASH_STEP;
    while (dt->rehashIndex < maxReshIndex) {
        cur = &dt->hashtable[0].table[dt->rehashIndex];
        while (cur) {
            if (cur->key != NULL) {
                
            }
            
            cur = cur->next;
        }
        dt->rehashIndex++;
    }
}

#define DICT_TEST
#ifdef DICT_TEST

void testNewDict(dict *dt) {
    int success = true;
    if ((dt != NULL) && (dt->rehashIndex == -1) &&
        (dt->hashtable[0].size == DICT_INIT_SIZE) &&
        (dt->hashtable[0].sizemask == DICT_INIT_SIZE - 1) &&
        (dt->hashtable[0].used == 0) && (dt->hashtable[0].table != NULL) &&
        (dt->hashtable[1].table == NULL)) {
        for (int i = 0, size = dt->hashtable[0].size; i < size; i++) {
            if ((dt->hashtable[0].table[i].next != NULL) ||
                (dt->hashtable[0].table[i].key != NULL) ||
                (dt->hashtable[0].table[i].val != NULL)) {
                success = false;
                break;
            }
        }
    } else {
        success = false;
    }

    if (success == true) {
        printf("newDict success\n");
    } else {
        printf("newDict fail\n");
    }
}
int main() {
    dict *dt = NULL;
    dt = newDict();

    // test newDict()
    testNewDict(dt);
}

#endif