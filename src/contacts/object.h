#include "dict.h"
#include "sds.h"
#include "linkedlist.h"

#ifndef OBJ_H
#define OBJ_H

#define CONTACTS_DICT 0
#define CONTACTS_SDS 1
#define CONTACTS_LINKEDLIST 2

typedef struct obj {
    unsigned int refCount;
    int encode;
    void *ptr;
    void (*destroyObj)(void *ptr);
} obj;

#endif