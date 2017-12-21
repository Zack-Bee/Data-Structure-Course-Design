/**
 *     此模块参考了"喜欢兰花山丘"的实现, blog:
 *     http://www.cnblogs.com/life2refuel/p/5277111.html
 */

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#include "sds.h"

#define SERV "127.0.0.1"
#define SERVER_PORT 3006
#define PATH_PREFIX "./src"
#define QUEUE_SIZE 64

// 缓冲区大小
#define BUFF_SIZE 1024

// listen监听队列的大小
#define QUEUE_SIZE 64

/**
 *     创建一个server fileDesc
 *     @param return 返回创建的server fileDesc
 */
int newServer();

/**
 *     在客户端链接过来, 多线程处理的函数
 *     @param clientFileDesc 客户端文件描述符
 *     @return 返回处理结果,这里默认返回 NULL
 */
void *responseClient(int *clientFileDesc);

/**
 *     将文件 发送给客户端
 *     @param clientFileDesc 客户端文件描述符
 *     @param path 发送的文件路径
 */
void responseFile(int clientFileDesc, const char *path);

/**
 *     处理客户端的http请求
 *     @param clientFileDesc 客户端文件描述符
 *     @param path 请求的文件路径
 *     @param query 请求发送的过来的数据
 */
void responseMessage(int clientFileDesc);

/**
 *     返回404, 请求文件没有找到
 *     @param clientFileDesc 客户端文件描述符
 */
void response_404(int clientFileDesc);

/**
 *     // 返回200 请求成功
 *     @param clientFileDesc 客户端文件描述符
 */
void response_200(int clientFileDesc, const char *type);
#define isSpace(c) ((c == ' ') || (c >= '\t' && c <= '\r'))


int main() {
    pthread_attr_t attr;
    int serverFileDesc = newServer();

    // 初始化线程属性
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    while (1) {
        // pthread_t tid;
        struct sockaddr_in caddr;
        socklen_t clen = sizeof caddr;
        int clientFileDesc =
                accept(serverFileDesc, (struct sockaddr *)&caddr, &clen);
        if (clientFileDesc < 0) {
            printf("clientFileDesc < 0");
            // CERR("accept serverFileDesc = %d is error!", serverFileDesc);
            break;
        }
        responseClient(&clientFileDesc);
        // if (pthread_create(&tid, &attr, responseClient, &clientFileDesc) < 0)
        // { CERR("pthread_create run is error!"); break;
        // }
    }

    // 销毁线程
    pthread_attr_destroy(&attr);
    close(serverFileDesc);
    return 0;
}

int newServer() {
    int serverFileDesc;
    struct sockaddr_in saddr;

    serverFileDesc = socket(PF_INET, SOCK_STREAM, 0);
    saddr.sin_family = AF_INET; // ipv4
    saddr.sin_port = htons(SERVER_PORT);
    saddr.sin_addr.s_addr = inet_addr(SERV);
    printf("server start\n");

    // bind，绑定 socket 和 sockaddr_in
    if (bind(serverFileDesc, (struct sockaddr *)&saddr, sizeof(saddr)) == -1) {
        perror("bind error\n");
        exit(1);
    }

    // listen，开始添加端口
    if (listen(serverFileDesc, QUEUE_SIZE) == -1) {
        perror("listen error");
        exit(1);
    }
    return serverFileDesc;
}

