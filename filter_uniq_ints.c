/**
 * 
 * This code is distributed under the license: MIT License
 * Originally written by Zhenrong WANG
 * mailto: zhenrongwang@live.com 
 * GitHub: https://github.com/zhenrong-wang
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "filter_uniq_ints.h"

/**
 * @brief Convert a string to a posivie number
 *  The string cannot contain characters other than 0~9
 * 
 * @returns
 *  -1 if illegal chars found or NULL pointer
 *  A positive number if succeeded
 */
unsigned int convert_string_to_positive_num(const char* string) {
    if(string == NULL) {
        return -1;
    }
    size_t str_length = strlen(string);
    int result = 0;
    for(size_t i = 0; i < str_length; i++) {
        if(string[i] < 48 || string[i] > 56) {
            return -1;
        }
        result = result * 10 + (string[i] - 48);
    }
    return result;
}

/**
 * 
 * @brief Filter out the unique integers from a given array in the 
 *  brute/naive way
 * 
 * @param [in]
 *  *input_arr is the given array pointer
 *  num_elems is the number of the integer elems in the given array
 *  
 * @param [out]
 *  *num_elems_out is the number of the output unique integers
 *  *err_flag is for debugging errors
 * 
 * @returns
 *  The pointer of the allocated output array if succeeded
 *  NULL if any error happens
 * 
 */
int* filter_unique_elems_naive(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag){
    unsigned int i, j = 1, k;
    int tmp = 0;
    int *final_output_arr = NULL;
    *err_flag = 0;
    *num_elems_out = 0;
    if (input_arr == NULL) {
        *err_flag = -5;
        return NULL;
    }
    if (num_elems < 1){
        *err_flag = -3;
        return NULL;
    }
    int *output_arr = (int *)calloc(num_elems, sizeof(int));
    if (output_arr == NULL) {
        *err_flag = -1;
        return NULL;
    }
    output_arr[0] = input_arr[0];
    for(i = 1; i < num_elems; i++) {
        tmp = input_arr[i];
        for(k = 0; k < j; k++) {
            if(tmp == output_arr[k]) {
                break;
            }
        }
        if(k == j) {
            output_arr[j] = tmp;
            j++;
        }
    }
    final_output_arr = (int *)realloc(output_arr, j*sizeof(int));
    if(final_output_arr == NULL) {
        free(output_arr);
        *err_flag = 1;
        return NULL;
    }
    *num_elems_out = j;
    return final_output_arr;
}

/**
 * 
 * @brief Filter out the unique integers from a given array in improved
 *  algorithm
 * 
 * @param [in]
 *  *input_arr is the given array pointer
 *  num_elems is the number of the integer elems in the given array
 *  
 * @param [out]
 *  *num_elems_out is the number of the output unique integers
 *  *err_flag is for debugging errors
 * 
 * @returns
 *  The pointer of the allocated output array if succeeded
 *  NULL if any error happens
 * 
 */
int* filter_unique_elems_naive_improved(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag){
    unsigned int i, j = 1, k;
    int max_current, min_current, diff_to_max = 0, diff_to_min = 0, tmp_diff_to_max = 0, tmp_diff_to_min = 0;
    int tmp = 0;
    int *final_output_arr = NULL;
    *err_flag = 0;
    *num_elems_out = 0;
    if (input_arr == NULL) {
        *err_flag = -5;
        return NULL;
    }
    if (num_elems < 1){
        *err_flag = -3;
        return NULL;
    }
    int *output_arr = (int *)calloc(num_elems, sizeof(int));
    if (output_arr == NULL) {
        *err_flag = -1;
        return NULL;
    }
    max_current = input_arr[0];
    min_current = input_arr[0];
    output_arr[0] = input_arr[0];
    for(i = 1; i < num_elems; i++) {
        tmp = input_arr[i];
        tmp_diff_to_max = max_current - tmp;
        tmp_diff_to_min = tmp - min_current;
        if(tmp_diff_to_max == 0 || tmp_diff_to_min == 0 || tmp_diff_to_max == diff_to_max || tmp_diff_to_min == diff_to_min) {
            continue;
        }
        if(tmp_diff_to_max < 0 || tmp_diff_to_min < 0) {
            output_arr[j] = tmp;
            if(tmp_diff_to_max < 0) {
                diff_to_max = -tmp_diff_to_max;
                max_current = tmp;
            }
            else {
                diff_to_min = -tmp_diff_to_min;
                min_current = tmp;
            }
            j++;
            continue;
        }
        if(tmp_diff_to_min < diff_to_min || tmp_diff_to_max < diff_to_max) {
            output_arr[j] = tmp;
            if(tmp_diff_to_min < diff_to_min) {
                diff_to_min = tmp_diff_to_min;
            }
            else {
                diff_to_max = tmp_diff_to_max;
            }
            j++;
            continue;
        }
        for(k = 0; k < j; k++) {
            if(tmp == output_arr[k]) {
                break;
            }
        }
        if(k == j) {
            output_arr[j] = tmp;
            j++;
        }
    }
    final_output_arr = (int *)realloc(output_arr, j*sizeof(int));
    if(final_output_arr == NULL) {
        free(output_arr);
        *err_flag = 1;
        return NULL;
    }
    *num_elems_out = j;
    return final_output_arr;
}

