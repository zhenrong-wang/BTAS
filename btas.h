/**
 * 
 * This code is distributed under the license: MIT License
 * Originally written by Zhenrong WANG
 * mailto: zhenrongwang@live.com 
 * GitHub: https://github.com/zhenrong-wang
 * 
 */

#ifndef BTAS_H_
#define BTAS_H_

#include <stdint.h> /* C99 is required. */

/**
 * If the header failed to get included. Define the types by typedefs.
 * 
 * PORTABILITY WARNING: we assert the widths below, meaning that the portability
 * is not fully-guaranteed! 
 * PLEASE DO CHECK THE WIDTH AND MODIFY THE typedefs ACCORDINGLY!
 */
#ifndef _STDINT_H
typedef unsigned long long  uint64_t;
typedef signed long long    int64_t;
typedef unsigned char       uint8_t;
typedef signed char         int8_t;
    #if defined(__GNUC__) || defined(__clang__)
        #if __SIZEOF_INT__ < 4
        typedef unsigned long       uint32_t;
        typedef signed long         int32_t;
        typedef unsigned int        uint16_t;
        typedef signed int          int16_t;
        #else
        typedef unsigned int        uint32_t;
        typedef signed int          int32_t;
        typedef unsigned short      uint16_t;
        typedef signed short        int16_t;
        #endif
    #else
    typedef unsigned int        uint32_t;
    typedef signed int          int32_t;
    typedef unsigned short      uint16_t;
    typedef signed short        int16_t;
    #endif
#endif

/**
 * Section A. Auxiliary functions:
 *  - Convert a string to an unsigned 32bit number
 *  - Print out an array
 *  - Compare 2 arrays
 *  - Generate a RANDOM input signed integer array
 *  - Generate a GROWING input signed integer array
 */
int string_to_u64_num(const char* string, uint64_t *unsigned_num);
void print_arr(const uint32_t *arr, uint64_t num_elems, uint64_t max_elems);
int compare_arr(const uint32_t *arr_a, const uint32_t *arr_b, uint64_t num_elems);
int generate_random_input_arr(uint32_t *arr, uint64_t num_elems, uint32_t rand_max);
int generate_growing_arr(uint32_t *arr, uint64_t num_elems);
int cmd_flag_parser(int argc, char **argv, const char *cmd_flag);

/**
 * Section B. Brute and Brute-Opt algorithms
 */
uint32_t* fui_brute(const uint32_t *input_arr, const uint64_t num_elems, uint64_t *num_elems_out, int *err_flag);
uint64_t fui_brute_count(const uint32_t *input_arr, const uint64_t num_elems, int *err_flag);
uint32_t* fui_brute_opt(const uint32_t *input_arr, const uint64_t num_elems, uint64_t *num_elems_out, int *err_flag);
uint64_t fui_brute_opt_count(const uint32_t *input_arr, const uint64_t num_elems, int *err_flag);


/**
 * Section C. HASH_TABLE algorithms. 
 * 
 * These algorithms are based on hash table construction. They are time
 * efficient but not memory efficient (space optimized)
 * 
 */
#define HT_STEM_SIZE 65536
#define HT_BRANCH_SIZE 65536
#define HT_DYN_INI_SIZE 32

typedef struct {
    uint32_t branch_size;
    uint8_t *ptr_branch;
} htable_base;

void free_hash_table(uint8_t *hash_table[], uint32_t num_elems);
void free_hash_table_new(htable_base hash_table_new[], uint32_t num_elems);

uint32_t* fui_htable(const uint32_t *input_arr, const uint64_t num_elems, uint64_t *num_elems_out, int *err_flag);
uint64_t fui_htable_count(const uint32_t *input_arr, const uint64_t num_elems, int *err_flag);

uint32_t* fui_htable_new(const uint32_t *input_arr, const uint64_t num_elems, uint64_t *num_elems_out, int *err_flag);
uint64_t fui_htable_new_count(const uint32_t *input_arr, const uint64_t num_elems, int *err_flag);

uint32_t* fui_htable_dyn(const uint32_t *input_arr, const uint64_t num_elems, uint64_t *num_elems_out, int *err_flag);
uint64_t fui_htable_dyn_count(const uint32_t *input_arr, const uint64_t num_elems, int *err_flag);

/**
 * Section D. BitTree Algorithms.
 * 
 * The main algorithms recommended and introduced with both time and space
 * efficiency.
 * 
 */
#define BITMAP_BRANCH_SIZE  8192
#define BITMAP_INIT_LENGTH  1024
#define BITMAP_LENGTH_MAX   65536
#define IDX_ADJ_BRCH_SIZE   65536
#define BITMAP_BRCH_TREE    65536

struct dup_idx_struct {
    uint64_t index_a;
    uint64_t index_b;
    struct dup_idx_struct *ptr_next;
};

typedef struct dup_idx_struct dup_idx_list;

typedef struct {
    uint32_t out_elem;
    uint64_t raw_index;
} out_idx;

typedef struct {
    uint32_t branch_size;
    uint8_t *ptr_branch;
} bitmap_base;

/* Adjacent index hashtable */
typedef struct {
    uint32_t branch_size;
    uint8_t *ptr_branch;
} idx_ht_8;

typedef struct {
    uint32_t branch_size;
    uint16_t *ptr_branch;
} idx_ht_16;

typedef struct {
    uint32_t branch_size;
    uint32_t *ptr_branch;
} idx_ht_32;

typedef struct {
    uint32_t branch_size;
    uint64_t *ptr_branch;
} idx_ht_64;

#define flip_bit(byte_a, bit_position) ((byte_a) |= (0x80 >> (bit_position)))
#define check_bit(byte_a, bit_position) ((byte_a) & (0x80 >> (bit_position)))

void free_dup_idx_list(dup_idx_list *dup_idx_head);
int insert_dup_idx_list(dup_idx_list **dup_idx_head, uint64_t idx_a, uint64_t idx_b);
void print_dup_idx_list(dup_idx_list *dup_idx_head, uint64_t max_nodes);
void print_out_idx(out_idx *output_index, uint64_t num_elems, uint64_t max_elems);

void free_bitmap(bitmap_base *bitmap_head, uint32_t num_elems);
void free_idx_ht_8(idx_ht_8 *idx_ht_head, uint32_t num_elems);
void free_idx_ht_16(idx_ht_16 *idx_ht_head, uint32_t num_elems);
void free_idx_ht_32(idx_ht_32 *idx_ht_head, uint32_t num_elems);

uint32_t* fui_bitmap_stc(const uint32_t *input_arr, const uint64_t num_elems, uint64_t *num_elems_out, int *err_flag);
uint64_t fui_bitmap_stc_count(const uint32_t *input_arr, const uint64_t num_elems, int *err_flag);
uint32_t* fui_bitmap_dyn(const uint32_t *input_arr, const uint64_t num_elems, uint64_t *num_elems_out, int *err_flag);
uint64_t fui_bitmap_dyn_count(const uint32_t *input_arr, const uint64_t num_elems, int *err_flag);
out_idx* fui_bitmap_idx(const uint32_t *input_arr, const uint64_t num_elems, uint64_t *num_elems_out, int *err_flag, dup_idx_list **dup_idx_head);

#endif