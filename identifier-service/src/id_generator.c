#include "id_generator.h"

static int uuid_list_hd = 0;
static uuid_t uuid_list[CL_MAX_FREE_UUIDS];

void update_uuid_pool(void) {
  int i;
  if (uuid_list_hd < 0) uuid_list_hd = 0;
  for (i = uuid_list_hd; i < CL_MAX_FREE_UUIDS; i++) {
    uuid_generate_time_safe(uuid_list[i]);
  }
  uuid_list_hd = CL_MAX_FREE_UUIDS;
}

void next_uuid(uuid_t uuid) {
  if (uuid_list_hd == -1) {
    update_uuid_pool();
  }

  uuid = uuid_list[uuid_list_hd];
  uuid_list_hd--;
}
