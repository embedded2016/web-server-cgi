#ifndef CGI_TEMPLATE_ENGINE_H
#define CGI_TEMPLATE_ENGINE_H

#include "cgi.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern void cgi_template_engine_render(cgi_http_connection_t *connection, char *fpath);

#ifdef __cplusplus
}
#endif

#endif
