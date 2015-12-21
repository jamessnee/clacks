#define _GNU_SOURCE
#include "clacks_common.h"
#include "../transport-client/cl_transport_domsock_client.h"
#include "id_client.h"
#include "TraceMessage.pb-c.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>

#include "clacks.h"

/* Helpers */
static inline long long read_time(void) {
  long long us;
  struct timespec time;
  clock_gettime(CLOCK_MONOTONIC, &time);
  us = (time.tv_sec * 1000000) + (time.tv_nsec / 1000);
  return us;
}

/* API Back-End */
void _clacks_trace_id_string(char *id, char *str, va_list args) {
  int ret;
  char *msg;

  // Prepare the message
  ret = vasprintf(&msg, str, args);
  if (ret >= 0) {
    TraceMessage t_msg = TRACE_MESSAGE__INIT;
    t_msg.act_id = id;
    t_msg.msg = msg;
    t_msg.ts_origin = read_time();
    t_msg.flags = 0;

    ret = send_trace_message(&t_msg);
  }
  free(msg);
}

void _clacks_join_id(char *id, char *j_id) {
  int ret;

  // Prepare the Join
  TraceMessage t_msg = TRACE_MESSAGE__INIT;
  t_msg.act_id = id;
  t_msg.join_id = j_id;
  t_msg.ts_origin = read_time();
  t_msg.flags = 0;

  ret = send_trace_message(&t_msg);
}

/* API Front-End */
void clacks_trace_string(char *str, ...) {
  if (str != NULL) {
    va_list args;
    va_start(args, str);
    _clacks_trace_id_string("NULLID", str, args);
    va_end(args);
  }
}

void clacks_trace_id_string(char *id, char *str, ...) {
  if (str != NULL && id != NULL) {
    va_list args;
    va_start(args, str);
    _clacks_trace_id_string(id, str, args);
    va_end(args);
  }
}

int clacks_new_id(char *id) {
  if (id == NULL) {
    return -1;
  }
  return get_new_id(id);
}

void clacks_tag_id(char *cl_id, char *derrived) {
  cl_id_tag_id(cl_id, derrived);
}

void clacks_join_id(char *id, char *j_id) {
  if (id && j_id) {
    _clacks_join_id(id, j_id);
  }
}
