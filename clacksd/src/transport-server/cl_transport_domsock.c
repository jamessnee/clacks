#include "clacks_common.h"
#include <errno.h>
#include <pthread.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "cl_transport_domsock.h"

int check_dom_dir(char *sock_path) {
  struct stat sb;
  if (stat(CL_DOM_DIR, &sb) != 0 || !S_ISDIR(sb.st_mode)){
    // Create the directory, into which we'll put the dom socket
    mkdir(CL_DOM_DIR, 777);
    // Yes, this is horrid.
    if (stat(CL_DOM_DIR, &sb) != 0 || !S_ISDIR(sb.st_mode)){
      return -1;
    }
  }
  return 1;
}

void handle_connection(int sock_con) {
  /* Receive the message */
}

void init_transport(/* Add storage struct here */) {
  /* Save the storage -- init it */
}

void * start_transport(void *args) {
  int dom_sock, sock_msg;
  struct sockaddr_un server;

  if (!check_dom_dir(CL_TRANSPORT_DOM_SOCKET)) {
    syslog(LOG_ERR, "clacks_transport_dom: Couldn't create domain socket directory");
    goto exit;
  }

  dom_sock = socket(AF_UNIX, SOCK_STREAM, 0);
  if(dom_sock < 0) {
    goto exit;
  }
  server.sun_family = AF_UNIX;
  strcpy(server.sun_path, CL_TRANSPORT_DOM_SOCKET);
  if ( bind(dom_sock, (struct sockaddr *)&server, sizeof(struct sockaddr_un)) ){
    syslog(LOG_ERR, "clacks_transport_dom: Couldn't bind to domain socket");
    goto error;
  }

  if (listen(dom_sock, 5) != 0) {
    syslog(LOG_ERR, "clacks_transport_dom: Couldn't listen on socket");
    goto error;
  }

  cl_debug("Domain socket transport open and ready...");
  for (;;) {
    int server_sock_len = sizeof(server);
    sock_msg = accept(dom_sock, (struct sockaddr *)&server, (socklen_t *)&server_sock_len);
    if (sock_msg < 0) {
      syslog(LOG_ERR, "clacks_transport_dom: Couldn't accept incoming connection: %d", errno);
    }
    handle_connection(sock_msg);
    close(sock_msg);
  }

error:
  close(dom_sock);
  unlink(CL_TRANSPORT_DOM_SOCKET);
  syslog(LOG_ERR, "clacks_transport_dom: socket closed, file unlinked");
  goto exit;

exit:
  pthread_exit(0);
}
