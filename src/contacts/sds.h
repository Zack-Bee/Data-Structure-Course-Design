#include <string.h>
#include <stdint.h>

#ifndef SDS_H
#define SDS_H

#define SDS_INIT_SIZE 32

typedef struct sds {
    uint64_t length;    // 空间的大小
    uint64_t used;    // 使用的空间大小
    char *str;
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
 *     得到sds已经使用的长度
 *     @param s 进行操作的sds
 *     @return 长度
*/
uint64_t getSdsLength(sds *s);

/**
 *     销毁sds，释放空间
 *     @param s 进行操作的sds
*/
void destroySds(sds *s);

/**
 *     释放sds中str的空间，将sds初始化
 *     @param s 进行操作的sds
*/
void clearSds(sds *s);
#endif