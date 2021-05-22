/************************************************
 * @addtogroup CITS
 * @{
 * @file  : file.h
 * @brief : 
 * @author: wangshaobo
 * @email : wangshaobo@nebula-link.com
 * @date  : 2019-05-10 10:43:51
************************************************/

//-----------------------------------------------
// Copyright (c) CITS
//-----------------------------------------------

#ifndef _FILE_H_
#define _FILE_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
	char filepath[32];
	int flag;
	mode_t mode;
	pthread_mutex_t mutex;
} file_t;

void file_init(file_t *file);

int file_read(char *buffer, int size, file_t *file);

int file_write(file_t *file, char *buffer, int size);

#ifdef __cplusplus
}
#endif

#endif
