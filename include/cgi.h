#ifndef CGI_H
#define CGI_H

#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <semaphore.h>

#include <stdint.h>

#include "utils/cgi_slist.h"
#include "utils/cgi_dltrie.h"

#define CGI_HTTP_CONNECTION_READ_BUFFER_SIZE 4096
#define CGI_HTTP_CONNECTION_WRITE_BUFFER_SIZE 4096
#define CGI_URL_DLTRIE_KEY_SIZE 32
#define CGI_CONNECTION_SIZE 1024
#define CGI_EVENT_SIZE	1024
#define CGI_THREAD_POOL_SIZE 8
#define CGI_NAME_BUFFER_SIZE 128
#define CGI_FILE_BUFFER_SIZE 4096

#define CGI_WEB_ROOT            "../web/"
#define CGI_WEB_DLPATH          "../web/lib/"
#define CGI_WEB_PUBLIC_PATH     "../web/public/"

typedef enum CGI_OBJECT CGI_OBJECT;
typedef enum LINE_STATUS LINE_STATUS;
typedef enum CHECK_STATUS CHECK_STATUS;
typedef enum HTTP_STATUS HTTP_STATUS;
typedef enum HTTP_METHOD HTTP_METHOD;

typedef struct cgi_http_connection cgi_http_connection_t;
typedef struct cgi_param_slist cgi_pslist_t;
typedef struct cgi_url_dltrie cgi_url_dltrie_t;
typedef struct cgi_task_queue cgi_task_queue_t;
typedef struct cgi_thread_pool cgi_thread_pool_t;
typedef struct cgi_event_dispatcher cgi_event_dispatcher_t;
typedef struct cgi_template_engine cgi_template_engine_t;

typedef void (*cgi_handler_t)(cgi_http_connection_t*);

enum CGI_OBJECT {
    HTTP_CONNECTION,
    PARAM_SLIST,
    URL_DLTRIE,
    EVENT_DISPATCHER,
    TASK_QUEUE,
    THREAD_POOL
};

enum LINE_STATUS {
    LINE_OPEN,
    LINE_OK,
    LINE_BAD
};

enum CHECK_STATUS {
    CHECK_REQUEST_LINE,
    CHECK_HEADER,
    CHECK_CONTENT
};

enum HTTP_STATUS {
    CHECKING,
    OK,
    FOUND,
    NOT_MODIFIED,
    BAD_REQUEST,
    FORBIDDEN,
    NOT_FOUND,
    INTERNAL_SERVER_ERROR,
    HTTP_VERSION_NOT_SUPPORTED
};

enum HTTP_METHOD {
    GET,
    POST,
    HEAD,
    PUT,
    DELETE,
    TRACE,
    OPTIONS,
    CONNECT,
    PATCH
};

struct cgi_http_connection {
    cgi_event_dispatcher_t *dispatcher;
    char *rbuffer;
    char *wbuffer;
    char *version;
    char *url;
    char *cookie;
    char *content;
    cgi_pslist_t *head;
    uint32_t rsize;
    uint32_t wsize;
    uint32_t read_idx;
    uint32_t write_idx;
    uint32_t checked_idx;
    uint32_t start_line_idx;
    uint32_t content_length;
    int linger;
    HTTP_METHOD method;
    CHECK_STATUS cstatus;
    uint64_t fsize;
    int ffd;
    int sockfd;
    struct sockaddr clientaddr;
    socklen_t clientlen;
};

struct cgi_param_slist {
    char *key;
    char *value;
    CGI_SLIST_ENTRY(cgi_pslist_t) linker;
};

struct cgi_url_dltrie {
    char *key;
    cgi_handler_t handler;
    void *dlhandle;
    uint32_t ksize;
    CGI_DLTRIE_ENTRY(cgi_url_dltrie_t) linker;
};

struct cgi_task_queue {
    void *(*callback)(void *);
    void *arg;
    CGI_SLIST_ENTRY(cgi_task_queue_t) linker;
};

struct cgi_thread_pool {
    sem_t semaphore;
    pthread_mutex_t mutex;
    pthread_t *tids;
    cgi_task_queue_t *head;
    uint32_t size;
    int sflag;
};

struct cgi_event_dispatcher {
    cgi_http_connection_t *connections;
    cgi_thread_pool_t *pool;
    struct epoll_event *events;
    uint32_t csize;
    uint32_t evsize;
    int epfd;
    int listenfd;
    int timeout;
};

struct cgi_template_engine {
    char *rbuffer;
    char *wbuffer;
    uint32_t rsize;
    uint32_t wsize;
    uint32_t rd_idx;
    uint32_t wr_idx;
};

#endif
