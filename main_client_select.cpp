#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>

#define DEBUG

#ifdef DEBUG
struct message_pack {
  int num;
  float age;
  std::string message;
  char buffer_char[1024];
  message_pack() {
    num = 0;
    age = 0.f;
    memset(buffer_char, 0, sizeof(buffer_char));
    message = "Hello there!";
  }
};
#endif  // DEBUG

int main(int argc, char *argv[]) {

#ifdef DEBUG
  message_pack pack;
// message_pack* pack_st = &pack;
#endif  // DEBUG

  if (argc != 3) {
    printf("usage:./tcpclient ip port\n");
    return -1;
  }

  int sockfd;
  struct sockaddr_in servaddr;
  char buf[1024];

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("socket() failed.\n");
    return -1;
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(atoi(argv[2]));
  servaddr.sin_addr.s_addr = inet_addr(argv[1]);

  if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
    printf("connect(%s:%s) failed.\n", argv[1], argv[2]);
    close(sockfd);
    return -1;
  }

  printf("connect ok.\n");

  for (int ii = 0; ii < 10000; ii++) {
    #ifdef DEBUG
    // 从命令行输入内容。
    memset(&pack, 0, sizeof(pack));
    pack.message = "Hello there!";
    printf("please input:");
    scanf("%d", &pack.num);
    scanf("%f", &pack.age);
    scanf("%s", pack.buffer_char);

    #elif
    // 从命令行输入内容。
    memset(buf, 0, sizeof(buf));
    printf("please input:");
    scanf("%s", buf);
    // buf[0] = 0x53;
    // sprintf(buf,"1111111111111111111111ii=%08d",ii);
    #endif // DEBUG

#ifdef DEBUG
    if (write(sockfd, &pack, sizeof(pack)) <= 0) {
      printf("write() failed.\n");
      close(sockfd);
      return -1;
    }
    memset(&pack, 0, sizeof(pack));
    if (read(sockfd, &pack, sizeof(pack)) <= 0) {
      printf("read() failed.\n");
      close(sockfd);
      return -1;
    }

    printf("recv:\n num=%d\n age=%f\n,message=%s\n,buff_char=%s\n",pack.num,pack.age,pack.message.c_str(),pack.buffer_char);

#elif
    if (write(sockfd, buf, strlen(buf)) <= 0) {
      printf("write() failed.\n");
      close(sockfd);
      return -1;
    }

    memset(buf, 0, sizeof(buf));
    if (read(sockfd, buf, sizeof(buf)) <= 0) {
      printf("read() failed.\n");
      close(sockfd);
      return -1;
    }

    printf("recv:%s\n", buf);
#endif // DEBUG

    // close(sockfd); break;
  }
}
