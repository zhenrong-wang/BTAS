/**
 * 
 * This code is distributed under the license: MIT License
 * Originally written by Zhenrong WANG
 * mailto: zhenrongwang@live.com 
 * GitHub: https://github.com/zhenrong-wang
 * 
 */

#ifndef FILTER_UNIQ_INTS_H
#define FILTER_UNIQ_INTS_H

/**
 *  Use aliases to make the code clear and short
 */
typedef     unsigned char       uint_8;
typedef     unsigned short      uint_16;
typedef     unsigned int        uint_32;
#ifdef _WIN32
typedef     long long           int_64bit;
typedef     unsigned long long  uint_64bit;
#else
typedef     long                int_64bit;
typedef     unsigned long       uint_64bit;
#endif

typedef struct {
    uint_64bit h64bit;
    uint_64bit l64bit;
} uint_128bit;

/**
 * The maximum 32-bit signed integer is -2^31 - 2^31, 
 * aka, -32768 x 65536 to 32768 x 65536
 * So, any given integer x, x / 65536 < = 32768, and the mod
 * x % 65536 < 65536, Therefore, the HASH_TABLE_SIZE 
 * should be 32769 to cover 0 ~ 32768, and the MOD_TABLE_SIZE
 * should be 65536 to cover 0 ~ 65535
 */
#define HASH_TABLE_SIZE 32769
#define MOD_TABLE_SIZE  65536

/**
 * With the initial size of 32, the initial hash table is able
 * cover the range of -2097152 to 2097152
 */
#define HT_DYN_INI_SIZE 32
#define BIT_HT_INI_SIZE 64

typedef struct {
    uint_32 branch_size_p;
    uint_32 branch_size_n;
    uint_8 *ptr_branch_p;
    uint_8 *ptr_branch_n;
} htable_base;

/**
 * Auxiliary functions, e.g.
 *  - Print out an array (Caution: when the array is very big, don't use it)
 *  - Compare 2 arrays
 *  - Generate a RANDOM input integer array
 *  - Generate a GROWING input integer array
*/
uint_32 convert_string_to_positive_num(const char* string);
void print_arr(const int *arr, uint_32 num_elems, uint_32 max_elems);
int compare_arr(const int *arr_a, const int *arr_b, uint_32 num_elems);
int generate_random_input_arr(int *arr, uint_32 num_elems, uint_32 rand_max);
int generate_growing_arr(int *arr, uint_32 num_elems);


/* Brute functions. */
int* fui_brute(const int *input_arr, const uint_32 num_elems, uint_32 *num_elems_out, int *err_flag);
int* fui_brute_opt(const int *input_arr, const uint_32 num_elems, uint_32 *num_elems_out, int *err_flag);

/* Memory-inefficient hash table algorithms */
void free_hash_table(uint_8 *hash_table[], uint_32 num_elems);
void free_hash_table_new(htable_base hash_table_new[], uint_32 num_elems);
int* fui_htable_dtree(const int *input_arr, const uint_32 num_elems, uint_32 *num_elems_out, int *err_flag);
int* fui_htable_stree(const int *input_arr, const uint_32 num_elems, uint_32 *num_elems_out, int *err_flag);
int* fui_htable_stree_dyn(const int *input_arr, const uint_32 num_elems, uint_32 *num_elems_out, int *err_flag);


/* Bitmap based algorithms. */

#define NEGATIVE_START_POS  8192
#define BIT_MOD_TABLE_SIZE  16384
#define BIT_MOD_DIV_FACTOR  65536
#define BITMAP_INIT_LENGTH  128
#define BITMAP_LENGTH_MAX   32769

#define UINT_16_MAX         65536
#define UINT_8_MAX          256
#define IDX_ADJ_BRCH_SIZE   65536

struct dup_idx_struct {
    uint_32 index_a;
    uint_32 index_b;
    struct dup_idx_struct *ptr_next;
};

typedef struct dup_idx_struct       dup_idx_list;

typedef struct {
    int out_elem;
    uint_32 raw_index;
} out_idx;

typedef struct {
    int out_elem;
    uint_32 raw_index;
} out_idx_i64;