void *responseClient(int *clientFileDesc) {
    char buf[BUFF_SIZE], path[BUFF_SIZE >> 1], method[BUFF_SIZE >> 5];
    char *lPtr, *rPtr, *query, *nb = buf;
    // int iscgi;
    int typeSize = sizeof(method);
    read(*clientFileDesc, buf, BUFF_SIZE);
    fputs(buf, stdout);

    // printf("i am here\n");
    // 如果buff中存储的不是空格, 并且type中还能容纳字符的话,
    // 在method从存储表示method类型的字符
    for (lPtr = method, rPtr = nb;
         !isSpace(*rPtr) && ((lPtr - method) < (typeSize - 1));
         *lPtr++ = *rPtr++) {
        // nothing
    }

    // 将buf中开始不为empty的部分塞入了 method 中
    *lPtr = '\0';

    // 在buf中, 跳过空字符
    while (*rPtr && isSpace(*rPtr)) {
        ++rPtr;
    }

    // 存储得到的path信息
    // *path = '.';
    for (lPtr = path; (lPtr - path) < sizeof path - 1 && !isSpace(*rPtr);
         *lPtr++ = *rPtr++) {
        // nothing
    }
    *lPtr = '\0';
    printf("method: %s\npath: %s\n", method, path);

    // 处理GET请求
    if (strcasecmp("GET", method) == 0) {
        // printf("here GET\n");
        responseFile(*clientFileDesc, path);
    } else if (strcasecmp("POST", method)) { // 处理post请求
        responseMessage(*clientFileDesc);
    } else {
        response_404(*clientFileDesc);
        close(*clientFileDesc);
        return NULL;
    }
    close(*clientFileDesc);
    return NULL;
}

#define isSpace(c) ((c == ' ') || (c >= '\t' && c <= '\r'))


void responseFile(int clientFileDesc, const char *path) {
    struct stat st;
    FILE *file;
    char buf[BUFF_SIZE];
    char newPath[BUFF_SIZE] = "";
    strcat(newPath, PATH_PREFIX);
    strcat(newPath, path);
    if (strcmp(path, "/") == 0) {
        strcat(newPath, "index.html");
    }

    // 如果文件读取错误
    if (stat(newPath, &st) < 0) {
        response_404(clientFileDesc);
        close(clientFileDesc);
        return;
    }
    // printf("read error\n");

    // 处理文件内容
    if ((file = fopen(newPath, "rb")) == NULL) { //文件解析错误, 返回404
        printf("read null\n");
        response_404(clientFileDesc);
    } else {

        // 判断content-type
        if (strcmp((path + strlen(path) - 5), ".html") == 0) {
            response_200(clientFileDesc, "text/html"); //发送给200的报文头过去
        } else if (strcmp((path + strlen(path) - 4), ".css") == 0) {
            response_200(clientFileDesc, "text/css");
        } else if (strcmp((path + strlen(path) - 3), ".js") == 0) {
            response_200(clientFileDesc, "application/javascript");
        }
        // // 先判断文件内容存在
        while (!feof(file) && fgets(buf, sizeof buf, file)) {
            write(clientFileDesc, buf, strlen(buf));
        // while (size = read(fd, buf, BUFF_SIZE) > 0) {
        //     write(clientFileDesc, buf, BUFF_SIZE);
        }
    }
    // printf("file will close\n");
    fclose(file);
    // printf("file closed\n");    
}

void responseMessage(int clientFileDesc) {
    char buf[BUFF_SIZE];
    char c;
    int size;
    size = read(clientFileDesc, buf, BUFF_SIZE);


    // 处理POST, 在浏览器实现post操作时, 会用tcp发送两个package, 一个是http
    // header, 一个是http body, 所以这个地方再次读取了从服务器发送过来的数据,
    // 即http body
}

inline void response_404(int clientFileDesc) {
    const char *content = "HTTP/1.1 404 NOT FOUND\r\n"
                          "Server: zackbee'server 1.0\r\n"
                          "Content-Type: text/html; charset=UTF-8\r\n"
                          "\r\n"
                          "<html>"
                          "<head><title>404 NOT FOUND</title></head>\r\n"
                          "<body><p>404: 你请求的页面不存在</p></body>"
                          "</html>";

    // printf("strlen: %d\n", strlen(content));
    write(clientFileDesc, content, strlen(content));
    printf("404\n");
}

inline void response_200(int clientFileDesc, const char *type) {
    char templete[BUFF_SIZE] = "HTTP/1.1 200 OK\r\n"
                               "Server: zackbee'server 1.0\r\n"
                               "Content-Type: %s; charset=UTF-8\r\n"
                               "\r\n";
    char header[BUFF_SIZE];
    sprintf(header, templete, type);
    write(clientFileDesc, header, strlen(header));
}