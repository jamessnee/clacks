#include "clacks_common.h"
#include "id_generator.h"
#include <errno.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "id_dom_socket.h"

void parse_message(char *buf) {
  syslog(LOG_INFO, "Message: %s\n", buf);
}

int send_uuid(int sock_con) {
  char uuid[37];

  next_uuid(uuid);
  return write(sock_con, uuid, sizeof(uuid));
}

void handle_connection(int sock_con) {
  int read_val, ret;
  char buf[1024];
  if ((read_val = read(sock_con, buf, 1024)) < 0) {
    syslog(LOG_ERR, "clacksidd: Couldn't read from dom socket: %d", errno);
  } else if (strcmp(buf, CL_ID_GET) == 0) {
    if ((ret = send_uuid(sock_con)) <= 0) {
      syslog(LOG_ERR, "Problem sending UUID: %d", errno);
    }
  } else if (strcmp(buf, CL_ID_TAG) == 0) {
    syslog(LOG_INFO, "Got a TAG message");
  }
}

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

int listen_on_dom_socket(char *sock_path) {
  int dom_sock, sock_msg;
  struct sockaddr_un server;

  // Setup the domain socket
  if (!check_dom_dir(sock_path)) {
    syslog(LOG_ERR, "clacksidd: Couldn't create domain socket directory");
    return -1;
  }

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

  if (listen(dom_sock, 5) != 0) {
    syslog(LOG_ERR, "clacksidd: Couldn't listen on socket: %d", errno);
    goto error;
  }

  for (;;) {
    int server_sock_len = sizeof(server);
    sock_msg = accept(dom_sock, (struct sockaddr *) &server, (socklen_t *)&server_sock_len);
    if (sock_msg < 0) {
      syslog(LOG_ERR, "clacksidd: Couldn't accept incoming connection: %d", errno);
      goto error;
    }
    handle_connection(sock_msg);
    close(sock_msg);
  }

error:
  close(dom_sock);
  unlink(sock_path);
  syslog(LOG_ERR, "clacksidd: socket closed, file unlinked");
  return -1;

}

