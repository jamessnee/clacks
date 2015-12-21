#include "clacks_common.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "id_client.h"

static int dom_sock = -1;
static struct sockaddr_un server;

int connect_to_id_service() {
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
  return 0;
}

int get_new_id(char *id) {
  int ret;
  char buf[1024];

  if(connect_to_id_service() == -1) {
    return -1;
  }

  ret = write(dom_sock, CL_ID_GET, sizeof(CL_ID_GET));
  if (ret < 0) {
    fprintf(stderr, "libclacks: Error writing to the dom socket: %d\n", errno);
    close(dom_sock);
    dom_sock = -1;
    return -1;
  }

  ret = read(dom_sock, buf, sizeof(buf)); // TODO: wrap this into an id struct (better size checking)
  if (ret < 0) {
    fprintf(stderr, "libclacks: Error reading from dom socket: %d\n", errno);
    close(dom_sock);
    dom_sock = -1;
    return -1;
  }

  memcpy(id, buf, 37);
  close(dom_sock);
  dom_sock = -1;

  return 0;
}

/* Tagging comms. protocol
 * Send CL_ID_TAG command
 * server -> 1 -> client
 * server <- uuid <- client
 * server -> 1 -> client
 * server <- derrived <- client
 */
void cl_id_tag_id(char *cl_id, char *derrived) {
  int rtn;
  int cmd_rcv_buf = -1;

  if (connect_to_id_service() == -1) {
    return;
  }

  rtn = write(dom_sock, CL_ID_TAG, sizeof(CL_ID_TAG));
  if (rtn < 0) {
    fprintf(stderr, "libclacks: Error writing to the dom socket: %d\n", errno);
    close(dom_sock);
    dom_sock = -1;
  }

  // server -> 1 -> client
  rtn = read(dom_sock, &cmd_rcv_buf, sizeof(cmd_rcv_buf));
  if (rtn < 0) {
    fprintf(stderr, "libclacks: Error reading tagging command from server: %d\n", errno);
    close(dom_sock);
    dom_sock = -1;
  }

  // server <- uuid <- client
  rtn = write(dom_sock, cl_id, 37);
  if (rtn < 0) {
    fprintf(stderr, "libclacks: Error writing UUID to server for tagging: %d\n", errno);
    close(dom_sock);
    dom_sock = -1;
  }

  // server -> 1 -> client
  rtn = read(dom_sock, &cmd_rcv_buf, sizeof(cmd_rcv_buf));
  if (rtn < 0) {
    fprintf(stderr, "libclacks: Error reading tagging command from server: %d\n", errno);
    close(dom_sock);
    dom_sock = -1;
  }

  // server <- derrived <- client
  rtn = write(dom_sock, derrived, strlen(derrived));
  if (rtn < 0) {
    fprintf(stderr, "libclacks: Error writing UUID to server for tagging: %d\n", errno);
    close(dom_sock);
    dom_sock = -1;
  }

  close(dom_sock);
  dom_sock = -1;

}
