#include "clacks.h"
#include <stdio.h>

int main (int argc, char *argv[]) {

  char id[37];
  int ret;
  char j_id[37];

  // Test whether we can get an ID
  ret = clacks_new_id(id);
  if (ret == -1) {
    fprintf(stdout, "Couldn't get a new ID\n");
    return -1;
  } else {
    fprintf(stdout, "ID: %s\n", id);
  }

  // Test whether we can tag an ID
  clacks_tag_id(id, "TEST");

  // Test whether we can send a trace message
  fprintf(stdout, "Testing tracing\n");
  clacks_trace_string("DIFFERENT STRING, %d", 100);


  // Test the JOIN message
  fprintf(stdout, "Testing Joining\n");
  ret = clacks_new_id(j_id);
  if (ret == -1) {
    fprintf(stdout, "Error getting a new ID\n");
    return -1;
  } else {
    clacks_join_id(id, j_id);
  }

  return 0;
}
