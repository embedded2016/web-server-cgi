#ifndef CGI_THREAD_POOL_H
#define CGI_THREAD_POOL_H

#include <stdint.h>

#include "cgi.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern cgi_thread_pool_t *cgi_thread_pool_create();
extern void cgi_thread_pool_start(cgi_thread_pool_t *pool);
extern void cgi_thread_pool_execute(cgi_thread_pool_t *pool,
                                    void *(*callback)(void *), void *arg);
extern void cgi_thread_pool_stop(cgi_thread_pool_t *pool);
extern void cgi_thread_pool_destroy(cgi_thread_pool_t *pool);

#ifdef __cplusplus
}
#endif

#endif
