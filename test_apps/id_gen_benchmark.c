#include "clacks.h"
#include <stdio.h>
#include <sys/time.h>
#include <inttypes.h>
#include <math.h>

int main (int argc, char *argv[]) {

  char id[1024];
  int ret, i;
  long long ms_pre, ms_post, time_total;
  struct timeval tv;

  gettimeofday(&tv, NULL);
  ms_pre = tv.tv_sec * 1000LL + tv.tv_usec / 1000;

  // Generate 100k UUIDs
  for (i = 0; i < 100000; i++) {
    ret = clacks_new_id(id);
    //fprintf(stdout, "Ret: %d ID: %s\n", ret, id);
  }

  gettimeofday(&tv, NULL);
  ms_post = tv.tv_sec * 1000LL + tv.tv_usec / 1000;
  time_total = ms_post - ms_pre;
  fprintf(stdout, "Time: %lld\n", time_total);

  return 0;
}
