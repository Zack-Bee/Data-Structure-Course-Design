#include"list.h"
#include<stdio.h>
#include<malloc.h>

list *newList(void){
    list *li=(list*)malloc(sizeof(list));
    if(li==NULL){
        return NULL;
    }else{
        li->length=0;
        li->head=NULL;
        li->tail=NULL;
        return li;
    }
}

void destroy(list *li){
    listNode *p=li->head,*q;
    while(p!=NULL){
        q=p->next;
        if(p->key!=NULL){
        destroySds(p->key);
        }
        free(p);
        p=q;
    }
    li->length=0;
    free(li);
}

uint32_t getListLength(list *li){
    if(li== NULL){
        return 0;
    }else{
        return li->length;
    }
}

listNode *getListHead(list *li){
    if(li==NULL){
        return NULL;
    }else{
        return li->head;
    }
}

uint32_t setListNode(list *li, char *key, void *val){
    listNode *p,*q;
    for(p=li->head;p!=NULL;p=p->next){
        if(p->key==key) break;
    }
    if(p==NULL){
        q=(listNode *)malloc(sizeof(listNode));
        q->prev=li->tail;
        li->length++;
        li->tail=q;
        q->next=NULL;
        q->key=key;
        q->val=val;
        return 1;
    }else{
        p->val=val;
        return 0;
    }
}

uint32_t delListNode(list *li, char *key){
    listNode *cur,*pre;
    if(li==NULL){
        return 0;
    }else{
        if(li->head->key==key){
            pre=li->head;
            li->head=pre->next;
            li->head->prev=NULL;
            if(pre->key!=NULL){
            destroySds(pre->key);
            }
            free(pre);
            li->length--;
            return 1;
        }else if(li->tail->key==key){
            pre=li->tail;
            li->tail=li->tail->prev;
            li->tail->next=NULL;
            if(pre->key!=NULL){
            destroySds(pre->key);
            }
            free(pre);
            li->length--;
            return 1;
        }else {
            for(cur=li->head;cur!=NULL;cur=cur->next){
                if(cur->key==key){
                    break;
                }
            }
            if(cur==NULL) {
                return 0;
            }else {
                cur->prev->next=cur->next;
                cur->next->prev=cur->prev;
                if(cur->key!=NULL){
                    destroySds(cur->key);
                }
                free(cur);
                li->length--;
                return 1;
            }
        }
    }
}

void traverseList(list *li, void (*function)(void *param)){
    if(li!=NULL){
        listNode *p=li->head;
        while(p!=NULL){
            function(p->val);
            p=p->next;
        }
    }
}
void function(void *n){

}
