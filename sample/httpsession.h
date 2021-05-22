/************************************************
 * @addtogroup CITS
 * @{
 * @file  : httpsession.h
 * @brief : 
 * @author: wangshaobo
 * @email : wangshaobo@nebula-link.com
 * @date  : 2019-05-10 10:43:51
************************************************/

//-----------------------------------------------
// Copyright (c) CITS
//-----------------------------------------------

#ifndef _HTTPSESSION_H_
#define _HTTPSESSION_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
	char version;
	char action;
	char path[32];
} http_request_t;

int http_parse_msg(http_request_t *http_request, char *buffer, int buffer_len);

int http_handle(char *buffer, int *buffer_len, http_request_t *http_request);

#ifdef __cplusplus
}
#endif

#endif
