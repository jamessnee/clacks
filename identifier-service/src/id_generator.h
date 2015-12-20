#ifndef _CLACKS_ID_GENERATOR_H
#define _CLACKS_ID_GENERATOR_H

#include <uuid/uuid.h>

#define CL_MAX_FREE_UUIDS 128

void update_uuid_pool(void);
void next_uuid(char *);
void check_pool(void);

#endif
