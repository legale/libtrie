/* file: libtrie.c
 *
 * libtrie - simple Trie data structure library v0.1.4
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
#include "precount.h"

static const unsigned char precount[256] = {PRECOUNT};
static const unsigned char precount16[65536] = {PRECOUNT16};

uint8_t static precount2[12][256][8] = {};
uint8_t static const precount_mask[8] = {0b0, 0b1, 0b11, 0b111, 0b1111, 0b11111, 0b111111, 0b1111111};
uint8_t static const precount_offset_mask[8] = {0b11111111, 0b11111110, 0b11111100, 0b11111000,
                                                0b11110000, 0b11100000, 0b11000000, 0b10000000};
uint32_t static const precount32_offset_mask[32] = {
        0b11111111111111111111111111111111, 0b11111111111111111111111111111110, 0b11111111111111111111111111111100,
        0b11111111111111111111111111111000, 0b11111111111111111111111111110000, 0b11111111111111111111111111100000,
        0b11111111111111111111111111000000, 0b11111111111111111111111110000000, 0b11111111111111111111111100000000,
        0b11111111111111111111111000000000, 0b11111111111111111111110000000000, 0b11111111111111111111100000000000,
        0b11111111111111111111000000000000, 0b11111111111111111110000000000000, 0b11111111111111111100000000000000,
        0b11111111111111111000000000000000, 0b11111111111111110000000000000000, 0b11111111111111100000000000000000,
        0b11111111111111000000000000000000, 0b11111111111110000000000000000000, 0b11111111111100000000000000000000,
        0b11111111111000000000000000000000, 0b11111111110000000000000000000000, 0b11111111100000000000000000000000,
        0b11111111000000000000000000000000, 0b11111110000000000000000000000000, 0b11111100000000000000000000000000,
        0b11111000000000000000000000000000, 0b11110000000000000000000000000000, 0b11100000000000000000000000000000,
        0b11000000000000000000000000000000, 0b10000000000000000000000000000000,
};

uint8_t bca[][3] = {
        {},     /*   [0] not used  */
        {},     /*   [1] reserverd */
        {48,}, /* 0 [2] */
        {49,}, /* 1 [3] */
        {50,}, /* 2 [4] */
        {51,}, /* 3 [5] */
        {52,}, /* 4 [6] */
        {53,}, /* 5 [7] */
        {54,}, /* 6 [8] */
        {55,}, /* 7 [9] */
        {56,}, /* 8 [10] */
        {57,}, /* 9 [11] */
        {208, 176,}, /* а [12] */
        {208, 177,}, /* б [13] */
        {208, 178,}, /* в [14] */
        {208, 179,}, /* г [15] */
        {208, 180,}, /* д [16] */
        {208, 181,}, /* е [17] */
        {209, 145,}, /* ё [18] */
        {208, 182,}, /* ж [19] */
        {208, 183,}, /* з [20] */
        {208, 184,}, /* и [21] */
        {208, 185,}, /* й [22] */
        {208, 186,}, /* к [23] */
        {208, 187,}, /* л [24] */
        {208, 188,}, /* м [25] */
        {208, 189,}, /* н [26] */
        {208, 190,}, /* о [27] */
        {208, 191,}, /* п [28] */
        {209, 128,}, /* р [29] */
        {209, 129,}, /* с [30] */
        {209, 130,}, /* т [31] */
        {209, 131,}, /* у [32] */
        {209, 132,}, /* ф [33] */
        {209, 133,}, /* х [34] */
        {209, 134,}, /* ц [35] */
        {209, 135,}, /* ч [36] */
        {209, 136,}, /* ш [37] */
        {209, 137,}, /* щ [38] */
        {209, 138,}, /* ъ [39] */
        {209, 139,}, /* ы [40] */
        {209, 140,}, /* ь [41] */
        {209, 141,}, /* э [42] */
        {209, 142,}, /* ю [43] */
        {209, 143,}, /* я [44] */
        {97,}, /* a [45] */
        {98,}, /* b [46] */
        {99,}, /* c [47] */
        {100,}, /* d [48] */
        {101,}, /* e [49] */
        {102,}, /* f [50] */
        {103,}, /* g [51] */
        {104,}, /* h [52] */
        {105,}, /* i [53] */
        {106,}, /* j [54] */
        {107,}, /* k [55] */
        {108,}, /* l [56] */
        {109,}, /* m [57] */
        {110,}, /* n [58] */
        {111,}, /* o [59] */
        {112,}, /* p [60] */
        {113,}, /* q [61] */
        {114,}, /* r [62] */
        {115,}, /* s [63] */
        {116,}, /* t [64] */
        {117,}, /* u [65] */
        {118,}, /* v [66] */
        {119,}, /* w [67] */
        {120,}, /* x [68] */
        {121,}, /* y [69] */
        {122,}, /* z [70] */
        {39,}, /* ' [71] */
        {34,}, /* " [72] */
        {96,}, /* ` [73] */
        {126,}, /* ~ [74] */
        {32,}, /*   [75] */
        {33,}, /* ! [76] */
        {64,}, /* @ [77] */
        {35,}, /* # [78] */
        {36,}, /* $ [79] */
        {37,}, /* % [80] */
        {94,}, /* ^ [81] */
        {38,}, /* & [82] */
        {42,}, /* * [83] */
        {40,}, /* ( [84] */
        {41,}, /* ) [85] */
        {95,}, /* _ [86] */
        {43,}, /* + [87] */
        {61,}, /* = [88] */
        {45,}, /* - [89] */
};


