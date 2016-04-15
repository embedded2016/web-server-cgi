#include <stdio.h>

#include "cgi.h"
#include "http/cgi_http_parser.h"
#include "utils/cgi_param_slist.h"

#include "web.h"

void do_response(cgi_http_connection_t *connection)
{
    while (!cgi_pslist_is_empty(connection->head)) {
        cgi_pslist_t *head = connection->head;
        printf("%s: %s\n", head->key, head->value);
        cgi_pslist_remove_head(&connection->head);
    }
    cgi_http_write_request_line(connection, OK);
}