typedef struct {
    uint_32 branch_size;
    uint_8 *ptr_branch;
} bitmap_base;

typedef struct {
    uint_16 branch_size[2];
    uint_8 *ptr_branch[2];
} bitmap_dtree;

/* Adjacent index hashtable */
typedef struct {
    uint_16 branch_size[2];
    uint_8 *ptr_branch[2];
} idx_ht_8;

typedef struct {
    uint_16 branch_size[2];
    uint_16 *ptr_branch[2];
} idx_ht_16;

typedef struct {
    uint_16 branch_size[2];
    uint_32 *ptr_branch[2];
} idx_ht_32;

#define flip_bit(byte_a, bit_position) ((byte_a) |= (0x80 >> (bit_position)))
#define check_bit(byte_a, bit_position) ((byte_a) & (0x80 >> (bit_position)))

/**
 * Using linked-list makes the performance really bad when the 
 * Original inpu array is medium and large. Abandon it. If you
 * would like to test this algorithm (int* fui_bitmap_idx_llist),
 * Please uncomment the contents below. * 
 * 
struct bmap_idx_tbl_struct {
    uint_16 byte_index;
    uint_8 bit_position;
    uint_32 raw_index;
    struct bmap_idx_tbl_struct *ptr_next;
};*/

/* typedef struct bmap_idx_tbl_struct  bmap_idx_brch; */

/* typedef struct {
    uint_16 bit_branch_size;
    uint_8 *ptr_bit_branch;
    bmap_idx_brch *ptr_idx_branch;
} bitmap_idx_base; */

/* void free_bmap_idx_branch(bmap_idx_brch *bmap_idx_head); */
/* int insert_idx_branch(bmap_idx_brch **bmap_idx_head, uint_16 byte_idx, uint_8 bit_pos, uint_32 raw_idx); */
/* int get_raw_idx(bmap_idx_brch *bmap_idx_head, uint_16 byte_idx, uint_8 bit_pos, uint_32 *raw_idx); */
/* void free_bitmap_idx(bitmap_idx_base *bitmap_head, uint_16 num_elems); */
/* int* fui_bitmap_idx_llist(const int *input_arr, const uint_32 num_elems, uint_32 *num_elems_out, int *err_flag, dup_idx_list **dup_idx_head); */

void free_dup_idx_list(dup_idx_list *dup_idx_head);
int insert_dup_idx_list(dup_idx_list **dup_idx_head, uint_32 idx_a, uint_32 idx_b);
void print_dup_idx_list(dup_idx_list *dup_idx_head, uint_32 max_nodes);
void print_out_idx(out_idx *output_index, uint_32 num_elems, uint_32 max_elems);

void free_bitmap(bitmap_base *bitmap_head, uint_16 num_elems);
void free_bitmap_dtree(bitmap_dtree *bitmap_head, uint_16 num_elems);
void free_idx_ht_8(idx_ht_8 *idx_ht_head, uint_16 num_elems);
void free_idx_ht_16(idx_ht_16 *idx_ht_head, uint_16 num_elems);
void free_idx_ht_32(idx_ht_32 *idx_ht_head, uint_16 num_elems);

int* fui_bitmap_stc(const int *input_arr, const uint_32 num_elems, uint_32 *num_elems_out, int *err_flag);
int* fui_bitmap_dyn(const int *input_arr, const uint_32 num_elems, uint_32 *num_elems_out, int *err_flag);
out_idx* fui_bitmap_dtree_idx(const int *input_arr, const uint_32 num_elems, uint_32 *num_elems_out, int *err_flag, dup_idx_list **dup_idx_head);


/**
 * Extending the algo to 64bit or even longer input element. Quite Challanging.
 * 
int assemble_h32(int_64bit a);
int assemble_l32(int_64bit a);

int hash_64_to_32(int_64bit in64);
int_64bit* transform_32_to_64_arr(const int *arr_input_32, uint_32 num_elems, const char *option);
out_idx_i64* fui_bitmap_dtree_idx_64(const int_64bit *input_arr, const uint_32 num_elems, uint_32 *num_elems_out, int *err_flag, dup_idx_list **dup_idx_head);
*/

#endif
