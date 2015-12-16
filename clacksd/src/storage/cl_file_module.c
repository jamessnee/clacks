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

#include "cl_file_module.h"

#define OUTPUT_FN "/var/log/clacks.txt"

static int out_file = -1;

int _store_trace_message(TraceMessage *msg) {
  char *to_write;
  int rtn;

  // Construct the output
  rtn = asprintf(&to_write, "%s||%s\n", msg->act_id, msg->msg);
  if (rtn < 0)
    return rtn;

  if (out_file == -1) {
    // Init the outfile
    out_file = open(OUTPUT_FN, O_CREAT | O_WRONLY | O_APPEND);
    if (out_file == -1)
      return -1;
  }

  if (write(out_file, to_write, strlen(to_write)) < 0){
    return -1;
  }

  return 0;
}

int store_trace_message(TraceMessage *msg) {
  if (msg) {
    return  _store_trace_message(msg);
  }
  return -1;
}
