#ifndef CL_REDIS_MODULE_H
#define CL_REDIS_MODULE_H

/* Store a trace message in the Redis backend
 * Params:
 *   id: The unique ID for the message
 *   message: The message itself
 * Return:
 *   0 - On sucess
 *   -1 - On error
 */
int store_trace(char *id, char *message);

#endif
