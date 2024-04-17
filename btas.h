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
int string_to_u32_num(const char* string, uint32_t *unsigned_num);
void print_arr(const int32_t *arr, uint32_t num_elems, uint32_t max_elems);
int compare_arr(const int32_t *arr_a, const int32_t *arr_b, uint32_t num_elems);
int generate_random_input_arr(int32_t *arr, uint32_t num_elems, uint32_t rand_max);
int generate_growing_arr(int32_t *arr, uint32_t num_elems);

/**
 * Section B. Brute and Brute-Opt algorithms
 */
int32_t* fui_brute(const int32_t *input_arr, const uint32_t num_elems, uint32_t *num_elems_out, int *err_flag);
int32_t* fui_brute_opt(const int32_t *input_arr, const uint32_t num_elems, uint32_t *num_elems_out, int *err_flag);

/**
 * Section C. HASH_TABLE algorithms. 
 * 
 * These algorithms are based on hash table construction. They are time
 * efficient but not memory efficient (space optimized)
 * 
 */
#define HASH_TABLE_SIZE 32769
#define MOD_TABLE_SIZE  65536
#define HT_DYN_INI_SIZE 32

typedef struct {
    uint32_t branch_size_p;
    uint32_t branch_size_n;
    uint8_t *ptr_branch_p;
    uint8_t *ptr_branch_n;
} htable_base;

void free_hash_table(uint8_t *hash_table[], uint32_t num_elems);
void free_hash_table_new(htable_base hash_table_new[], uint32_t num_elems);
int32_t* fui_htable_dtree(const int32_t *input_arr, const uint32_t num_elems, uint32_t *num_elems_out, int *err_flag);
int32_t* fui_htable_stree(const int32_t *input_arr, const uint32_t num_elems, uint32_t *num_elems_out, int *err_flag);
int32_t* fui_htable_stree_dyn(const int32_t *input_arr, const uint32_t num_elems, uint32_t *num_elems_out, int *err_flag);

/**
 * Section D. BitTree Algorithms.
 * 
 * The main algorithms recommended and introduced with both time and space
 * efficiency.
 * 
 */
#define NEGATIVE_START_POS  8192
#define BIT_MOD_TABLE_SIZE  16384
#define BIT_MOD_DIV_FACTOR  65536
#define BITMAP_INIT_LENGTH  1024
#define BITMAP_LENGTH_MAX   32769

#define UINT_16_MAX         65536
#define UINT_8_MAX          256
#define IDX_ADJ_BRCH_SIZE   65536
#define BITMAP_BRCH_DTREE   65536

struct dup_idx_struct {
    uint32_t index_a;
    uint32_t index_b;
    struct dup_idx_struct *ptr_next;
};

typedef struct dup_idx_struct       dup_idx_list;

typedef struct {
    int32_t out_elem;
    uint32_t raw_index;
} out_idx;

typedef struct {
    uint32_t branch_size;
    uint8_t *ptr_branch;
} bitmap_base;

typedef struct {
    uint16_t branch_size[2];
    uint8_t *ptr_branch[2];
} bitmap_dtree;

/* Adjacent index hashtable */
typedef struct {
    uint16_t branch_size[2];
    uint8_t *ptr_branch[2];
} idx_ht_8;

typedef struct {
    uint16_t branch_size[2];
    uint16_t *ptr_branch[2];
} idx_ht_16;

typedef struct {
    uint16_t branch_size[2];
    uint32_t *ptr_branch[2];
} idx_ht_32;

#define flip_bit(byte_a, bit_position) ((byte_a) |= (0x80 >> (bit_position)))
#define check_bit(byte_a, bit_position) ((byte_a) & (0x80 >> (bit_position)))

void free_dup_idx_list(dup_idx_list *dup_idx_head);
int insert_dup_idx_list(dup_idx_list **dup_idx_head, uint32_t idx_a, uint32_t idx_b);
void print_dup_idx_list(dup_idx_list *dup_idx_head, uint32_t max_nodes);
void print_out_idx(out_idx *output_index, uint32_t num_elems, uint32_t max_elems);

void free_bitmap(bitmap_base *bitmap_head, uint16_t num_elems);
void free_bitmap_dtree(bitmap_dtree *bitmap_head, uint16_t num_elems);
void free_idx_ht_8(idx_ht_8 *idx_ht_head, uint16_t num_elems);
void free_idx_ht_16(idx_ht_16 *idx_ht_head, uint16_t num_elems);
void free_idx_ht_32(idx_ht_32 *idx_ht_head, uint16_t num_elems);

int32_t* fui_bitmap_stc_stree(const int32_t *input_arr, const uint32_t num_elems, uint32_t *num_elems_out, int *err_flag);
int32_t* fui_bitmap_dyn_stree(const int32_t *input_arr, const uint32_t num_elems, uint32_t *num_elems_out, int *err_flag);
int32_t* fui_bitmap_dyn_dtree(const int32_t *input_arr, const uint32_t num_elems, uint32_t *num_elems_out, int *err_flag);
out_idx* fui_bitmap_idx_stree(const int32_t *input_arr, const uint32_t num_elems, uint32_t *num_elems_out, int *err_flag, dup_idx_list **dup_idx_head);
out_idx* fui_bitmap_idx_dtree(const int32_t *input_arr, const uint32_t num_elems, uint32_t *num_elems_out, int *err_flag, dup_idx_list **dup_idx_head);


/**
 * Extending the algo to 64bit or even longer input element. Quite Challanging.
 * 
 
 typedef struct {
    int out_elem;
    uint32_t raw_index;
} out_idx_i64;

int assemble_h32(int_64bit a);
int assemble_l32(int_64bit a);

int hash_64_to_32(int_64bit in64);
int_64bit* transform_32_to_64_arr(const int *arr_input_32, uint32_t num_elems, const char *option);
out_idx_i64* fui_bitmap_dtree_idx_64(const int_64bit *input_arr, const uint32_t num_elems, uint32_t *num_elems_out, int *err_flag, dup_idx_list **dup_idx_head);
*/

/**
 * Using linked-list makes the performance really bad when the 
 * Original inpu array is medium and large. Abandon it. If you
 * would like to test this algorithm (int* fui_bitmap_idx_llist),
 * Please uncomment the contents below. * 
 * 
struct bmap_idx_tbl_struct {
    uint16_t byte_index;
    uint8_t bit_position;
    uint32_t raw_index;
    struct bmap_idx_tbl_struct *ptr_next;
};*/

/* typedef struct bmap_idx_tbl_struct  bmap_idx_brch; */

/* typedef struct {
    uint16_t bit_branch_size;
    uint8_t *ptr_bit_branch;
    bmap_idx_brch *ptr_idx_branch;
} bitmap_idx_base; */

/* void free_bmap_idx_branch(bmap_idx_brch *bmap_idx_head); */
/* int insert_idx_branch(bmap_idx_brch **bmap_idx_head, uint16_t byte_idx, uint8_t bit_pos, uint32_t raw_idx); */
/* int get_raw_idx(bmap_idx_brch *bmap_idx_head, uint16_t byte_idx, uint8_t bit_pos, uint32_t *raw_idx); */
/* void free_bitmap_idx(bitmap_idx_base *bitmap_head, uint16_t num_elems); */
/* int* fui_bitmap_idx_llist(const int *input_arr, const uint32_t num_elems, uint32_t *num_elems_out, int *err_flag, dup_idx_list **dup_idx_head); */

#endif