void free_hash_table(int *hash_table[], unsigned int num_elems) {
    for(unsigned int i = 0; i < num_elems; i++) {
        if(hash_table[i] != NULL) {
            free(hash_table[i]);
        }
    }
}

/**
 * 
 * @brief Filter out the unique integers from a given array 
 *  Using the basic hash table algorithm
 * 
 * @param [in]
 *  *input_arr is the given array pointer
 *  num_elems is the number of the integer elems in the given array
 *  
 * @param [out]
 *  *num_elems_out is the number of the output unique integers
 *  *err_flag is for debugging errors
 * 
 * @returns
 *  The pointer of the allocated output array if succeeded
 *  NULL if any error happens
 * 
 */
int* filter_unique_elems_ht(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag) {
    unsigned int i, j = 0;
    unsigned int tmp_quotient = 0, tmp_mod = 0;
    int tmp = 0;
    int *final_output_arr = NULL;

    int *hash_table_base_p[HASH_TABLE_SIZE] = {NULL,};
    int *hash_table_base_n[HASH_TABLE_SIZE] = {NULL,};

    *err_flag = 0;
    *num_elems_out = 0;

    if (input_arr == NULL) {
        *err_flag = -5;
        return NULL;
    }
    if (num_elems < 1){
        *err_flag = -3;
        return NULL;
    }
    int *output_arr = (int *)calloc(num_elems, sizeof(int));
    if (output_arr == NULL) {
        *err_flag = -1;
        return NULL;
    }
    for(i = 0; i < num_elems; i++) {
        tmp = input_arr[i];
        tmp_quotient = abs(tmp / MOD_TABLE_SIZE);
        tmp_mod = abs(tmp % MOD_TABLE_SIZE);
        if(tmp > 0) {
            if(hash_table_base_p[tmp_quotient] == NULL) {
                if((hash_table_base_p[tmp_quotient] = (int *)calloc(MOD_TABLE_SIZE, sizeof(int))) == NULL) {
                    *err_flag = 1;
                    goto free_memory;
                }
            }
            if(hash_table_base_p[tmp_quotient] != NULL && (hash_table_base_p[tmp_quotient])[tmp_mod] != 0) {
                continue;
            }
        }
        else {
            if(hash_table_base_n[tmp_quotient] == NULL) {
                if((hash_table_base_n[tmp_quotient] = (int *)calloc(MOD_TABLE_SIZE, sizeof(int))) == NULL) {
                    *err_flag = 1;
                    goto free_memory;
                }
            }
            if(hash_table_base_n[tmp_quotient] != NULL && (hash_table_base_n[tmp_quotient])[tmp_mod] != 0) {
                continue;
            }
        }
        output_arr[j] = tmp;
        j++;
        if(tmp > 0) {
            (hash_table_base_p[tmp_quotient])[tmp_mod] = 1;
        }
        else {
            (hash_table_base_n[tmp_quotient])[tmp_mod] = 1;
        }
    }

free_memory:
    free_hash_table(hash_table_base_p, HASH_TABLE_SIZE);
    free_hash_table(hash_table_base_n, HASH_TABLE_SIZE);
    if(*err_flag != 0) {
        free(output_arr);
        return NULL;
    }

    final_output_arr = (int *)realloc(output_arr, j*sizeof(int));
    if(final_output_arr == NULL) {
        free(output_arr);
        *err_flag = 3;
        return NULL;
    }
    *num_elems_out = j;
    return final_output_arr;
}

