EXECPATH = bin
OBJPATH = obj
WEBPATH = web/plugins
CC ?= gcc

CFLAGS = -std=gnu99 -Wall -O2 -g -I include
LDFLAGS = -lpthread -ldl

EXEC = \
	cgi_slist_test \
	cgi_param_slist_test \
	cgi_http_parser_test \
	cgi_url_dltrie_test \
	cgi_event_dispatcher_test \
	cgi_thread_pool_test \
	cgi_task_queue_test \
	cgi_server_test
EXEC := $(addprefix $(EXECPATH)/,$(EXEC))

OBJS = \
	cgi_factory.o \
	cgi_http_parser.o \
	cgi_param_slist.o \
	cgi_url_dltrie.o \
	cgi_event_dispatcher.o \
	cgi_task_queue.o \
	cgi_thread_pool.o
OBJS := $(addprefix $(OBJPATH)/,$(OBJS))

TESTOBJS = \
	cgi_slist_test.o \
	cgi_param_slist_test.o \
	cgi_http_parser_test.o \
	cgi_url_dltrie_test.o \
	cgi_event_dispatcher_test.o \
	cgi_thread_pool_test.o \
	cgi_task_queue_test.o
TESTOBJS := $(addprefix $(OBJPATH)/,$(TESTOBJS))

PLUGINS = \
	default \
	index \
	signup \
	verify_signup \
	error \
	signin \
	verify_signin
PLUGINS := $(addprefix $(WEBPATH)/web_,$(PLUGINS))
PLUGINS := $(addsuffix .so,$(PLUGINS))

all: $(OBJPATH) $(OBJS) $(TESTOBJS) \
     $(EXECPATH) $(EXEC) \
     $(WEBPATH) $(PLUGINS)

$(EXECPATH)/%_test: $(OBJS) $(OBJPATH)/%_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(EXECPATH):
	@mkdir -p $@

$(OBJPATH)/%.o: test/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJPATH)/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJPATH):
	@mkdir -p $@

$(WEBPATH)/%.so: web/src/%.c
	$(CC) $(CFLAGS) -I web/include -fPIC -shared -nostartfiles \
		$< -o $@ \
		src/cgi_factory.c src/cgi_http_parser.c \
		src/cgi_param_slist.c

$(WEBPATH):
	@mkdir -p $@

clean:
	-rm -rf $(EXEC) $(OBJS) $(TESTOBJS) $(PLUGINS)
distclean: clean
	-rm -rf $(EXECPATH) $(OBJPATH) $(WEBPATH)
