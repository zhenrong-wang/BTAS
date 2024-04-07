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

typedef struct {
    unsigned int branch_size_p;
    unsigned int branch_size_n;
    int *ptr_branch_p;
    int *ptr_branch_n;
} hash_table_base_node;

int convert_string_to_positive_num(const char* string);
int* filter_unique_elems_naive(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag);
int* filter_unique_elems_naive_improved(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag);
void free_hash_table(int *hash_table[], unsigned int num_elems);
void free_hash_table_new(hash_table_base_node hash_table_new[], unsigned int num_elems);
int* filter_unique_elems_ht(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag);
int* filter_unique_elems_ht_new(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag);
int* filter_unique_elems_ht_dyn(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag);
void print_arr(int *arr, unsigned int num_elems);
int compare_arr(int *arr_a, int *arr_b, unsigned int num_elems);
int generate_random_input_arr(int *arr, unsigned int num_elems, unsigned int rand_max);
int generate_growing_arr(int *arr, unsigned int num_elems);

/**
 * Testing the bit-based hash table algorithm
 * This algorithm would save 8x memory usage
 */
typedef struct {
    unsigned int branch_size_p;
    unsigned int branch_size_n;
    unsigned char *ptr_branch_p;
    unsigned char *ptr_branch_n;
} bit_hash_table_node;

void flip_bit(unsigned char *byte_a, unsigned char bit_position);
int check_bit(unsigned char byte_a, unsigned char bit_position);
int* filter_unique_elems_ht_bit(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag);

#endif
