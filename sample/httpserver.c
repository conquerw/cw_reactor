/************************************************
 * @addtogroup CITS
 * @{
 * @file  : httpserver.c
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
#include <queue.h>
#include <thread.h>
#include <work.h>
#include <tcpserver.h>
#include <httpserver.h>
#include <httpsession.h>

static tcpserver_t *tcpserver = NULL;

void printf_char(char *buffer, int buffer_len)
{
	printf("recv: ");
	for(int i = 0; i < buffer_len; i++)
		printf("%c",buffer[i]);
	printf("\n");
}

int http_connect(int fd)
{
	// printf("http_connect: %d\n", fd);
	return 1;
}

typedef struct
{
	int fd;
	void *para;
	char *recv_buffer;
	int recv_buffer_len;
} dealdata_t;

int run2(void *para)
{
	dealdata_t *dealdata = (dealdata_t *)para;
	
	// printf_char(dealdata->recv_buffer, dealdata->recv_buffer_len);
	
	http_request_t *http_request = (http_request_t *)calloc(1, sizeof(http_request_t));
	http_parse_msg(http_request, dealdata->recv_buffer, dealdata->recv_buffer_len);
	
	char buffer[1024] = "";
	memset(buffer, 0x00, sizeof(buffer));
	int buffer_len = 0;
	http_handle(buffer, &buffer_len, http_request);
	
	// printf_char(buffer, buffer_len);
	
	tcpserver_send(dealdata->fd, buffer, buffer_len, dealdata->para);
	
	free(http_request);
	free(dealdata->recv_buffer);
	free(dealdata);
	return 1;
}

int http2_connect(int fd, char *buffer, int buffer_len, void *para)
{
	dealdata_t *dealdata = (dealdata_t *)calloc(1, sizeof(dealdata_t));
	
	dealdata->fd = fd;
	dealdata->recv_buffer = buffer;
	dealdata->recv_buffer_len = buffer_len;
	dealdata->para = para;
	
	reactor_t *reactor = (reactor_t *)para;
	
	work_t *work = (work_t *)calloc(1, sizeof(work_t));
	
	work->para = dealdata;
	work->run = run2;
	enqueue(reactor->queue, &work->node);
	
	return 1;
}

#include <arpa/inet.h>

int httpserver_init(void)
{
	if(tcpserver)
		return -1;
	
	tcpserver = (tcpserver_t *)calloc(1, sizeof(tcpserver_t));
	tcpserver->ip_addr = "0.0.0.0";
	tcpserver->port = 50000;
	tcpserver->backlog = 2048;
	tcpserver->tcpserver_connect_cb = http_connect;
	tcpserver->tcpserver_recv_cb = http2_connect;
	tcpserver_init(tcpserver);
	return 1;
}

int httpserver_exit(void)
{
	if(!tcpserver)
		return -1;
	
	tcpserver_exit();
	free(tcpserver);
	
	return 1;
}
