#ifndef CGI_URL_DLTRIE_H
#define CGI_URL_DLTRIE_H

#include "cgi.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern cgi_url_dltrie_t *cgi_url_dltrie_create();
extern cgi_url_dltrie_t *cgi_url_dltrie_default_root();
extern void cgi_url_dltrie_init(cgi_url_dltrie_t **head_ptr);
extern void cgi_url_dltrie_insert(cgi_url_dltrie_t **head_ptr,char *url,
                                  cgi_handler_t handler,void *dlhandle);
extern cgi_handler_t cgi_url_dltrie_find(cgi_url_dltrie_t *head,char *url);
extern void cgi_url_dltrie_delete(cgi_url_dltrie_t *elem);
extern void cgi_url_dltrie_destroy(cgi_url_dltrie_t **head_ptr);

#ifdef __cplusplus
}
#endif

#endif
