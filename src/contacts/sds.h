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