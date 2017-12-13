#include "dict.h"
#include <malloc.h>

#define true 1
#define false 0

int isDictRehashing(dict *dt) {
    if (dt->rehashIndex == -1) {
        return true;
    } else {
        return false;
    }
}

dict *newDict(void) {
    dict *dt = malloc(sizeof(dict));
    dt->rehashIndex = -1;
    dt->hashtable[0].size = DICT_INIT_SIZE;
    dt->hashtable[1].size = DICT_INIT_SIZE;
    dt->hashtable[0].sizemask = DICT_INIT_SIZE - 1;
    dt->hashtable[1].sizemask = DICT_INIT_SIZE - 1;
    dt->hashtable[0].used = 0;
    dt->hashtable[1].used = 0;
    dt->hashtable[0].table = malloc(sizeof(dictEntry) * DICT_INIT_SIZE);
    dt->hashtable[1].table = malloc(sizeof(dictEntry) * DICT_INIT_SIZE);

    // 初始化dictEntry
    for(int i = 0; i < DICT_INIT_SIZE; i++) {
        dt->hashtable[0].table[i].val = NULL;
        dt->hashtable[1].table[i].val = NULL;
        dt->hashtable[0].table[i].key = NULL;
        dt->hashtable[1].table[i].key = NULL;
    }

    return dt;
}

#define DICT_TEST
#ifdef DICT_TEST

int main() {
    dict *dt = NULL;
    dt = newDict();

    // test newDict
    if ((dt->hashtable[0].size == DICT_INIT_SIZE) &&
        (dt->hashtable[1].size == DICT_INIT_SIZE) &&
        (dt->hashtable[0].sizemask == DICT_INIT_SIZE - 1) &&
        (dt->hashtable[1].sizemask == DICT_INIT_SIZE - 1)) {
        printf("newDict success\n");
    } else {
        printf("newDict fail\n");
    }
}

#endif