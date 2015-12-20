#include <string.h>
#include <stdlib.h>
#include <syslog.h>
#include "uthash.h"

#include "id_tag_manager.h"

struct tagged_id {
  char derrived_id[128];
  char cl_id[37];
  UT_hash_handle hh;
};

static struct tagged_id *all_tagged_ids = NULL;

int tag_id(char *cl_id, char *derrived_id) {
  struct tagged_id *curr_tag = NULL;

  syslog(LOG_INFO, "Tagging %s with %s", cl_id, derrived_id);

  HASH_FIND_STR(all_tagged_ids, derrived_id, curr_tag);
  if (curr_tag == NULL) { // It's not in the hash table
    curr_tag = (struct tagged_id *) malloc(sizeof(struct tagged_id));
    strncpy(curr_tag->derrived_id, derrived_id, 128);
    strncpy(curr_tag->cl_id, cl_id, 37);
    HASH_ADD_STR(all_tagged_ids, derrived_id, curr_tag);
    return 0;
  }
  return -1;
}

int get_id(char *derrived_id, char *tagged_id) {
  return -1;
}