uint8_t abc[256][256] = {};

//global vars end

void init_alphabet() {
/* 0 [2] */ abc[48][0] = 2;
/* 1 [3] */ abc[49][0] = 3;
/* 2 [4] */ abc[50][0] = 4;
/* 3 [5] */ abc[51][0] = 5;
/* 4 [6] */ abc[52][0] = 6;
/* 5 [7] */ abc[53][0] = 7;
/* 6 [8] */ abc[54][0] = 8;
/* 7 [9] */ abc[55][0] = 9;
/* 8 [10] */ abc[56][0] = 10;
/* 9 [11] */ abc[57][0] = 11;
/* а [12] */ abc[208][176] = 12;
/* б [13] */ abc[208][177] = 13;
/* в [14] */ abc[208][178] = 14;
/* г [15] */ abc[208][179] = 15;
/* д [16] */ abc[208][180] = 16;
/* е [17] */ abc[208][181] = 17;
/* ё [18] */ abc[209][145] = 18;
/* ж [19] */ abc[208][182] = 19;
/* з [20] */ abc[208][183] = 20;
/* и [21] */ abc[208][184] = 21;
/* й [22] */ abc[208][185] = 22;
/* к [23] */ abc[208][186] = 23;
/* л [24] */ abc[208][187] = 24;
/* м [25] */ abc[208][188] = 25;
/* н [26] */ abc[208][189] = 26;
/* о [27] */ abc[208][190] = 27;
/* п [28] */ abc[208][191] = 28;
/* р [29] */ abc[209][128] = 29;
/* с [30] */ abc[209][129] = 30;
/* т [31] */ abc[209][130] = 31;
/* у [32] */ abc[209][131] = 32;
/* ф [33] */ abc[209][132] = 33;
/* х [34] */ abc[209][133] = 34;
/* ц [35] */ abc[209][134] = 35;
/* ч [36] */ abc[209][135] = 36;
/* ш [37] */ abc[209][136] = 37;
/* щ [38] */ abc[209][137] = 38;
/* ъ [39] */ abc[209][138] = 39;
/* ы [40] */ abc[209][139] = 40;
/* ь [41] */ abc[209][140] = 41;
/* э [42] */ abc[209][141] = 42;
/* ю [43] */ abc[209][142] = 43;
/* я [44] */ abc[209][143] = 44;
/* a [45] */ abc[97][0] = 45;
/* b [46] */ abc[98][0] = 46;
/* c [47] */ abc[99][0] = 47;
/* d [48] */ abc[100][0] = 48;
/* e [49] */ abc[101][0] = 49;
/* f [50] */ abc[102][0] = 50;
/* g [51] */ abc[103][0] = 51;
/* h [52] */ abc[104][0] = 52;
/* i [53] */ abc[105][0] = 53;
/* j [54] */ abc[106][0] = 54;
/* k [55] */ abc[107][0] = 55;
/* l [56] */ abc[108][0] = 56;
/* m [57] */ abc[109][0] = 57;
/* n [58] */ abc[110][0] = 58;
/* o [59] */ abc[111][0] = 59;
/* p [60] */ abc[112][0] = 60;
/* q [61] */ abc[113][0] = 61;
/* r [62] */ abc[114][0] = 62;
/* s [63] */ abc[115][0] = 63;
/* t [64] */ abc[116][0] = 64;
/* u [65] */ abc[117][0] = 65;
/* v [66] */ abc[118][0] = 66;
/* w [67] */ abc[119][0] = 67;
/* x [68] */ abc[120][0] = 68;
/* y [69] */ abc[121][0] = 69;
/* z [70] */ abc[122][0] = 70;
/* ' [71] */ abc[39][0] = 71;
/* " [72] */ abc[34][0] = 72;
/* ` [73] */ abc[96][0] = 73;
/* ~ [74] */ abc[126][0] = 74;
/*   [75] */ abc[32][0] = 75;
/* ! [76] */ abc[33][0] = 76;
/* @ [77] */ abc[64][0] = 77;
/* # [78] */ abc[35][0] = 78;
/* $ [79] */ abc[36][0] = 79;
/* % [80] */ abc[37][0] = 80;
/* ^ [81] */ abc[94][0] = 81;
/* & [82] */ abc[38][0] = 82;
/* * [83] */ abc[42][0] = 83;
/* ( [84] */ abc[40][0] = 84;
/* ) [85] */ abc[41][0] = 85;
/* _ [86] */ abc[95][0] = 86;
/* + [87] */ abc[43][0] = 87;
/* = [88] */ abc[61][0] = 88;
/* - [89] */ abc[45][0] = 89;
}

