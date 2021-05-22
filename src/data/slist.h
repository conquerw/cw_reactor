/************************************************
 * @addtogroup CITS
 * @{
 * @file  : slist.h
 * @brief : 
 * @author: wangshaobo
 * @email : wangshaobo@nebula-link.com
 * @date  : 2019-05-10 10:43:51
************************************************/

//-----------------------------------------------
// Copyright (c) CITS
//-----------------------------------------------

#ifndef _SLIST_H_
#define _SLIST_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>

#define offset(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:   the type of the container struct this is embedded in.
 * @member: the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({          \
	const typeof(((type *)0)->member)*__mptr = (ptr);    \
		     (type *)((char *)__mptr - offset(type, member)); })
			 
#define WRITE_ONCE(var, val) \
	(*((volatile typeof(val) *)(&(var))) = (val))
	
#define READ_ONCE(var) (*((volatile typeof(var) *)(&(var))))

/*
 * Simple Single-linked linked list implementation.
 */

struct slist_node {
	struct slist_node *next;
};

struct slist_head {
	struct slist_node first, *last;
};

#define SLIST_HEAD_INIT(name) { { NULL }, &(name).first }

#define SLIST_HEAD(name) \
	struct slist_head name = SLIST_HEAD_INIT(name)

static inline void INIT_SLIST_HEAD(struct slist_head *head)
{
	head->first.next = NULL;
	head->last = &head->first;
}

static inline void slist_add_head(struct slist_node *node, struct slist_head *head)
{
	node->next = head->first.next;
	head->first.next = node;
	if(!node->next)
		head->last = node;
}

static inline void slist_add_tail(struct slist_node *node, struct slist_head *head)
{
	node->next = NULL;
	head->last->next = node;
	head->last = node;
}

static inline void slist_add_after(struct slist_node *node, struct slist_node *prev, struct slist_head *head)
{
	node->next = prev->next;
	prev->next = node;
	if(!node->next)
		head->last = node;
}

static inline void slist_del_head(struct slist_head *head)
{
	head->first.next = head->first.next->next;
	if(!head->first.next)
		head->last = &head->first;
}

static inline void slist_del_after(struct slist_node *prev, struct slist_head *head)
{
	prev->next = prev->next->next;
	if(!prev->next)
		head->last = prev;
}

static inline void slist_move(struct slist_head *list, struct slist_head *head)
{
	list->first.next = head->first.next;
	list->last = head->last;
	INIT_SLIST_HEAD(head);
}

static inline int slist_empty(struct slist_head *head)
{
	return !head->first.next;
}

#define slist_entry(ptr, type, member) \
	container_of(ptr, type, member)
	
#define slist_first_entry(ptr, type, member) \
	slist_entry((ptr)->first.next, type, member)
	
#define slist_next_entry(pos, member) \
	slist_entry((pos)->member.next, typeof(*(pos)), member)
	
#define slist_last_entry(ptr, type, member) \
	slist_entry((ptr)->last, type, member)
	
#define slist_for_each(pos, head) \
	for (pos = (head)->first.next; pos; pos = pos->next)
		
#define slist_for_each_safe(pos, prev, head) \
	for (prev = &(head)->first, pos = prev->next; pos; \
		 prev = prev->next == pos ? pos : prev, pos = prev->next)
		 
#define slist_for_each_entry(pos, head, member)				\
	for (pos = slist_first_entry(head, typeof(*pos), member);	\
	     &pos->member != NULL;					\
	     pos = slist_next_entry(pos, member))
		 
#ifdef __cplusplus
}
#endif

#endif
