/* file: libtrie.c
 *
 * libtrie - simple Trie data structure library v0.0.1
 *
 * Copyright (C) 2018  legale.legale <legale.legale@gmail.com>
 * This software is provided under MIT license.
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>     //memcpy() memcmp()
#include <stdlib.h>     //malloc() calloc() free()

#include "libtrie.h"
#include "single_list.h"


//global static vars
//precounted bits
uint8_t static precount[256] = {};
uint8_t static precount2[MASK_INDEX][256][9] = {};
uint8_t static const precount_mask[8] = {0b0, 0b1, 0b11, 0b111, 0b1111, 0b11111, 0b111111, 0b1111111};
uint8_t static const precount_offset_mask[8] = {0b11111111, 0b11111110, 0b11111100, 0b11111000, 0b11110000, 0b11100000,
                                                0b11000000, 0b10000000};


uint8_t bca[][3] = {
        {},     /* void [0] */
        {48,}, /* 0 [1] */
        {49,}, /* 1 [2] */
        {50,}, /* 2 [3] */
        {51,}, /* 3 [4] */
        {52,}, /* 4 [5] */
        {53,}, /* 5 [6] */
        {54,}, /* 6 [7] */
        {55,}, /* 7 [8] */
        {56,}, /* 8 [9] */
        {57,}, /* 9 [10] */
        {208, 176,}, /* а [11] */
        {208, 177,}, /* б [12] */
        {208, 178,}, /* в [13] */
        {208, 179,}, /* г [14] */
        {208, 180,}, /* д [15] */
        {208, 181,}, /* е [16] */
        {209, 145,}, /* ё [17] */
        {208, 182,}, /* ж [18] */
        {208, 183,}, /* з [19] */
        {208, 184,}, /* и [20] */
        {208, 185,}, /* й [21] */
        {208, 186,}, /* к [22] */
        {208, 187,}, /* л [23] */
        {208, 188,}, /* м [24] */
        {208, 189,}, /* н [25] */
        {208, 190,}, /* о [26] */
        {208, 191,}, /* п [27] */
        {209, 128,}, /* р [28] */
        {209, 129,}, /* с [29] */
        {209, 130,}, /* т [30] */
        {209, 131,}, /* у [31] */
        {209, 132,}, /* ф [32] */
        {209, 133,}, /* х [33] */
        {209, 134,}, /* ц [34] */
        {209, 135,}, /* ч [35] */
        {209, 136,}, /* ш [36] */
        {209, 137,}, /* щ [37] */
        {209, 138,}, /* ъ [38] */
        {209, 139,}, /* ы [39] */
        {209, 140,}, /* ь [40] */
        {209, 141,}, /* э [41] */
        {209, 142,}, /* ю [42] */
        {209, 143,}, /* я [43] */
        {97,}, /* a [44] */
        {98,}, /* b [45] */
        {99,}, /* c [46] */
        {100,}, /* d [47] */
        {101,}, /* e [48] */
        {102,}, /* f [49] */
        {103,}, /* g [50] */
        {104,}, /* h [51] */
        {105,}, /* i [52] */
        {106,}, /* j [53] */
        {107,}, /* k [54] */
        {108,}, /* l [55] */
        {109,}, /* m [56] */
        {110,}, /* n [57] */
        {111,}, /* o [58] */
        {112,}, /* p [59] */
        {113,}, /* q [60] */
        {114,}, /* r [61] */
        {115,}, /* s [62] */
        {116,}, /* t [63] */
        {117,}, /* u [64] */
        {118,}, /* v [65] */
        {119,}, /* w [66] */
        {120,}, /* x [67] */
        {121,}, /* y [68] */
        {122,}, /* z [69] */
        {39,}, /* ' [70] */
        {34,}, /* " [71] */
        {96,}, /* ` [72] */
        {126,}, /* ~ [73] */
        {32,}, /*   [74] */
        {33,}, /* ! [75] */
        {64,}, /* @ [76] */
        {35,}, /* # [77] */
        {36,}, /* $ [78] */
        {37,}, /* % [79] */
        {94,}, /* ^ [80] */
        {38,}, /* & [81] */
        {42,}, /* * [82] */
        {40,}, /* ( [83] */
        {41,}, /* ) [84] */
        {95,}, /* _ [85] */
        {43,}, /* + [86] */
        {61,}, /* = [87] */
        {45,}, /* - [88] */
};


