/************************************************
 * @addtogroup CITS
 * @{
 * @file  : map.h
 * @brief : 
 * @author: wangshaobo
 * @email : wangshaobo@nebula-link.com
 * @date  : 2019-05-10 10:43:51
************************************************/

//-----------------------------------------------
// Copyright (c) CITS
//-----------------------------------------------

#ifndef _MAP_H_
#define _MAP_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <rbtree.h>

typedef struct rb_root map_root;

typedef struct
{
    unsigned long long key;
    struct rb_node node;
} map_node;

static inline void map_add(map_root *root, map_node *node)
{
    struct rb_node **new_node = &(root->rb_node);
	struct rb_node *parent = NULL;
    map_node *target_node;
    unsigned long long key = node->key;
	
    while(*new_node)
    {
        target_node = container_of(*new_node, map_node, node);
		parent = *new_node;

        if(key < target_node->key)
            new_node = &((*new_node)->rb_left);
		else if(key > target_node->key)
            new_node = &((*new_node)->rb_right);
		else
            return;
    }
	
    rb_link_node(&node->node, parent, new_node);
    rb_insert_color(&node->node, root);
}

static inline map_node *map_remove(map_root *root, unsigned long long key)
{
    struct rb_node *node = root->rb_node;
	
    while(node)
    {
        map_node *data = container_of(node, map_node, node);
        if(key < data->key)
            node = node->rb_left;
        else if(key > data->key)
            node = node->rb_right;
        else
        {
            rb_erase(&data->node, root);
            return data;
        }
    }
    return NULL;
}

#define map_remove_entry(root, key, type, member) \
        map_remove(root, key) ? container_of(map_remove(root, key), type, member) : NULL

static inline map_node *map_first(map_root *root)
{
    struct rb_node *first_node = rb_first(root);
	
    if(first_node)
        return (container_of(first_node, map_node, node));
	else
        return NULL;
	return NULL;
}

static inline map_node *map_last(map_root *root)
{
	struct rb_node *last_node = rb_last(root);
	
	if(last_node)
		return (container_of(last_node, map_node, node));
	else
		return NULL;
	return NULL;
}

static inline map_node *map_get(map_root *root, unsigned long long key)
{
    struct rb_node *node = root->rb_node;

    while(node)
    {
        map_node *data = container_of(node, map_node, node);

        if(key < data->key)
            node = node->rb_left;
		else if(key > data->key)
            node = node->rb_right;
		else 
            return data;
    }
    return NULL;
}

#define map_first_entry(root, type, member) \
		map_first(root) ? rb_entry(map_first(root), type, member) : NULL

#define map_last_entry(root, type, member) \
		map_last(root) ? rb_entry(map_last(root), type, member) : NULL

#define map_get_entry(key, root, type, node) \
        map_get(root, key) ? rb_entry(map_get(root, key), type, node) : NULL

static inline map_node *map_next(map_node *node)
{
    if(rb_next(&node->node))
        return container_of(rb_next(&node->node), map_node, node);
	else 
        return NULL;
}

static inline void map_replace(map_root *root, map_node *to, map_node *from)
{
    if(!root || !to || !from)
        return;
    rb_replace_node(&to->node, &from->node, root);
}

#define map_next_entry(node, type, member) \
        map_next(node) ? rb_entry(map_next(node), type, member) : NULL
		
#define map_for_each_entry(pos, root, member) \
		for(pos = map_first_entry(root, typeof(*pos), member); pos != NULL; pos = map_next_entry(&pos->member, typeof(*pos), member))
			
#define map_for_each_entry_safe(pos, n, root, member) \
		for(pos = map_first_entry(root, typeof(*pos), member), n = pos ? map_next_entry(&pos->member, typeof(*pos), member) : NULL; pos != NULL; pos = n, n = pos ? map_next_entry(&pos->member, typeof(*pos), member) : NULL)
			
#define map_empty(root) \
		rb_first(root) ? 0 : 1
		
#ifdef __cplusplus
}
#endif

#endif
