#ifndef CL_FILE_MODULE_H
#define CL_FILE_MODULE_H

#include "TraceMessage.pb-c.h"

/* Store a trace message in the Redis backend
 * Params:
 *   id: The unique ID for the message
 *   message: The message itself
 * Return:
 *   0 - On sucess
 *   -1 - On error
 */
int store_trace_message(TraceMessage *msg);
int store_join_message(TraceMessage *msg);

#endif
