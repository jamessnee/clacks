#include "clacks_common.h"
#include "TraceMessage.pb-c.h"
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

void handle_message(const unsigned char *rd_buf, size_t size) {
  TraceMessage *t_msg;
  t_msg = trace_message__unpack(NULL, size, rd_buf);
  if (t_msg == NULL) {
    syslog(LOG_ERR, "clacks_transport_dom: error unpacking trace message: %d", errno);
    return;
  }

  // Message seems to be good
  syslog(LOG_INFO, "clacks_transport_dom: received: %s", t_msg->msg);
  trace_message__free_unpacked(t_msg, NULL);
}

void init_transport(/* Add storage struct here */) {
  /* Save the storage -- init it */
}

void * start_transport(void *args) {
  int dom_sock;
  struct sockaddr_un server;
  unsigned char rd_buf[1024]; // TODO: Change this to a constant
  int recv_size;

  if (!check_dom_dir(CL_TRANSPORT_DOM_SOCKET)) {
    syslog(LOG_ERR, "clacks_transport_dom: Couldn't create domain socket directory");
    goto exit;
  }

  dom_sock = socket(AF_UNIX, SOCK_DGRAM, 0);
  if(dom_sock < 0) {
    goto exit;
  }
  server.sun_family = AF_UNIX;
  strcpy(server.sun_path, CL_TRANSPORT_DOM_SOCKET);
  if ( bind(dom_sock, (struct sockaddr *)&server, sizeof(struct sockaddr_un)) ){
    syslog(LOG_ERR, "clacks_transport_dom: Couldn't bind to domain socket");
    goto error;
  }

  cl_debug("Domain socket transport open and ready...");
  for (;;) {
    memset(rd_buf, 0, sizeof(rd_buf));
    recv_size = 0;
    //if (read(dom_sock, rd_buf, sizeof(rd_buf)) < 0) {
    if ((recv_size = recv(dom_sock, rd_buf, sizeof(rd_buf), MSG_WAITALL)) < 0) {
      syslog(LOG_ERR, "clacks_transport_dom: read error from socket: %d", errno);
    } else {
      /* Good read */
      handle_message(rd_buf, (size_t) recv_size);
    }
  }

error:
  close(dom_sock);
  unlink(CL_TRANSPORT_DOM_SOCKET);
  syslog(LOG_ERR, "clacks_transport_dom: socket closed, file unlinked");
  goto exit;

exit:
  pthread_exit(0);
}
