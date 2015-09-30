#include "clacks_common.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>

#include "id_client.h"

static int dom_sock = -1;
static struct sockaddr_un server;

int get_new_id(char *id) {
  int ret;

  if (dom_sock == -1) {
    // Setup a new domain socket
    dom_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (dom_sock < 0) {
      fprintf(stderr, "libclacks: Couldn't open domain socket: %d\n", errno);
      dom_sock = -1;
      return -1;
    }
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, CL_ID_DOM_SOCKET);
  }

  if (connect(dom_sock, (struct sockaddr *)&server, sizeof(struct sockaddr_un)) < 0) {
    fprintf(stderr, "libclacks: Couldn't connect domain socket: %d\n", errno);
    close(dom_sock);
    dom_sock = -1;
    return -1;
  }

  ret = write(dom_sock, CL_ID_GET, sizeof(CL_ID_GET));
  if (ret < 0) {
    fprintf(stderr, "libclacks: Error writing to the dom socket: %d\n", errno);
    close(dom_sock);
    dom_sock = -1;
    return -1;
  }

  ret = read(dom_sock, id, sizeof(id)); // TODO: wrap this into an id struct (better size checking)
  if (ret < 0) {
    fprintf(stderr, "libclacks: Error reading from dom socket: %d\n", errno);
    close(dom_sock);
    dom_sock = -1;
    return -1;
  }

  return 0;
}
