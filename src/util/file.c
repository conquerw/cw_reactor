/************************************************
 * @addtogroup CITS
 * @{
 * @file  : file.c
 * @brief : 
 * @author: wangshaobo
 * @email : wangshaobo@nebula-link.com
 * @date  : 2019-05-10 10:43:51
************************************************/

//-----------------------------------------------
// Copyright (c) CITS
//-----------------------------------------------

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <file.h>

void file_init(file_t *file)
{
	// pthread_mutex_init(&file->mutex, NULL);
}

int file_read(char *buffer, int size, file_t *file)
{
	// pthread_mutex_lock(&file->mutex);
	int fd = open(file->filepath, O_RDONLY);
	if(fd < 0)
	{
		printf("the open the file is failure\n");
		// pthread_mutex_unlock(&file->mutex);
		return -1;
	}
	int ret = read(fd, buffer, size);
	if((ret > size) || (ret < -1))
	{
		printf("the open the file is failure\n");
		close(fd);
		// pthread_mutex_unlock(&file->mutex);
		return -1;
	}
	close(fd);
	// pthread_mutex_unlock(&file->mutex);
	return ret;
}

int file_write(file_t *file, char *buffer, int size)
{
	pthread_mutex_lock(&file->mutex);
	int fd = open(file->filepath, file->flag, file->mode);
	if(fd < 0)
	{
		printf("the open the file is failure\n");
		pthread_mutex_unlock(&file->mutex);
		return -1;
	}
	int ret = write(fd, buffer, size);
	if(ret < -1)
	{
		printf("the open the file is failure\n");
		close(fd);
		pthread_mutex_unlock(&file->mutex);
		return -1;
	}
	fsync(fd);
	close(fd);
	pthread_mutex_unlock(&file->mutex);
	return ret;
}
