#ifndef CLACKS_COMMON_H
#define CLACKS_COMMON_H

#define CL_ID_DOM_SOCKET "/var/run/clacksid.sock"
#define CL_ID_GET "GET"
#define CL_ID_TAG "TAG"

typedef enum {
  cl_socket,
  cl_domain_socket
} CL_Transport_Method;

struct CL_Discovery_Transport {
  char *client_name;
  CL_Transport_Method transport_method;
};

#endif