void decode_string(uint8_t dst[], uint8_t src[]) {
    int j = 0;
    for (int i = 0; src[i]; i++) {
        for (int k = 0; bca[src[i]][k]; ++k) {
            dst[j++] = bca[src[i]][k];
        }
    }
    dst[j] = '\0'; //terminator
}

void encode_string(uint8_t *dst, uint8_t *src) {
    if (abc[48][0] != 1) {
        init_alphabet();
    }

    int j = 0;
    for (int i = 0; src[i] != '\0'; i++) {
        if (abc[src[i]][0] != '\0') {
            dst[j] = abc[src[i]][0];
        } else {
            dst[j] = abc[src[i]][src[i + 1]];
            i++;
        }
        j++;
    }
}

uint8_t char_bit_count(uint8_t i, uint8_t length) {
    //if i == 0 return 0
    if (i == 0) {
        return 0;
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

uint8_t int32_bit_count(uint32_t i, uint8_t length) {
    //if i == 0 return 0
    if (i == 0) {
        return 0;
    }

    //cut bitmask counter
    if (UNLIKELY(length == 0)) {
        return 0;
    } else {
        i &= (2 << (length - 1)) - 1;
    }
    return precount16[i & 0xffff] + precount16[(i >> 16) & 0xffff];
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

        res += CHAR_BIT_COUNT(bitmask[i] >> head);
        ++i;
    }

    //full bytes
    for (; i < i_max; ++i) {
        res += CHAR_BIT_COUNT(bitmask[i]);
    }

    //last partial byte
    if (tail) {
        res += char_bit_count(bitmask[i], tail);
    }

    return res;
}

uint8_t bit_int32_count(uint32_t bitmask[], uint8_t offset, uint8_t last_bit) {
    uint16_t res = 0;
    uint16_t i = offset >> 5;
    uint16_t i_max = last_bit >> 5;
    uint16_t head = offset & 31;
    uint16_t tail = last_bit & 31;

    //first partial byte
    if (head) {
        //if offset and last bit within the same int
        if (i == i_max && tail) {
            res += int32_bit_count(bitmask[i] & precount32_offset_mask[head], tail);
            return res;
        }

        uint32_t bitmask_tmp = bitmask[i] >> head; //tmp variable to shift bitmask only once
        res += INT32_BIT_COUNT(bitmask_tmp);
        ++i;
    }

    //full int
    for (; i < i_max; ++i) {
        res += INT32_BIT_COUNT(bitmask[i]);
    }

    //last partial int
    if (tail) {
        res += int32_bit_count(bitmask[i], tail);
    }

    return res;
}

uint32_t yatrie_add(uint8_t *string, uint32_t parent_id, trie_s *trie) {
    //we save first char to the root node_s mask and create new node_s then second char to the just created node_s etc
    uint8_t indexes[MAX_WORD_LENGTH] = {};
    encode_string(indexes, string);

    for (int32_t i = 0; indexes[i]; ++i) {
        parent_id = yatrie_char_add(parent_id, indexes[i], trie);
    }
    //add last char flag for the last char
    trie->nodes->data[parent_id].mask[0] = BIT_SET(trie->nodes->data[parent_id].mask[0], 1);
    return parent_id;
}

uint32_t yatrie_char_add(uint32_t parent_id, uint8_t char_index, trie_s *trie) {
    uint32_t next_node_id;
    uint8_t bits;
    uint8_t pos;
    node_s *parent_node = NODE_GET(parent_id, trie);

    //number of chars (bits) before the current char (bit) position in the alphabet
    //count the number of bits to determine the position of the char
    pos = bit_int32_count(parent_node->mask, 1, char_index - 1);

    //if char exists in the bitmask we need to get next node_s id from the references block
    if (BIT_ARRAY_GET(parent_node->mask, char_index)) {
        next_node_id = REF_GET(parent_node->ref_id, pos, trie);
    } else {
        //else we create new references block bigger than before to insert new reference
        next_node_id = node_insert_ref(char_index, pos, parent_node, trie);
    }

    return next_node_id;
}

uint32_t node_insert_ref(uint8_t char_index, uint8_t insert_pos, node_s *parent_node, trie_s *trie) {
    //first we need to calculate allocated memory for the current node_s references
    uint32_t size = bit_int32_count(parent_node->mask, 1, MASK_BITS); //skip bit 1 (leaf flag)
    //old reference id may need in future
    uint32_t ref_id_old = parent_node->ref_id;
    //new size is bigger than old by one
    int32_t size_new = size + 1;

    //try to get ref_id from the deallocated array
    if (trie->refs_free->data[size_new] != NULL && trie->refs_free->data[size_new]->elements > 0) {
        parent_node->ref_id = (ref_t) list_shift(trie->refs_free->data[size_new]);
        //else allocate new ref_id
    } else {
        parent_node->ref_id = trie->refs->increment;
        trie->refs->increment += size_new;
    }

    //get next node_s id
    uint32_t next_node_id = trie->nodes->increment++;


    //if there is some memory allocated
    if (size > 0) {

        //save its id for the future reuse
        trie->refs_free->data[size] = list_push(trie->refs_free->data[size], NULL, 0, ref_id_old);

        //if insert position is not from the beginning (>0) copy already saved references to the just allocated ref_id
        if (insert_pos > 0) {
            //this is the part before current char
            memcpy(&trie->refs->data[parent_node->ref_id], &trie->refs->data[ref_id_old], insert_pos << REF_SIZE_POWER);
        }

        //if there is a references after the current insert position copy them too
        if (size > insert_pos) {
            //this is references part after current char
            memcpy(&trie->refs->data[parent_node->ref_id + insert_pos + 1], &trie->refs->data[ref_id_old + insert_pos],
                   (size - insert_pos) << REF_SIZE_POWER);
        }
    }
    //save current char next_node_id
    trie->refs->data[parent_node->ref_id + insert_pos] = next_node_id;


    //now save bit to the bitmask
    BIT_ARRAY_SET(parent_node->mask, char_index);
    return next_node_id;

}


void node_get_children(children_s *children, uint32_t node_id, trie_s *trie) {
    node_s *parent_node = NODE_GET(node_id, trie);

    //if mask is empty return NULL
    //skip first bit because 1 bit is an end word flag so we no need to count it
    children->length = bit_int32_count(parent_node->mask, 1, MASK_BITS);
    //if there is no letters here
    if (children->length == 0) {
        //try to get leaf flag
        children->letters[0] = parent_node->mask[0];
        return;
    }

    //now we get children letters from the node mask
    char_mask_get_bits_raised_pre(children->letters, (uint8_t *) parent_node->mask);

    //now we get children nodes
    memcpy(&children->nodes, &trie->refs->data[parent_node->ref_id], children->length << REF_SIZE_POWER);

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
            head2.letters[head2.length++] = children.letters[i + 1]; // +1 to skip first letter with bit 0
            node_traverse(words, children.nodes[i], &head2, trie);
        }
    } else {
        //recurse call node traverse on each node_id
        for (uint32_t i = 0; i < children.length; ++i) {
            //now we need to create heads for each traverse
            string_s head2 = {};
            memcpy(&head2, head, sizeof(string_s));
            head2.letters[head2.length++] = children.letters[i];
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
void char_mask_get_bits_raised_pre(uint8_t *res, uint8_t *bitmask) {
    memset(res, 0, MASK_BITS);
    for (int8_t i = 0; i < 12; ++i) {
        if (bitmask[i] == 0) {
            continue;
        } else {
            //if bitmask != 0
            uint8_t *raised = char_get_bits_raised_pre(bitmask[i], i);
            uint8_t cnt = CHAR_BIT_COUNT(bitmask[i]);
            memcpy(res, raised, cnt);
            res += cnt; //shift res array address by cnt
        }
    }
}


//fill array res with raised bit of the bitmask
void char_get_bits_raised(uint8_t *res, uint8_t bitmask) {
    if (bitmask & 1) *res++ = 1;
    if (bitmask >> 1 & 1) *res++ = 2;
    if (bitmask >> 2 & 1) *res++ = 3;
    if (bitmask >> 3 & 1) *res++ = 4;
    if (bitmask >> 4 & 1) *res++ = 5;
    if (bitmask >> 5 & 1) *res++ = 6;
    if (bitmask >> 6 & 1) *res++ = 7;
    if (bitmask >> 7 & 1) *res = 8;
}

//fill array res with raised bit of the bitmask
void int32_get_bits_raised(uint8_t *res, uint32_t bitmask) {
    memset(res, 0, 32);
    for (int8_t i = 0; i < 32; ++i) {
        if (bitmask >> i & 1) {
            *res++ = i + 1;
        }
    }
}

//return a pointer to the array with raised bits of the bitmask. array[0] value is a number of the raised bits
uint8_t *char_get_bits_raised_pre(uint8_t bitmask, uint8_t index) {
    //init precounted bits array
    if (UNLIKELY(precount2[0][1][0] != 1)) {
        precount_char_init();
    }

    return precount2[index][bitmask];
}


//fill precount2 array with arrays of raised bits for the any 1 byte value
void precount_char_init() {
    for (uint8_t i = 0; i < 256; ++i) { //each byte value cycle
        uint8_t res[8] = {};
        char_get_bits_raised(res, i);
        for (uint8_t j = 0, k = 0; j < 12; ++j) { //each bitmask index value cycle
                //this construction works even on -O3 and -Ofast
                if(res[k]) precount2[j][i][k] = res[k++] + (j << 3);
                if(res[k]) precount2[j][i][k] = res[k++] + (j << 3);
                if(res[k]) precount2[j][i][k] = res[k++] + (j << 3);
                if(res[k]) precount2[j][i][k] = res[k++] + (j << 3);
                if(res[k]) precount2[j][i][k] = res[k++] + (j << 3);
                if(res[k]) precount2[j][i][k] = res[k++] + (j << 3);
                if(res[k]) precount2[j][i][k] = res[k++] + (j << 3);
                if(res[k]) precount2[j][i][k] = res[k] + (j << 3);
        }
    }
}


uint32_t trie_get_id(uint8_t *word, uint32_t parent_id, trie_s *trie) {
    uint8_t indexes[MAX_WORD_LENGTH] = {};
    encode_string(indexes, word);
    node_s *parent_node = NODE_GET(parent_id, trie);

    for (uint32_t i = 0; indexes[i]; ++i) {
        //if char exists in the bitmask we need to get next node_s id from the references block
        if (LIKELY(BIT_ARRAY_GET(parent_node->mask, indexes[i]))) {
            //number of chars (bits) before the current char (bit) position in the alphabet
            //count the number of bits before char to determine its position
            uint8_t pos = bit_int32_count(parent_node->mask, 1, indexes[i] - 1);
            parent_id = REF_GET(parent_node->ref_id, pos, trie);
            parent_node = NODE_GET(parent_id, trie);
        } else {
            return 0;
        }
    }
    return parent_id;
}

size_t yatrie_save(uint8_t *filepath, trie_s *trie) {
    FILE *file = fopen(filepath, "w");

    //refs_free block prepare
    ref_t ref_ids[1000];
    uint32_t j = 0;
    for (int i = 0; i < trie->refs_free->size; ++i) {
        if (trie->refs_free->data[i] != NULL) {
            while (trie->refs_free->data[i]->elements > 0) {
                ref_ids[j++] = (ref_t) list_shift(trie->refs_free->data[i]);
            }
            //terminator
            ref_ids[j++] = '-';
        }
    }
    trie->refs_free->disk_size = j * sizeof(ref_t);


    //trie structure
    fwrite(trie, sizeof(trie_s), 1, file);

    //refs_free_s *refs_free;
    fwrite(trie->refs_free, sizeof(refs_free_s), 1, file);

    //nodes_s *nodes;
    fwrite(trie->nodes, sizeof(nodes_s), 1, file);

    //refs_s *refs;
    fwrite(trie->refs, sizeof(refs_s), 1, file);

    //data
    fwrite(trie->nodes, sizeof(nodes_s) + trie->nodes->increment * sizeof(node_s), 1, file);
    fwrite(trie->refs, sizeof(refs_s) + trie->refs->increment * sizeof(ref_t), 1, file);
    fwrite(&ref_ids, trie->refs_free->disk_size, 1, file);

    //list_s *nodes_free;
    /* Todo: extract data from linked lists (nodes_free refs_free) and fwrite it*/

    size_t size = sizeof(file);
    fclose(file);
    return size;
}

trie_s *yatrie_load(uint8_t *filepath) {
    FILE *file;
    file = fopen(filepath, "r");
    //1 allocate trie_s memory
    trie_s *trie = (trie_s *) malloc(sizeof(trie_s));
    //trie_s *trie;
    fread(trie, sizeof(trie_s), 1, file);
    //refs_free_s *refs_free;
    refs_free_s refs_free_tmp = {};
    fread(&refs_free_tmp, sizeof(refs_free_s), 1, file);
    //nodes_s *nodes;
    nodes_s nodes_tmp = {};
    fread(&nodes_tmp, sizeof(nodes_s), 1, file);
    //refs_s *refs;
    refs_s refs_tmp = {};
    fread(&refs_tmp, sizeof(refs_s), 1, file);

    //load with data
    //3 allocate memory for nodes
    //we use calloc() because we need clear mask on new nodes
    trie->nodes = (nodes_s *) calloc(1, sizeof(nodes_s) + sizeof(node_s) * nodes_tmp.size);
    fread(trie->nodes, sizeof(nodes_s) + sizeof(node_s) * nodes_tmp.increment, 1, file);

    //4 allocate memory for refs
    trie->refs = (refs_s *) malloc(sizeof(refs_s) + sizeof(ref_t) * refs_tmp.size);
    fread(trie->refs, sizeof(refs_s) + sizeof(ref_t) * refs_tmp.increment, 1, file);

    //2 allocate memory for deallocated refs block
    trie->refs_free = (refs_free_s *) calloc(1, sizeof(refs_free_s) + sizeof(list_s *) * refs_free_tmp.size);
    trie->refs_free->size = refs_free_tmp.size;


    //refs_free block prepare
    ref_t ref_ids[1000] = {}; //array to read data from disk
    fread(&ref_ids, refs_free_tmp.disk_size, 1, file);

    uint32_t last = refs_free_tmp.disk_size / sizeof(ref_t); //last used index
    uint32_t len = 0; //reference block length
    //cycle array, on each '-' sign increment len
    for (int i = 0; i < last; ++i) {
        if (UNLIKELY(ref_ids[i] != '-')) { //minus sign is a terminator symbol
            trie->refs_free->data[len] = list_push(trie->refs_free->data[len], NULL, 0, ref_ids[i]);
        } else {
            ++len;
        }
    }


    /* Todo: nodes_free load*/
    fclose(file);
    return trie;
}


//create new trie
trie_s *yatrie_new(uint32_t max_nodes, uint32_t max_refs, uint32_t max_deallocated_size) {
    //1 allocate trie_s memory
    trie_s *trie = (trie_s *) calloc(1, sizeof(trie_s));

    //2 allocate memory for deallocated refs block
    refs_free_s *refs_free = (refs_free_s *) (calloc(1, sizeof(refs_free_s) + sizeof(list_s *) * max_deallocated_size));
    refs_free->size = max_deallocated_size;

    //3 allocate memory for nodes
    //we use calloc() because we need clear mask on new nodes
    nodes_s *nodes = (nodes_s *) (calloc(1, sizeof(nodes_s) + sizeof(node_s) * max_nodes));
    nodes->size = max_nodes;
    nodes->increment = 1; //node 0 is the root node

    //4 allocate memory for refs
    refs_s *refs = (refs_s *) (malloc(sizeof(refs_s) + sizeof(ref_t) * max_refs));
    refs->size = max_refs;
    refs->increment = 0;


    //assign allocated structures to the trie structure
    trie->nodes = nodes;
    trie->refs = refs;
    trie->refs_free = refs_free;

    return trie;
}

void yatrie_free(trie_s *trie) {
    if (UNLIKELY(trie == NULL)) {
        return;
    }


    //2 deallocate memory for deallocated refs block
    for (uint32_t i = 0; i < trie->refs_free->size; ++i) {
        list_free(trie->refs_free->data[i]);
    }
    free(trie->refs_free);

    //4 deallocate memory for refs
    free(trie->refs);

    //3 deallocate memory for nodes
    free(trie->nodes);

    //1 deallocate trie_s memory
    free(trie);
    trie = NULL;
    return;
}

//fill res array with the bits of the bitmask big-endian order (from the higher bit to the lower)
void char_mask_get_bits(uint8_t res[], uint8_t bitmask) {
    for (int8_t i = 7; i >= 0; --i) {
        *res++ = bitmask >> i & 1;
    }
}
