#ifndef CLACKS_COMMON_H
#define CLACKS_COMMON_H

typedef enum {
  cl_socket,
  cl_domain_socket
} CL_Transport_Method;

struct CL_Discovery_Transport {
  char *client_name;
  CL_Transport_Method transport_method;
};

#endif