uint8_t abc[256][256] = {};

//global vars end

void init_alphabet() {
/* 0 [1] */ abc[48][0] = 1;
/* 1 [2] */ abc[49][0] = 2;
/* 2 [3] */ abc[50][0] = 3;
/* 3 [4] */ abc[51][0] = 4;
/* 4 [5] */ abc[52][0] = 5;
/* 5 [6] */ abc[53][0] = 6;
/* 6 [7] */ abc[54][0] = 7;
/* 7 [8] */ abc[55][0] = 8;
/* 8 [9] */ abc[56][0] = 9;
/* 9 [10] */ abc[57][0] = 10;
/* а [11] */ abc[208][176] = 11;
/* б [12] */ abc[208][177] = 12;
/* в [13] */ abc[208][178] = 13;
/* г [14] */ abc[208][179] = 14;
/* д [15] */ abc[208][180] = 15;
/* е [16] */ abc[208][181] = 16;
/* ё [17] */ abc[209][145] = 17;
/* ж [18] */ abc[208][182] = 18;
/* з [19] */ abc[208][183] = 19;
/* и [20] */ abc[208][184] = 20;
/* й [21] */ abc[208][185] = 21;
/* к [22] */ abc[208][186] = 22;
/* л [23] */ abc[208][187] = 23;
/* м [24] */ abc[208][188] = 24;
/* н [25] */ abc[208][189] = 25;
/* о [26] */ abc[208][190] = 26;
/* п [27] */ abc[208][191] = 27;
/* р [28] */ abc[209][128] = 28;
/* с [29] */ abc[209][129] = 29;
/* т [30] */ abc[209][130] = 30;
/* у [31] */ abc[209][131] = 31;
/* ф [32] */ abc[209][132] = 32;
/* х [33] */ abc[209][133] = 33;
/* ц [34] */ abc[209][134] = 34;
/* ч [35] */ abc[209][135] = 35;
/* ш [36] */ abc[209][136] = 36;
/* щ [37] */ abc[209][137] = 37;
/* ъ [38] */ abc[209][138] = 38;
/* ы [39] */ abc[209][139] = 39;
/* ь [40] */ abc[209][140] = 40;
/* э [41] */ abc[209][141] = 41;
/* ю [42] */ abc[209][142] = 42;
/* я [43] */ abc[209][143] = 43;
/* a [44] */ abc[97][0] = 44;
/* b [45] */ abc[98][0] = 45;
/* c [46] */ abc[99][0] = 46;
/* d [47] */ abc[100][0] = 47;
/* e [48] */ abc[101][0] = 48;
/* f [49] */ abc[102][0] = 49;
/* g [50] */ abc[103][0] = 50;
/* h [51] */ abc[104][0] = 51;
/* i [52] */ abc[105][0] = 52;
/* j [53] */ abc[106][0] = 53;
/* k [54] */ abc[107][0] = 54;
/* l [55] */ abc[108][0] = 55;
/* m [56] */ abc[109][0] = 56;
/* n [57] */ abc[110][0] = 57;
/* o [58] */ abc[111][0] = 58;
/* p [59] */ abc[112][0] = 59;
/* q [60] */ abc[113][0] = 60;
/* r [61] */ abc[114][0] = 61;
/* s [62] */ abc[115][0] = 62;
/* t [63] */ abc[116][0] = 63;
/* u [64] */ abc[117][0] = 64;
/* v [65] */ abc[118][0] = 65;
/* w [66] */ abc[119][0] = 66;
/* x [67] */ abc[120][0] = 67;
/* y [68] */ abc[121][0] = 68;
/* z [69] */ abc[122][0] = 69;
/* ' [70] */ abc[39][0] = 70;
/* " [71] */ abc[34][0] = 71;
/* ` [72] */ abc[96][0] = 72;
/* ~ [73] */ abc[126][0] = 73;
/*   [74] */ abc[32][0] = 74;
/* ! [75] */ abc[33][0] = 75;
/* @ [76] */ abc[64][0] = 76;
/* # [77] */ abc[35][0] = 77;
/* $ [78] */ abc[36][0] = 78;
/* % [79] */ abc[37][0] = 79;
/* ^ [80] */ abc[94][0] = 80;
/* & [81] */ abc[38][0] = 81;
/* * [82] */ abc[42][0] = 82;
/* ( [83] */ abc[40][0] = 83;
/* ) [84] */ abc[41][0] = 84;
/* _ [85] */ abc[95][0] = 85;
/* + [86] */ abc[43][0] = 86;
/* = [87] */ abc[61][0] = 87;
/* - [88] */ abc[45][0] = 88;
}

