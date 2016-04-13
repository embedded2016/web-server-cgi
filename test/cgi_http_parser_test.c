#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgi.h"
#include "http/cgi_http_parser.h"

static void print_param_slist(cgi_pslist_t *head)
{
    cgi_pslist_t *var = NULL;
    CGI_SLIST_FOREACH(var,head,linker) {
        printf("key:\t%s\t\tvalue:\t%s\n",var->key,var->value);
    }
}

static char* strmethod(HTTP_METHOD method)
{
    char *mstr = NULL;
    switch(method) {
        case GET:
            mstr = "GET";
            break;

        case POST:
            mstr = "POST";
            break;

        default:
            break;
    }
    return mstr;
}

int main(int argc,char **argv)
{
    if(argc < 2) {
        fprintf(stderr,"No enough parameter.\n");
        exit(EXIT_FAILURE);
    }

    cgi_http_connection_t *connection = cgi_http_connection_create();
    char buffer[1024];

    char *rbuffer = connection->rbuffer;
    uint32_t rdsize;
    FILE *fp = fopen(argv[1],"r");
    while(fgets(buffer,1024,fp)) {
        uint32_t rdsize = strlen(buffer) - 1;
        memcpy(rbuffer + connection->read_idx,buffer,rdsize);
        connection->read_idx += rdsize;
        rbuffer[connection->read_idx++] = '\r';
        rbuffer[connection->read_idx++] = '\n';
    }

    LINE_STATUS lstatus = cgi_http_parse_line(connection);
    HTTP_STATUS hstatus = cgi_http_parse_request_line(connection);

    switch(hstatus) {
        case CHECKING:
            printf("%s\n",strmethod(connection->method));
            printf("%s\n",connection->url);
            printf("%s\n",connection->version);
            break;

        case BAD_REQUEST:
            printf("Bad Request\n");
            break;

        case HTTP_VERSION_NOT_SUPPORTED:
            printf("HTTP Version Not Supported\n");
            break;

        default:
            break;
    }

    while(connection->cstatus == CHECK_HEADER) {
        lstatus = cgi_http_parse_line(connection);
        printf("Checking Header.\n");
        hstatus = cgi_http_parse_header(connection);
        if(hstatus != CHECKING) {
            printf("Header error\n");
        }
    }

    printf("Content-Length:\t%d\n",connection->content_length);
    printf("Cookie:\t%s\n",connection->cookie);

    if(connection->cstatus == CHECK_CONTENT) {
        printf("Check content.\n");
    } else {
        printf("Request Error");
    }

    hstatus = cgi_http_parse_content(connection);
    printf("%s\n",connection->content);

    cgi_http_parse_param(connection);
    print_param_slist(connection->head);
    cgi_http_connection_destroy(connection);
    printf("\n");

    connection = cgi_http_connection_create();
    if(fseek(fp,SEEK_SET,0) == -1) {
        perror("fseek");
    }

    while(fgets(buffer,1024,fp)) {
        uint32_t rdsize = strlen(buffer) - 1;
        memcpy(rbuffer + connection->read_idx,buffer,rdsize);
        connection->read_idx += rdsize;
        rbuffer[connection->read_idx++] = '\r';
        rbuffer[connection->read_idx++] = '\n';
    }

    hstatus = cgi_http_process_read(connection);
    printf("%s\n",strmethod(connection->method));
    printf("%s\n",connection->url);
    printf("%s\n",connection->version);
    printf("%d\n",connection->content_length);
    printf("%s\n",connection->cookie);
    printf("%s\n",connection->content);
    hstatus = cgi_http_parse_content(connection);
    cgi_http_parse_param(connection);
    print_param_slist(connection->head);

    fclose(fp);
    cgi_http_connection_destroy(connection);
    return 0;
}
