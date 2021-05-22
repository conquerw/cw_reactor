/************************************************
 * @addtogroup CITS
 * @{
 * @file  : httpsession.c
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
#include <fcntl.h>
#include <file.h>
#include <httpsession.h>

int http_parse_msg(http_request_t *http_request, char *buffer, int buffer_len)
{
	char *link = NULL, *next = NULL;
	link = strchr(buffer, 0x20);
	if(link)
	{
		char temp[8] = "";
		memset(temp, 0x00, sizeof(temp));
		memcpy(temp, buffer, link - buffer);
		// printf("action: %s\n", temp);
		if(memcmp("GET", temp, strlen("GET")) == 0)
			http_request->action = 1;
		else
			http_request->action = 2;
		
		next = link;
		next++;
	}
	if(!next)
		return -1;
	
	link = strchr(next, 0x20);
	if(link)
	{
		memcpy(http_request->path, next + 1, link - next - 1);
		// printf("path: %s\n", http_request->path);
		
		next = link;
		next++;
	}
	if(!next)
		return -1;
	
	link = strstr(next, "\r\n");
	if(link)
	{
		char temp[16] = "";
		memset(temp, 0x00, sizeof(temp));
		memcpy(temp, next, link - next);
		// printf("version: %s\n", temp);
		if(memcmp("HTTP/1.1", temp, strlen("HTTP/1.1")) == 0)
			http_request->version = 1;
		else
			http_request->version = 2;
	}
	
	return 1;
}

int http_handle(char *buffer, int *buffer_len, http_request_t *http_request)
{	
	if(http_request->version != 1)
		return -1;
	
	if(http_request->action != 1)
		return -1;
	
	char *link = buffer;
	memcpy(link, "HTTP/1.1 200 OK\r\n", strlen("HTTP/1.1 200 OK\r\n"));
	link += strlen("HTTP/1.1 200 OK\r\n");
	
	memcpy(link, "Content-Type: text/html; charset=utf-8\r\n", strlen("Content-Type: text/html; charset=utf-8\r\n"));
	link += strlen("Content-Type: text/html; charset=utf-8\r\n");
	
	memcpy(link, "Connection: Keep-Alive\r\n", strlen("Connection: Keep-Alive\r\n"));
	link += strlen("Connection: Keep-Alive\r\n");
	
	memcpy(link, "Content-Length: 11\r\n\r\n", strlen("Content-Length: 11\r\n\r\n"));
	link += strlen("Content-Length: 11\r\n\r\n");
	
	memcpy(link, "hello world", strlen("hello world"));
	link += strlen("hello world");
	
	// file_t file;
	// memset(&file, 0x00, sizeof(file));
	// // memcpy(file.filepath, http_request->path, strlen(http_request->path));
	// memcpy(file.filepath, "index.html", strlen("index.html"));
	// file.flag = O_RDONLY;
	// file_init(&file);
	
	// char buffer2[1024] = "";
	// memset(buffer2, 0x00, sizeof(buffer2));
	// int ret = file_read(buffer2, sizeof(buffer2), &file);
	// memcpy(link, buffer2, ret);
	// link += ret;
	
	*buffer_len = link - buffer;
	
	return 1;
}
