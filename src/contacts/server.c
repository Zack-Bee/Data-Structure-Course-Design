#include <arpa/inet.h>
#include <fcntl.h> // open close
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/socket.h> // socket
#include <sys/types.h>  // 基本数据类型
#include <unistd.h>     // read write

#include <signal.h>

#define PORT 3000
#define SERV "127.0.0.1"
#define QUEUE 64
#define BUFF_SIZE 1024
#define PATH_PREFIX "src/contacts"


typedef struct fileType {
    char *key;
    char *value;
} fileType;

fileType httpContentType[] = {{"html", "text/html"},
                              {"gif", "image/gif"},
                              {"jpeg", "image/jpeg"},
                              {"css", "text/css"},
                              {"json", "text/json"}};

int sockfd;
char *httpResponseTemplete = "HTTP/1.1 200 OK\r\n"
                             "Server: Cleey's Server V1.0\r\n"
                             "Accept-Ranges: bytes\r\n"
                             "Content-Length: %d\r\n"
                             "Connection: close\r\n"
                             "Content-Type: %s; charset=UTF-8\r\n\r\n";

void handle_signal(int sign);            // 退出信号处理
void http_send(int sock, char *content); // http 发送相应报文

int main() {

    // 用来注册对信号的监听, SIGINT代表注册的信号类型, 代表Interrupt (ANSI),
    // handle_signal, 处理相应信号的函数
    signal(SIGINT, handle_signal);

    int count = 0; // 计数

    // 定义 socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // 定义 sockaddr_in
    struct sockaddr_in skaddr;

    // 将skaddr的所有字节置为0, 再重新设置
    memset(&skaddr, 0, sizeof(struct sockaddr_in));
    skaddr.sin_family = AF_INET; // ipv4
    skaddr.sin_port = htons(PORT);
    skaddr.sin_addr.s_addr = inet_addr(SERV);

    // bind，绑定 socket 和 sockaddr_in
    if (bind(sockfd, (struct sockaddr *)&skaddr, sizeof(skaddr)) == -1) {
        perror("bind error");
        exit(1);
    }

    // listen，开始添加端口
    if (listen(sockfd, QUEUE) == -1) {
        perror("listen error");
        exit(1);
    }

    // 客户端信息
    char buff[BUFF_SIZE];
    struct sockaddr_in claddr;
    socklen_t length = sizeof(claddr);

    while (1) {
        int sock_client = accept(sockfd, (struct sockaddr *)&claddr, &length);
        printf("%d\n", ++count);
        if (sock_client < 0) {
            perror("accept error");
            exit(1);
        }
        memset(buff, 0, sizeof(buff));
        int len = recv(sock_client, buff, sizeof(buff), 0);
        fputs(buff, stdout);

        // send(sock_client,buff,len,0);
        http_send(sock_client, "烫烫汤困境考");
        close(sock_client);
    }
    fputs("Bye Cleey", stdout);
    close(sockfd);

    return 0;
}

void http_send(int sock_client, char *content) {
    char HTTP_HEADER[BUFF_SIZE], HTTP_INFO[BUFF_SIZE];
    int len = strlen(content);
    sprintf(HTTP_HEADER, httpResponseTemplete, len, "text/html");
    len = sprintf(HTTP_INFO, "%s%s", HTTP_HEADER, content);

    send(sock_client, HTTP_INFO, len, 0);
}

void handle_signal(int sign) {
    fputs("\nSIGNAL INTERRUPT \nBye Cleey! \nSAFE EXIT\n", stdout);
    close(sockfd);
    exit(0);
}

// 从文件描述符 file description 中得到一行
int getFileDescLine(int fd, char buf[], int sz) {
    char *tp = buf;
    char c;

    --sz;
    while ((tp - buf) < sz) {

        //如果没有读到字符, 结束
        if (read(fd, &c, 1) <= 0) 
            break;
        if (c == '\r') { //全部以\r分割
            if (recv(fd, &c, 1, MSG_PEEK) > 0 && c == '\n') {
                read(fd, &c, 1);
            } else { //意外的结束,填充 \n 结束读取
                *tp++ = '\n';
            }
            break;
        }
        *tp++ = c;
    }
    *tp = '\0';
    return tp - buf;
}

#define isSpace(c) ((c == ' ') || (c >= '\t' && c <= '\r'))

void request_accept(int arg) {
    char buf[BUFF_SIZE], path[BUFF_SIZE >> 1], type[BUFF_SIZE >> 5];
    char *lt, *rt, *query, *nb = buf;
    struct stat st;
    int iscgi, cfd = (int)arg;

    // 合法请求处理
    for (lt = type, rt = nb; !isSpace(*rt) && (lt - type) < sizeof type - 1;
         *lt++ = *rt++)
        ;
    *lt = '\0'; //已经将 buf中开始不为empty 部分塞入了 type 中

    // 在buf中 去掉空字符
    while (*rt && isSpace(*rt))
        ++rt;
    // 这里得到路径信息
    *path = '.';
    for (lt = path + 1; (lt - path) < sizeof path - 1 && !isSpace(*rt);
         *lt++ = *rt++)
        ;
    *lt = '\0'; // query url路径就拼接好了

    //单独处理 get 获取 ? 后面数据, 不是POST那就是GET
    if (iscgi != 0) {
        for (query = path; *query && *query != '?'; ++query)
            ;
        if (*query == '?') {
            iscgi = 0;
            *query++ = '\0';
        }
    }

    // type , path 和 query 已经构建好了
    if (stat(path, &st) < 0) {
        while (getfdline(cfd, buf, sizeof buf) > 0 &&
               strcmp("\n", buf)) // 读取内容直到结束
            ;
        response_404(cfd);
        close(cfd);
        return NULL;
    }
    // 合法情况, 执行,写入,读取权限
    if ((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) ||
        (st.st_mode & S_IXOTH)) {
        iscgi = 0;
    }
    if (iscgi) {
        response_file(cfd, path);
    } else {
        request_cgi(cfd, path, type, query);
    }
    close(cfd);
    return;
}