void free_hash_table_new(hash_table_base_node hash_table_new[], unsigned int num_elems) {
    for(unsigned int i = 0; i < num_elems; i++) {
        if(hash_table_new[i].ptr_branch_p != NULL) {
            free(hash_table_new[i].ptr_branch_p);
        }
        if(hash_table_new[i].ptr_branch_n != NULL) {
            free(hash_table_new[i].ptr_branch_n);
        }
    }
}

/**
 * 
 * @brief Filter out the unique integers from a given array 
 *  Using the single base hash table algorithm
 * 
 * @param [in]
 *  *input_arr is the given array pointer
 *  num_elems is the number of the integer elems in the given array
 *  
 * @param [out]
 *  *num_elems_out is the number of the output unique integers
 *  *err_flag is for debugging errors
 * 
 * @returns
 *  The pointer of the allocated output array if succeeded
 *  NULL if any error happens
 * 
 */
int* filter_unique_elems_ht_new(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag) {
    unsigned int i, j = 0;
    unsigned int tmp_quotient = 0, tmp_mod = 0;
    int tmp = 0;
    int *final_output_arr = NULL;
    int *tmp_realloc_ptr = NULL;
    hash_table_base_node hash_table_base[HASH_TABLE_SIZE] = {{0, 0, NULL, NULL},};
    *err_flag = 0;
    *num_elems_out = 0;
    if (input_arr == NULL) {
        *err_flag = -5;
        return NULL;
    }
    if (num_elems < 1){
        *err_flag = -3;
        return NULL;
    }
    int *output_arr = (int *)calloc(num_elems, sizeof(int));
    if (output_arr == NULL) {
        *err_flag = -1;
        return NULL;
    }
    for(i = 0; i < num_elems; i++) {
        tmp = input_arr[i];
        tmp_quotient = abs(tmp / MOD_TABLE_SIZE);
        tmp_mod = abs(tmp % MOD_TABLE_SIZE);
        if(tmp > 0) {
            if(hash_table_base[tmp_quotient].ptr_branch_p == NULL) {
                if((hash_table_base[tmp_quotient].ptr_branch_p = (int *)calloc(tmp_mod + 1, sizeof(int))) == NULL) {
                    *err_flag = 1;
                    goto free_memory;
                }
                else {
                    hash_table_base[tmp_quotient].branch_size_p = tmp_mod + 1;
                }
            }
            else {
                if(hash_table_base[tmp_quotient].branch_size_p < (tmp_mod + 1)){
                    if((tmp_realloc_ptr = (int *)realloc(hash_table_base[tmp_quotient].ptr_branch_p, (tmp_mod + 1)*sizeof(int))) == NULL) {
                        *err_flag = 1;
                        goto free_memory;
                    }
                    else {
                        hash_table_base[tmp_quotient].ptr_branch_p = tmp_realloc_ptr;
                        memset(tmp_realloc_ptr + hash_table_base[tmp_quotient].branch_size_p, 0, sizeof(int) * (tmp_mod + 1 - hash_table_base[tmp_quotient].branch_size_p));
                        hash_table_base[tmp_quotient].branch_size_p = tmp_mod + 1;
                    }
                }
            }
            if(hash_table_base[tmp_quotient].ptr_branch_p != NULL && (hash_table_base[tmp_quotient].ptr_branch_p)[tmp_mod] != 0) {
                continue;
            }
        }
        else {
            if(hash_table_base[tmp_quotient].ptr_branch_n == NULL) {
                if((hash_table_base[tmp_quotient].ptr_branch_n = (int *)calloc(tmp_mod + 1, sizeof(int))) == NULL) {
                    *err_flag = 1;
                    goto free_memory;
                }
                else {
                    hash_table_base[tmp_quotient].branch_size_n = tmp_mod + 1;
                }
            }
            else {
                if(hash_table_base[tmp_quotient].branch_size_n < (tmp_mod + 1)){
                    if((tmp_realloc_ptr = (int *)realloc(hash_table_base[tmp_quotient].ptr_branch_n, (tmp_mod + 1)*sizeof(int))) == NULL) {
                        *err_flag = 1;
                        goto free_memory;
                    }
                    else {
                        hash_table_base[tmp_quotient].ptr_branch_n = tmp_realloc_ptr;
                        memset(tmp_realloc_ptr + hash_table_base[tmp_quotient].branch_size_n , 0, sizeof(int) * (tmp_mod + 1 - hash_table_base[tmp_quotient].branch_size_n));
                        hash_table_base[tmp_quotient].branch_size_n = tmp_mod + 1;
                    }
                }
            }
            if(hash_table_base[tmp_quotient].ptr_branch_n != NULL && (hash_table_base[tmp_quotient].ptr_branch_n)[tmp_mod] != 0) {
                continue;
            }
        }
        output_arr[j] = tmp;
        j++;
        if(tmp > 0) {
            (hash_table_base[tmp_quotient].ptr_branch_p)[tmp_mod] = 1;
        }
        else {
            (hash_table_base[tmp_quotient].ptr_branch_n)[tmp_mod] = 1;
        }
    }

free_memory:
    free_hash_table_new(hash_table_base, HASH_TABLE_SIZE);
    if(*err_flag != 0) {
        free(output_arr);
        return NULL;
    }
    final_output_arr = (int *)realloc(output_arr, j*sizeof(int));
    if(final_output_arr == NULL) {
        free(output_arr);
        *err_flag = 3;
        return NULL;
    }
    *num_elems_out = j;
    return final_output_arr;
}

