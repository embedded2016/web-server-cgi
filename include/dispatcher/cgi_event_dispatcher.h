#ifndef CGI_EVENT_DISPATCHER_H
#define CGI_EVENT_DISPATCHER_H

#include <stdint.h>

#include "cgi.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern cgi_event_dispatcher_t *cgi_event_dispatcher_create();
extern void cgi_event_dispatcher_init(cgi_event_dispatcher_t *dispatcher,
                                      int epfd,int listenfd,int timeout);
extern void cgi_event_dispatcher_addsig(int sig);
extern void cgi_event_dispatcher_addpipe(cgi_event_dispatcher_t *dispatcher);
extern void cgi_event_dispatcher_addfd(cgi_event_dispatcher_t *dispatcher,
                                       int fd, int in, int oneshot);
extern void cgi_event_dispatcher_rmfd(cgi_event_dispatcher_t *dispatcher,
                                      int fd);
extern void cgi_event_dispatcher_modfd(cgi_event_dispatcher_t *dispatcher,
                                       int fd,int ev);
extern void cgi_event_dispatcher_set_nonblocking(int fd);
extern void cgi_event_dispatcher_loop(cgi_event_dispatcher_t *dispatcher);
extern void cgi_event_dispatcher_destroy(cgi_event_dispatcher_t *dispatcher);

#ifdef __cplusplus
}
#endif

#endif
