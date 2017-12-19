#include <arpa/inet.h>
#include <errno.h>
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

#define SERVER_PORT 3000
#define isSpace(c) ((c == ' ') || (c >= '\t' && c <= '\r'))

// 缓冲区大小
#define BUFF_SIZE (1024)

// listen监听队列的大小
#define QUEUE_SIZE (64)

/**
 *     读取文件描述符 fileDesc 一行的内容,保存在buf中,返回读取内容长度
 *     @param fileDesc 文件描述符
 *     @param buf 保存的内容
 *     @param size buf 的大小
 *     @return 返回读取的长度
 */
int getFileDescLine(int fileDesc, char buf[], int size);

/**
 *     返回400 请求解析失败, 客户端代码错误
 *     @param clientFileDesc 客户端文件描述符
 */
inline void response_400(int clientFileDesc);

/**
 *     返回404, 请求文件没有找到
 *     @param clientFileDesc 客户端文件描述符
 */
inline void response_404(int clientFileDesc);

/**
 *     返回501 错误, 不支持的请求
 *     @param clientFileDesc 客户端文件描述符
 */
inline void response_501(int clientFileDesc);

/**
 *     服务器内部错误,无法处理等
 *     @param clientFileDesc 客户端文件描述符
 */
inline void response_500(int clientFileDesc);

/**
 *     // 返回200 请求成功
 *     @param clientFileDesc 客户端文件描述符
 */
inline void response_200(int clientFileDesc);

/**
 *     将文件 发送给客户端
 *     @param clientFileDesc 客户端文件描述符
 *     @param path 发送的文件路径
 */
void responseFile(int clientFileDesc, const char *path);

/**
 *     启动服务器
 *     @return 返回服务的文件修饰符
 */
int startServer();

/**
 *     在客户端链接过来, 多线程处理的函数
 *     @param clientFileDesc 客户端文件描述符
 *     @return 返回处理结果,这里默认返回 NULL
 */
void *responseClient(int *clientFileDesc);

/**
 *     处理客户端的http请求
 *     @param clientFileDesc 客户端文件描述符
 *     @param path 请求的文件路径
 *     @param method 请求类型
 *     @param query 请求发送的过来的数据
 */
void responseMessage(int clientFileDesc, const char *path, const char *type);

int main() {
    pthread_attr_t attr;
    int serverFileDesc = startServer();

    // 初始化线程属性
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    while (1) {
        pthread_t tid;
        struct sockaddr_in caddr;
        socklen_t clen = sizeof caddr;
        int clientFileDesc =
                accept(serverFileDesc, (struct sockaddr *)&caddr, &clen);
        if (clientFileDesc < 0) {
            CERR("accept serverFileDesc = %d is error!", serverFileDesc);
            break;
        }
        if (pthread_create(&tid, &attr, responseClient,
                           (void *)clientFileDesc) < 0) {
            CERR("pthread_create run is error!");
            break;
        }
    }

    // 销毁线程
    pthread_attr_destroy(&attr);
    close(serverFileDesc);
    return 0;
}

int getFileDescLine(int fileDesc, char buf[], int size) {
    char *tp = buf;
    char c;

    --size;
    while ((tp - buf) < size) {
        if (read(fileDesc, &c, 1) <= 0) //伪造结束条件
            break;
        if (c == '\r') { //全部以\r分割
            if (recv(fileDesc, &c, 1, MSG_PEEK) > 0 && c == '\n')
                read(fileDesc, &c, 1);
            else //意外的结束,填充 \n 结束读取
                *tp++ = '\n';
            break;
        }
        *tp++ = c;
    }
    *tp = '\0';
    return tp - buf;
}

// 返回400 请求解析失败,客户端代码错误
inline void response_400(int clientFileDesc) {
    const char *estr = "HTTP/1.0 400 BAD REQUEST\r\n"
                       "Server: wz simple httpd 1.0\r\n"
                       "Content-Type: text/html\r\n"
                       "\r\n"
                       "<p>你的请求有问题,请检查语法!</p>\r\n";

    write(clientFileDesc, estr, strlen(estr));
}

