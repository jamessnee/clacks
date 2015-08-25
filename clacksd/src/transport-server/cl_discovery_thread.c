#include <stdlib.h>
#include <syslog.h>
#include "cl_discovery.h"

#include "cl_discovery_thread.h"

void * start_discovery(void * args) {
  syslog(LOG_INFO, "started discovery thread");

  for (;;) {
    struct CL_Discovery_Transport *discovered_transport = malloc(sizeof(struct CL_Discovery_Transport));
    wait_for_transport(discovered_transport);
  }
}
