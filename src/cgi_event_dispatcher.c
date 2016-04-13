#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "cgi.h"
#include "factory/cgi_factory.h"
#include "http/cgi_http_parser.h"
#include "pool/cgi_thread_pool.h"
#include "dispatcher/cgi_event_dispatcher.h"

static pthread_mutex_t mlock = PTHREAD_MUTEX_INITIALIZER;
static int usable = 0;
static int pipefd[2];

static void cgi_event_dispatcher_signal_handler(int sig);

cgi_event_dispatcher_t *cgi_event_dispatcher_create()
{
    cgi_event_dispatcher_t *dispatcher = cgi_factory_create(EVENT_DISPATCHER);
    dispatcher->timeout = -1;
    dispatcher->pool = cgi_thread_pool_create();
    return dispatcher;
}

void cgi_event_dispatcher_init(cgi_event_dispatcher_t *dispatcher,
                               int epfd, int listenfd, int timeout)
{
    dispatcher->epfd = epfd;
    dispatcher->listenfd = listenfd;
    dispatcher->timeout = timeout;
}

void cgi_event_dispatcher_signal_handler(int sig)
{
    int eno = errno;
    char msg = sig;
    send(pipefd[1],&msg,1,0);
    errno = eno;
}

void cgi_event_dispatcher_addpipe(cgi_event_dispatcher_t *dispatcher)
{
    if (!usable) {
        pthread_mutex_lock(&mlock);
        if (!usable) {
            usable = 1;
            pthread_mutex_unlock(&mlock);
            if(socketpair(AF_UNIX,SOCK_STREAM,0,pipefd) != -1) {
                cgi_event_dispatcher_set_nonblocking(pipefd[1]);
                cgi_event_dispatcher_addfd(dispatcher,pipefd[0],1,0);
            }
        } else {
            pthread_mutex_unlock(&mlock);
        }
    }
}

void cgi_event_dispatcher_addsig(int sig)
{
    signal(sig,cgi_event_dispatcher_signal_handler);
}

void cgi_event_dispatcher_addfd(cgi_event_dispatcher_t *dispatcher,
                                int fd, int in, int oneshot)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLET | EPOLLRDHUP;
    if (in) {
        event.events |= EPOLLIN;
    } else {
        event.events |= EPOLLOUT;
    }
    if (oneshot) {
        event.events |= EPOLLONESHOT;
    }
    if (epoll_ctl(dispatcher->epfd,EPOLL_CTL_ADD,fd,&event) == -1) {
        perror("epoll_ctl");
    }
    cgi_event_dispatcher_set_nonblocking(fd);
}

void cgi_event_dispatcher_rmfd(cgi_event_dispatcher_t *dispatcher,
                               int fd)
{
    if (epoll_ctl(dispatcher->epfd, EPOLL_CTL_DEL, fd, NULL) == -1) {
        perror("epoll_ctl");
    }
}

void cgi_event_dispatcher_modfd(cgi_event_dispatcher_t *dispatcher,
                                int fd, int ev)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = ev | EPOLLET | EPOLLRDHUP | EPOLLONESHOT;
    epoll_ctl(dispatcher->epfd,EPOLL_CTL_MOD,fd,&event);
}

void cgi_event_dispatcher_set_nonblocking(int fd)
{
    int fsflags = fcntl(fd,F_GETFL);
    fsflags |= O_NONBLOCK;
    if (fcntl(fd,F_SETFL,fsflags) == -1) {
        perror("fcntl");
    }
}

void cgi_event_dispatcher_loop(cgi_event_dispatcher_t *dispatcher)
{
    int stop = 0;
    int nfds;
    int tmpfd;
    int cfd;
    int retcode;
    struct epoll_event event;
    struct sockaddr clientaddr;
    socklen_t clientlen;
    char signum;

    cgi_thread_pool_start(dispatcher->pool);

    while (!stop) {
        nfds = epoll_wait(dispatcher->epfd, dispatcher->events,
                          dispatcher->evsize, dispatcher->timeout);
        for (int i = 0; i < nfds; ++i) {
            event = dispatcher->events[i];
            tmpfd = event.data.fd;
            if (tmpfd == dispatcher->listenfd) {
                cfd = accept(tmpfd, &clientaddr, &clientlen);
                if (cfd == -1) {
                    perror("cfd");
                }
                cgi_event_dispatcher_addfd(dispatcher, cfd, 1, 1);
                cgi_http_connection_init5(dispatcher->connections + cfd,
                                          dispatcher,
                                          cfd, &clientaddr, clientlen);
            } else if (event.events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
                cgi_event_dispatcher_rmfd(dispatcher,tmpfd);
            } else if (event.events & EPOLLIN) {
                if (tmpfd == pipefd[0]) {
                    retcode = recv(pipefd[0], &signum, sizeof(signum),0);
                    if (retcode <= 0) {
                        continue;
                    }
                    switch (signum) {
                        case SIGCHLD:
                        case SIGHUP:
                            break;

                        case SIGTERM:
                        case SIGINT:
                            stop = 1;
                            break;

                        default:
                            break;
                    }
                    continue;
                }
                cgi_http_connection_read(dispatcher->connections + tmpfd);
                cgi_thread_pool_execute(dispatcher->pool, cgi_http_process,
                                        dispatcher->connections + tmpfd);
            } else if (event.events & EPOLLOUT) {
                cgi_http_connection_write(dispatcher->connections + tmpfd,
                                          dispatcher);
            } else {
                printf("Error!");
            }
        }
    }
    cgi_thread_pool_stop(dispatcher->pool);
}

void cgi_event_dispatcher_destroy(cgi_event_dispatcher_t *dispatcher)
{
    cgi_factory_destroy(dispatcher, EVENT_DISPATCHER);
}
