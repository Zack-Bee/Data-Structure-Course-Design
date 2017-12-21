#include "sds.h"
#include <malloc.h>
#include <stdio.h>

sds *newSds(void) {
    sds *s = (sds *)malloc(sizeof(sds));
    s->length = SDS_INIT_SIZE;
    s->used = 0;
    s->str = (char *)malloc(SDS_INIT_SIZE * sizeof(char));
    s->str[0] = '\0';
    return s;
}

sds *newCopySds(char *str) {
    sds *s = (sds *)malloc(sizeof(sds));
    s->length = SDS_INIT_SIZE;
    s->used = 0;
    s->str = (char *)malloc(SDS_INIT_SIZE * sizeof(char));
    setSds(s, str);

    return s;
}

void setSds(sds *s, char *str) {
    uint32_t size = s->length;
    size_t length = strlen(str);
    while (length > (s->length - 1)) {
        s->length *= 2;
    }
    if (size != s->length) {
        free(s->str);
    }
    s->str = malloc(sizeof(char) * s->length);
    s->str[0] = '\0';
    strcpy(s->str, str);
    s->used = length;
}

void destroySds(sds **s) {
    if (*s == NULL) {
        return;
    }
    free((*s)->str);
    free(*s);
    *s = NULL;
}

int compareStr(sds *s, char *str) { return strcmp(s->str, str); }

int compareSds(sds *s1, sds *s2) { return (strcmp(s1->str, s2->str)); }

char *getSdsStr(sds *s) { return s->str; }

#define SDS_TEST
#ifdef SDS_TEST

void testNewSds() {
    sds *s = NULL;
    s = newSds();
    if ((s == NULL) || (s->length != SDS_INIT_SIZE) || (s->used != 0) ||
        (s->str = NULL)) {
        printf("newSds fail\n");
    } else {
        printf("newSds success\n");
    }
}

void testNewCopySds() {
    sds *s = newCopySds("hhhhh");
    if (strcmp(s->str, "hhhhh") != 0) {
        printf("newCopySds fail\n");
    } else {
        printf("newCopySds success\n");
    }
}

void testSetSds() {
    sds *s = newSds();
    setSds(s, "hhhhh");
    if ((s->used == 5) && (strcmp(s->str, "hhhhh") == 0)) {
        printf("setSds success\n");
    } else {
        printf("setSds fail\n");
    }
}

void testDestroySds() {
    sds *s = newSds();
    destroySds(&s);
    if (s == NULL) {
        printf("testDestroy success\n");
    } else {
        printf("testDestroy fail\n");
    }
}

void testCompareSds() {
    sds *s1 = newSds();
    sds *s2 = newSds();
    int success = 1;
    setSds(s1, "aaa");
    setSds(s2, "bbb");
    if (compareSds(s1, s2) == 0) {
        success = 0;
    }
    setSds(s2, "aaa");
    if (compareSds(s1, s2) != 0) {
        success = 0;
    }
    if (success == 1) {
        printf("compareSds success\n");
    } else {
        printf("compareSds fail\n");
    }
}

void testCompareStr() {
    sds *s = newCopySds("wwww");
    if ((compareStr(s, "wwww") != 0) || (compareStr(s, "hhhh") == 0)) {
        printf("compare fail\n");
    } else {
        printf("compare success\n");        
    }
}

int main() {
    testNewSds();
    testDestroySds();
    testCompareSds();
    testCompareStr();
    testNewCopySds();
    testSetSds();
    return 0;
}
#endif