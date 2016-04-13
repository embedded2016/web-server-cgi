#ifndef CGI_SERVER_H
#define CGI_SERVER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

extern void listen(char *ip, uint16_t port);

#ifdef __cplusplus
}
#endif

#endif
