#include "dict.h"
#include "sds.h"
#include "list.h"

#ifndef OBJ_H
#define OBJ_H

#define CONTACTS_DICT 0
#define CONTACTS_SDS 1
#define CONTACTS_LIST 2

typedef struct obj {
    uint32_t refCount;
    unsigned int encode;
    void *ptr;
    void (*destroyObj)(void *ptr);
} obj;

#endif