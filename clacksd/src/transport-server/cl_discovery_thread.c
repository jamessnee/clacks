#include <syslog.h>
#include "cl_discovery.h"

#include "cl_discovery_thread.h"

void * start_discovery(void * args) {
  syslog(LOG_INFO, "started discovery thread");
  for (;;) {
    continue;
  }
}
