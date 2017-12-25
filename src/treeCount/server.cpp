/**
 *     虽然几乎所有内容都被我重构了, 但是此模块参考了"喜欢兰花山丘"的实现,
 *     blog: http://www.cnblogs.com/life2refuel/p/5277111.html
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
#include <sys/mman.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "treeCount.h"

#define PATH_PREFIX "./src"

#define QUEUE_SIZE 64

// 缓冲区大小
#define BUFF_SIZE 1024

// listen监听队列的大小
#define QUEUE_SIZE 64


/**
 *     创建一个server fileDesc
 *     @param server server监听的地址
 *     @port port 监听的端口
 *     @param return 返回创建的server fileDesc
 */
int newServer(char *server, int port);

/**
 *     在客户端链接过来, 多线程处理的函数
 *     @param clientFileDesc 客户端文件描述符
 *     @return 返回处理结果,这里默认返回 NULL
 */
void *responseClient(int *clientFileDesc, string &str);

/**
 *     将文件 发送给客户端
 *     @param clientFileDesc 客户端文件描述符
 *     @param path 发送的文件路径
 */
void responseFile(int clientFileDesc, const char *path);

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

#define isSpace(ch) (ch == ' ')

#define isEnter(ch) (ch == '\n')


int main(int argc, char **argv) {

    // 初始化数据库
    int port = atoi(argv[2]);
    int serverFileDesc = newServer(argv[1], port);
    string str = treeCount(argv[3]);
    while (1) {
        struct sockaddr_in caddr;
        socklen_t clen = sizeof caddr;
        int clientFileDesc =
                accept(serverFileDesc, (struct sockaddr *)&caddr, &clen);

        // 客户端秒速符错误
        if (clientFileDesc < 0) {
            printf("clientFileDesc < 0");
            break;
        }
        responseClient(&clientFileDesc, str);
    }

    close(serverFileDesc);
    return 0;
}


int newServer(char *server, int port) {
    int serverFileDesc;
    struct sockaddr_in saddr;

    serverFileDesc = socket(PF_INET, SOCK_STREAM, 0);
    saddr.sin_family = AF_INET; // ipv4
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = inet_addr(server);
    printf("server start\n");
    printf("server adress: %s\n", server);
    printf("server port: %d\n", port);

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

void *responseClient(int *clientFileDesc, string &str) {
    char buf[BUFF_SIZE], path[BUFF_SIZE >> 1], method[BUFF_SIZE >> 5];
    char *lPtr, *rPtr, *query, *nb = buf;
    int bufSize;
    int typeSize = sizeof(method);
    bufSize = read(*clientFileDesc, buf, BUFF_SIZE);
    fputs(buf, stdout);

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
    for (lPtr = path; (lPtr - path) < sizeof path - 1 && !isSpace(*rPtr);
         *lPtr++ = *rPtr++) {
        // nothing
    }
    *lPtr = '\0';

    // printf("method: %s\npath: %s\n", method, path);

    // 处理GET请求
    if (strcasecmp("GET", method) == 0) {
        responseFile(*clientFileDesc, path);
        close(*clientFileDesc);        
    } else if (strcasecmp("POST", method) == 0) { // 处理post请求
        printf("get post\n");
        response_200(*clientFileDesc, "text/plain");
        write(*clientFileDesc, str.c_str(), str.length());
        close(*clientFileDesc);
    } else {
        response_404(*clientFileDesc);
        close(*clientFileDesc);
        return NULL;
    }
    close(*clientFileDesc);
    return NULL;
}

void responseFile(int clientFileDesc, const char *path) {
    struct stat filestat;
    FILE *file;
    char buf[BUFF_SIZE];
    char newPath[BUFF_SIZE] = "";
    strcat(newPath, PATH_PREFIX);
    strcat(newPath, path);
    if (strcmp(path, "/") == 0) {
        strcat(newPath, "html/index.html");
    }
    printf("final path: %s\n", newPath);

    // 如果文件读取错误
    if (stat(newPath, &filestat) < 0) {
        printf("read error\n");
        response_404(clientFileDesc);
        close(clientFileDesc);
        return;
    }

    // 处理文件内容
    if ((file = fopen(newPath, "rb")) == NULL) { //文件解析错误, 返回404
        printf("read null\n");
        response_404(clientFileDesc);
    } else {

        // 判断content-type
        if (strcmp((newPath + strlen(newPath) - 5), ".html") == 0) {
            response_200(clientFileDesc, "text/html"); //发送给200的报文头过去
        } else if (strcmp((newPath + strlen(newPath) - 4), ".css") == 0) {
            response_200(clientFileDesc, "text/css");
        } else if (strcmp((newPath + strlen(newPath) - 3), ".js") == 0) {
            response_200(clientFileDesc, "application/javascript");
        } else if (strcmp((newPath + strlen(newPath) - 6), ".woff2") == 0) {
            response_200(clientFileDesc, "font/woff2");
        } else if (strcmp((newPath + strlen(newPath) - 5), ".woff") == 0) {
            response_200(clientFileDesc, "font/woff");
        } else if (strcmp((newPath + strlen(newPath) - 5), ".json") == 0) {
            response_200(clientFileDesc, "application/json");
        } else if ((strcmp((newPath + strlen(newPath) - 4), ".png") == 0) ||
                   (strcmp((newPath + strlen(newPath) - 4), ".png") == 0)) {
            response_200(clientFileDesc, "image/png");
        } else {
            response_404(clientFileDesc);
            return;
        }

        // 先判断文件内容存在
        uint64_t size = 0;

        // 不用sendfile或fread的原因是, sendfile压根没有数据传输,
        // fread读取不完正常文件, 亲测
        while (!feof(file) && fgets(buf, sizeof buf, file)) {
            size += strlen(buf);
            write(clientFileDesc, buf, strlen(buf));
        }
        printf("\n%lu\n", size);
    }
    fclose(file);
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
                               "Content-Type: %s;%s\r\n"
                               "\r\n";
    char header[BUFF_SIZE];

    // 是图片时不加上编码
    if (strcmp(type, "image/png") == 0) {
        sprintf(header, templete, type, "");
    } else {
        sprintf(header, templete, type, "charset=UTF-8");
    }

    write(clientFileDesc, header, strlen(header));
}

int getFileDescLine(int fileDesc, char buf[], int size) {
    char *tp = buf;
    char c;

    --size;
    while ((tp - buf) < size) {
        if (read(fileDesc, &c, 1) <= 0) //伪造结束条件
            break;
        if (c == '\n') { //全部以\r分割
            break;
        }
        *tp++ = c;
    }
    *tp = '\0';
    return tp - buf;
}