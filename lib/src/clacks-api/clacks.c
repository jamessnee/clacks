#include "clacks_common.h"
#include "id_client.h"
#include <stdio.h>

#include "clacks.h"

void clacks_trace_string(char *str) {
  printf("Tracing string %s\n", str);
}

int clacks_new_id(char *id) {
  if (id == NULL) {
    return -1;
  }
  return get_new_id(id);
}

