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

int convert_string_to_positive_num(const char* string);
int* filter_unique_elems_naive(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag);
int* filter_unique_elems_naive_improved(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag);
void free_hash_table(int *hash_table[], unsigned int num_elems);
int* filter_unique_elems_ht(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag);
void print_arr(int *arr, unsigned int num_elems);
int compare_arr(int *arr_a, int *arr_b, unsigned int num_elems);
int generate_random_input_arr(int *arr, unsigned int num_elems, unsigned int rand_max);
int generate_growing_arr(int *arr, unsigned int num_elems);

#endif