void decode_string(uint8_t dst[], uint8_t src[]) {
    int j = 0;
    for (int i = 0; src[i]; i++) {
        for (int k = 0; bca[src[i]][k]; ++k) {
            dst[j++] = bca[src[i]][k];
        }
    }
}


void encode_string(uint8_t *dst, uint8_t *src) {
    if (abc[48][0] != 1) {
        init_alphabet();
    }

    int j = 0;
    for (int i = 0; src[i] != '\0'; i++) {
        if (abc[src[i]][0] != '\0' || src[i] == 48) { //code 48 is 0 char
            dst[j] = abc[src[i]][0];
        } else {
            dst[j] = abc[src[i]][src[i + 1]];
            i++;
        }
        j++;
    }
}


uint8_t char_bit_set(uint8_t bitmask, uint8_t bit) {
    bitmask |= 1 << bit - 1;
    return bitmask;
}

uint8_t char_bit_clear(uint8_t bitmask, uint8_t bit) {
    bitmask &= ~(1 << bit - 1);
    return bitmask;
}

uint8_t char_bit_get(uint8_t bitmask, uint8_t bit) {
    return (bitmask >> bit - 1) & 1;
}

uint8_t char_bit_count(uint8_t i, uint8_t length) {
    //if i == 0 return 0
    if (i == 0) {
        return 0;
    }
    //init precounted bits array
    if (precount[1] != 1) {
        precount_init();
    }
    //cut bitmask counter
    switch (length) {
        case 8:
            break;
        case 0:
            return 0;
        default:
            i &= precount_mask[length]; //replacement for bitmask count (2 << counter - 1) - 1
    }

    return precount[i];
}

uint32_t int32_bit_set(uint32_t bitmask, uint8_t bit) {
    bitmask |= 1 << bit - 1;
    return bitmask;
}

uint8_t int32_bit_get(uint32_t bitmask, uint8_t bit) {
    return (bitmask >> bit - 1) & 1;
}

uint32_t int32_bit_clear(uint32_t bitmask, uint8_t bit) {
    bitmask &= ~(1 << bit - 1);
    return bitmask;
}

uint8_t int32_bit_count(uint32_t i, uint8_t length) {
    //if i == 0 return 0
    if (i == 0) {
        return 0;
    }
    //init precounted bits array
    if (precount[1] != 1) {
        precount_init();
    }
    //cut bitmask counter
    if (length == 0) {
        return 0;
    } else {
        i &= (2 << length - 1) - 1;
    }
    return precount[i & 0xff] +
           precount[(i >> 8) & 0xff] +
           precount[(i >> 16) & 0xff] +
           precount[i >> 24];
}

uint8_t bit_get(uint8_t bitmask[], uint8_t bit) {
    //first we need to calculate char index in the bitmask array
    int i = (bit - 1) >> 3; //(bit-1)>>3 same as (bit-1)/8
    int rest = bit & 7; //same as bit % 8
    return rest ? CHAR_BIT_GET(bitmask[i], rest) : CHAR_BIT_GET(bitmask[i], 8);
}