/**
 * 
 * @brief Filter out the unique integers from a given array 
 *  Using the single base hash table algorithm with fully dynamic 
 *  memory allocation
 * 
 * @param [in]
 *  *input_arr is the given array pointer
 *  num_elems is the number of the integer elems in the given array
 *  
 * @param [out]
 *  *num_elems_out is the number of the output unique integers
 *  *err_flag is for debugging errors
 * 
 * @returns
 *  The pointer of the allocated output array if succeeded
 *  NULL if any error happens
 * 
 */
int* filter_unique_elems_ht_dyn(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag) {
    unsigned int i, j = 0;
    unsigned int tmp_quotient = 0, tmp_mod = 0;
    int tmp = 0;
    int *final_output_arr = NULL;
    int *tmp_realloc_ptr = NULL;
    hash_table_base_node *hash_table_base = NULL, *tmp_ht_realloc_ptr = NULL;
    unsigned int ht_base_length = HT_DYN_INI_SIZE;
    *err_flag = 0;
    *num_elems_out = 0;
    if (input_arr == NULL) {
        *err_flag = -5;
        return NULL;
    }
    if (num_elems < 1){
        *err_flag = -3;
        return NULL;
    }
    hash_table_base = (hash_table_base_node *)calloc(HT_DYN_INI_SIZE, sizeof(hash_table_base_node));
    if(hash_table_base == NULL) {
        *err_flag = 5;
        return NULL;
    }
    int *output_arr = (int *)calloc(num_elems, sizeof(int));
    if (output_arr == NULL) {
        free(hash_table_base);
        *err_flag = -1;
        return NULL;
    }
    for(i = 0; i < num_elems; i++) {
        tmp = input_arr[i];
        tmp_quotient = abs(tmp / MOD_TABLE_SIZE);
        tmp_mod = abs(tmp % MOD_TABLE_SIZE);
        if((tmp_quotient + 1) > ht_base_length) {
            tmp_ht_realloc_ptr = (hash_table_base_node *)realloc(hash_table_base, (tmp_quotient + 1) * sizeof(hash_table_base_node));
            if(tmp_ht_realloc_ptr == NULL) {
                *err_flag = 7;
                goto free_memory;
            }
            memset(tmp_ht_realloc_ptr + ht_base_length, 0, (tmp_quotient + 1 - ht_base_length) * sizeof(hash_table_base_node));
            hash_table_base = tmp_ht_realloc_ptr;
            ht_base_length = (tmp_quotient + 1);
        }
        if(tmp > 0) {
            if(hash_table_base[tmp_quotient].ptr_branch_p == NULL) {
                if((hash_table_base[tmp_quotient].ptr_branch_p = (int *)calloc(tmp_mod + 1, sizeof(int))) == NULL) {
                    *err_flag = 1;
                    goto free_memory;
                }
                else {
                    hash_table_base[tmp_quotient].branch_size_p = tmp_mod + 1;
                }
            }
            else {
                if(hash_table_base[tmp_quotient].branch_size_p < (tmp_mod + 1)){
                    if((tmp_realloc_ptr = (int *)realloc(hash_table_base[tmp_quotient].ptr_branch_p, (tmp_mod + 1)*sizeof(int))) == NULL) {
                        *err_flag = 1;
                        goto free_memory;
                    }
                    else {
                        hash_table_base[tmp_quotient].ptr_branch_p = tmp_realloc_ptr;
                        memset(tmp_realloc_ptr + hash_table_base[tmp_quotient].branch_size_p, 0, sizeof(int) * (tmp_mod + 1 - hash_table_base[tmp_quotient].branch_size_p));
                        hash_table_base[tmp_quotient].branch_size_p = tmp_mod + 1;
                    }
                }
            }
            if(hash_table_base[tmp_quotient].ptr_branch_p != NULL && (hash_table_base[tmp_quotient].ptr_branch_p)[tmp_mod] != 0) {
                continue;
            }
        }
        else {
            if(hash_table_base[tmp_quotient].ptr_branch_n == NULL) {
                if((hash_table_base[tmp_quotient].ptr_branch_n = (int *)calloc(tmp_mod + 1, sizeof(int))) == NULL) {
                    *err_flag = 1;
                    goto free_memory;
                }
                else {
                    hash_table_base[tmp_quotient].branch_size_n = tmp_mod + 1;
                }
            }
            else {
                if(hash_table_base[tmp_quotient].branch_size_n < (tmp_mod + 1)){
                    if((tmp_realloc_ptr = (int *)realloc(hash_table_base[tmp_quotient].ptr_branch_n, (tmp_mod + 1)*sizeof(int))) == NULL) {
                        *err_flag = 1;
                        goto free_memory;
                    }
                    else {
                        hash_table_base[tmp_quotient].ptr_branch_n = tmp_realloc_ptr;
                        memset(tmp_realloc_ptr + hash_table_base[tmp_quotient].branch_size_n , 0, sizeof(int) * (tmp_mod + 1 - hash_table_base[tmp_quotient].branch_size_n));
                        hash_table_base[tmp_quotient].branch_size_n = tmp_mod + 1;
                    }
                }
            }
            if(hash_table_base[tmp_quotient].ptr_branch_n != NULL && (hash_table_base[tmp_quotient].ptr_branch_n)[tmp_mod] != 0) {
                continue;
            }
        }
        output_arr[j] = tmp;
        j++;
        if(tmp > 0) {
            (hash_table_base[tmp_quotient].ptr_branch_p)[tmp_mod] = 1;
        }
        else {
            (hash_table_base[tmp_quotient].ptr_branch_n)[tmp_mod] = 1;
        }
    }
free_memory:
    free_hash_table_new(hash_table_base, ht_base_length);
    if(*err_flag != 0) {
        free(output_arr);
        return NULL;
    }
    final_output_arr = (int *)realloc(output_arr, j*sizeof(int));
    if(final_output_arr == NULL) {
        free(output_arr);
        *err_flag = 3;
        return NULL;
    }
    *num_elems_out = j;
    return final_output_arr;
}

