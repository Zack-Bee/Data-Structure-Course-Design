#include "sds.h"
#include <malloc.h>
#include <stdio.h>

sds *newSds(void) {
    sds *s = (sds *)malloc(sizeof(sds));
    s->length = SDS_INIT_SIZE;
    s->used = 0;
    s->str = (char *)malloc(SDS_INIT_SIZE * sizeof(char));
    return s;
}

sds *newCopySds(char *str) {
    sds *s = (sds *)malloc(sizeof(sds));
    s->length = SDS_INIT_SIZE;
    s->used = 0;
    s->str = (char *)malloc(SDS_INIT_SIZE * sizeof(char));
    char *c, *ch;
    uint32_t i = 1;
    for (c = str, ch = s->str; *c != '\0'; c++, ch++) {
        if (i >= s->length) {
            s->str = (char *)realloc(s->str, s->length * 2 * sizeof(char));
            s->length *= 2;
        }
        *ch = *c;
        s->length++;
        s->used++;
        i++;
    }
    ch = NULL;
    return s;
}

void setSds(sds *s, char *str) {
    s->str = str;
    s->used = 0;
    char *c;
    for (c = str; *c != '\0'; c++) {
        s->used++;
    }
    s->length = sizeof(str);
}
#include <string.h>
#include <stdint.h>

#ifndef SDS_H
#define SDS_H

#define SDS_INIT_SIZE 16

typedef struct sds {

    /** 空间的大小 */
    uint32_t length;  

    /** 使用的空间大小 */  
    uint32_t used;  

    /** 存储的字符串 */  
    char *str;

    /** 销毁sds的函数的指针 */
    void (*destroy)(struct sds *s);
} sds;

/**
 *     创建一个sds，大小为SDS_INIT_SIZE
 *     @return 返回创建的sds
 */
sds *newSds(void);

/**
 *     从字符串创建sds，如果空间不够，将length*2，直到能够容纳str
 *     @param str 被复制的原生字符串
 *     @return 返回创建的sds
 */
sds *newCopySds(char *str);

/**
 *     设置sds的字符串为str
 *     @param s 进行操作的sds
 *     @param str 设置的str
*/
void setSds(sds *s, char *str);

/**
 *     销毁sds，释放空间
 *     @param s 进行操作的sds
 */
void destroySds(sds **s);

/**
 *     释放sds中str的空间，将sds初始化
 *     @param s 进行操作的sds
 */
void clearSds(sds *s);

/**
 *     将str与sds中的字符串进行比较
 *     @param s 进行操作的sds
 *     @param str 进行比较的str
 *     @return 如果相等, 返回0, sds大于str, 返回1, 否则返回-1
*/
int compareStr(sds *s, char *str);

/**
 *     将s1与s2中的字符串进行比较
 *     @param s1 进行操作的sds
 *     @param s2 进行比较的str
 *     @return 如果相等, 返回0, sds大于str, 返回1, 否则返回-1
*/
int compareSds(sds *s1, sds *s2);

/**
 *     得到sds中保存的字符串 
 *     @param s 进行操作的sds
 *     @return 返回保存的字符串
 */ 
char *getSdsStr(sds *s);

#endif    // SDS_H
void destroySds(sds **s) {
    free((*s)->str);
    (*s)->length = 0;
    (*s)->used = 0;
    free(*s);
    *s = NULL;
}

void clearSds(sds *s) {
    free(s->str);
    s->str = (char *)malloc(SDS_INIT_SIZE * sizeof(char));
    s->length = SDS_INIT_SIZE;
    s->used = 0;
}

int compareStr(sds *s, char *str) {
    char *c1 = s->str, *c2 = str;
    uint32_t i;
    for (i = 1; i <= s->length && *c2 != '\0'; ++i, ++c1, ++c2) {
        if (*c1 != *c2) {
            return (*c1 - *c2);
        }
    }
    while (*c2 != '\0') {
        i++;
    }
    return (s->length - i);
}

int compareSds(sds *s1, sds *s2) {
    char *c1 = s1->str, *c2 = s2->str;
    for (uint32_t i = 1; i <= s1->length && i <= s2->length; ++i, ++c1, ++c2) {
        if (*c1 != *c2) {
            return (*c1 - *c2);
        }
    }
    return (s1->length - s2->length);
}

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
    char *str;
    char *c, *ch;
    sds *s = newCopySds(str);
    int i = 0, n = SDS_INIT_SIZE, success = 0;
    for (c = str; *c != '\0'; c++) {
        if (i >= s->length) {
            n *= 2;
        }
        i++;
    }
    if ((s != NULL) && (s->length == n) && (s->used = i)) {
        success = 1;
        c = str, ch = s->str;
        while (i--) {
            if (*ch != *c) {
                success = 0;
                break;
            }
            ch++;
            c++;
        }
        if (success == 1) {
            printf("newCopySds success\n");
        } else {
            printf("newCopySds fail\n");
        }
    }
}

void testSetSds() {
    sds *s = newSds();
    char *str;
    char *n;
    int i;
    for (n = str; *n != '\0'; n++) {
        i++;
    }
    if ((s->used == i) && (s->str == str) && (s->length = sizeof(str))) {
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
    setSds(s1, "aaa");
    setSds(s2, "bbb");
    int n = compareSds(s1, s2), success = 0;
    char *c1 = s1->str, *c2 = s2->str;
    for (uint32_t i = 1; i <= s1->length && i <= s2->length; ++i, ++c1, ++c2) {
        if ((*c1 != *c2) && (n == *c1 - *c2)) {
            success = 1;
        }
    }
    if ((success == 0) && (n == s1->length - s2->length)) {
        success = 1;
    }
    if (success == 1) {
        printf("compareSds success\n");
    } else {
        printf("compareSds fail\n");
    }
}

void testCompareStr() {
    sds *s = newSds();
    sds *str = newSds();
    setSds(s, "ddd");
    setSds(str, "ddd");

    int n = compareStr(s, str), success = 0;
    char *c1 = s->str, *c2 = str;
    uint32_t i;
    for (i = 1; i <= s->length && *c2 != '\0'; ++i, ++c1, ++c2) {
        if ((*c1 != *c2) && (n == *c1 - *c2)) {
            success = 1;
        }
    }
    while (*c2 != '\0') {
        i++;
        c2++;
    }
    if ((success == 0) && (n == s->length - i)) {
        success = 1;
    }
    if (success == 1) {
        printf("compareSdr success\n");
    } else {
        printf("compareSdr fail\n");
    }
}

void testClearSds() {
    sds *s = newSds();
    clearSds(s);
    if ((s->length == SDS_INIT_SIZE) && (s->length == 0) && (s->str != NULL)) {
        printf("clearSds success\n");
    } else {
        printf("clearSds fail\n");
    }
}

int main() {
    testNewSds();
    testDestroySds();
    testClearSds();
    testCompareSds();
    testCompareStr();
    testNewCopySds();
    testSetSds();
    return 0;
}
#endif