void bit_set(uint8_t bitmask[], uint8_t bit) {
    //first we need to calculate char index in the bitmask array
    int i = (bit - 1) >> 3; //(bit-1)>>3 same as (bit-1)/8
    int rest = bit & 7; //same as bit % 8
    bitmask[i] = rest ? CHAR_BIT_SET(bitmask[i], rest) : CHAR_BIT_SET(bitmask[i], 8);
    return;
}

uint8_t bit_count(uint8_t bitmask[], uint8_t offset, uint8_t last_bit) {
    uint16_t res = 0;
    uint16_t i = offset >> 3;
    uint16_t i_max = last_bit >> 3;
    uint16_t head = offset & 7;
    uint16_t tail = last_bit & 7;

    //first partial byte
    if (head) {
        //if offset and last bit within the same byte
        if (i == i_max && tail) {
            res += char_bit_count(bitmask[i] & precount_offset_mask[head], tail);
            return res;
        }

        res += char_bit_count(bitmask[i] >> head, 8);
        ++i;
    }

    //full bytes
    for (; i < i_max; ++i) {
        res += char_bit_count(bitmask[i], 8);
    }

    //last partial byte
    if (tail) {
        res += char_bit_count(bitmask[i], tail);
    }

    return res;
}

void precount_init() {
    //fill precount array
    for (int i = 0; i < 256; i++) {
        precount[i] = (i & 1) + precount[i / 2];
    }
}

node_s *node_set(unsigned int id, node_s *_node, trie_s *trie) {
    //nodes is a global array
    memcpy(&trie->nodes[id], _node, sizeof(node_s));
    return &trie->nodes[id];
}

node_s *node_get(uint32_t node_id, trie_s *trie) {
    return &trie->nodes[node_id];
}

uint32_t trie_add(uint8_t string[], uint32_t parent_id, trie_s *trie) {
    //we save first char to the root node_s mask and create new node_s then second char to the just created node_s etc
    uint8_t indexes[MAX_WORD_LENGTH] = {};
    encode_string(indexes, string);

    for (int32_t i = 0; indexes[i]; ++i) {
        parent_id = trie_char_add(parent_id, indexes[i], trie);
    }
    //add last char flag for the last char
    trie->nodes[parent_id].mask[0] = CHAR_BIT_SET(trie->nodes[parent_id].mask[0], 1);
    return parent_id;
}

uint32_t trie_char_add(uint32_t parent_id, uint8_t char_index, trie_s *trie) {
    uint32_t next_node_id;
    uint8_t bits;
    uint8_t pos;
    node_s *parent_node = NODE_GET(parent_id, trie);

    //number of chars (bits) before the current char (bit) position in the alphabet
    //count the number of bits to determine the position of the char
    pos = bit_count(parent_node->mask, 1, char_index);

    //if char exists in the bitmask we need to get next node_s id from the references block
    //char_index + 1 because char_index 0 is 1 bit
    if (bit_get(parent_node->mask, char_index + 1)) {
        next_node_id = REF_GET(parent_node->ref_id, pos, trie);
    } else {
        //else we create new references block bigger than before to insert new reference
        next_node_id = node_insert_ref(char_index, pos, parent_node, trie);
    }

    return next_node_id;
}

