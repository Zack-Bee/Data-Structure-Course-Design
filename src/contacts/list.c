#include "list.h"
#include "sds.h"
#include <malloc.h>
#include <stdio.h>

list *newList(void) {
    list *li = (list *)malloc(sizeof(list));
    if (li == NULL) {
        return NULL;
    } else {
        li->length = 0;
        li->head = NULL;
        li->tail = li->head;
        li->destroy = destroyList;
        return li;
    }
}

void destroyList(list **li) {
    if (*li == NULL) {
        return;
    }
    listNode *p = (*li)->head, *q;
    while (p != NULL) {
        q = p->next;
        if (p->key != NULL) {
            destroySds(&(p->key));
        }
        free(p);
        p = q;
    }
    free(*li);
    (*li) = NULL;
}

uint32_t getListLength(list *li) {
    if (li != NULL) {
        return li->length;
    } else {
        return 0;
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
    listNode *p = NULL, *q = NULL;
    for (p = li->head; p != NULL; p = p->next) {
        if (sdsCompareStr(p->key, key) == 0) {
            break;
        }
    }
    if (p == NULL) {
        q = (listNode *)malloc(sizeof(listNode));
        if (li->tail != NULL) {
            li->tail->next = q;
            q->prev = li->tail;
            q->next = NULL;
        } else {
            li->head = q;
            li->tail = q;
            q->prev = NULL;
            q->next = NULL;
        }
        q->key = newCopySds(key);
        q->val = val;
        li->length++;
        li->tail = q;

        return 1;
    } else {
        free(p->val);
        p->val = val;
        return 0;
    }
}

uint32_t delListNode(list *li, char *key) {
    listNode *cur, *pre;
    if (li == NULL || li->head == NULL || li->tail == NULL) {
        return 0;
    } else {
        if (sdsCompareStr(li->head->key, key) == 0) {
            pre = li->head;
            li->head = pre->next;
            if (li->head != NULL) {
                li->head->prev = NULL;
            } else {
                li->tail = NULL;
            }
            if (pre->key != NULL) {
                destroySds(&(pre->key));
            }
            free(pre);
            li->length--;

            return 1;
        } else if (sdsCompareStr(li->tail->key, key) == 0) {
            pre = li->tail;
            li->tail = li->tail->prev;
            li->tail->next = NULL;
            if (pre->key != NULL) {
                destroySds(&(pre->key));
            }
            free(pre);
            li->length--;

            return 1;
        } else {
            for (cur = li->head; cur != NULL; cur = cur->next) {
                // printf("%s", cur->key->str);
                if (sdsCompareStr(cur->key, key) == 0) {
                    break;
                }
            }

            if (cur == NULL) {
                return 0;
            } else {

                cur->prev->next = cur->next;
                cur->next->prev = cur->prev;
                if (cur->key != NULL) {
                    destroySds(&(cur->key));
                }
                free(cur);
                li->length--;

                return 1;
            }
        }
    }
}

void traverseList(list *li, void (*function)(void **param)) {
    if (li != NULL) {
        listNode *p = li->head;
        while (p != NULL) {
            function(&(p->val));
            p = p->next;
        }
    }
}

void *getListVal(list *li, char *key) {
    if (li != NULL) {
        listNode *p;
        for (p = li->head; p != NULL; p = p->next) {
            if (sdsCompareStr(p->key, key) == 0) {
                return (p->val);
            }
        }
    }
    return NULL;
}

// #define LIST_TEST
#ifdef LIST_TEST
void testNewList() {
    list *li = NULL;
    li = newList();
    if ((li == NULL) || (li->length != 0) || (li->head != NULL) ||
        (li->tail != NULL)) {
        printf("newList fail\n");
    } else {
        printf("newList success\n");
    }
}
void testDestroyList() {
    list *li = newList();
    destroyList(&li);
    if (li == NULL) {
        printf("destroyList success\n");
    } else {
        printf("destroyList fail\n");
    }
}

void testSetListNode() {
    list *li = newList();
    sds *s = newCopySds("hhhh");
    setListNode(li, "tuhao", s);
    setListNode(li, "lele", s);
    if ((sdsCompareStr(li->head->key, "tuhao") == 0) &&
        (sdsCompareStr(li->head->next->key, "lele") == 0) &&
        (li->length == 2) && (li->head->val) && (li->head->next->val)) {
        printf("setListNode success\n");
    } else {
        printf("setListNode fail\n");
    }
}

void testDelListNode() {
    list *li = newList();
    uint32_t a1, a2, a3, a4;
    sds *s = newCopySds("hhhh");
    if (setListNode(li, "del", s) == 1) {
        a1 = delListNode(li, "del");
        a2 = delListNode(li, "del");
    }
    if (setListNode(li, "del", s) == 1) {
        a1 = delListNode(li, "del");
        a2 = delListNode(li, "del");
    }
    if ((setListNode(li, "s1", s) == 1) && (setListNode(li, "s2", s) == 1) &&
        (setListNode(li, "s3", s) == 1) && (setListNode(li, "s4", s) == 1)) {
        a3 = delListNode(li, "s2");
        a4 = delListNode(li, "s4");
    }

    if ((a1 == 1) && (a2 == 0) && (a3 == 1) && (a4 == 1)) {
        printf("delListNode success\n");
    } else {
        printf("delListNode fail\n");
    }
}


void testGetListVal() {
    list *li = newList();
    int success = 0, c;
    if (setListNode(li, "get", &c)) {
        if ((getListVal(li, "get") == &c) && (getListVal(li, "set") == NULL)) {
            success = 1;
        }
    }
    if (success = 1) {
        printf("getListVal success\n");
    } else {
        printf("getListVal fail\n");
    }
}

int main() {
    testNewList();
    testDestroyList();
    testSetListNode();
    testDelListNode();
    testGetListVal();
}

#endif
