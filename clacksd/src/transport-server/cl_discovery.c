#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "cl_discovery.h"

#define CL_DISCOVER_PORT 12344
#define CL_DISCOVER_BUFF_S 2048

int wait_for_transport(struct CL_Discovery_Transport *disc_transport) {
  struct sockaddr_in localaddr;
  struct sockaddr_in remoteaddr;
  socklen_t addrlen = sizeof(remoteaddr);
  int recvlen;
  int fd;
  unsigned char buf[CL_DISCOVER_BUFF_S];

  if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    syslog(LOG_ERR, "can't open UDP socket for discovery");
    return 1;
  }

  memset((char *)&localaddr, 0, sizeof(localaddr));
  localaddr.sin_family = AF_INET;
  localaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  localaddr.sin_port = htons(CL_DISCOVER_PORT);

  if (bind(fd, (struct sockaddr *)&localaddr, sizeof(localaddr)) < 0) {
    syslog(LOG_ERR, "can't bind discovery UDP socket");
    return 1;
  }

  recvlen = recvfrom(fd, buf, CL_DISCOVER_BUFF_S, 0, (struct sockaddr *)&remoteaddr, &addrlen);
  printf("Discovery: Recieved %d bytes\n", recvlen);
  close(fd);
  return 0;
}