/**
 * 
 * @brief Print out an integer array for debugging
 * 
 * @param [in]
 *  *arr is the given array pointer
 *  num_elems is the number of the integer elems in the given array
 * 
 * @returns
 *  void
 * 
 */
void print_arr(int *arr, unsigned int num_elems) {
    if(arr == NULL || num_elems < 1) {
        printf("ERROR: NULL array input.\n");
        return;
    }
    for(unsigned int i = 0; i < num_elems; i++) {
        printf("%d\t", arr[i]);
        if((i+1)%10 == 0) {
            printf("\n");
        }
    }
    printf("\n\n");
}

int compare_arr(const int *arr_a, const int *arr_b, unsigned int num_elems) {
    if(arr_a == NULL || arr_b == NULL) {
        return -3;
    }
    if(num_elems < 1) {
        return -1;
    }
    for(unsigned int i = 0; i < num_elems; i++) {
        if(arr_a[i] != arr_b[i]) {
            return 1;
        }
    }
    return 0;
}

/**
 * 
 * @brief Generate random integers and put them into an given array
 * 
 * @param [in]
 *  *arr is the array pointer
 *  num_elems is the size of the given integer array
 *  rand_max is the MAXIMUM of the generated random number
 * 
 * @returns
 *  0 if succeeded
 *  -5 if the arr pointer is null
 *  -3 if the num_elems is 0
 *  -1 if the rand_max is 0
 * 
 */
