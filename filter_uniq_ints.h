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
    unsigned int branch_size_p;
    unsigned int branch_size_n;
    int *ptr_branch_p;
    int *ptr_branch_n;
} hash_table_base_node;

unsigned int convert_string_to_positive_num(const char* string);
int* filter_unique_elems_naive(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag);
int* filter_unique_elems_naive_improved(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag);
void free_hash_table(int *hash_table[], unsigned int num_elems);
void free_hash_table_new(hash_table_base_node hash_table_new[], unsigned int num_elems);
int* filter_unique_elems_ht(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag);
int* filter_unique_elems_ht_new(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag);
int* filter_unique_elems_ht_dyn(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag);
void print_arr(int *arr, unsigned int num_elems);
int compare_arr(const int *arr_a, const int *arr_b, unsigned int num_elems);
int generate_random_input_arr(int *arr, unsigned int num_elems, unsigned int rand_max);
int generate_growing_arr(int *arr, unsigned int num_elems);

/**
 * Testing the bit-based hash table algorithm
 * This algorithm would save 8x memory usage
 */

#define NEGATIVE_START_POS  8192
#define BIT_MOD_TABLE_SIZE  16384
#define BIT_MOD_DIV_FACTOR  65536
#define BITMAP_INIT_LENGTH  128
#define BITMAP_LENGTH_MAX   32769

/* STATIC BITMAP SIZE */
#define BITMAP_STATIC_ROW   4096
#define BITMAP_STATIC_COL   16384

typedef struct {
    unsigned short branch_size;
    unsigned char *ptr_branch;
} bitmap_base_node;

typedef struct {
    unsigned short branch_size_n;
    unsigned short branch_size_p;
    unsigned char *ptr_branch_n;
    unsigned char *ptr_branch_p;
} bitmap_dbase_node;

#define flip_bit(byte_a, bit_position) ((byte_a) |= (0x80 >> (bit_position)))
#define check_bit(byte_a, bit_position) ((byte_a) & (0x80 >> (bit_position)))

int* fui_bitmap_stc(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag);
int* fui_bitmap_base_dyn(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag);

/* To be continued. */
int* fui_bitmap_full_dyn(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag);
int* fui_bitmap_dbase(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag);
int* fui_bitmap_dbase_dyn(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag);
int* fui_bitmap_array(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag);
int* fui_bitmap_array_dyn(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag);

#endif
