#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include "cgi.h"
#include "utils/cgi_url_dltrie.h"
#include "dispatcher/cgi_event_dispatcher.h"

int main()
{
    cgi_event_dispatcher_t *dispatcher = cgi_event_dispatcher_create();
    cgi_url_dltrie_t *url_dltrie = cgi_url_dltrie_default_root();

    int epfd = epoll_create(512);
    assert(epfd != -1);

    int listenfd = socket(AF_INET,SOCK_STREAM,0);
    assert(listenfd != -1);

    int retcode;
    int flag = 1;
    retcode = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                         &flag, sizeof(flag));
    assert(retcode != -1);

    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    retcode = inet_aton("0.0.0.0", &addr.sin_addr);
    assert(retcode != -1);

    retcode = bind(listenfd,(struct sockaddr *) &addr, sizeof(addr));
    assert(retcode != -1);

    retcode = listen(listenfd, 1024);
    assert(retcode != -1);

    cgi_event_dispatcher_init(dispatcher, epfd, listenfd, -1);
    cgi_event_dispatcher_addpipe(dispatcher);
    cgi_event_dispatcher_addfd(dispatcher, listenfd, 1, 0);

    cgi_event_dispatcher_addsig(SIGHUP);
    cgi_event_dispatcher_addsig(SIGCHLD);
    cgi_event_dispatcher_addsig(SIGTERM);
    cgi_event_dispatcher_addsig(SIGINT);

    cgi_event_dispatcher_loop(dispatcher);

    cgi_event_dispatcher_rmfd(dispatcher, listenfd);

    retcode = close(listenfd);
    assert(retcode != -1);
    retcode = close(epfd);
    assert(retcode != -1);

    cgi_event_dispatcher_destroy(dispatcher);
    return 0;
}
