/************************************************
 * @addtogroup CITS
 * @{
 * @file  : socket.c
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
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <socket.h>

#define RECV_BUFFER_LEN 4096

int socket_open(void)
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0)
	{
		printf("socket\n");
		return -1;
	}
	
	return fd;
}

int socket_close(int fd)
{
	close(fd);
	return 1;
}

int socket_bind(int fd, char *ip_addr, int port)
{
	if(!ip_addr)
		return -1;
	
	struct sockaddr_in addr;
	memset(&addr, 0x00, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	if(inet_addr(ip_addr) == INADDR_NONE)
	{
		struct hostent *hptr = gethostbyname(ip_addr);  
		if(!hptr)
		{
			printf("gethostbyname fail\n");
			return -1;
		}
		addr.sin_addr.s_addr = *((unsigned long*)hptr->h_addr_list[0]);  
	}
	else
		addr.sin_addr.s_addr = inet_addr(ip_addr);
	
	int ret = bind(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
	if(ret < 0)
	{
		printf("bind\n");
		return -1;
	}
	
	return 1;
}

int socket_listen(int fd, int backlog)
{
	int ret = listen(fd, backlog);
	if(ret < 0)
	{
		printf("listen\n");
		return -1;
	}
	return 1;
}

int socket_accept(int fd)
{
	struct sockaddr_in addr;
	memset(&addr, 0x00, sizeof(struct sockaddr_in));
	socklen_t addr_len = sizeof(addr);
	
	int ret = accept(fd, (struct sockaddr*)&addr, &addr_len);
	if(ret < 0)
	{
		if(errno == EAGAIN)
			return 0;
		return -1;
	}
	
	// printf("client_fd: %d, %s.%d\n", ret, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
	
	return ret;
}

int socket_send(int fd, char *buffer, int buffer_len)
{
	int buffer2_len = 0;
	while(1)
	{
		int ret = send(fd, buffer + buffer2_len, buffer_len - buffer2_len, 0);
		if(ret > 0)
		{
			if(ret == buffer_len - buffer2_len)
				return ret;
			else
				buffer2_len += ret;
		}
		else if(ret == 0)
			return 0;
		else
		{
			if(errno == EAGAIN)
				return buffer2_len;
			else if(errno == EINTR)
				continue;
			else
				return -1;
		}
	}
	
	return 1;
}

int socket_recv(int fd, char **buffer)
{
	char recv_buffer[RECV_BUFFER_LEN] = "";
	
	char *buffer2 = NULL;
	int buffer2_len = 0;
	
	while(1)
	{
		memset(recv_buffer, 0x00, RECV_BUFFER_LEN);
		int ret = recv(fd, recv_buffer, RECV_BUFFER_LEN, 0);
		if(ret > 0)
		{
			 buffer2 = (char *)realloc(buffer2, buffer2_len + ret);
			 memcpy(buffer2 + buffer2_len, recv_buffer, ret);
			 buffer2_len += ret;
			 if(ret < RECV_BUFFER_LEN)
			 {
				*buffer = buffer2;
				return buffer2_len;
			 }
		}
		else if(ret == 0)
			return 0;
		else
		{
			if(errno == EAGAIN)
			{
				*buffer = buffer2;
				return buffer2_len;
			}
			else if(errno == EINTR)
				continue;
			else
				return -1;
		}
	}
	
	return 1;
}

int non_blocking_set(int fd)
{
    int ret = fcntl(fd, F_GETFL);
    if(ret < 0)
    {
        printf("fcntl F_GETFL\n");
        return -1;
    }
	
	ret |= O_NONBLOCK;
	ret = fcntl(fd, F_SETFL, ret);
    if(ret < 0)
    {
        printf("fcntl F_SETFL\n");
        return -1;
    }
	
	return 1;
}

int blocking_set(int fd)
{
    int ret = fcntl(fd, F_GETFL);
    if(ret < 0)
    {
        printf("fcntl F_GETFL\n");
        return -1;
    }
	
	ret &= ~O_NONBLOCK;
	ret = fcntl(fd, F_SETFL, ret);
    if(ret < 0)
    {
        printf("fcntl F_SETFL\n");
        return -1;
    }
	
	return 1;
}
