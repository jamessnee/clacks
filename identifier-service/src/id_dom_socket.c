#include <syslog.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>

#include "id_dom_socket.h"

int listen_on_dom_socket(char *sock_path) {
  int dom_sock, sock_msg;
  struct sockaddr_un server;

  // Setup the domain socket
  dom_sock = socket(AF_UNIX, SOCK_STREAM, 0);
  if (dom_sock < 0) {
    return dom_sock;
  }
  server.sun_family = AF_UNIX;
  strcpy(server.sun_path, sock_path);
  if ( bind(dom_sock, (struct sockaddr *)&server, sizeof(struct sockaddr_un)) ) {
    syslog(LOG_ERR, "clacksidd: Couldn't bind to domain socket");
    goto error;
  }

  listen(dom_sock, 5); // TODO: Change this queue length to a parameter
  for (;;) {
    sock_msg = accept(dom_sock, 0, 0);
    if (sock_msg == -1) {
      syslog(LOG_ERR, "clacksidd: Couldn't accept incoming connection");
      goto error;
    } else {
      // Read the incoming message
    }
  }

error:
  close(dom_sock);
  unlink(sock_path);
  return -1;

}
