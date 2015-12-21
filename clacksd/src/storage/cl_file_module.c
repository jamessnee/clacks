#define _GNU_SOURCE
#include <syslog.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>

#include "cl_file_module.h"

#define OUTPUT_FN "/var/log/clacks.txt"

static int out_file = -1;

/* Helpers */
static inline long long get_local_time_ms(void) {
  long long us;
  struct timespec time;
  clock_gettime(CLOCK_MONOTONIC, &time);
  // Convert time to ms
  us = (time.tv_sec * 1000000) + (time.tv_nsec / 1000);
  return us;
}

static inline int open_out_file(void) {
  out_file = open(OUTPUT_FN, O_CREAT | O_WRONLY | O_APPEND);
  if (out_file == -1)
    return -1;
  return 0;
}

/* API-backend */
int _store_trace_message(TraceMessage *msg) {
  char *to_write;
  int rtn;
  long long us = get_local_time_ms();

  // Construct the output
  if (msg->join_id) {
    rtn = asprintf(&to_write, "J:%lld||%lu||%s||%s\n", us, msg->ts_origin, msg->act_id, msg->join_id);
  } else {
    rtn = asprintf(&to_write, "T:%lld||%lu||%s||%s\n", us, msg->ts_origin, msg->act_id, msg->msg);
  }
  if (rtn < 0)
    return rtn;

  if (out_file == -1) {
    if (open_out_file() == -1) {
      return -1;
    }
  }

  if (write(out_file, to_write, strlen(to_write)) < 0){
    return -1;
  }

  return 0;
}

/* API-frontend */
int store_trace_message(TraceMessage *msg) {
  if (msg) {
    return  _store_trace_message(msg);
  }
  return -1;
}

int store_join_message(TraceMessage *msg) {
  if (msg) {
    return _store_trace_message(msg);
  }
  return -1;
}