int generate_random_input_arr(int *arr, unsigned int num_elems, unsigned int rand_max) {
    int sign_flag, rand_num;
    if(arr == NULL) {
        return -5;
    }
    if(num_elems < 1) {
        return -3;
    }
    if(rand_max < 1) {
        return -1;
    }
    srand(time(0));
    for(unsigned int i = 0; i < num_elems; i++) {
        sign_flag = rand();
        rand_num = rand()%rand_max;
        if(sign_flag%2 == 0) {
            arr[i] = rand_num;
        }
        else {
            arr[i] = 0 - rand_num;
        }
    }
    //print_arr(arr, num_elems);
    return 0;
}

/**
 * 
 * @brief Generate a single-direction growing integer array
 *  This is the best case for the new algorighm
 * 
 * @param [in]
 *  *arr is the array pointer
 *  num_elems is the size of the given integer array
 * 
 * @returns
 *  0 if succeeded
 *  -5 if the arr pointer is null
 *  -3 if the num_elems is 0
 * 
 */
int generate_growing_arr(int *arr, unsigned int num_elems) {
    if(arr == NULL) {
        return -5;
    }
    if(num_elems < 1) {
        return -3;
    }
    for(unsigned int i = 0; i < num_elems; i++) {
        arr[i] = i;
    }
    return 0;
}

void free_bitmap(bitmap_base_node *bitmap_head, unsigned short num_elems) {
    for(unsigned int i = 0; i < num_elems; i++) {
        if(bitmap_head[i].ptr_branch != NULL) {
            free(bitmap_head[i].ptr_branch);
        }
    }
}

int* fui_bitmap_stc(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag) {
unsigned int i, j = 0;
    unsigned int tmp_quotient = 0, tmp_mod = 0;
    int tmp = 0, *final_output_arr = NULL;
    bitmap_base_node bitmap_head[BITMAP_LENGTH_MAX] = {{0, NULL},};
    unsigned short tmp_byte_index = 0;
    unsigned char tmp_bit_position = 0;
    *err_flag = 0;
    *num_elems_out = 0;
    if (input_arr == NULL) {
        *err_flag = -5;
        return NULL;
    }
    if (num_elems < 1){
        *err_flag = -3;
        return NULL;
    }
    int *output_arr = (int *)calloc(num_elems, sizeof(int));
    if (output_arr == NULL) {
        *err_flag = -1;
        return NULL;
    }
    for(i = 0; i < num_elems; i++) {
        tmp = input_arr[i];
        tmp_quotient = abs(tmp / BIT_MOD_DIV_FACTOR);
        tmp_mod = abs(tmp % BIT_MOD_DIV_FACTOR);
        tmp_byte_index = (tmp < 0) ? (NEGATIVE_START_POS + (tmp_mod / 8)) : (tmp_mod / 8);
        tmp_bit_position = tmp_mod % 8;
        if(bitmap_head[tmp_quotient].ptr_branch == NULL) {
            if((bitmap_head[tmp_quotient].ptr_branch = (unsigned char *)calloc(BIT_MOD_TABLE_SIZE, sizeof(unsigned char))) == NULL) {
                *err_flag = 1;
                goto free_memory;
            }
        }
        if(bitmap_head[tmp_quotient].ptr_branch != NULL && check_bit((bitmap_head[tmp_quotient].ptr_branch)[tmp_byte_index], tmp_bit_position)) {
            continue;
        }
        output_arr[j] = tmp;
        j++;
        flip_bit((bitmap_head[tmp_quotient].ptr_branch)[tmp_byte_index], tmp_bit_position);
    }
free_memory:
    free_bitmap(bitmap_head, BITMAP_LENGTH_MAX);
    if(*err_flag != 0) {
        free(output_arr);
        return NULL;
    }
    final_output_arr = (int *)realloc(output_arr, j * sizeof(int));
    if(final_output_arr == NULL) {
        free(output_arr);
        *err_flag = 3;
        return NULL;
    }
    *num_elems_out = j;
    return final_output_arr;
}

