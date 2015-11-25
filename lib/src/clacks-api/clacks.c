#include "clacks_common.h"
#include "id_client.h"
#include "TraceMessage.pb-c.h"
#include "../transport-client/cl_transport_domsock_client.h"
#include <stdio.h>

#include "clacks.h"

void clacks_trace_string(char *str) {
  int ret;

  TraceMessage t_msg = TRACE_MESSAGE__INIT;
  t_msg.act_id = "TEST";
  t_msg.msg = str;
  t_msg.flags = 0;

  ret = send_trace_message(&t_msg);
}

int clacks_new_id(char *id) {
  if (id == NULL) {
    return -1;
  }
  return get_new_id(id);
}

