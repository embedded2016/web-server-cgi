#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <string.h>

#include "cgi.h"
#include "http/cgi_http_parser.h"
#include "web/cgi_template_engine.h"
#include "utils/cgi_param_slist.h"
#include "utils/cgi_url_dltrie.h"

static int cgi_template_engine_file_exists(char *path);
static void cgi_template_engine_instantiate(cgi_http_connection_t *connection);

void cgi_template_engine_render(cgi_http_connection_t *connection, char *path)
{
    if (!cgi_template_engine_file_exists(path)) {
        cgi_http_write_request_line(connection, INTERNAL_SERVER_ERROR);
        cgi_http_write_header(connection, "Content-Type", 12);
        cgi_http_write_content(connection, "<h1>500</h1>");
        return;
    }
    // TODO
}

int cgi_template_engine_file_exists(char *path)
{
    struct stat st;
    return stat(path, &st) != -1 && S_ISREG(st.st_mode);
}

void cgi_template_engine_instantiate(cgi_http_connection_t *connection)
{
    // TODO
}