// 返回404 文件内容, 请求文件没有找见
inline void response_404(int clientFileDesc) {
    const char *estr = "HTTP/1.0 404 NOT FOUND\r\n"
                       "Server: wz simple httpd 1.0\r\n"
                       "Content-Type: text/html\r\n"
                       "\r\n"
                       "<html>"
                       "<head><title>你请求的界面被查水表了!</title></head>\r\n"
                       "<body><p>404: 估计是回不来了</p></body>"
                       "</html>";

    //开始发送数据
    write(clientFileDesc, estr, strlen(estr));
}

// 返回501 错误, 请求解析失败,不支持的请求
inline void response_501(int clientFileDesc) {
    const char *estr = "HTTP/1.0 501 Method Not Implemented\r\n"
                       "Server: wz simple httpd 1.0\r\n"
                       "Content-Type: text/html\r\n"
                       "\r\n"
                       "<html>"
                       "<head><title>小伙子不要乱请求</title></head>\r\n"
                       "<body><p>too young too simple, "
                       "年轻人别总想弄出个大新闻.</p></body>"
                       "</html>";

    //这里还有一个好的做法是将这些内容定义在文件中输出文件
    write(clientFileDesc, estr, strlen(estr));
}


// 服务器内部错误,无法处理等
inline void response_500(int clientFileDesc) {
    const char *estr = "HTTP/1.0 500 Internal Server Error\r\n"
                       "Server: wz simple httpd 1.0\r\n"
                       "Content-Type: text/html\r\n"
                       "\r\n"
                       "<html>"
                       "<head><title>Sorry </title></head>\r\n"
                       "<body><p>最近有点方了!</p></body>"
                       "</html>";

    write(clientFileDesc, estr, strlen(estr));
}

// 返回200 请求成功 内容, 后面可以加上其它参数,处理文件输出
inline void response_200(int clientFileDesc) {
    // 打印返回200的报文头
    const char *str = "HTTP/1.0 200 OK\r\n"
                      "Server: wz simple httpd 1.0\r\n"
                      "Content-Type: text/html\r\n"
                      "\r\n";

    write(clientFileDesc, str, strlen(str));
}

/*
 * 将文件 发送给客户端
 * clientFileDesc        : 客户端文件描述符
 * path        : 发送的文件路径
 */
void responseFile(int clientFileDesc, const char *path) {
    FILE *txt;
    char buf[BUFF_SIZE];

    // 读取报文头,就是过滤
    while (getFileDescLine(clientFileDesc, buf, sizeof buf) > 0 &&
           strcmp("\n", buf))
        ;
    // 这里开始处理 文件内容
    if ((txt = fopen(path, "r")) == NULL) //文件解析错误,给它个404
        response_404(clientFileDesc);
    else {
        response_200(clientFileDesc); //发送给200的报文头过去
        // 先判断文件内容存在
        while (!feof(txt) && fgets(buf, sizeof buf, txt))
            write(clientFileDesc, buf, strlen(buf));
    }
    fclose(txt);
}

int startServer() {
    int serverFileDesc;
    struct sockaddr_in saddr = {AF_INET};

    IF_CHECK(serverFileDesc = socket(PF_INET, SOCK_STREAM, 0));
    saddr.sin_port = htons(SERVER_PORT);
    saddr.sin_addr.s_addr = INADDR_ANY;
    printf("server start\n");

    return serverFileDesc;
}

