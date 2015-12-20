#include "id_generator.h"

static int uuid_list_hd = 0;
static uuid_t uuid_list[CL_MAX_FREE_UUIDS];

void update_uuid_pool(void) {
  int i;
  char tmp_uuid[37];

  if (uuid_list_hd < 0) uuid_list_hd = 0;
  for (i = uuid_list_hd; i < CL_MAX_FREE_UUIDS; i++) {
    uuid_generate_time_safe(uuid_list[i]);
  }
  uuid_list_hd = CL_MAX_FREE_UUIDS - 1;
}

void next_uuid(char *out) {
  uuid_unparse(uuid_list[uuid_list_hd], out);
  uuid_list_hd--;
}

void check_pool(void) {
  if (uuid_list_hd == -1) {
    update_uuid_pool();
  }
}
