#include "clacks.h"
#include <stdio.h>

int main (int argc, char *argv[]) {

  char id[1024];
  int ret;

  // Test whether we can get an ID
  ret = clacks_new_id(id);
  if (ret == -1) {
    fprintf(stdout, "Couldn't get a new ID\n");
    return -1;
  } else {
    fprintf(stdout, "ID: %s\n", id);
  }

  return 0;
}
