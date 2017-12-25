#include "dict.h"
#include <malloc.h>
#include <stdint.h>


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

uint32_t _dictHashFunction(char *str, uint32_t size) {
    return (_BKDRHash(str) % size);
}


/********************************PUBLIC****************************************/
dict *newDict(void) {
    dict *dt = malloc(sizeof(dict));
    dt->size = DICT_INIT_SIZE;
    dt->used = 0;
    dt->table = malloc(sizeof(list *) * DICT_INIT_SIZE);
    dt->destroy = destroyDict;

    // 初始化dictEntry
    for (int i = 0; i < DICT_INIT_SIZE; i++) {
        dt->table[i] = newList();
    }

    return dt;
}

void expandDict(dict *dt) {
    list **newTable = malloc(sizeof(list *) * dt->size * 2);
    list *cur = NULL;
    uint32_t hash;

    for (uint32_t i = 0; i < dt->size; i++) {
        if (getListLength(dt->table[i]) > 0) {
            char *str = getSdsStr(getListHead(dt->table[i])->key);
            hash = _dictHashFunction(str, dt->size * 2);
            newTable[hash] = dt->table[i];
        }
    }
    dt->size *= 2;
    free(dt->table);
    dt->table = newTable;
}

double getDictRadio(dict *dt) { return ((dt->used + 0.0) / dt->size); }

void setDictEntry(dict *dt, char *key, void *val) {
    printf("set key %s\n", key);
    uint32_t hash = _dictHashFunction(key, dt->size);
    printf("hash is :%u\n", hash);
    dt->used += setListNode(dt->table[hash], key, val);
}

void delDictEntry(dict *dt, char *key) {
    uint32_t hash = _dictHashFunction(key, dt->size);
    dt->used -= delListNode(dt->table[hash], key);
}

void *getDictVal(dict *dt, char *key) {
    printf("get key %s\n", key);    
    uint32_t hash = _dictHashFunction(key, dt->size);
    printf("hash is :%u\n", hash);
    return getListVal(dt->table[hash], key);
}

void destroyDict(dict **dt) {
    for (uint32_t i = 0, size = (*dt)->size; i < size; i++) {
        printf("%u\n", size);
        printf("begin traverse list\n");
        traverseList((*dt)->table[i], destroySds);
        printf("begin destroy list\n");
        destroyList(&(*dt)->table[i]);
    }
    free((*dt)->table);
    free(*dt);
    *dt = NULL;
    printf("destroyDict done\n");
}

void traverseDict(dict *dt, void (*function)(void **param)) {
    for (uint32_t i = 0; i < dt->size; i++) {
        traverseList(dt->table[i], function);
    }
}

// #define DICT_TEST
#ifdef DICT_TEST


void testNewDict() {
    dict *dt = NULL;
    dt = newDict();
    int success = 1;
    if ((dt != NULL) && (dt->size == DICT_INIT_SIZE) && (dt->used == 0) &&
        (dt->table != NULL)) {
        for (int i = 0; i < dt->size; i++) {
            if (getListLength(dt->table[i]) != 0) {
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