#ifndef CLACKS_H
#define CLACKS_H

/*
 * Get a fresh new id to track activities.
 * Params:
 *   id: a char array (min 37 bytes) that will be assigned with the id
 * Return:
 *   0: on success
 *   -1: on error
 */
int clacks_new_id(char *id);

void clacks_trace_string(char *str);

#endif
