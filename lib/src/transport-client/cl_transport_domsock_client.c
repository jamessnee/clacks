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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cl_transport_domsock_client.h"

static int dom_sock = -1;
static struct sockaddr_un server;

int send_trace_message(const TraceMessage *t_msg) {
  int ret;
  unsigned len;
  void *t_msg_buf;

  if (dom_sock == -1) {
    dom_sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (dom_sock < 0) {
      fprintf(stderr, "libclacks: Couldn't open domain socket: %d\n", errno);
      dom_sock = -1;
      return -1;
    }
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, CL_TRANSPORT_DOM_SOCKET);
  }

  // Serialize the message into a buffer for sending
  len = trace_message__get_packed_size(t_msg);
  t_msg_buf = malloc(len);
  trace_message__pack(t_msg, t_msg_buf);

  ret = sendto(dom_sock, t_msg_buf, len, 0, (struct sockaddr *) &server, sizeof(struct sockaddr_un));
  if (ret == -1) {
    fprintf(stderr, "libclacks: Error sending trace message: %d\n", errno);
    free(t_msg_buf);
    return -1;
  }

  free(t_msg_buf);
  return 0;

}
