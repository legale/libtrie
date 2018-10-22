/* file: single_list.c
 *
 * Single linked list structure v0.3
 *
 * Copyright (C) 2018  legale.legale <legale.legale@gmail.com>
 * This software is provided under MIT license.
 */


#include <stdlib.h>     //malloc() calloc() free()
#include <stdbool.h>    //bool true false
#include <string.h>     //memcpy() memcmp()

#include "single_list.h"


//internal function to find parent node_s by child element key
list_node *list_nodes_traverse(list_node *parent, list_node *node, unsigned char *key, unsigned int len) {
    if (memcmp(node->key, key, len) == 0) {
        return parent;
    } else if (node->next != NULL) {
        return list_nodes_traverse(node, node->next, key, len);
    } else {
        return NULL;
    }
}

//add list element in the beginning
 list *list_unshift(list *lst, unsigned char *key, unsigned int len, unsigned long value) {
    if (lst == NULL) {
        lst = (list *) calloc(1, sizeof(list)); //init list with zeroes
    }

    list_node *new = (list_node *) malloc(sizeof(list_node)); //allocate memory for the new element
    new->value = value;
    if (len > 0) {
        new->key = (unsigned char *) malloc(len);
        memcpy(new->key, key, len);
    } else {
        new->key = NULL;
    }

    new->next = NULL;
    ++lst->elements;

    if (lst->head == NULL) {
        lst->tail = new;
        lst->head = new;
    } else {
        new->next = lst->head;
        lst->head = new;
    }

    return lst;
}


//add list element in the end
 list *list_push(list *lst, unsigned char *key, unsigned int len, unsigned long value) {
    if (lst == NULL) {
        lst = (list *) calloc(1, sizeof(list)); //init new list with zeroes
    }

    list_node *new = (list_node *) malloc(sizeof(list_node)); //allocate memory for the new element
    new->value = value;
    if (len > 0) {
        new->key = (unsigned char *) malloc(len);
        memcpy(new->key, key, len);
    } else {
        new->key = NULL;
    }

    new->next = NULL;
    ++lst->elements;

    if (lst->tail == NULL) {
        lst->tail = new;
        lst->head = new;
    } else {
        lst->tail->next = new;
        lst->tail = new;
    }
    return lst;
};


//get list element value by the key
 unsigned long list_get(list *lst, unsigned char *key, unsigned int len) {
    if (lst == NULL || lst->head == NULL) {
        return 0;
    }
    //if the first element (head) key == key return return it's value
    if (memcmp(lst->head->key, key, len) == 0) {
        return lst->head->value;
    }

    list_node *parent = list_nodes_traverse(NULL, lst->head, key, len);
    if (parent != NULL) {
        return parent->next->value;
    }
}

//get last element of the linked list
 unsigned long list_pop(list *lst) {
    if (lst == NULL || lst->tail == NULL) {
        return 0;
    } else {
        unsigned long value = lst->tail->value;
        if (lst->head == lst->tail) { //if head = tail
            if (lst->tail->key != NULL) {
                free(lst->tail->key); //free key memory
                lst->tail->key = NULL; //set list key pointer to NULL
            }
            free(lst->tail); //free list element memory
            lst->head = NULL; //set list pointer to NULL
            lst->tail = NULL; //set list pointer to NULL
        } else {
            list_node *next = lst->tail->next;
            if (lst->tail->key != NULL) {
                free(lst->tail->key); //free key memory
                lst->tail->key = NULL; //set list key pointer to NULL
            }
            free(lst->tail);
            lst->tail = NULL; //set list pointer to NULL
            lst->tail = next;
        }
        --lst->elements;
        return value;
    }
}

//get first element of the linked list
 unsigned long list_shift(list *lst) {
    if (lst == NULL || lst->head == NULL) {
        return 0;
    } else {
        unsigned long value = lst->head->value;
        if (lst->head == lst->tail) { //if head = tail
            if (lst->head->key != NULL) {
                free(lst->head->key); //free key memory
                lst->head->key = NULL; //set list key pointer to NULL
            }
            free(lst->head); //free list element memory
            lst->head = NULL; //set list pointer to NULL
            lst->tail = NULL; //set list pointer to NULL
        } else {
            list_node *next = lst->head->next;
            if (lst->head->key != NULL) {
                free(lst->head->key); //free key memory
                lst->head->key = NULL; //set list key pointer to NULL
            }

            free(lst->head);
            lst->head = NULL; //set list pointer to NULL
            lst->head = next;
        }
        --lst->elements;
        return value;
    }
}

//remove list element by the key and key counter
 int list_remove(list *lst, unsigned char *key, unsigned int len) {
    if (memcmp(lst->head->key, key, len) == 0) {
        if (lst->head == lst->tail) { //if head = tail
            free(lst->head->key); //free key memory
            lst->head->key = NULL; //set list key pointer to NULL
            free(lst->head); //free list element memory
            lst->head = NULL; //set list pointer to NULL
            free(lst); //free list pointer
            lst = NULL; //set list pointer to NULL
            return 0;
        } else {
            list_node *next = lst->head->next;
            free(lst->head->key);
            lst->head->key = NULL; //set list pointer to NULL
            free(lst->head);
            lst->head = NULL; //set list pointer to NULL
            lst->head = next;
            return --lst->elements;
        }
    }

    list_node *parent = list_nodes_traverse(NULL, lst->head, key, len);
    if (parent != NULL) {
        list_node *next = parent->next->next;
        free(parent->next->key);
        parent->next->key = NULL; //set list key pointer to NULL
        free(parent->next);
        parent->next = next;
        --lst->elements;
        return true;
    } else {
        return false;
    }
}

 void list_free(list *lst) {
    if (lst == NULL) {
        return;
    }

    if (lst->head != NULL) {
        if (lst->head->key != NULL) {
            free(lst->head->key); //free key memory
            lst->head->key = NULL;
        }

        list_node *old_head = lst->head;
        if (lst->head->next != NULL) {
            lst->head = lst->head->next;
        } else {
            lst->head = NULL;
        }
        free(old_head);

        return list_free(lst);
    }

    free(lst);
    lst == NULL;
}

