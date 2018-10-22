/* file: single_list.h
 *
 * Single linked list structure v0.3
 *
 * Copyright (C) 2018  legale.legale <legale.legale@gmail.com>
 * This software is provided under MIT license.
 */

#ifndef _SINGLE_LIST_H
#define _SINGLE_LIST_H
#include <inttypes.h>
//#include "../utils/inline.h"    //INLINE constant


//structures
//linked list
typedef struct list_node {
    unsigned char *key;
    unsigned long value;
    struct list_node *next;
} list_node;

//list
typedef struct list {
    unsigned int elements; //list elements counter
    struct list_node *head; //head node_s
    struct list_node *tail; //tail node_s
} list;


//functions declaration
 list *list_unshift(list *lst, unsigned char *key, unsigned int len, unsigned long value);

 list *list_push(list *lst, unsigned char *key, unsigned int len, unsigned long value);

 unsigned long list_get(list *lst, unsigned char *key, unsigned int len);

 unsigned long list_pop(list *lst);

 unsigned long list_shift(list *lst);

 int list_remove(list *lst, unsigned char *key, unsigned int len);

 list_node *list_nodes_traverse(list_node *parent, list_node *node, unsigned char *key, unsigned int len);

 void list_free(list *lst);

#endif /* single_list.h  */