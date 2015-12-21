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
int clacks_new_id(char *);

void clacks_trace_string(char *, ...);
void clacks_trace_id_string(char *, char *, ...);

void clacks_tag_id(char *id, char *derrived);
void clacks_join_id(char *id, char *j_id);

#endif
