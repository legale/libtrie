/* file: libtrie.h
 *
 * libtrie - simple Trie data structure library v0.1.2
 *
 * Copyright (C) 2018  legale.legale <legale.legale@gmail.com>
 * This software is provided under MIT license.
 */

#ifndef YATRIE_LIBRARY_H
#define YATRIE_LIBRARY_H

#include <inttypes.h>
#include <stddef.h>
#include "single_list.h"

//prediction macros
#ifdef __GNUC__
#define LIKELY(x)       __builtin_expect(!!(x), 1)
#define UNLIKELY(x)     __builtin_expect(!!(x), 0)
#else
#define LIKELY(x)       (x)
#define UNLIKELY(x)     (x)
#endif

//constants
#define MASK_SIZE 32 //mask index size in bits
#define MASK_INDEX 3 //mask index qty
#define MASK_BITS MASK_SIZE * MASK_INDEX //maximum allowed characters in bitmask
#define MAX_DEAL_SIZE MASK_BITS //deallocated memory array length
#define MAX_NODES  6000000 //maximum allowed nodes
#define MAX_REFS  10000000 //maximum allowed references
#define MAX_NODE_LEAFS 100 //maximum allowed node leafs
#define MAX_WORD_LENGTH 100 //maximum word letters
#define REF_SIZE 4 //reference size in bytes
#define REF_SIZE_POWER 2   //reference size as the number of powers of two


//structures
typedef struct _string_s {
    uint32_t length;      //string current counter
    uint8_t letters[MAX_WORD_LENGTH];       //string array
} string_s;

typedef struct _words_s {
    uint32_t counter;             //words counter
    uint8_t words[MAX_NODE_LEAFS][MAX_WORD_LENGTH];       //words array
} words_s;

//node chidlren nodes structure
typedef struct children_s {
    uint8_t length;    //refs block counter
    uint8_t letters[MASK_BITS];       //children letters
    uint32_t nodes[MASK_BITS]; //children node ids
} children_s;

//ref type
typedef uint32_t ref_t;

//trie node_s structure
typedef struct _node_s {
    uint32_t mask[MASK_INDEX]; // bitmask
    uint32_t ref_id; //32 bit references id (offset)
} node_s;

//nodes block structure
typedef struct _nodes_s {
    uint32_t increment;
    uint32_t size;
    node_s data[];
} nodes_s;

//nodes block structure
typedef struct _refs_s {
    uint32_t increment;
    uint32_t size;
    ref_t data[];
} refs_s;

//deallocated (free) refs block structure
typedef struct _refs_free_s {
    uint32_t disk_size;
    uint32_t size;
    list_s *data[];
} refs_free_s;

//trie structure
typedef struct _trie_s {
    nodes_s *nodes;
    refs_s *refs;
    refs_free_s *refs_free;
    list_s *nodes_free;
} trie_s;



//macro functions
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define NODE_GET(node_id, trie) &trie->nodes->data[node_id]
#define REF_GET(ref_id, offset, trie) trie->refs->data[ref_id + offset]


#define BIT_GET(bitmask, bit) ((bitmask) >> (bit - 1) & 1)
#define BIT_SET(bitmask, bit) ((bitmask) | (1 << bit - 1))
#define CHAR_BIT_COUNT(bitmask) (precount[(bitmask)])
#define INT32_BIT_COUNT(bitmask) (precount16[(bitmask) & 0xffff] + precount16[((bitmask) >> 16) & 0xffff])

//macro functions that requires bit_array_rest and bit_array_index
//global static var for the BIT_ARRAY_GET(), BIT_ARRAY_SET() macros.
static uint8_t bit_array_rest = 0;
static uint8_t bit_array_index = 0;
#define BIT_ARRAY_GET(bitmask, bit) ({ \
bit_array_rest = bit & 31; \
BIT_GET(bitmask[(bit - 1) >> 5], (LIKELY(bit_array_rest) ? bit_array_rest : 32) ); \
})

#define BIT_ARRAY_SET(bitmask, bit) ({ \
bit_array_index = (bit - 1) >> 5; \
bit_array_rest = bit & 31; \
bitmask[bit_array_index] = BIT_SET(bitmask[bit_array_index], (LIKELY(bit_array_rest) ? bit_array_rest : 32) ); \
})



//functions definitions
void init_alphabet();

void encode_string(uint8_t *dst, uint8_t *src);

uint8_t int32_bit_count(uint32_t i, uint8_t length);

uint8_t bit_count(uint8_t bitmask[], uint8_t offset, uint8_t last_bit);

uint8_t bit_int32_count(uint32_t bitmask[], uint8_t offset, uint8_t last_bit);

void precount_char_init();

uint8_t char_bit_count(uint8_t i, uint8_t length);

uint32_t yatrie_add(uint8_t *string, uint32_t parent_id, trie_s *trie);

uint32_t yatrie_char_add(uint32_t parent_id, uint8_t char_index, trie_s *trie);

uint32_t node_insert_ref(uint8_t char_index, uint8_t insert_pos, node_s *parent_node, trie_s *trie);

void node_get_children(children_s *children, uint32_t node_id, trie_s *trie);

void char_mask_get_bits(uint8_t res[], uint8_t bitmask);

void char_get_bits_raised(uint8_t *res, uint8_t bitmask);

uint8_t *char_get_bits_raised_pre(uint8_t bitmask, uint8_t index);

void char_mask_get_bits_raised_pre(uint8_t *res, uint8_t *bitmask);

void node_traverse(words_s *words, uint32_t node_id, string_s *head, trie_s *trie);

uint32_t trie_get_id(uint8_t *word, uint32_t parent_id, trie_s *trie);

size_t yatrie_save(uint8_t *filepath, trie_s *trie);

trie_s *yatrie_load(uint8_t *filepath);

void decode_string( uint8_t dst[], uint8_t src[]);

void yatrie_free(trie_s *trie);

trie_s *yatrie_new(uint32_t max_nodes, uint32_t max_refs, uint32_t max_deallocated_size);

#endif /* libtrie.h  */