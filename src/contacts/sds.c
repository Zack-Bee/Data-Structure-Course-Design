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

int sdsCompareStr(sds *s, char *str) { return strcmp(s->str, str); }

int sdsCompareSds(sds *s1, sds *s2) { return (strcmp(s1->str, s2->str)); }

char *getSdsStr(sds *s) { return s->str; }

void sdsCatStr(sds *s, char *str) {
    if(s->used+strlen(str)>=s->length){
        s->str=(char*)realloc(s->str,2*s->length*sizeof(char));
        s->length*=2;
    }
    strcat(s->str,str);
  }

void sdsCatSds(sds *s1, sds *s2)  { 
    if(s1->used+s2->used>=s1->length){
        s1->str=(char*)realloc(s1->str,2*s1->length*sizeof(char));
        s1->length*=2;
    }
    strcat(s1->str,s2->str);
 }

void sdsReduceStr(sds *s, char *str) {
    uint32_t len = strlen(str);
    if (len < s->used) {
        if (strcmp(&(s->str[s->used - len]), str) == 0) {
            s->str[s->used - len] = '\0';
            s->used -= len;
        }
    }
}


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

void testSdsCompareSds() {
    sds *s1 = newSds();
    sds *s2 = newSds();
    int success = 1;
    setSds(s1, "aaa");
    setSds(s2, "bbb");
    if (sdsCompareSds(s1, s2) == 0) {
        success = 0;
    }
    setSds(s2, "aaa");
    if (sdsCompareSds(s1, s2) != 0) {
        success = 0;
    }
    if (success == 1) {
        printf("sdsCompareSds success\n");
    } else {
        printf("sdsCompareSds fail\n");
    }
}

void testSdsCompareStr() {
    sds *s = newCopySds("wwww");
    if ((sdsCompareStr(s, "wwww") != 0) || (sdsCompareStr(s, "hhhh") == 0)) {
        printf("compare fail\n");
    } else {
        printf("compare success\n");        
    }
}

void testSdsCatStr() {
    sds *s = newCopySds("wwww");
    sdsCatStr(s, "dddd");
    if (sdsCompareStr(s, "wwwwdddd") == 0) {
        printf("sdsCatStr success\n");
    } else {
        printf("sdsCatStr fail\n");        
    }
}

void testSdsCatSds() {
    sds *s1 = newCopySds("wwww");
    sds *s2 = newCopySds("dddd");
    sdsCatSds(s1, s2);
    if (sdsCompareStr(s1, "wwwwdddd") == 0) {
        printf("sdsCatSds success\n");
    } else {
        printf("sdsCatSds fail\n");        
    }
}

void testSdsReduceStr() {
    sds *s1 = newCopySds("hhhhhh");
    sdsReduceStr(s1, "hhh");
    sds *s2 = newCopySds("h");
    sdsReduceStr(s2, "hhh");
    if ((sdsCompareStr(s1, "hhh") != 0) || (sdsCompareStr(s2, "h") != 0)) {
        printf("sdsReduceStr fail\n");
    } else {
        printf("sdsReduceStr success\n");   
    }
}

int main() {
    testNewSds();
    testDestroySds();
    testSdsCompareSds();
    testSdsCompareStr();
    testNewCopySds();
    testSetSds();
    testSdsCatStr();
    testSdsCatSds();
    testSdsReduceStr();

    return 0;
}
#endif
