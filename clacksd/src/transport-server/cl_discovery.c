#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "clacks_common.h"

#include "cl_discovery.h"

#define CL_DISCOVER_PORT 12344
#define CL_DISCOVER_BUFF_S sizeof(struct CL_Discovery_Transport)

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

  syslog(LOG_INFO, "Discovery socket bound. Waiting for messages");
  recvlen = recvfrom(fd, buf, CL_DISCOVER_BUFF_S, 0, (struct sockaddr *)&remoteaddr, &addrlen);

  // Check we received a whole discovery message
  if (recvlen != CL_DISCOVER_BUFF_S) {
    syslog(LOG_ERR, "Received a discovery message that was of the wrong size. Dropping");
    return 1;
  }

  close(fd);
  return 0;
}
