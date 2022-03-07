#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF_SIZE 3

int main(int argc, char **argv) {
  int num = 0;

  // 创建套接字
  int servSock = socket(PF_INET, SOCK_STREAM, 0);
  printf("servSock is %d\n", servSock);

  // int yes = 1;
  // //设置socket的属性，socket关闭后立即收回 用以加强程序的健壮性
  // if (setsockopt(servSock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1 ||
  //     setsockopt(servSock, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(int)) == -1) {
  //   perror("setsockopt error!");
  //   return 0;
  // }

  // 绑定套接字
  sockaddr_in sockAddr;
  memset(&sockAddr, 0, sizeof(sockAddr));
  sockAddr.sin_family = AF_INET;
  // sockAddr.sin_addr.s_addr = INADDR_ANY;
  sockAddr.sin_addr.s_addr = inet_addr("192.168.1.49");
  sockAddr.sin_port = htons(5699);
  bind(servSock, (struct sockaddr *)&sockAddr, sizeof(struct sockaddr));
  printf("After binded, servSock is %d\n", servSock);

  // 进入监听状态
  listen(servSock, 20);

  // 接收客户端请求
  sockaddr_in clntAddr;
  socklen_t nSize = sizeof(sockaddr_in);
  char buffer[BUF_SIZE] = {0};
  while (1) {
    printf("ready for accept\n");
    int clntSock;
    // tcflush(clntSock, );
    if ((clntSock = accept(servSock, (struct sockaddr *)&clntAddr, &nSize)) <
        0) {
      perror("accept error");
      close(clntSock);
      memset(buffer, 0, BUF_SIZE);
      break;
    }
    // int clntSock = accept(servSock,(struct sockaddr*)&clntAddr, &nSize);
    printf("accept completed\n");
    // buffer[BUF_SIZE] = '\0';
    int strLen = recv(clntSock, buffer, BUF_SIZE, 0);
    printf("Message from client:%s\n", buffer);
    send(clntSock, buffer, strLen, 0);

    close(clntSock);
    memset(buffer, 0, BUF_SIZE);
    printf("---------%d----------\n", num);
    ++num;
  }
  close(servSock);

  return 0;
}