uint32_t node_insert_ref(uint8_t char_index, uint8_t insert_pos, node_s *parent_node, trie_s *trie) {
    //first we need to calculate allocated memory for the current node_s references
    uint32_t size = bit_count(parent_node->mask, 1, MASK_BITS); //skip bit 1 (leaf flag)
    //old reference id if at least one bit was set
    uint32_t ref_id_old = parent_node->ref_id;

    //new size is bigger than old by one
    int32_t size_new = size + 1;

    //try to get ref_id from the deallocated array
    if (trie->deal[size_new] != NULL && trie->deal[size_new]->elements > 0) {
        parent_node->ref_id = list_shift(trie->deal[size_new]);
        //else allocate new ref_id
    } else {
        parent_node->ref_id = trie->refs_increment;
        trie->refs_increment += size_new;
    }

    //get next node_s id
    uint32_t next_node_id = trie->nodes_increment++;


    //if there is some memory allocated
    if (size > 0) {

        //save its id for the future reuse
        trie->deal[size] = list_push(trie->deal[size], NULL, 0, ref_id_old);

        //if insert position is not from the beginning (>0) copy already saved references to the just allocated ref_id
        if (insert_pos > 0) {
            //this is the part before current char
            memcpy(&trie->refs[parent_node->ref_id], &trie->refs[ref_id_old], insert_pos << REF_SIZE_POWER);
        }

        //if there is a references after the current insert position copy them too
        if (size > insert_pos) {
            //this is the part after current char
            memcpy(&trie->refs[parent_node->ref_id + insert_pos + 1], &trie->refs[ref_id_old + insert_pos],
                   (size - insert_pos) << REF_SIZE_POWER);
        }
    }
    //save current char next_node_id
    trie->refs[parent_node->ref_id + insert_pos] = next_node_id;


    //now save bit to the bitmask
    bit_set(parent_node->mask, char_index + 1);
    return next_node_id;

}

uint32_t ref_get(uint32_t ref_id, uint8_t pos, trie_s *trie) {
    return trie->refs[ref_id + pos];
}


void node_get_children(children_s *children, uint32_t node_id, trie_s *trie) {
    node_s *parent_node = NODE_GET(node_id, trie);

    //if mask is empty return NULL
    //skip first bit because 1 bit is an end word flag so we no need to count it
    children->length = bit_count(parent_node->mask, 1, MASK_BITS);
    //if there is no letters here
    if (children->length == 0) {
        //try to get leaf flag
        children->letters[0] = parent_node->mask[0];
        return;
    }

    //now we get children letters from the node mask
    mask_get_bits_raised_pre(children->letters, parent_node->mask);

    //now we get children nodes
    memcpy(children->nodes, &trie->refs[parent_node->ref_id], children->length << REF_SIZE_POWER);

}

void node_traverse(words_s *words, uint32_t node_id, string_s *head, trie_s *trie) {
    children_s children = {};
    node_get_children(&children, node_id, trie);


    //on the last node (leaf) save head to the res
    if (children.letters[0] == 1) {
        memcpy(&words->words[words->counter++], head->letters, head->length);
        //recurse call node traverse on each node_id
        for (uint32_t i = 0; i < children.length; ++i) {
            //now we need to create heads for each traverse
            string_s head2 = {};
            memcpy(&head2, head, sizeof(string_s));
            head2.letters[head2.length++] = children.letters[i + 1] - 1;
            node_traverse(words, children.nodes[i], &head2, trie);
        }
    } else {
        //recurse call node traverse on each node_id
        for (uint32_t i = 0; i < children.length; ++i) {
            //now we need to create heads for each traverse
            string_s head2 = {};
            memcpy(&head2, head, sizeof(string_s));
            head2.letters[head2.length++] = children.letters[i] - 1;
            node_traverse(words, children.nodes[i], &head2, trie);
        }
    }
}


//fill array res with bits of the bitmask
void mask_get_bits(uint8_t res[], uint8_t bitmask[]) {
    for (int8_t j = 0; j < MASK_INDEX; ++j) {
        for (int8_t i = 0; i < 8; ++i) {
            *res++ = bitmask[j] >> i & 1;
        }
    }
}

//to fill array res with numbers of raised bits of the bitmask
void mask_get_bits_raised(uint8_t res[], uint8_t bitmask[]) {
    for (int8_t j = 0; j < 12; ++j) {
        for (int8_t i = 0; i < 8; ++i) {
            if (bitmask[j] >> i & 1) {
                *res++ = (j << 3) + i + 1;
            }
        }
    }
}