void *responseClient(int *clientFileDesc) {
    char buf[BUFF_SIZE], path[BUFF_SIZE >> 1], method[BUFF_SIZE >> 5];
    char *lPtr, *rPtr, *query, *nb = buf;
    struct stat st;
    // int iscgi;
    int typeSize = sizeof(method);

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
    *path = '.';
    for (lPtr = path + 1; (lPtr - path) < sizeof path - 1 && !isSpace(*rPtr);
         *lPtr++ = *rPtr++) {
        // nothing
    }
    *lPtr = '\0';

    if (stat(path, &st) < 0) {
        while (getFileDescLine(*clientFileDesc, buf, sizeof buf) > 0 &&
               strcmp("\n", buf)) // 读取内容直到结束
            ;
        response_404(*clientFileDesc);
        close(*clientFileDesc);
        return NULL;
    }
    // 合法情况, 执行,写入,读取权限
    // if ((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) ||
    // (st.st_mode & S_IXOTH))
    // iscgi = 0;
    if (strcasecmp("GET", path) == 0) {
        responseFile(*clientFileDesc, path);
    } else {
        responseMessage(*clientFileDesc, path, method);
    }
    close(*clientFileDesc);
    return NULL;
}

void responseMessage(int clientFileDesc, const char *path, const char *method) {
    char buf[BUFF_SIZE];
    int pocgi[2], picgi[2];
    pid_t pid;
    int contlen = -1; //报文长度
    char c;

    if (strcasecmp(method, "POST") == 0) {
        while (getFileDescLine(clientFileDesc, buf, sizeof buf) > 0 &&
               strcmp("\n", buf)) {
            buf[15] = '\0';
            if (!strcasecmp(buf, "Content-Length:")) {
                contlen = atoi(buf + 16);
            }
        }
        if (contlen == -1) { //错误的报文,直接返回错误结果
            response_400(clientFileDesc);
            return;
        }
    } else { // 读取报文头,就是过滤, 后面就假定是 GET
        while (getFileDescLine(clientFileDesc, buf, sizeof buf) > 0 &&
               strcmp("\n", buf)) {
        };
    }

    //这里处理请求内容, 先处理错误信息
    if (pipe(pocgi) < 0) {
        response_500(clientFileDesc);
        return;
    }

    //管道, 0读取, 1写入
    if (pipe(picgi) < 0) {
        close(pocgi[0]), close(pocgi[1]);
        response_500(clientFileDesc);
        return;
    }
    if ((pid = fork()) < 0) {
        close(pocgi[0]), close(pocgi[1]);
        close(picgi[0]), close(picgi[1]);
        response_500(clientFileDesc);
        return;
    }

    // 这里就是多进程处理了, 先处理子进程
    if (pid == 0) {
        // dup2 让 前者共享后者同样的文件表
        dup2(pocgi[1], STDOUT_FILENO); //标准输出算作 pocgi管道 的写入端
        dup2(picgi[0], STDIN_FILENO); //标准输入做为picgif管道的读取端
        close(pocgi[0]);
        close(pocgi[1]);

        // 添加环境变量,用于当前会话中
        sprintf(buf, "REQUEST_METHOD=%s", method);
        putenv(buf);
        // 继续凑环境变量串,放到当前会话种
        if (strcasecmp(buf, "POST") == 0) {
            sprintf(buf, "CONTENT_LENGTH=%d", contlen);
        }
        putenv(buf);
        // 成功的话调到 新的执行体上
        execl(path, path, NULL);

        // 这行代码原本是不用的, 但是防止 execl执行失败, 子进程没有退出.妙招
        exit(EXIT_SUCCESS);
    }
    // 父进程, 为所欲为了,先发送个OK
    write(clientFileDesc, "HTTP/1.0 200 OK\r\n", 17);
    close(pocgi[1]);
    close(picgi[0]);

    if (strcasecmp(method, "POST") == 0) {
        int i; //将数据都写入到 picgi 管道中, 让子进程在 picgi[0]中读取 =>
               // STDIN_FILENO
        for (i = 0; i < contlen; ++i) {
            read(clientFileDesc, &c, 1);
            write(picgi[1], &c, 1);
        }
    }
    //从子进程中 读取数据 发送给客户端, 多线程跨进程阻塞模型
    while (read(pocgi[0], &c, 1) > 0)
        write(clientFileDesc, &c, 1);

    close(pocgi[0]);
    close(picgi[1]);
    //等待子进程结束
    waitpid(pid, NULL, 0);
}
