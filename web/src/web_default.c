#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdio.h>

#include "cgi.h"
#include "http/cgi_http_parser.h"
#include "utils/cgi_url_dltrie.h"

#include "web.h"

void do_response(cgi_http_connection_t *connection)
{
    char buffer[CGI_NAME_BUFFER_SIZE];
    snprintf(buffer, CGI_NAME_BUFFER_SIZE,
             "%spublic%s", CGI_WEB_ROOT, connection->url);
    struct stat st;
    if (stat(buffer, &st) != -1 && S_ISREG(st.st_mode)) {
        snprintf(buffer, CGI_NAME_BUFFER_SIZE, "public%s", connection->url);
        cgi_http_write_request_line(connection, OK);
        cgi_http_write_file(connection, buffer);
    } else {
        cgi_url_dltrie_t *url_dltrie = cgi_url_dltrie_default_root();
        cgi_url_dltrie_find(url_dltrie, "/error.html")(connection);
    }
}
