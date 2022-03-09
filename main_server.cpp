#include "tcp_server/tcp_server.hpp"

int main(int argc, char *argv[]) {
  // network_com::tcp_com communication =
  //     network_com::tcp_com(atoi(argv[1]), argc);
  network_com::tcp_com communication =
      network_com::tcp_com(argv, argc,"xml/tcp_com_config.xml");

  int com_status = communication.initServer();
  if (com_status < 0) {
    printf("initserver() failed.\n");
    return -1;
  }

  while (1) {

    int communication_status = communication.recvData();

    if (communication_status < 0) {
      break;
    } else if (communication_status == 0) {
      continue;
    }

    communication.sendData();
    communication.resetEventfdBox();
  }

  return 0;
}
