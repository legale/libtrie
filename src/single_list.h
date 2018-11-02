/* file: single_list.h
 *
 * Single linked list_s structure v0.3.1
 *
 * Copyright (C) 2018  legale.legale <legale.legale@gmail.com>
 * This software is provided under MIT license.
 */

#ifndef _SINGLE_LIST_H
#define _SINGLE_LIST_H
#include <inttypes.h>
//#include "../utils/inline.h"    //INLINE constant


//structures
typedef struct _list_node_s {
    unsigned char *key;
    unsigned long value;
    struct _list_node_s *next;
} list_node_s;

//list_s
typedef struct _list_s {
    unsigned int elements; //list_s elements counter
    list_node_s *head; //head node_s
    list_node_s *tail; //tail node_s
} list_s;


//functions declaration
 list_s *list_unshift(list_s *lst, unsigned char *key, unsigned int len, unsigned long value);

 list_s *list_push(list_s *lst, unsigned char *key, unsigned int len, unsigned long value);

 unsigned long list_get(list_s *lst, unsigned char *key, unsigned int len);

 unsigned long list_pop(list_s *lst);

 unsigned long list_shift(list_s *lst);

 int list_remove(list_s *lst, unsigned char *key, unsigned int len);

 list_node_s *list_nodes_traverse(list_node_s *parent, list_node_s *node, unsigned char *key, unsigned int len);

 void list_free(list_s *lst);

#endif /* single_list.h  */