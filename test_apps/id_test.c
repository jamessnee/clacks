#include "clacks_common.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>

int main (int argc, char *argv[]) {
  int dom_sock, i, w_ret, r_ret;
  struct sockaddr_un server;
  char buf[1024];

  for (i = 0; i < 2; i++) {
    dom_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (dom_sock < 0) {
      printf("Error opening socket\n");
      return -1;
    }
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, CL_ID_DOM_SOCKET);

    if (connect(dom_sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un)) < 0) {
      printf("Error connecting to socket: %d\n", errno);
      return -1;
    }

    if (i % 2 == 0) {
      w_ret = write(dom_sock, CL_ID_GET, sizeof(CL_ID_GET));
    } else {
      w_ret = write(dom_sock, CL_ID_TAG, sizeof(CL_ID_TAG));
    }
    if (w_ret < 0) {
      printf("Error writing to socket\n");
      return -1;
    }

    r_ret = read(dom_sock, buf, sizeof(buf));
    if (r_ret < 0) {
      printf("Error reading from socket: %d\n", errno);
    } else {
      printf("Got uuid: %s\n", buf);
    }
    close(dom_sock);
  }

  return 0;
}
