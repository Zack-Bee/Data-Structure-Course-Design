#ifndef OBJ_H
#define OBJ_H

typedef struct obj {
    unsigned int refCount;
    int encode;
    void *ptr;
} obj;

#endif