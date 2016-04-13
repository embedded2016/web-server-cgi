#ifndef CGI_HTTP_PARSER_H
#define CGI_HTTP_PARSER_H

#include <sys/types.h>
#include <sys/socket.h>

#include "cgi.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern cgi_http_connection_t *cgi_http_connection_create();
extern void cgi_http_connection_init(cgi_http_connection_t *connection);
extern void cgi_http_connection_init5(cgi_http_connection_t *connection,
                                      cgi_event_dispatcher_t *dispatcher,int sockfd,
                                      struct sockaddr *clientaddr,socklen_t clientlen);
extern void cgi_http_connection_read(cgi_http_connection_t *connection);
extern void cgi_http_connection_write(cgi_http_connection_t *connection,
                                      cgi_event_dispatcher_t *dispatcher);
extern LINE_STATUS cgi_http_parse_line(cgi_http_connection_t *connection);
extern HTTP_STATUS cgi_http_parse_request_line(cgi_http_connection_t *connection);
extern HTTP_STATUS cgi_http_parse_header(cgi_http_connection_t *connection);
extern HTTP_STATUS cgi_http_parse_content(cgi_http_connection_t *connection);
extern HTTP_STATUS cgi_http_process_read(cgi_http_connection_t *connection);
extern HTTP_STATUS cgi_http_process_write(cgi_http_connection_t *connection);
extern void cgi_http_process(cgi_http_connection_t *connection);
extern void cgi_http_write_request_line(cgi_http_connection_t *connection,
                                        HTTP_STATUS hstatus);
extern void cgi_http_parse_param(cgi_http_connection_t *connection);
extern void cgi_http_connection_destroy(cgi_http_connection_t *connection);

#ifdef __cplusplus
}
#endif

#endif