int* fui_bitmap_base_dyn(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag) {
    unsigned int i, j = 0;
    unsigned int tmp_quotient = 0, tmp_mod = 0;
    int tmp = 0, *final_output_arr = NULL;
    bitmap_base_node *bitmap_head = NULL, *tmp_bitmap_realloc = NULL;
    unsigned short tmp_byte_index = 0, bitmap_base_size = BITMAP_INIT_LENGTH;
    unsigned char tmp_bit_position = 0;
    *err_flag = 0;
    *num_elems_out = 0;
    if (input_arr == NULL) {
        *err_flag = -5;
        return NULL;
    }
    if (num_elems < 1){
        *err_flag = -3;
        return NULL;
    }
    bitmap_head = (bitmap_base_node *)calloc(BITMAP_INIT_LENGTH, sizeof(bitmap_base_node));
    if(bitmap_head == NULL) {
        *err_flag = 5;
        return NULL;
    }
    int *output_arr = (int *)calloc(num_elems, sizeof(int));
    if (output_arr == NULL) {
        free(bitmap_head);
        *err_flag = -1;
        return NULL;
    }
    for(i = 0; i < num_elems; i++) {
        tmp = input_arr[i];
        tmp_quotient = abs(tmp / BIT_MOD_DIV_FACTOR);
        tmp_mod = abs(tmp % BIT_MOD_DIV_FACTOR);
        tmp_byte_index = (tmp < 0) ? (NEGATIVE_START_POS + (tmp_mod / 8)) : (tmp_mod / 8);
        tmp_bit_position = tmp_mod % 8;

        /* Grow the tree if needed. */
        if((tmp_quotient + 1) > bitmap_base_size) {
            if((tmp_bitmap_realloc = (bitmap_base_node *)realloc(bitmap_head, (tmp_quotient + 1) * sizeof(bitmap_base_node))) == NULL) {
                *err_flag = 7;
                goto free_memory;
            }
            memset(tmp_bitmap_realloc + bitmap_base_size, 0, (tmp_quotient + 1 - bitmap_base_size) * sizeof(bitmap_base_node));
            bitmap_head = tmp_bitmap_realloc;
            bitmap_base_size = (tmp_quotient + 1);
        }
        if(bitmap_head[tmp_quotient].ptr_branch == NULL) {
            if((bitmap_head[tmp_quotient].ptr_branch = (unsigned char *)calloc(BIT_MOD_TABLE_SIZE, sizeof(unsigned char))) == NULL) {
                *err_flag = 1;
                goto free_memory;
            }
        }
        if(bitmap_head[tmp_quotient].ptr_branch != NULL && check_bit((bitmap_head[tmp_quotient].ptr_branch)[tmp_byte_index], tmp_bit_position)) {
            continue;
        }
        output_arr[j] = tmp;
        j++;
        flip_bit((bitmap_head[tmp_quotient].ptr_branch)[tmp_byte_index], tmp_bit_position);
    }
free_memory:
    free_bitmap(bitmap_head, bitmap_base_size);
    free(bitmap_head);
    if(*err_flag != 0) {
        free(output_arr);
        return NULL;
    }
    final_output_arr = (int *)realloc(output_arr, j * sizeof(int));
    if(final_output_arr == NULL) {
        free(output_arr);
        *err_flag = 3;
        return NULL;
    }
    *num_elems_out = j;
    return final_output_arr;
}
