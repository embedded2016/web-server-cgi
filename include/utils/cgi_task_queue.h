#ifndef CGI_TASK_QUEUE_H
#define CGI_TASK_QUEUE_H

#include "cgi.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern cgi_task_queue_t *cgi_task_queue_create(void *(*callback)(void *),
                                               void *arg);
extern cgi_task_queue_t *cgi_task_queue_take(cgi_thread_pool_t *pool);
extern void cgi_task_queue_put(cgi_thread_pool_t *pool,
                               void *(*callback)(void *), void *arg);
extern void cgi_task_queue_push(cgi_task_queue_t **head_ptr,
                                void *(*callback)(void *), void *arg);
extern cgi_task_queue_t *cgi_task_queue_pop(cgi_task_queue_t **head_ptr);
extern void cgi_task_queue_delete(cgi_task_queue_t *node);
extern void cgi_task_queue_destroy(cgi_task_queue_t **head_ptr);

#ifdef __cplusplus
}
#endif

#endif
