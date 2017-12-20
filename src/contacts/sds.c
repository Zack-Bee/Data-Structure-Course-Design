#include"sds.h"
#include <stdio.h>
#include<malloc.h>

sds *newSds(void){
    sds *s=(sds *)malloc(sizeof(sds));
    s->length=SDS_INIT_SIZE;
    s->used=0;
    s->str=(char *)malloc(SDS_INIT_SIZE*sizeof(char));
    return s;
}

sds *newCopySds(char *str){
    sds *s=(sds *)malloc(sizeof(sds));
    s->length=SDS_INIT_SIZE;
    s->used=0;
    s->str=(char *)malloc(SDS_INIT_SIZE*sizeof(char));
    char *c,*ch;
    uint32_t i=1;
    for(c=str,ch=s->str;*c!='\0';c++,ch++){
        if(i>=s->length){
            s->str=(char *)realloc(s->str,s->length*2*sizeof(char));
            s->length*=2;
        }
        *ch=*c;
        s->length++;
        s->used++;
        i++;
    }
    ch=NULL;
    return s;
}

void setSds(sds *s, char *str){
    s->str=str;
    s->used=0;
    char *c;
    for(c=str;*c!='\0';c++){
        s->used++;
    }
    s->length=sizeof(str);
}

void destroySds(sds *s){
    free(s->str);
    s->length=0;
    s->used=0;
    free(s);
}

void clearSds(sds *s){
    free(s->str);
    s->str=(char *)malloc(SDS_INIT_SIZE*sizeof(char));
    s->length=SDS_INIT_SIZE;
    s->used=0;
}

int compareStr(sds *s, char *str){
    char *c1=s->str,*c2=str;
    uint32_t i;
    for(i=1;i<=s->length&&*c2!='\0';++i,++c1,++c2){
        if(*c1!=*c2){
            return(*c1-*c2);
        }
    }
    while(*c2!='\0'){
            i++;
    }
    return(s->length-i);
}

int compareSds(sds *s1, sds *s2){
    char *c1=s1->str,*c2=s2->str;
    for(uint32_t i=1;i<=s1->length&&i<=s2->length;++i,++c1,++c2){
        if(*c1!=*c2) {
            return (*c1-*c2);
        }
    }
    return (s1->length-s2->length);
}

char *getSdsStr(sds *s){
    return s->str;
}

#define SDS_TEST
#ifdef SDS_TEST

void testNewSds(){
    sds *s = NULL;
    s = newSds();
    if((s==NULL)||(s->length!=SDS_INIT_SIZE)||(s->used!=0)||(s->str=NULL)){
        printf("newSds fail\n");
    }else{
        printf("newSds success\n");
    }
}

void testNewCopySds(char *str){
    char *c,*ch;
    sds *s=newCopySds(str);
    int i=0,n=SDS_INIT_SIZE,success=0;
     for(c=str;*c!='\0';c++){
        if(i>=s->length){
            n*=2;
        }
        i++;
    }
    if((s!=NULL)&&(s->length==n)&&(s->used=i)){
        success=1;
        c=str,ch=s->str;
        while(i--){
            if(*ch!=*c){
                success =0;
                break;
            }
            ch++;
            c++;
        }
        if(success==1){
            printf("newCopySds success\n");
        }else{
            printf("newCopySds fail\n");
        }
}
}

void testSetSds(sds *s,char *str){
    char *n;
    int i;
    for(n=str;*n!='\0';n++){
        i++;
    }
    if((s->used==i)&&(s->str==str)&&(s->length=sizeof(str))){
        printf("setSds success\n");
    }else{
        printf("setSds fail\n");
    }
}

void testDestroySds(){
    sds *s = newSds();
    destroySds(s);
    if((s->length==0)&&(s->used==0)&&(s==NULL)&&(s->str==NULL)){
        printf("testDestroy success\n");
    }else {
        printf("testDestroy fail\n");
    }
}

void testCompareSds(sds *s1,sds *s2){
    int n=compareSds(s1,s2),success=0;
    char *c1=s1->str,*c2=s2->str;
    for(uint32_t i=1;i<=s1->length&&i<=s2->length;++i,++c1,++c2){
        if((*c1!=*c2)&&(n==*c1-*c2)){
            success =1;
        }
    }
    if((success==0)&&(n==s1->length-s2->length)){
        success=1;
    }
    if(success==1){
        printf("compareSds success\n");
    }else{
        printf("compareSds fail\n");
    }
}

void testCompareStr(){
    sds *s = newSds();
    sds *str = newSds();
    setSds(s, "ddd");
    setSds(str, "ddd");

    int n=compareStr(s,str),success=0;
    char *c1=s->str,*c2=str;
    uint32_t i;
    for(i=1;i<=s->length&&*c2!='\0';++i,++c1,++c2){
        if((*c1!=*c2)&&(n==*c1-*c2)){
            success =1;
        }
    }
    while(*c2!='\0'){
        i++;
        c2++;
    }
    if((success==0)&&(n==s->length-i)){
        success=1;
    }
    if(success==1){
        printf("compareSds success\n");
    }else{
        printf("compareSds fail\n");
    }
}

void testClearSds(sds *s){
    clearSds(s);
    if((s->length==SDS_INIT_SIZE)&&(s->length==0)&&(s->str!=NULL)){
        printf("clearSds success\n");
    }else {
        printf("clearSds fail\n");
    }
}

int main(){
    testNewSds();
    testDestroySds();
    return 0;
}
#endif