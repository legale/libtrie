/* file: libtrie.h
 *
 * libtrie - simple Trie data structure library v0.0.1
 *
 * Copyright (C) 2018  legale.legale <legale.legale@gmail.com>
 * This software is provided under MIT license.
 */

#ifndef YATRIE_LIBRARY_H

#include <inttypes.h>
#include "single_list.h"

#define YATRIE_LIBRARY_H

//constants
#define MASK_SIZE 8 //mask index size in bits
#define MASK_INDEX 12 //mask index qty
#define MASK_BITS MASK_SIZE * MASK_INDEX //maximum allowed characters in bitmask
#define MAX_DEAL_SIZE MASK_BITS //deallocated memory array length
#define MAX_NODES  6000000 //maximum allowed nodes
#define MAX_REFS  10000000 //maximum allowed references
#define MAX_NODE_LEAFS 5000000 //maximum allowed node leafs
#define MAX_WORD_LENGTH 100 //maximum word letters
#define REF_SIZE 4 //reference size in bytes
#define REF_SIZE_POWER 2   //reference size as the number of powers of two

#define TRIE_INIT (nodes_qty) {.nodes = node nodes_qty, .nodes_increment = 1, .refs_increment = 0 };

#define REFS_SIZE 10000  //references memory block maximum elements
#define NODES_SIZE 100 //nodes memory block maximum elements


//structures
typedef struct string_s {
    uint32_t length;      //string current counter
    uint8_t letters[MAX_WORD_LENGTH];       //string array
} string_s;

typedef struct words_s {
    uint32_t counter;             //words counter
    uint8_t words[MAX_NODE_LEAFS][MAX_WORD_LENGTH];       //words array
} words_s;

//node chidlren nodes structure
typedef struct children_s {
    uint8_t length;    //refs block counter
    uint8_t letters[MASK_BITS];       //children letters
    uint32_t nodes[MASK_BITS]; //children node ids
} children_s;

//trie node_s structure
typedef struct node_s {
    uint8_t mask[MASK_INDEX]; // bitmask
    uint32_t ref_id; //32 bit references id (offset)
} node_s;


//trie structure
typedef struct trie_s {
    node_s nodes[MAX_NODES];
    uint32_t refs[MAX_REFS];
    list *deal[MAX_DEAL_SIZE];
    uint32_t nodes_increment; //first unused node_s is 1
    uint32_t refs_increment; //first unused ref_id is 0
} trie_s;


//macro functions
#define NODE_GET(node_id, trie) &trie->nodes[node_id]
#define REF_GET(ref_id, offset, trie) trie->refs[ref_id + offset]
#define CHAR_BIT_GET(bitmask, bit) (bitmask >> (bit - 1) & 1)
#define CHAR_BIT_SET(bitmask, bit) (bitmask | (1 << bit - 1))



//functions definitions
void init_alphabet();

void encode_string(uint8_t *dst, uint8_t *src);

uint8_t char_bit_set(uint8_t bitmask, uint8_t bit);

uint8_t char_bit_get(uint8_t bitmask, uint8_t bit);

uint8_t char_bit_clear(uint8_t bitmask, uint8_t bit);

uint32_t int32_bit_set(uint32_t bitmask, uint8_t bit);

uint8_t int32_bit_get(uint32_t bitmask, uint8_t bit);

uint32_t int32_bit_clear(uint32_t bitmask, uint8_t bit);

uint8_t int32_bit_count(uint32_t i, uint8_t length);

uint8_t bit_get(uint8_t bitmask[], uint8_t bit);

void bit_set(uint8_t bitmask[], uint8_t bit);

uint8_t bit_count(uint8_t bitmask[], uint8_t offset, uint8_t last_bit);

void precount_init();

void precount_init2();

uint8_t char_bit_count(uint8_t i, uint8_t length);

node_s *node_set(uint32_t id, node_s *_node, trie_s *trie);

uint32_t trie_add(uint8_t string[], uint32_t parent_id, trie_s *trie);

uint32_t trie_char_add(uint32_t parent_id, uint8_t char_index, trie_s *trie);

uint32_t node_insert_ref(uint8_t char_index, uint8_t insert_pos, node_s *parent_node, trie_s *trie);

void node_get_children(children_s *children, uint32_t node_id, trie_s *trie);

void char_mask_get_bits(uint8_t res[], uint8_t bitmask);

void char_mask_get_bits_raised(uint8_t res[], uint8_t bitmask);

uint8_t *char_mask_get_bits_raised_pre(uint8_t bitmask, uint8_t index);

void mask_get_bits_raised_pre(uint8_t res[], uint8_t bitmask[]);

void node_traverse(words_s *words, uint32_t node_id, string_s *head, trie_s *trie);

uint32_t trie_get_id(uint8_t *word, uint32_t parent_id, trie_s *trie);

size_t yatrie_save(uint8_t *filepath, trie_s *trie);

void yatrie_load(uint8_t *filepath, trie_s *trie);

void decode_string( uint8_t dst[], uint8_t src[]);

void yatrie_free(trie_s *trie);

#endif