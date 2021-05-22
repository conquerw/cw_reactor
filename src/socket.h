/************************************************
 * @addtogroup CITS
 * @{
 * @file  : socket.h
 * @brief : 
 * @author: wangshaobo
 * @email : wangshaobo@nebula-link.com
 * @date  : 2019-05-10 10:43:51
************************************************/

//-----------------------------------------------
// Copyright (c) CITS
//-----------------------------------------------

#ifndef _SOCKET_H_
#define _SOCKET_H_

#ifdef __cplusplus
extern "C"
{
#endif

int socket_open(void);

int socket_close(int fd);

int socket_bind(int fd, char *ip_addr, int port);

int socket_listen(int fd, int backlog);

int socket_accept(int fd);

int socket_send(int fd, char *buffer, int buffer_len);

int socket_recv(int fd, char **buffer);

int non_blocking_set(int fd);

int blocking_set(int fd);

#ifdef __cplusplus
}
#endif

#endif
