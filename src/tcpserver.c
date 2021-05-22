/************************************************
 * @addtogroup CITS
 * @{
 * @file  : tcpserver.c
 * @brief : 
 * @author: wangshaobo
 * @email : wangshaobo@nebula-link.com
 * @date  : 2019-05-10 10:43:51
************************************************/

//-----------------------------------------------
// Copyright (c) CITS
//-----------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <thread.h>
#include <event.h>
#include <socket.h>
#include <reactor.h>
#include <tcpserver.h>
#include <work.h>

static reactors_t *reactors = NULL;
static int (*tcpserver_connect_cb)(int fd) = NULL;
static int (*tcpserver_recv_cb)(int fd, char *buffer, int buffer_len, void *para) = NULL;

static int recv_cb(int fd, int events, void *para)
{
	reactor_t *reactor = (reactor_t *)para;
	
	event_t *event = map_get_entry(fd, &reactor->root, event_t, node);
	if(!event)
		return -1;
	
	char *recv_buffer = NULL;
	int recv_buffer_len = socket_recv(fd, &recv_buffer);
	if(recv_buffer_len > 0)
	{
		if(tcpserver_recv_cb)
			tcpserver_recv_cb(fd, recv_buffer, recv_buffer_len, reactor);
	}
	else
	{
		reactor_delete_event(reactor, event);
		map_remove(&reactor->root, event->node.key);
		free(event);
		socket_close(fd);
	}
	
	return 1;
}

static int accpet_run(void *para)
{
	reactor_t *reactor = (reactor_t *)para;
	
	int client_fd = 0;
	while((client_fd = socket_accept(reactor->socket_fd)) > 0)
	{
		non_blocking_set(client_fd);
		
		if(tcpserver_connect_cb)
			tcpserver_connect_cb(client_fd);
		
		event_t *event = (event_t *)calloc(1, sizeof(event_t));
		
		event->node.key = client_fd;
		map_add(&reactor->root, &event->node);
		
		event_set(event, client_fd, NULL, recv_cb, NULL, reactor);
		reactor_add_event(reactor, event, EPOLLIN | EPOLLET);
	}
	
	return 1;
}

static int accpet_cb(int fd, int events, void *para)
{
	reactor_t *reactor = (reactor_t *)para;
	
	work_t *work = (work_t *)calloc(1, sizeof(work_t));
	
	work->para = reactor;
	work->run = accpet_run;
	enqueue(reactor->queue, &work->node);
	
	return 1;
}

int tcpserver_init(tcpserver_t *tcpserver)
{
	if(!tcpserver || reactors)
		return -1;
	
	reactors_t *reactors = (reactors_t *)calloc(1, sizeof(reactors_t));
	reactors_init(reactors);
	reactors_run(reactors);
	work_init(reactors);
	
	for(int i = 0; i < reactors->reactor_len; i++)
	{
		int fd = socket_open();
		
		int opt = 1;
		int ret = setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
		if(ret < 0)
		{
			printf("setsockopt\n");
			return -1;
		}
		
		ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
		if(ret < 0)
		{
			printf("setsockopt\n");
			return -1;
		}
		
		non_blocking_set(fd);
		socket_bind(fd, tcpserver->ip_addr, tcpserver->port);
		socket_listen(fd, tcpserver->backlog);
		
		tcpserver_connect_cb = tcpserver->tcpserver_connect_cb;
		tcpserver_recv_cb = tcpserver->tcpserver_recv_cb;
		
		(reactors->reactor + i)->socket_fd = fd;
		
		event_t *event = (event_t *)calloc(1, sizeof(event_t));
		event_set(event, fd, NULL, accpet_cb, NULL, reactors->reactor + i);
		reactor_add_event(reactors->reactor + i, event, EPOLLIN | EPOLLET);
	}
	
	return 1;
}

int tcpserver_exit(void)
{
	if(!reactors)
		return -1;
	
	for(int i = 0; i < reactors->reactor_len; i++)
		socket_close((reactors->reactor + i)->socket_fd);
	
	work_exit(reactors);
	reactors_stop(reactors);
	reactors_exit(reactors);
	
	return 1;
}

int tcpserver_send(int fd, char *buffer, int buffer_len, void *para)
{
	reactor_t *reactor = (reactor_t *)para;
	
	event_t *event = map_get_entry(fd, &reactor->root, event_t, node);
	if(!event)
		return -1;
	
	int ret = socket_send(fd, buffer, buffer_len);
	if(ret > 0)
		return 1;
	else
	{
		reactor_delete_event(reactor, event);
		map_remove(&reactor->root, event->node.key);
		free(event);
		socket_close(fd);
	}
	
	return 1;
}

int tcpserver_close(int fd)
{
	socket_close(fd);
	return 1;
}