//to fill array res with numbers of raised bits of the bitmask using precount2 array
void mask_get_bits_raised_pre(uint8_t res[], uint8_t bitmask[]) {
    for (int8_t i = 0; i < MASK_INDEX; ++i) {
        if (bitmask[i] == 0) {
            continue;
        } else {
            //if bitmask != 0
            uint8_t *byte = char_mask_get_bits_raised_pre(bitmask[i], i);
            uint8_t cnt = byte[0];
            memcpy(res, &byte[1], cnt);
            res += cnt; //shift res array address by cnt
        }
    }
}

//fill array res with raised bit of the bitmask
void char_mask_get_bits_raised(uint8_t res[], uint8_t bitmask) {
    for (int8_t i = 0; i < 8; ++i) {
        if (bitmask >> i & 1) {
            *res++ = i + 1;
        }
    }
}

//return a pointer to the array with raised bits of the bitmask. array[0] is a number of the raised bits
uint8_t *char_mask_get_bits_raised_pre(uint8_t bitmask, uint8_t index) {
    //init precounted bits array
    if (precount2[0][1][0] != 1) {
        precount_init2();
    }

    return precount2[index][bitmask];
}


//fill precount2 array with arrays of raised bits for the any 1 byte value
void precount_init2() {
    for (int i = 0; i < 256; ++i) {
        uint8_t res[8] = {};
        char_mask_get_bits_raised(res, i);
        //count number of bits raised
        uint8_t cnt = char_bit_count(i, 8);
        for (int j = 0; j < MASK_INDEX; ++j) {
            precount2[j][i][0] = cnt; //save number of bits raised
            for (int k = 0; res[k] && k < 8; ++k) {
                precount2[j][i][k + 1] = res[k] + (j << 3);
            }
        }
    }
}

void yatrie_free(trie_s *trie) {
    for (uint32_t i = 0; i < MAX_DEAL_SIZE; ++i) {
        list_free(trie->deal[i]);
    }
    free(trie);
}

uint32_t trie_get_id(uint8_t *word, uint32_t parent_id, trie_s *trie) {
    uint8_t indexes[MAX_WORD_LENGTH] = {};
    encode_string(indexes, word);
    node_s *parent_node = NODE_GET(parent_id, trie);

    for (uint32_t i = 0; indexes[i]; ++i) {
        //if char exists in the bitmask we need to get next node_s id from the references block
        //char_index + 1 because char_index 0 is 1 bit
        if (bit_get(parent_node->mask, indexes[i] + 1)) {
            //number of chars (bits) before the current char (bit) position in the alphabet
            //count the number of bits to determine the position of the char
            uint8_t pos = bit_count(parent_node->mask, 1, indexes[i]);
            parent_id = REF_GET(parent_node->ref_id, pos, trie);
            parent_node = NODE_GET(parent_id, trie);
        } else {
            return 0;
        }
    }
    return parent_id;
}

size_t yatrie_save(uint8_t *filepath, trie_s *trie) {
    FILE *file;
    file = fopen(filepath, "w");
    fwrite(&trie->nodes_increment, 1, sizeof(trie->nodes_increment), file);
    fwrite(&trie->refs_increment, 1, sizeof(trie->refs_increment), file);
    fwrite(trie->nodes, 1, sizeof(node_s) * trie->nodes_increment, file);
    fwrite(trie->refs, 1, REF_SIZE * trie->refs_increment, file);
    size_t size = sizeof(file);
    fclose(file);
    return size;
}

void yatrie_load(uint8_t *filepath, trie_s *trie) {
    FILE *file;
    file = fopen(filepath, "r");
    fread(&trie->nodes_increment, 1, sizeof(trie->nodes_increment), file);
    fread(&trie->refs_increment, 1, sizeof(trie->refs_increment), file);
    fread(trie->nodes, 1, sizeof(node_s) * trie->nodes_increment, file);
    fread(trie->refs, 1, REF_SIZE * trie->refs_increment, file);
    fclose(file);
}

//fill res array with the bits of the bitmask big-endian order (from the higher bit to the lower)
void char_mask_get_bits(uint8_t res[], uint8_t bitmask) {
    for (int8_t i = 7; i >= 0; --i) {
        *res++ = bitmask >> i & 1;
    }
}

