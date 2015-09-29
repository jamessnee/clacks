#include "clacks_common.h"
#include <syslog.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>

void parse_message(char *buf) {
  printf("Message: %s\n", buf);
}

int listen_on_dom_socket(char *sock_path) {
  int dom_sock, sock_msg, read_val;
  struct sockaddr_un server;
  char buf[1024];

  // Setup the domain socket
  dom_sock = socket(AF_UNIX, SOCK_STREAM, 0);
  if (dom_sock < 0) {
    return dom_sock;
  }
  server.sun_family = AF_UNIX;
  strcpy(server.sun_path, sock_path);
  if ( bind(dom_sock, (struct sockaddr *)&server, sizeof(struct sockaddr_un)) ) {
    printf("clacksidd: Couldn't bind to domain socket\n");
    goto error;
  }
  printf("Domain socket bound: %d\n", dom_sock);

  if (listen(dom_sock, 5) != 0) {
    printf("clacksidd: Couldn't listen on socket: %d\n", errno);
    goto error;
  }

  for (;;) {
    int server_sock_len = sizeof(server);
    bzero(buf, sizeof(buf));
    sock_msg = accept(dom_sock, (struct sockaddr *) &server, (socklen_t *)&server_sock_len);
    if (sock_msg < 0) {
      printf("clacksidd: Couldn't accept incoming connection: %d\n", errno);
      goto error;
    } else do {
      if ((read_val = read(sock_msg, buf, 1024)) < 0)
        printf("clacksidd: Couldn't read from dom socket: %d\n", errno);
      else if (read_val == 0)
        parse_message(buf);
    } while (read_val > 0);
    close(sock_msg);
  }

error:
  close(dom_sock);
  unlink(sock_path);
  return -1;

}

int main (int argc, char *argv[]) {
  listen_on_dom_socket(CL_ID_DOM_SOCKET);
}
