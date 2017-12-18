#include "list.h"
#include <malloc.h>
#include <stdio.h>

list *newList(void) {
    list *li = (list *)malloc(sizeof(list));
    if (li == NULL) {
        return NULL;
    } else {
        li->length = 0;
        li->head = NULL;
        li->tail = NULL;
        return li;
    }
}

void destroy(list *li) {
    listNode *p = li->head, *q;
    while (p != NULL) {
        q = p->next;
        if (p->key != NULL) {
            destroySds(p->key);
        }
        free(p);
        p = q;
    }
    li->length = 0;
    free(li);
}

uint32_t getListLength(list *li) {
    if (li == NULL) {
        return 0;
    } else {
        return li->length;
    }
}

listNode *getListHead(list *li) {
    if (li == NULL) {
        return NULL;
    } else {
        return li->head;
    }
}

uint32_t setListNode(list *li, char *key, void *val) {
    listNode *p, *q;
    for (p = li->head; p != NULL; p = p->next) {
        if (compareStr(p->key, key)==0){
            break;
        }
    }
    if (p == NULL) {
        q = (listNode *)malloc(sizeof(listNode));
        q->prev = li->tail;
        li->length++;
        li->tail = q;
        q->next = NULL;
        setSds(q->key, key);
        q->val = val;
        return 1;
    } else {
        p->val = val;
        return 0;
    }
}

uint32_t delListNode(list *li, char *key) {
    listNode *cur, *pre;
    if (li == NULL) {
        return 0;
    } else {
        if (compareStr(li->head->key, key)==0){
            pre = li->head;
            li->head = pre->next;
            li->head->prev = NULL;
            if (pre->key != NULL) {
                destroySds(pre->key);
            }
            free(pre);
            li->length--;
            return 1;
        } else if (compareStr(li->tail->key, key)==0){
            pre = li->tail;
            li->tail = li->tail->prev;
            li->tail->next = NULL;
            if (pre->key != NULL) {
                destroySds(pre->key);
            }
            free(pre);
            li->length--;
            return 1;
        } else {
            for (cur = li->head; cur != NULL; cur = cur->next) {
                if (compareStr(cur->key, key)==0){
                    break;
                }
            }
            if (cur == NULL) {
                return 0;
            } else {
                cur->prev->next = cur->next;
                cur->next->prev = cur->prev;
                if (cur->key != NULL) {
                    destroySds(cur->key);
                }
                free(cur);
                li->length--;
                return 1;
            }
        }
    }
}

void traverseList(list *li, void (*function)(void *param)) {
    if (li != NULL) {
        listNode *p = li->head;
        while (p != NULL) {
            function(p->val);
            p = p->next;
        }
    }
}

void *getListVal(list *li, char *key) {
    if (li != NULL) {
        listNode *p;
        for (p = li->head; p != NULL; p = p->next) {
            if (compareStr(p->key, key)==0) {
                return (p->val);
            }
        }
    }
    return NULL;
}

#define LIST_TEST
#ifdef LIST_TEST
void testNewList() {
    list *li = NULL;
    li = newList();
    if ((li != NULL) || (li->length != 0) || (li->head != NULL) ||
        (li->tail != NULL)) {
        printf("newList fail\n");
    } else {
        printf("newList success\n");
    }
}
void testDestroy(list *li) {
    destroyList(li);
    if ((li == NULL) && (li->length == 0)) {
        printf("destroyList fail\n");
    } else {
        printf("destroyLidt success\n");
    }
}
void testSetListNode(list *li, char *key, void *val) {
    int success = 0;
    listNode *p;
    for (p = li->head; p != NULL; p = p->next) {
        if ((compareStr(p->key, key)==0)&& (p->val == val)) {
            printf("testSetListNode success\n");
            success = 1;
            break;
        }
    }
    if (success == 0) {
        printf("testSetListNode fail\n");
    }
}

void testDelListNode(list *li, char *key) {
    listNode *p;
    int success = 1;
    for (p = li->head; p != NULL; p = p->next) {
        if (compareStr(p->key, key)==0) {
            success = 0;
            break;
        }
    }
    if (success = 0) {
        printf("delListNode success\n");
    } else {
        printf("delListNode fail\n");
    }
}

void testGetListVal(list *li, char *key) {
    void *v = getListVal(li, key);
    int success = 0;
    listNode *p;
    for (p = li->head; p != NULL; p = p->next) {
        if ((compareStr(p->key, key)==0) && (p->val == v)) {
            success = 1;
            break;
        }
    }
    if ((v == NULL) && (p == NULL)) {
        success = 1;
    }
    if (success == 1) {
        printf("getListVal success\n");
    } else {
        printf("getListVal fail\n");
    }
}

int main() {
    testNewList();
    return 0;
}
#endif