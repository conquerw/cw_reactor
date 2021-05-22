/************************************************
 * @addtogroup CITS
 * @{
 * @file  : tcpserver.h
 * @brief : 
 * @author: wangshaobo
 * @email : wangshaobo@nebula-link.com
 * @date  : 2019-05-10 10:43:51
************************************************/

//-----------------------------------------------
// Copyright (c) CITS
//-----------------------------------------------

#ifndef _TCPSERVER_H_
#define _TCPSERVER_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
	char *ip_addr;
	int port;
	int backlog;
	int (*tcpserver_connect_cb)(int fd);
	int (*tcpserver_recv_cb)(int fd, char *buffer, int buffer_len, void *para);
} tcpserver_t;

int tcpserver_init(tcpserver_t *tcpserver);

int tcpserver_exit(void);

int tcpserver_send(int fd, char *buffer, int buffer_len, void *para);

int tcpserver_close(int fd);

#ifdef __cplusplus
}
#endif

#endif
