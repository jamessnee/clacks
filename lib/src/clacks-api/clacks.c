#include "clacks_common.h"
#include "id_client.h"
#include "TraceMessage.pb-c.h"
#include "../transport-client/cl_transport_domsock_client.h"
#include <stdio.h>

#include "clacks.h"

/* API Back-End */
void _clacks_trace_string_id(char *str, char *id) {
  int ret;

  TraceMessage t_msg = TRACE_MESSAGE__INIT;
  t_msg.act_id = id;
  t_msg.msg = str;
  t_msg.flags = 0;

  ret = send_trace_message(&t_msg);
}

/* API Front-End */
void clacks_trace_string(char *str) {
  clacks_trace_string_id(str, "NULLID");
}

void clacks_trace_string_id(char *str, char *id) {
  if (str != NULL && id != NULL) {
    _clacks_trace_string_id(str, id);
  }
}

int clacks_new_id(char *id) {
  if (id == NULL) {
    return -1;
  }
  return get_new_id(id);
}

