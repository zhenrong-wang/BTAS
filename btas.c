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
#include "btas.h"

/**
 * @brief Convert a *unsigned* string ('\0' terminated!) 
 *  to a UINT32 posivie number or 0
 *  The string cannot contain characters other than 0~9
 *  E.g. 12345678
 * 
 * @param 
 *  [in] const char* string: a '\0'-terminated string. An 
 *   array without '\0' would cause undefined behaviors. Please
 *   make sure to preprocess the string with snprintf or strncpy
 *   or other functions to construct the string. A constant
 *   string such as "100909" is good to go.
 * 
 *  [out] *unsigned_num: an address to an unsigned 32bit integer.
 *    Make sure the type is correct.
 * 
 * @returns
 *  -3 if the string is a null pointer
 *  -1 if invalid chars found
 *   1 if overflow occurred (the *unsigned_num would be set to UINT32_MAX)
 *   0 if succeeded and the result is in the range of 0 ~ UINT32_MAX
 */
int string_to_u32_num(const char* string, uint32_t *unsigned_num) {
    *unsigned_num = 0; /* Set the output to 0. */
    if(string == NULL) {
        /* If the string pointer is null, reject to proceed
           because strlen(null) would cause stack overflow. */
        return -3; 
    }
    size_t str_length = strlen(string);
    uint64_t result = 0; /* Define a 64bit unsigned integer as a larger buffer. */
    for(size_t i = 0; i < str_length; i++) {
        /* Use chars '0' or '9' to guarantee the portability. */
        if(string[i] < '0' || string[i] > '9') {
            return -1; /* Illegal chars found. Reject to proceed. */
        }
        result = result * 10 + (string[i] - '0');
        if(result > 0xFFFFFFFF) {
            /* Overflow occurred. Set the output to UINT32_MAX and return,
               Users can decide to use the UINT32_MAX but it is clear that
               overflow has ocurred indicated by the return value 1. */
            *unsigned_num = 0xFFFFFFFF;
            return 1; 
        }
    }
    /* Everything is good, return the lower-32 bit of the 64bit result. 
       Or, just directly use *unsigned_num = (uint32_t)result to cast it.
       Let's avoid cast and use bitwise operation to manually cast. */
    *unsigned_num = result & 0xFFFFFFFF;
    return 0;
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
void print_arr(const int32_t *arr, uint32_t num_elems, uint32_t max_elems) {
    if(arr == NULL || num_elems < 1) {
        printf("ERROR: NULL array input.\n");
        return;
    }
    uint32_t i;
    for(i = 0; i < num_elems && i < max_elems; i++) {
        printf("%d\t", arr[i]);
        if((i+1)%10 == 0) {
            printf("\n");
        }
    }
    if(num_elems != max_elems && i == max_elems) {
        printf("... Remaining %u elements not printed ...\n", num_elems-max_elems);
    }
    printf("\n");
}

int compare_arr(const int32_t *arr_a, const int32_t *arr_b, uint32_t num_elems) {
    if(arr_a == NULL || arr_b == NULL) {
        return -3;
    }
    if(num_elems < 1) {
        return -1;
    }
    for(uint32_t i = 0; i < num_elems; i++) {
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
int generate_random_input_arr(int32_t *arr, uint32_t num_elems, uint32_t rand_max) {
    int32_t sign_flag, rand_num;
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
    for(uint32_t i = 0; i < num_elems; i++) {
        sign_flag = rand();
        if(RAND_MAX == 0x7fff) {
            rand_num = (rand()<<16 | rand()) % rand_max;
        }
        else{
            rand_num = rand() % rand_max;
        }
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
int generate_growing_arr(int32_t *arr, uint32_t num_elems) {
    if(arr == NULL) {
        return -5;
    }
    if(num_elems < 1) {
        return -3;
    }
    for(uint32_t i = 0; i < num_elems; i++) {
        arr[i] = i;
    }
    return 0;
}

/**
 * @brief Parse the command arguments and check whether the given
 *   flag (e.g. --force) is specified by one argument
 * 
 * @param [in]
 *   argc, **argv should come directly from the main();
 *   *cmd_flag is a given string (e.g. --force)
 * 
 * @returns
 *   -3: cmd_flag is NULL
 *   -1: argv or *argv is NULL
 *    1: not specified
 *    0: specified 
 */
int cmd_flag_parser(int argc, char **argv, const char *cmd_flag) {
    if(cmd_flag == NULL) {
        return -3;
    }
    if(argv == NULL || *argv == NULL) {
        return -1;
    }
    for(int i = 1; i < argc; i++) {
        if(strcmp(cmd_flag, argv[i]) == 0) {
            return 0;
        }
    }
    return 1;
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
int32_t* fui_brute(const int32_t *input_arr, const uint32_t num_elems, uint32_t *num_elems_out, int *err_flag){
    uint32_t i, j = 1, k;
    int32_t tmp = 0;
    int32_t *final_output_arr = NULL;
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
    int32_t *output_arr = (int32_t *)calloc(num_elems, sizeof(int32_t));
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
    final_output_arr = (int32_t *)realloc(output_arr, j*sizeof(int32_t));
    if(final_output_arr == NULL) {
        free(output_arr);
        *err_flag = 1;
        return NULL;
    }
    *num_elems_out = j;
    return final_output_arr;
}

uint32_t fui_brute_count(const int32_t *input_arr, const uint32_t num_elems, int *err_flag) {
    uint32_t i, j = 1, k;
    int32_t tmp = 0;
    *err_flag = 0;
    if (input_arr == NULL) {
        *err_flag = -5;
        return 0;
    }
    if (num_elems < 1){
        *err_flag = -3;
        return 0;
    }
    int32_t *output_arr = (int32_t *)calloc(num_elems, sizeof(int32_t));
    if (output_arr == NULL) {
        *err_flag = -1;
        return 0;
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
    free(output_arr);
    return j;
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
int32_t* fui_brute_opt(const int32_t *input_arr, const uint32_t num_elems, uint32_t *num_elems_out, int *err_flag){
    uint32_t i, j = 1, k;
    int32_t max_current, min_current, diff_to_max = 0, diff_to_min = 0, tmp_diff_to_max = 0, tmp_diff_to_min = 0;
    int32_t tmp = 0;
    int32_t *final_output_arr = NULL;
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
    int32_t *output_arr = (int32_t *)calloc(num_elems, sizeof(int32_t));
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
    final_output_arr = (int32_t *)realloc(output_arr, j*sizeof(int32_t));
    if(final_output_arr == NULL) {
        free(output_arr);
        *err_flag = 1;
        return NULL;
    }
    *num_elems_out = j;
    return final_output_arr;
}

uint32_t fui_brute_opt_count(const int32_t *input_arr, const uint32_t num_elems, int *err_flag) {
    uint32_t i, j = 1, k;
    int32_t max_current, min_current, diff_to_max = 0, diff_to_min = 0, tmp_diff_to_max = 0, tmp_diff_to_min = 0;
    int32_t tmp = 0;
    *err_flag = 0;
    if (input_arr == NULL) {
        *err_flag = -5;
        return 0;
    }
    if (num_elems < 1){
        *err_flag = -3;
        return 0;
    }
    int32_t *output_arr = (int32_t *)calloc(num_elems, sizeof(int32_t));
    if (output_arr == NULL) {
        *err_flag = -1;
        return 0;
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
    free(output_arr);
    return j;
}

void free_hash_table(uint8_t *hash_table[], uint32_t num_elems) {
    for(uint32_t i = 0; i < num_elems; i++) {
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
int32_t* fui_htable_dtree(const int32_t *input_arr, const uint32_t num_elems, uint32_t *num_elems_out, int *err_flag) {
    uint32_t i, j = 0;
    uint32_t tmp_quotient = 0, tmp_mod = 0;
    int32_t tmp = 0;
    int32_t *final_output_arr = NULL;
    uint8_t *hash_table_base_p[HASH_TABLE_SIZE] = {NULL,};
    uint8_t *hash_table_base_n[HASH_TABLE_SIZE] = {NULL,};
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
    int32_t *output_arr = (int32_t *)calloc(num_elems, sizeof(int32_t));
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
                if((hash_table_base_p[tmp_quotient] = (uint8_t *)calloc(MOD_TABLE_SIZE, sizeof(uint8_t))) == NULL) {
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
                if((hash_table_base_n[tmp_quotient] = (uint8_t *)calloc(MOD_TABLE_SIZE, sizeof(uint8_t))) == NULL) {
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
    final_output_arr = (int32_t *)realloc(output_arr, j*sizeof(int32_t));
    if(final_output_arr == NULL) {
        free(output_arr);
        *err_flag = 3;
        return NULL;
    }
    *num_elems_out = j;
    return final_output_arr;
}

uint32_t fui_htable_dtree_count(const int32_t *input_arr, const uint32_t num_elems, int *err_flag) {
    uint32_t i, j = 0;
    uint32_t tmp_quotient = 0, tmp_mod = 0;
    int32_t tmp = 0;
    uint8_t *hash_table_base_p[HASH_TABLE_SIZE] = {NULL,};
    uint8_t *hash_table_base_n[HASH_TABLE_SIZE] = {NULL,};
    *err_flag = 0;
    if (input_arr == NULL) {
        *err_flag = -5;
        return 0;
    }
    if (num_elems < 1){
        *err_flag = -3;
        return 0;
    }
    for(i = 0; i < num_elems; i++) {
        tmp = input_arr[i];
        tmp_quotient = abs(tmp / MOD_TABLE_SIZE);
        tmp_mod = abs(tmp % MOD_TABLE_SIZE);
        if(tmp > 0) {
            if(hash_table_base_p[tmp_quotient] == NULL) {
                if((hash_table_base_p[tmp_quotient] = (uint8_t *)calloc(MOD_TABLE_SIZE, sizeof(uint8_t))) == NULL) {
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
                if((hash_table_base_n[tmp_quotient] = (uint8_t *)calloc(MOD_TABLE_SIZE, sizeof(uint8_t))) == NULL) {
                    *err_flag = 1;
                    goto free_memory;
                }
            }
            if(hash_table_base_n[tmp_quotient] != NULL && (hash_table_base_n[tmp_quotient])[tmp_mod] != 0) {
                continue;
            }
        }
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
        return 0;
    }
    return j;
}


void free_hash_table_new(htable_base hash_table_new[], uint32_t num_elems) {
    for(uint32_t i = 0; i < num_elems; i++) {
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
int32_t* fui_htable_stree(const int *input_arr, const uint32_t num_elems, uint32_t *num_elems_out, int *err_flag) {
    uint32_t i, j = 0;
    uint32_t tmp_quotient = 0, tmp_mod = 0;
    int32_t tmp = 0;
    int32_t *final_output_arr = NULL;
    uint8_t *tmp_realloc_ptr = NULL;
    htable_base hash_table_base[HASH_TABLE_SIZE] = {{0, 0, NULL, NULL},};
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
    int32_t *output_arr = (int32_t *)calloc(num_elems, sizeof(int32_t));
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
                if((hash_table_base[tmp_quotient].ptr_branch_p = (uint8_t *)calloc(tmp_mod + 1, sizeof(uint8_t))) == NULL) {
                    *err_flag = 1;
                    goto free_memory;
                }
                else {
                    hash_table_base[tmp_quotient].branch_size_p = tmp_mod + 1;
                }
            }
            else {
                if(hash_table_base[tmp_quotient].branch_size_p < (tmp_mod + 1)){
                    if((tmp_realloc_ptr = (uint8_t *)realloc(hash_table_base[tmp_quotient].ptr_branch_p, (tmp_mod + 1)*sizeof(uint8_t))) == NULL) {
                        *err_flag = 1;
                        goto free_memory;
                    }
                    else {
                        hash_table_base[tmp_quotient].ptr_branch_p = tmp_realloc_ptr;
                        memset(tmp_realloc_ptr + hash_table_base[tmp_quotient].branch_size_p, 0, sizeof(uint8_t) * (tmp_mod + 1 - hash_table_base[tmp_quotient].branch_size_p));
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
                if((hash_table_base[tmp_quotient].ptr_branch_n = (uint8_t *)calloc(tmp_mod + 1, sizeof(uint8_t))) == NULL) {
                    *err_flag = 1;
                    goto free_memory;
                }
                else {
                    hash_table_base[tmp_quotient].branch_size_n = tmp_mod + 1;
                }
            }
            else {
                if(hash_table_base[tmp_quotient].branch_size_n < (tmp_mod + 1)){
                    if((tmp_realloc_ptr = (uint8_t *)realloc(hash_table_base[tmp_quotient].ptr_branch_n, (tmp_mod + 1) * sizeof(uint8_t))) == NULL) {
                        *err_flag = 1;
                        goto free_memory;
                    }
                    else {
                        hash_table_base[tmp_quotient].ptr_branch_n = tmp_realloc_ptr;
                        memset(tmp_realloc_ptr + hash_table_base[tmp_quotient].branch_size_n , 0, sizeof(uint8_t) * (tmp_mod + 1 - hash_table_base[tmp_quotient].branch_size_n));
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
    final_output_arr = (int32_t *)realloc(output_arr, j*sizeof(int32_t));
    if(final_output_arr == NULL) {
        free(output_arr);
        *err_flag = 3;
        return NULL;
    }
    *num_elems_out = j;
    return final_output_arr;
}


uint32_t fui_htable_stree_count(const int32_t *input_arr, const uint32_t num_elems, int *err_flag) {
    uint32_t i, j = 0;
    uint32_t tmp_quotient = 0, tmp_mod = 0;
    int32_t tmp = 0;
    uint8_t *tmp_realloc_ptr = NULL;
    htable_base hash_table_base[HASH_TABLE_SIZE] = {{0, 0, NULL, NULL},};
    *err_flag = 0;
    if (input_arr == NULL) {
        *err_flag = -5;
        return 0;
    }
    if (num_elems < 1){
        *err_flag = -3;
        return 0;
    }
    for(i = 0; i < num_elems; i++) {
        tmp = input_arr[i];
        tmp_quotient = abs(tmp / MOD_TABLE_SIZE);
        tmp_mod = abs(tmp % MOD_TABLE_SIZE);
        if(tmp > 0) {
            if(hash_table_base[tmp_quotient].ptr_branch_p == NULL) {
                if((hash_table_base[tmp_quotient].ptr_branch_p = (uint8_t *)calloc(tmp_mod + 1, sizeof(uint8_t))) == NULL) {
                    *err_flag = 1;
                    goto free_memory;
                }
                else {
                    hash_table_base[tmp_quotient].branch_size_p = tmp_mod + 1;
                }
            }
            else {
                if(hash_table_base[tmp_quotient].branch_size_p < (tmp_mod + 1)){
                    if((tmp_realloc_ptr = (uint8_t *)realloc(hash_table_base[tmp_quotient].ptr_branch_p, (tmp_mod + 1)*sizeof(uint8_t))) == NULL) {
                        *err_flag = 1;
                        goto free_memory;
                    }
                    else {
                        hash_table_base[tmp_quotient].ptr_branch_p = tmp_realloc_ptr;
                        memset(tmp_realloc_ptr + hash_table_base[tmp_quotient].branch_size_p, 0, sizeof(uint8_t) * (tmp_mod + 1 - hash_table_base[tmp_quotient].branch_size_p));
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
                if((hash_table_base[tmp_quotient].ptr_branch_n = (uint8_t *)calloc(tmp_mod + 1, sizeof(uint8_t))) == NULL) {
                    *err_flag = 1;
                    goto free_memory;
                }
                else {
                    hash_table_base[tmp_quotient].branch_size_n = tmp_mod + 1;
                }
            }
            else {
                if(hash_table_base[tmp_quotient].branch_size_n < (tmp_mod + 1)){
                    if((tmp_realloc_ptr = (uint8_t *)realloc(hash_table_base[tmp_quotient].ptr_branch_n, (tmp_mod + 1) * sizeof(uint8_t))) == NULL) {
                        *err_flag = 1;
                        goto free_memory;
                    }
                    else {
                        hash_table_base[tmp_quotient].ptr_branch_n = tmp_realloc_ptr;
                        memset(tmp_realloc_ptr + hash_table_base[tmp_quotient].branch_size_n , 0, sizeof(uint8_t) * (tmp_mod + 1 - hash_table_base[tmp_quotient].branch_size_n));
                        hash_table_base[tmp_quotient].branch_size_n = tmp_mod + 1;
                    }
                }
            }
            if(hash_table_base[tmp_quotient].ptr_branch_n != NULL && (hash_table_base[tmp_quotient].ptr_branch_n)[tmp_mod] != 0) {
                continue;
            }
        }
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
        return 0;
    }
    return j;
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
int32_t* fui_htable_stree_dyn(const int32_t *input_arr, const uint32_t num_elems, uint32_t *num_elems_out, int *err_flag) {
    uint32_t i, j = 0;
    uint32_t tmp_quotient = 0, tmp_mod = 0;
    int32_t tmp = 0;
    int32_t *final_output_arr = NULL;
    uint8_t *tmp_realloc_ptr = NULL;
    htable_base *hash_table_base = NULL, *tmp_ht_realloc_ptr = NULL;
    uint32_t ht_base_length = HT_DYN_INI_SIZE;
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
    hash_table_base = (htable_base *)calloc(HT_DYN_INI_SIZE, sizeof(htable_base));
    if(hash_table_base == NULL) {
        *err_flag = 5;
        return NULL;
    }
    int32_t *output_arr = (int32_t *)calloc(num_elems, sizeof(int32_t));
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
            tmp_ht_realloc_ptr = (htable_base *)realloc(hash_table_base, (tmp_quotient + 1) * sizeof(htable_base));
            if(tmp_ht_realloc_ptr == NULL) {
                *err_flag = 7;
                goto free_memory;
            }
            memset(tmp_ht_realloc_ptr + ht_base_length, 0, (tmp_quotient + 1 - ht_base_length) * sizeof(htable_base));
            hash_table_base = tmp_ht_realloc_ptr;
            ht_base_length = (tmp_quotient + 1);
        }
        if(tmp > 0) {
            if(hash_table_base[tmp_quotient].ptr_branch_p == NULL) {
                if((hash_table_base[tmp_quotient].ptr_branch_p = (uint8_t *)calloc(tmp_mod + 1, sizeof(uint8_t))) == NULL) {
                    *err_flag = 1;
                    goto free_memory;
                }
                else {
                    hash_table_base[tmp_quotient].branch_size_p = tmp_mod + 1;
                }
            }
            else {
                if(hash_table_base[tmp_quotient].branch_size_p < (tmp_mod + 1)){
                    if((tmp_realloc_ptr = (uint8_t *)realloc(hash_table_base[tmp_quotient].ptr_branch_p, (tmp_mod + 1) * sizeof(uint8_t))) == NULL) {
                        *err_flag = 1;
                        goto free_memory;
                    }
                    else {
                        hash_table_base[tmp_quotient].ptr_branch_p = tmp_realloc_ptr;
                        memset(tmp_realloc_ptr + hash_table_base[tmp_quotient].branch_size_p, 0, sizeof(uint8_t) * (tmp_mod + 1 - hash_table_base[tmp_quotient].branch_size_p));
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
                if((hash_table_base[tmp_quotient].ptr_branch_n = (uint8_t *)calloc(tmp_mod + 1, sizeof(uint8_t))) == NULL) {
                    *err_flag = 1;
                    goto free_memory;
                }
                else {
                    hash_table_base[tmp_quotient].branch_size_n = tmp_mod + 1;
                }
            }
            else {
                if(hash_table_base[tmp_quotient].branch_size_n < (tmp_mod + 1)){
                    if((tmp_realloc_ptr = (uint8_t *)realloc(hash_table_base[tmp_quotient].ptr_branch_n, (tmp_mod + 1)*sizeof(uint8_t))) == NULL) {
                        *err_flag = 1;
                        goto free_memory;
                    }
                    else {
                        hash_table_base[tmp_quotient].ptr_branch_n = tmp_realloc_ptr;
                        memset(tmp_realloc_ptr + hash_table_base[tmp_quotient].branch_size_n , 0, sizeof(uint8_t) * (tmp_mod + 1 - hash_table_base[tmp_quotient].branch_size_n));
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
    final_output_arr = (int32_t *)realloc(output_arr, j*sizeof(int32_t));
    if(final_output_arr == NULL) {
        free(output_arr);
        *err_flag = 3;
        return NULL;
    }
    *num_elems_out = j;
    return final_output_arr;
}

uint32_t fui_htable_stree_dyn_count(const int32_t *input_arr, const uint32_t num_elems, int *err_flag) {
    uint32_t i, j = 0;
    uint32_t tmp_quotient = 0, tmp_mod = 0;
    int32_t tmp = 0;
    uint8_t *tmp_realloc_ptr = NULL;
    htable_base *hash_table_base = NULL, *tmp_ht_realloc_ptr = NULL;
    uint32_t ht_base_length = HT_DYN_INI_SIZE;
    *err_flag = 0;
    if (input_arr == NULL) {
        *err_flag = -5;
        return 0;
    }
    if (num_elems < 1){
        *err_flag = -3;
        return 0;
    }
    hash_table_base = (htable_base *)calloc(HT_DYN_INI_SIZE, sizeof(htable_base));
    if(hash_table_base == NULL) {
        *err_flag = 5;
        return 0;
    }
    for(i = 0; i < num_elems; i++) {
        tmp = input_arr[i];
        tmp_quotient = abs(tmp / MOD_TABLE_SIZE);
        tmp_mod = abs(tmp % MOD_TABLE_SIZE);
        if((tmp_quotient + 1) > ht_base_length) {
            tmp_ht_realloc_ptr = (htable_base *)realloc(hash_table_base, (tmp_quotient + 1) * sizeof(htable_base));
            if(tmp_ht_realloc_ptr == NULL) {
                *err_flag = 7;
                goto free_memory;
            }
            memset(tmp_ht_realloc_ptr + ht_base_length, 0, (tmp_quotient + 1 - ht_base_length) * sizeof(htable_base));
            hash_table_base = tmp_ht_realloc_ptr;
            ht_base_length = (tmp_quotient + 1);
        }
        if(tmp > 0) {
            if(hash_table_base[tmp_quotient].ptr_branch_p == NULL) {
                if((hash_table_base[tmp_quotient].ptr_branch_p = (uint8_t *)calloc(tmp_mod + 1, sizeof(uint8_t))) == NULL) {
                    *err_flag = 1;
                    goto free_memory;
                }
                else {
                    hash_table_base[tmp_quotient].branch_size_p = tmp_mod + 1;
                }
            }
            else {
                if(hash_table_base[tmp_quotient].branch_size_p < (tmp_mod + 1)){
                    if((tmp_realloc_ptr = (uint8_t *)realloc(hash_table_base[tmp_quotient].ptr_branch_p, (tmp_mod + 1) * sizeof(uint8_t))) == NULL) {
                        *err_flag = 1;
                        goto free_memory;
                    }
                    else {
                        hash_table_base[tmp_quotient].ptr_branch_p = tmp_realloc_ptr;
                        memset(tmp_realloc_ptr + hash_table_base[tmp_quotient].branch_size_p, 0, sizeof(uint8_t) * (tmp_mod + 1 - hash_table_base[tmp_quotient].branch_size_p));
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
                if((hash_table_base[tmp_quotient].ptr_branch_n = (uint8_t *)calloc(tmp_mod + 1, sizeof(uint8_t))) == NULL) {
                    *err_flag = 1;
                    goto free_memory;
                }
                else {
                    hash_table_base[tmp_quotient].branch_size_n = tmp_mod + 1;
                }
            }
            else {
                if(hash_table_base[tmp_quotient].branch_size_n < (tmp_mod + 1)){
                    if((tmp_realloc_ptr = (uint8_t *)realloc(hash_table_base[tmp_quotient].ptr_branch_n, (tmp_mod + 1)*sizeof(uint8_t))) == NULL) {
                        *err_flag = 1;
                        goto free_memory;
                    }
                    else {
                        hash_table_base[tmp_quotient].ptr_branch_n = tmp_realloc_ptr;
                        memset(tmp_realloc_ptr + hash_table_base[tmp_quotient].branch_size_n , 0, sizeof(uint8_t) * (tmp_mod + 1 - hash_table_base[tmp_quotient].branch_size_n));
                        hash_table_base[tmp_quotient].branch_size_n = tmp_mod + 1;
                    }
                }
            }
            if(hash_table_base[tmp_quotient].ptr_branch_n != NULL && (hash_table_base[tmp_quotient].ptr_branch_n)[tmp_mod] != 0) {
                continue;
            }
        }
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
        return 0;
    }
    return j;
}

/**
 * Using linked-list makes the performance really bad when the 
 * Original inpu array is medium and large. Abandon it. If you
 * would like to test this algorithm (int* fui_bitmap_idx_llist),
 * Please uncomment the contents below. * 
 *  
void free_bmap_idx_branch(bmap_idx_brch *bmap_idx_head) {
    bmap_idx_brch *ptr_current = bmap_idx_head;
    bmap_idx_brch *ptr_next;
    while(ptr_current != NULL){
        ptr_next = ptr_current->ptr_next;
        free(ptr_current);
        ptr_current = ptr_next;
    }
} */

/* 
int insert_idx_branch(bmap_idx_brch **bmap_idx_head, uint16_t byte_idx, uint8_t bit_pos, uint32_t raw_idx) {
    bmap_idx_brch *new_node = (bmap_idx_brch *)calloc(1, sizeof(bmap_idx_brch));
    if(new_node == NULL) {
        return -1;
    }
    new_node->byte_index = byte_idx;
    new_node->bit_position = bit_pos;
    new_node->raw_index = raw_idx;
    if(*bmap_idx_head == NULL) {
        *bmap_idx_head = new_node;
        new_node->ptr_next = NULL;
        return 0;
    }
    new_node->ptr_next = *bmap_idx_head;
    *bmap_idx_head = new_node;
    return 0;
}*/

/*
int get_raw_idx(bmap_idx_brch *bmap_idx_head, uint16_t byte_idx, uint8_t bit_pos, uint32_t *raw_idx) {
    bmap_idx_brch *ptr_current = bmap_idx_head;
    *raw_idx = 0;
    if(ptr_current == NULL) {
        return -1;
    }
    while(ptr_current != NULL) {
        if(ptr_current->byte_index == byte_idx && ptr_current->bit_position == bit_pos) {
            *raw_idx = ptr_current->raw_index;
            return 0;
        }
        ptr_current = ptr_current->ptr_next;
    }
    return -3;
} */

/*
void free_bitmap_idx(bitmap_idx_base *bitmap_head, uint16_t num_elems) {
    for(uint32_t i = 0; i < num_elems; i++) {
        if(bitmap_head[i].ptr_bit_branch != NULL) {
            free(bitmap_head[i].ptr_bit_branch);
        }
        if(bitmap_head[i].ptr_idx_branch != NULL) {
            free_bmap_idx_branch(bitmap_head[i].ptr_idx_branch);
        }
    }
}*/

/*
int* fui_bitmap_idx_llist(const int *input_arr, const uint32_t num_elems, uint32_t *num_elems_out, int *err_flag, dup_idx_list **dup_idx_head) {
    uint32_t i, j = 0;
    uint32_t tmp_quotient = 0, tmp_mod = 0, tmp_dup_raw_index;
    int tmp = 0, *final_output_arr = NULL;
    bitmap_idx_base *bitmap_head = NULL, *tmp_bitmap_realloc = NULL;
    uint16_t tmp_byte_index = 0, bitmap_base_size = BITMAP_INIT_LENGTH;
    uint8_t tmp_bit_position = 0;
    *err_flag = 0;
    *num_elems_out = 0;
    if(*dup_idx_head != NULL) {
        *err_flag = -9;
        return NULL;
    }
    if (input_arr == NULL) {
        *err_flag = -5;
        return NULL;
    }
    if (num_elems < 1){
        *err_flag = -3;
        return NULL;
    }
    dup_idx_list *dup_idx_head_tmp = NULL;
    bitmap_head = (bitmap_idx_base *)calloc(BITMAP_INIT_LENGTH, sizeof(bitmap_idx_base));
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
        if((tmp_quotient + 1) > bitmap_base_size) {
            if((tmp_bitmap_realloc = (bitmap_idx_base *)realloc(bitmap_head, (tmp_quotient + 1) * sizeof(bitmap_idx_base))) == NULL) {
                *err_flag = 7;
                goto free_memory;
            }
            memset(tmp_bitmap_realloc + bitmap_base_size, 0, (tmp_quotient + 1 - bitmap_base_size) * sizeof(bitmap_base));
            bitmap_head = tmp_bitmap_realloc;
            bitmap_base_size = (tmp_quotient + 1);
        }
        if(bitmap_head[tmp_quotient].ptr_bit_branch == NULL) {
            if((bitmap_head[tmp_quotient].ptr_bit_branch = (uint8_t *)calloc(BIT_MOD_TABLE_SIZE, sizeof(uint8_t))) == NULL) {
                *err_flag = 1;
                goto free_memory;
            }
        }
        if(bitmap_head[tmp_quotient].ptr_bit_branch != NULL && check_bit((bitmap_head[tmp_quotient].ptr_bit_branch)[tmp_byte_index], tmp_bit_position)) {
            if(get_raw_idx(bitmap_head[tmp_quotient].ptr_idx_branch, tmp_byte_index, tmp_bit_position, &tmp_dup_raw_index) != 0) {
                *err_flag = 9;
                goto free_memory;
            }
            insert_dup_idx_list(&dup_idx_head_tmp, i, tmp_dup_raw_index);
            continue;
        }
        output_arr[j] = tmp;
        j++;
        flip_bit((bitmap_head[tmp_quotient].ptr_bit_branch)[tmp_byte_index], tmp_bit_position);
        insert_idx_branch(&(bitmap_head[tmp_quotient].ptr_idx_branch), tmp_byte_index, tmp_bit_position, i);
    }
free_memory:
    free_bitmap_idx(bitmap_head, bitmap_base_size);
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
    *dup_idx_head = dup_idx_head_tmp;
    return final_output_arr;
}*/

void free_dup_idx_list(dup_idx_list *dup_idx_head) {
    dup_idx_list *ptr_current = dup_idx_head;
    dup_idx_list *ptr_next;
    while(ptr_current != NULL){
        ptr_next = ptr_current->ptr_next;
        free(ptr_current);
        ptr_current = ptr_next;
    }
}

int insert_dup_idx_list(dup_idx_list **dup_idx_head, uint32_t idx_a, uint32_t idx_b) {
    dup_idx_list *new_node = (dup_idx_list *)calloc(1, sizeof(dup_idx_list));
    if(new_node == NULL) {
        return -1;
    }
    new_node->index_a = idx_a;
    new_node->index_b = idx_b;
    if(*dup_idx_head == NULL) {
        *dup_idx_head = new_node;
        new_node->ptr_next = NULL;
        return 0;
    }
    new_node->ptr_next = *dup_idx_head;
    *dup_idx_head = new_node;
    return 0;
}

void print_dup_idx_list(dup_idx_list *dup_idx_head, uint32_t max_nodes) {
    dup_idx_list *ptr_current = dup_idx_head;
    printf("\n");
    if(ptr_current == NULL) {
        printf("NULL LIST!\n");
        return;
    }
    uint32_t i = 0;
    printf("\nIndex pairs of duplicate elements:\n");
    while(ptr_current != NULL && i < max_nodes) {
        printf("{%u\t%u}\n", ptr_current->index_a, ptr_current->index_b);
        ptr_current = ptr_current->ptr_next;
        i++;
    }
    if( i == max_nodes) {
        printf("... Remaining elements not printed ...\n");
    }
    else {
        printf("Print done.\n");
    }
}

void print_out_idx(out_idx *output_index, uint32_t num_elems, uint32_t max_elems) {
    if(output_index == NULL) {
        printf("NULL OUTPUT AND INDEX!\n");
        return;
    }
    uint32_t i;
    printf("\nRaw index and duplicate elements:\n");
    for(i = 0; i < max_elems && i < num_elems; i++) {
        printf("%u\t%d\n", output_index[i].raw_index, output_index[i].out_elem);
    }
    if(max_elems != num_elems && i == max_elems) {
        printf("... %u remaining elements not printed ...\n", num_elems - max_elems);
    }
    else{
        printf("Print done.\n");
    }
}

void free_bitmap(bitmap_base *bitmap_head, uint16_t num_elems) {
    for(uint32_t i = 0; i < num_elems; i++) {
        if(bitmap_head[i].ptr_branch != NULL) {
            free(bitmap_head[i].ptr_branch);
        }
    }
}

void free_bitmap_dtree(bitmap_dtree *bitmap_head, uint16_t num_elems) {
    if(bitmap_head == NULL) {
        return;
    }
    for(uint16_t i = 0; i < num_elems; i++) {
        if(bitmap_head[i].ptr_branch[0] != NULL) {
            free(bitmap_head[i].ptr_branch[0]);
        }
        if(bitmap_head[i].ptr_branch[1] != NULL) {
            free(bitmap_head[i].ptr_branch[1]);
        }
    }
}

void free_idx_ht_8(idx_ht_8 *idx_ht_head, uint16_t num_elems) {
    if(idx_ht_head == NULL) {
        return;
    }
    for(uint16_t i = 0; i < num_elems; i++) {
        if(idx_ht_head[i].ptr_branch[0] != NULL) {
            free(idx_ht_head[i].ptr_branch[0]);
        }
        if(idx_ht_head[i].ptr_branch[1] != NULL) {
            free(idx_ht_head[i].ptr_branch[1]);
        }
    }
}
void free_idx_ht_16(idx_ht_16 *idx_ht_head, uint16_t num_elems) {
    if(idx_ht_head == NULL) {
        return;
    }
    for(uint16_t i = 0; i < num_elems; i++) {
        if(idx_ht_head[i].ptr_branch[0] != NULL) {
            free(idx_ht_head[i].ptr_branch[0]);
        }
        if(idx_ht_head[i].ptr_branch[1] != NULL) {
            free(idx_ht_head[i].ptr_branch[1]);
        }
    }
}

void free_idx_ht_32(idx_ht_32 *idx_ht_head, uint16_t num_elems) {
    if(idx_ht_head == NULL) {
        return;
    }
    for(uint16_t i = 0; i < num_elems; i++) {
        if(idx_ht_head[i].ptr_branch[0] != NULL) {
            free(idx_ht_head[i].ptr_branch[0]);
        }
        if(idx_ht_head[i].ptr_branch[1] != NULL) {
            free(idx_ht_head[i].ptr_branch[1]);
        }
    }
}

int32_t* fui_bitmap_stc_stree(const int32_t *input_arr, const uint32_t num_elems, uint32_t *num_elems_out, int *err_flag) {
    uint32_t i, j = 0;
    uint32_t tmp_quotient = 0, tmp_mod = 0;
    int32_t tmp = 0, *final_output_arr = NULL;
    bitmap_base bitmap_head[BITMAP_LENGTH_MAX] = {{0, NULL},};
    uint16_t tmp_byte_index = 0;
    uint8_t tmp_bit_position = 0;
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
    int32_t *output_arr = (int32_t *)calloc(num_elems, sizeof(int32_t));
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
            if((bitmap_head[tmp_quotient].ptr_branch = (uint8_t *)calloc(BIT_MOD_TABLE_SIZE, sizeof(uint8_t))) == NULL) {
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
    final_output_arr = (int32_t *)realloc(output_arr, j * sizeof(int32_t));
    if(final_output_arr == NULL) {
        free(output_arr);
        *err_flag = 3;
        return NULL;
    }
    *num_elems_out = j;
    return final_output_arr;
}

uint32_t fui_bitmap_stc_stree_count(const int32_t *input_arr, const uint32_t num_elems, int *err_flag) {
    uint32_t i, j = 0;
    uint32_t tmp_quotient = 0, tmp_mod = 0;
    int32_t tmp = 0;
    bitmap_base bitmap_head[BITMAP_LENGTH_MAX] = {{0, NULL},};
    uint16_t tmp_byte_index = 0;
    uint8_t tmp_bit_position = 0;
    *err_flag = 0;
    if (input_arr == NULL) {
        *err_flag = -5;
        return 0;
    }
    if (num_elems < 1){
        *err_flag = -3;
        return 0;
    }
    for(i = 0; i < num_elems; i++) {
        tmp = input_arr[i];
        tmp_quotient = abs(tmp / BIT_MOD_DIV_FACTOR);
        tmp_mod = abs(tmp % BIT_MOD_DIV_FACTOR);
        tmp_byte_index = (tmp < 0) ? (NEGATIVE_START_POS + (tmp_mod / 8)) : (tmp_mod / 8);
        tmp_bit_position = tmp_mod % 8;
        if(bitmap_head[tmp_quotient].ptr_branch == NULL) {
            if((bitmap_head[tmp_quotient].ptr_branch = (uint8_t *)calloc(BIT_MOD_TABLE_SIZE, sizeof(uint8_t))) == NULL) {
                *err_flag = 1;
                goto free_memory;
            }
        }
        if(bitmap_head[tmp_quotient].ptr_branch != NULL && check_bit((bitmap_head[tmp_quotient].ptr_branch)[tmp_byte_index], tmp_bit_position)) {
            continue;
        }
        j++;
        flip_bit((bitmap_head[tmp_quotient].ptr_branch)[tmp_byte_index], tmp_bit_position);
    }
free_memory:
    free_bitmap(bitmap_head, BITMAP_LENGTH_MAX);
    if(*err_flag != 0) {
        return 0;
    }
    return j;
}

int32_t* fui_bitmap_dyn_stree(const int32_t *input_arr, const uint32_t num_elems, uint32_t *num_elems_out, int *err_flag) {
    uint32_t i, j = 0;
    uint32_t tmp_quotient = 0, tmp_mod = 0;
    int32_t tmp = 0, *final_output_arr = NULL;
    bitmap_base *bitmap_head = NULL, *tmp_bitmap_realloc = NULL;
    uint16_t tmp_byte_index = 0, bitmap_base_size = BITMAP_INIT_LENGTH, bitmap_base_size_target = 0;
    uint8_t tmp_bit_position = 0;
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
    bitmap_head = (bitmap_base *)calloc(BITMAP_INIT_LENGTH, sizeof(bitmap_base));
    if(bitmap_head == NULL) {
        *err_flag = 5;
        return NULL;
    }
    int32_t *output_arr = (int32_t *)calloc(num_elems, sizeof(int32_t));
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
            bitmap_base_size_target = (((tmp_quotient + 1) << 1) > BITMAP_LENGTH_MAX) ? BITMAP_LENGTH_MAX : ((tmp_quotient + 1) << 1);
            if((tmp_bitmap_realloc = (bitmap_base *)realloc(bitmap_head, bitmap_base_size_target * sizeof(bitmap_base))) == NULL) {
                *err_flag = 7;
                goto free_memory;
            }
            memset(tmp_bitmap_realloc + bitmap_base_size, 0, (bitmap_base_size_target - bitmap_base_size) * sizeof(bitmap_base));
            bitmap_head = tmp_bitmap_realloc;
            bitmap_base_size = bitmap_base_size_target;
        }
        if(bitmap_head[tmp_quotient].ptr_branch == NULL) {
            if((bitmap_head[tmp_quotient].ptr_branch = (uint8_t *)calloc(BIT_MOD_TABLE_SIZE, sizeof(uint8_t))) == NULL) {
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
    final_output_arr = (int32_t *)realloc(output_arr, j * sizeof(int32_t));
    if(final_output_arr == NULL) {
        free(output_arr);
        *err_flag = 3;
        return NULL;
    }
    *num_elems_out = j;
    return final_output_arr;
}

uint32_t fui_bitmap_dyn_stree_count(const int32_t *input_arr, const uint32_t num_elems, int *err_flag) {
    uint32_t i, j = 0;
    uint32_t tmp_quotient = 0, tmp_mod = 0;
    int32_t tmp = 0;
    bitmap_base *bitmap_head = NULL, *tmp_bitmap_realloc = NULL;
    uint16_t tmp_byte_index = 0, bitmap_base_size = BITMAP_INIT_LENGTH, bitmap_base_size_target = 0;
    uint8_t tmp_bit_position = 0;
    *err_flag = 0;
    if (input_arr == NULL) {
        *err_flag = -5;
        return 0;
    }
    if (num_elems < 1){
        *err_flag = -3;
        return 0;
    }
    bitmap_head = (bitmap_base *)calloc(BITMAP_INIT_LENGTH, sizeof(bitmap_base));
    if(bitmap_head == NULL) {
        *err_flag = 5;
        return 0;
    }
    for(i = 0; i < num_elems; i++) {
        tmp = input_arr[i];
        tmp_quotient = abs(tmp / BIT_MOD_DIV_FACTOR);
        tmp_mod = abs(tmp % BIT_MOD_DIV_FACTOR);
        tmp_byte_index = (tmp < 0) ? (NEGATIVE_START_POS + (tmp_mod / 8)) : (tmp_mod / 8);
        tmp_bit_position = tmp_mod % 8;

        /* Grow the tree if needed. */
        if((tmp_quotient + 1) > bitmap_base_size) {
            bitmap_base_size_target = (((tmp_quotient + 1) << 1) > BITMAP_LENGTH_MAX) ? BITMAP_LENGTH_MAX : ((tmp_quotient + 1) << 1);
            if((tmp_bitmap_realloc = (bitmap_base *)realloc(bitmap_head, bitmap_base_size_target * sizeof(bitmap_base))) == NULL) {
                *err_flag = 7;
                goto free_memory;
            }
            memset(tmp_bitmap_realloc + bitmap_base_size, 0, (bitmap_base_size_target - bitmap_base_size) * sizeof(bitmap_base));
            bitmap_head = tmp_bitmap_realloc;
            bitmap_base_size = bitmap_base_size_target;
        }
        if(bitmap_head[tmp_quotient].ptr_branch == NULL) {
            if((bitmap_head[tmp_quotient].ptr_branch = (uint8_t *)calloc(BIT_MOD_TABLE_SIZE, sizeof(uint8_t))) == NULL) {
                *err_flag = 1;
                goto free_memory;
            }
        }
        if(bitmap_head[tmp_quotient].ptr_branch != NULL && check_bit((bitmap_head[tmp_quotient].ptr_branch)[tmp_byte_index], tmp_bit_position)) {
            continue;
        }
        j++;
        flip_bit((bitmap_head[tmp_quotient].ptr_branch)[tmp_byte_index], tmp_bit_position);
    }
free_memory:
    free_bitmap(bitmap_head, bitmap_base_size);
    free(bitmap_head);
    if(*err_flag != 0) {
        return 0;
    }
    return j;
}

int32_t* fui_bitmap_dyn_dtree(const int32_t *input_arr, const uint32_t num_elems, uint32_t *num_elems_out, int *err_flag) {
    uint32_t i, j = 0;
    uint16_t tmp_quotient = 0, tmp_mod = 0;
    int32_t tmp = 0, *final_output_arr = NULL;
    bitmap_dtree *bitmap_head = NULL, *tmp_bitmap_realloc = NULL;
    uint16_t tmp_byte_index = 0, bitmap_base_size = BITMAP_INIT_LENGTH, bitmap_base_size_target = 0;
    uint8_t tmp_bit_position = 0;
    uint8_t tree_index = 0;
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
    bitmap_head = (bitmap_dtree *)calloc(BITMAP_INIT_LENGTH, sizeof(bitmap_dtree));
    if(bitmap_head == NULL) {
        *err_flag = 5;
        return NULL;
    }
    int32_t *output_arr = (int32_t *)calloc(num_elems, sizeof(int32_t));
    if (output_arr == NULL) {
        free(bitmap_head);
        *err_flag = -1;
        return NULL;
    }
    for(i = 0; i < num_elems; i++) {
        tmp = input_arr[i];
        tree_index = (tmp < 0) ? 1 : 0;
        tmp_quotient = abs(tmp / BIT_MOD_DIV_FACTOR);
        tmp_mod = abs(tmp % BIT_MOD_DIV_FACTOR);
        tmp_byte_index = (tmp_mod / 8);
        tmp_bit_position = tmp_mod % 8;

        /* Grow the tree if needed. */
        if((tmp_quotient + 1) > bitmap_base_size) {
            bitmap_base_size_target = (((tmp_quotient + 1) << 1) > BITMAP_LENGTH_MAX) ? BITMAP_LENGTH_MAX : ((tmp_quotient + 1) << 1);
            if((tmp_bitmap_realloc = (bitmap_dtree *)realloc(bitmap_head, bitmap_base_size_target * sizeof(bitmap_dtree))) == NULL) {
                *err_flag = 7;
                goto free_memory;
            }
            memset(tmp_bitmap_realloc + bitmap_base_size, 0, (bitmap_base_size_target - bitmap_base_size) * sizeof(bitmap_dtree));
            bitmap_head = tmp_bitmap_realloc;
            bitmap_base_size = bitmap_base_size_target;
        }
        if(bitmap_head[tmp_quotient].ptr_branch[tree_index] == NULL) {
            if((bitmap_head[tmp_quotient].ptr_branch[tree_index] = (uint8_t *)calloc(BITMAP_BRCH_DTREE, sizeof(uint8_t))) == NULL) {
                *err_flag = 1;
                goto free_memory;
            }
        }
        if(bitmap_head[tmp_quotient].ptr_branch[tree_index] != NULL && check_bit((bitmap_head[tmp_quotient].ptr_branch[tree_index])[tmp_byte_index], tmp_bit_position)) {
            continue;
        }
        output_arr[j] = tmp;
        j++;
        flip_bit((bitmap_head[tmp_quotient].ptr_branch[tree_index])[tmp_byte_index], tmp_bit_position);
    }
free_memory:
    free_bitmap_dtree(bitmap_head, bitmap_base_size);
    free(bitmap_head);
    if(*err_flag != 0) {
        free(output_arr);
        return NULL;
    }
    final_output_arr = (int32_t *)realloc(output_arr, j * sizeof(int32_t));
    if(final_output_arr == NULL) {
        free(output_arr);
        *err_flag = 3;
        return NULL;
    }
    *num_elems_out = j;
    return final_output_arr;
}

uint32_t fui_bitmap_dyn_dtree_count(const int32_t *input_arr, const uint32_t num_elems, int *err_flag) {
    uint32_t i, j = 0;
    uint16_t tmp_quotient = 0, tmp_mod = 0;
    int32_t tmp = 0;
    bitmap_dtree *bitmap_head = NULL, *tmp_bitmap_realloc = NULL;
    uint16_t tmp_byte_index = 0, bitmap_base_size = BITMAP_INIT_LENGTH, bitmap_base_size_target = 0;
    uint8_t tmp_bit_position = 0;
    uint8_t tree_index = 0;
    *err_flag = 0;
    if (input_arr == NULL) {
        *err_flag = -5;
        return 0;
    }
    if (num_elems < 1){
        *err_flag = -3;
        return 0;
    }
    bitmap_head = (bitmap_dtree *)calloc(BITMAP_INIT_LENGTH, sizeof(bitmap_dtree));
    if(bitmap_head == NULL) {
        *err_flag = 5;
        return 0;
    }
    for(i = 0; i < num_elems; i++) {
        tmp = input_arr[i];
        tree_index = (tmp < 0) ? 1 : 0;
        tmp_quotient = abs(tmp / BIT_MOD_DIV_FACTOR);
        tmp_mod = abs(tmp % BIT_MOD_DIV_FACTOR);
        tmp_byte_index = (tmp_mod / 8);
        tmp_bit_position = tmp_mod % 8;

        /* Grow the tree if needed. */
        if((tmp_quotient + 1) > bitmap_base_size) {
            bitmap_base_size_target = (((tmp_quotient + 1) << 1) > BITMAP_LENGTH_MAX) ? BITMAP_LENGTH_MAX : ((tmp_quotient + 1) << 1);
            if((tmp_bitmap_realloc = (bitmap_dtree *)realloc(bitmap_head, bitmap_base_size_target * sizeof(bitmap_dtree))) == NULL) {
                *err_flag = 7;
                goto free_memory;
            }
            memset(tmp_bitmap_realloc + bitmap_base_size, 0, (bitmap_base_size_target - bitmap_base_size) * sizeof(bitmap_dtree));
            bitmap_head = tmp_bitmap_realloc;
            bitmap_base_size = bitmap_base_size_target;
        }
        if(bitmap_head[tmp_quotient].ptr_branch[tree_index] == NULL) {
            if((bitmap_head[tmp_quotient].ptr_branch[tree_index] = (uint8_t *)calloc(BITMAP_BRCH_DTREE, sizeof(uint8_t))) == NULL) {
                *err_flag = 1;
                goto free_memory;
            }
        }
        if(bitmap_head[tmp_quotient].ptr_branch[tree_index] != NULL && check_bit((bitmap_head[tmp_quotient].ptr_branch[tree_index])[tmp_byte_index], tmp_bit_position)) {
            continue;
        }
        j++;
        flip_bit((bitmap_head[tmp_quotient].ptr_branch[tree_index])[tmp_byte_index], tmp_bit_position);
    }
free_memory:
    free_bitmap_dtree(bitmap_head, bitmap_base_size);
    free(bitmap_head);
    if(*err_flag != 0) {
        return 0;
    }
    return j;
}

out_idx* fui_bitmap_idx_stree(const int32_t *input_arr, const uint32_t num_elems, uint32_t *num_elems_out, int *err_flag, dup_idx_list **dup_idx_head) {
    uint32_t i, j = 0;
    uint32_t tmp_dup_raw_index;
    uint16_t tmp_quotient = 0, tmp_mod = 0;
    int32_t tmp = 0;
    out_idx *final_output_arr = NULL;
    bitmap_base *bitmap_head = NULL, *tmp_bitmap_head = NULL;
    uint8_t tree_index_idx = 0;
    void *idx_adj_head = NULL, *tmp_idx_adj = NULL;
    dup_idx_list *dup_idx_head_tmp = NULL;
    uint16_t tmp_byte = 0, bitmap_base_size = BITMAP_INIT_LENGTH, bitmap_base_size_target = 0;
    uint8_t tmp_bit_position = 0;
    uint8_t raw_index_range = 0;
    *err_flag = 0;
    *num_elems_out = 0;
    if(*dup_idx_head != NULL) {
        *err_flag = -9;
        return NULL;
    }
    if (input_arr == NULL) {
        *err_flag = -7;
        return NULL;
    }
    bitmap_head = (bitmap_base *)calloc(BITMAP_INIT_LENGTH, sizeof(bitmap_base));
    if(bitmap_head == NULL) {
        *err_flag = 5;
        return NULL;
    }
    out_idx *output_arr = (out_idx *)calloc(num_elems, sizeof(out_idx));
    if (output_arr == NULL) {
        free(bitmap_head);
        *err_flag = -1;
        return NULL;
    }
    if (num_elems < 1){
        *err_flag = -5;
    }
    else if(num_elems > UINT_16_MAX ) {
        if((idx_adj_head = (idx_ht_32 *)calloc(BITMAP_INIT_LENGTH, sizeof(idx_ht_32))) == NULL) {
            *err_flag = -3;
        }
        else {
            raw_index_range = 32;
        }
    }
    else if(num_elems > UINT_8_MAX) {
        if((idx_adj_head = (idx_ht_16 *)calloc(BITMAP_INIT_LENGTH, sizeof(idx_ht_16))) == NULL) {
            *err_flag = -3;
        }
        else {
            raw_index_range = 16;
        }
    }
    else{
        if((idx_adj_head = (idx_ht_8 *)calloc(BITMAP_INIT_LENGTH, sizeof(idx_ht_8))) == NULL) {
            *err_flag = -3;
        }
        else {
            raw_index_range = 8;
        }
    }
    if(*err_flag != 0) {
        free(bitmap_head);
        free(output_arr);
        return NULL;
    }
    for(i = 0; i < num_elems; i++) {
        tmp = input_arr[i];
        tmp_quotient = abs(tmp / BIT_MOD_DIV_FACTOR);
        tmp_mod = abs(tmp % BIT_MOD_DIV_FACTOR);
        tmp_byte = (tmp < 0) ? (NEGATIVE_START_POS + (tmp_mod / 8)) : (tmp_mod / 8);
        tmp_bit_position = tmp_mod % 8;
        tree_index_idx = (tmp < 0) ? 1 : 0;
        /* Grow the tree if needed. */
        if((tmp_quotient + 1) > bitmap_base_size) {
            bitmap_base_size_target = (((tmp_quotient + 1) << 1) > BITMAP_LENGTH_MAX) ? BITMAP_LENGTH_MAX : ((tmp_quotient + 1) << 1);
            if((tmp_bitmap_head = (bitmap_base *)realloc(bitmap_head, bitmap_base_size_target * sizeof(bitmap_base))) == NULL) {
                *err_flag = 7;
                goto free_memory;
            }
            memset(tmp_bitmap_head + bitmap_base_size, 0, (bitmap_base_size_target - bitmap_base_size) * sizeof(bitmap_base));
            bitmap_head = tmp_bitmap_head;
            if(raw_index_range == 32 ) {
                if((tmp_idx_adj = (idx_ht_32 *)realloc((idx_ht_32 *)idx_adj_head, bitmap_base_size_target * sizeof(idx_ht_32))) == NULL) {
                    *err_flag = 7;
                    goto free_memory;
                }
                memset((idx_ht_32 *)tmp_idx_adj + bitmap_base_size, 0, (bitmap_base_size_target - bitmap_base_size) * sizeof(idx_ht_32));
                idx_adj_head = (idx_ht_32 *)tmp_idx_adj;
            }
            else if(raw_index_range == 16 ) {
                if((tmp_idx_adj = (idx_ht_16 *)realloc((idx_ht_16 *)idx_adj_head, bitmap_base_size_target * sizeof(idx_ht_16))) == NULL) {
                    *err_flag = 7;
                    goto free_memory;
                }
                memset((idx_ht_16 *)tmp_idx_adj + bitmap_base_size, 0, (bitmap_base_size_target - bitmap_base_size) * sizeof(idx_ht_16));
                idx_adj_head = (idx_ht_16 *)tmp_idx_adj;
            }
            else{
                if((tmp_idx_adj = (idx_ht_8 *)realloc((idx_ht_8 *)idx_adj_head, bitmap_base_size_target * sizeof(idx_ht_8))) == NULL) {
                    *err_flag = 7;
                    goto free_memory;
                }
                memset((idx_ht_8 *)tmp_idx_adj + bitmap_base_size, 0, (bitmap_base_size_target - bitmap_base_size) * sizeof(idx_ht_8));
                idx_adj_head = (idx_ht_8 *)tmp_idx_adj;
            }
            bitmap_base_size = bitmap_base_size_target;
        }
        if(bitmap_head[tmp_quotient].ptr_branch == NULL) {
            if(((bitmap_head[tmp_quotient].ptr_branch) = (uint8_t *)calloc(BIT_MOD_TABLE_SIZE, sizeof(uint8_t))) == NULL) {
                *err_flag = 1;
                goto free_memory;
            }
        }
        if(raw_index_range == 32 ) {
            if((((idx_ht_32 *)idx_adj_head)[tmp_quotient].ptr_branch)[tree_index_idx] == NULL) {
                if(((((idx_ht_32 *)idx_adj_head)[tmp_quotient].ptr_branch)[tree_index_idx] = (uint32_t *)calloc(IDX_ADJ_BRCH_SIZE, sizeof(uint32_t))) == NULL){
                    *err_flag = 1;
                    goto free_memory;
                }
            }
        }
        else if(raw_index_range == 16 ) {
            if((((idx_ht_16 *)idx_adj_head)[tmp_quotient].ptr_branch)[tree_index_idx] == NULL) {
                if((((idx_ht_16 *)idx_adj_head)[tmp_quotient].ptr_branch[tree_index_idx] = (uint16_t *)calloc(IDX_ADJ_BRCH_SIZE, sizeof(uint16_t))) == NULL){
                    *err_flag = 1;
                    goto free_memory;
                }
            }
        }
        else{
            if((((idx_ht_8 *)idx_adj_head)[tmp_quotient].ptr_branch)[tree_index_idx] == NULL) {
                if(((((idx_ht_8 *)idx_adj_head)[tmp_quotient].ptr_branch)[tree_index_idx] = (uint8_t *)calloc(IDX_ADJ_BRCH_SIZE, sizeof(uint8_t))) == NULL){
                    *err_flag = 1;
                    goto free_memory;
                }
            }
        }
        if((bitmap_head[tmp_quotient].ptr_branch) != NULL && check_bit(bitmap_head[tmp_quotient].ptr_branch[tmp_byte], tmp_bit_position)) {
            if(raw_index_range == 32 ) {
                tmp_dup_raw_index = ((((idx_ht_32 *)idx_adj_head)[tmp_quotient].ptr_branch)[tree_index_idx])[tmp_mod];
            }
            else if(raw_index_range == 16 ) {
                tmp_dup_raw_index = ((((idx_ht_16 *)idx_adj_head)[tmp_quotient].ptr_branch)[tree_index_idx])[tmp_mod];
            }
            else{
                tmp_dup_raw_index = ((((idx_ht_8 *)idx_adj_head)[tmp_quotient].ptr_branch)[tree_index_idx])[tmp_mod];
            }
            insert_dup_idx_list(&dup_idx_head_tmp, i, tmp_dup_raw_index);
            continue;
        }
        output_arr[j].out_elem = tmp;
        output_arr[j].raw_index = i;
        j++;
        flip_bit((bitmap_head[tmp_quotient].ptr_branch)[tmp_byte], tmp_bit_position);
        if(raw_index_range == 32 ) {
            (((idx_ht_32 *)idx_adj_head)[tmp_quotient].ptr_branch[tree_index_idx])[tmp_mod] = i;
        }
        else if(raw_index_range == 16 ) {
            ((idx_ht_16 *)idx_adj_head)[tmp_quotient].ptr_branch[tree_index_idx][tmp_mod] = (uint16_t)i;
        }
        else{
            (((idx_ht_8 *)idx_adj_head)[tmp_quotient].ptr_branch[tree_index_idx])[tmp_mod] = (uint8_t)i;
        }
    }
free_memory:
    free_bitmap(bitmap_head, bitmap_base_size);
    free(bitmap_head);
    if(raw_index_range == 32 ) {
        free_idx_ht_32((idx_ht_32 *)idx_adj_head, bitmap_base_size);
    }
    else if(raw_index_range == 16 ) {
        free_idx_ht_16((idx_ht_16 *)idx_adj_head, bitmap_base_size);
    }
    else{
        free_idx_ht_8((idx_ht_8 *)idx_adj_head, bitmap_base_size);    
    }
    if(*err_flag != 0) {
        free(output_arr);
        return NULL;
    }
    final_output_arr = (out_idx *)realloc(output_arr, j * sizeof(out_idx));
    if(final_output_arr == NULL) {
        free(output_arr);
        *err_flag = 3;
        return NULL;
    }
    *num_elems_out = j;
    *dup_idx_head = dup_idx_head_tmp;
    return final_output_arr;
}

out_idx* fui_bitmap_idx_dtree(const int32_t *input_arr, const uint32_t num_elems, uint32_t *num_elems_out, int *err_flag, dup_idx_list **dup_idx_head) {
    uint32_t i, j = 0;
    uint32_t tmp_dup_raw_index;
    uint16_t tmp_quotient = 0, tmp_mod = 0;
    int32_t tmp = 0;
    out_idx *final_output_arr = NULL;
    bitmap_dtree *bitmap_head = NULL, *tmp_bitmap_head = NULL;
    uint8_t tree_index = 0;
    void *idx_adj_head = NULL, *tmp_idx_adj = NULL;
    dup_idx_list *dup_idx_head_tmp = NULL;
    uint16_t tmp_byte_index = 0, bitmap_base_size = BITMAP_INIT_LENGTH, bitmap_base_size_target = 0;
    uint8_t tmp_bit_position = 0;
    uint8_t raw_index_range = 0;
    *err_flag = 0;
    *num_elems_out = 0;
    if(*dup_idx_head != NULL) {
        *err_flag = -9;
        return NULL;
    }
    if (input_arr == NULL) {
        *err_flag = -7;
        return NULL;
    }
    bitmap_head = (bitmap_dtree *)calloc(BITMAP_INIT_LENGTH, sizeof(bitmap_dtree));
    if(bitmap_head == NULL) {
        *err_flag = 5;
        return NULL;
    }
    out_idx *output_arr = (out_idx *)calloc(num_elems, sizeof(out_idx));
    if (output_arr == NULL) {
        free(bitmap_head);
        *err_flag = -1;
        return NULL;
    }
    if (num_elems < 1){
        *err_flag = -5;
    }
    else if(num_elems > UINT_16_MAX ) {
        if((idx_adj_head = (idx_ht_32 *)calloc(BITMAP_INIT_LENGTH, sizeof(idx_ht_32))) == NULL) {
            *err_flag = -3;
        }
        else {
            raw_index_range = 32;
        }
    }
    else if(num_elems > UINT_8_MAX) {
        if((idx_adj_head = (idx_ht_16 *)calloc(BITMAP_INIT_LENGTH, sizeof(idx_ht_16))) == NULL) {
            *err_flag = -3;
        }
        else {
            raw_index_range = 16;
        }
    }
    else{
        if((idx_adj_head = (idx_ht_8 *)calloc(BITMAP_INIT_LENGTH, sizeof(idx_ht_8))) == NULL) {
            *err_flag = -3;
        }
        else {
            raw_index_range = 8;
        }
    }
    if(*err_flag != 0) {
        free(bitmap_head);
        free(output_arr);
        return NULL;
    }
    for(i = 0; i < num_elems; i++) {
        tmp = input_arr[i];
        tree_index = (tmp < 0) ? 1 : 0;
        tmp_quotient = abs(tmp / BIT_MOD_DIV_FACTOR);
        tmp_mod = abs(tmp % BIT_MOD_DIV_FACTOR);
        tmp_byte_index = (tmp_mod / 8);
        tmp_bit_position = tmp_mod % 8;
        /* Grow the tree if needed. */
        if((tmp_quotient + 1) > bitmap_base_size) {
            bitmap_base_size_target = (((tmp_quotient + 1) << 1) > BITMAP_LENGTH_MAX) ? BITMAP_LENGTH_MAX : ((tmp_quotient + 1) << 1);
            if((tmp_bitmap_head = (bitmap_dtree *)realloc(bitmap_head, bitmap_base_size_target * sizeof(bitmap_dtree))) == NULL) {
                *err_flag = 7;
                goto free_memory;
            }
            memset(tmp_bitmap_head + bitmap_base_size, 0, (bitmap_base_size_target - bitmap_base_size) * sizeof(bitmap_dtree));
            bitmap_head = tmp_bitmap_head;
            if(raw_index_range == 32 ) {
                if((tmp_idx_adj = (idx_ht_32 *)realloc((idx_ht_32 *)idx_adj_head, bitmap_base_size_target * sizeof(idx_ht_32))) == NULL) {
                    *err_flag = 7;
                    goto free_memory;
                }
                memset((idx_ht_32 *)tmp_idx_adj + bitmap_base_size, 0, (bitmap_base_size_target - bitmap_base_size) * sizeof(idx_ht_32));
                idx_adj_head = (idx_ht_32 *)tmp_idx_adj;
            }
            else if(raw_index_range == 16 ) {
                if((tmp_idx_adj = (idx_ht_16 *)realloc((idx_ht_16 *)idx_adj_head, bitmap_base_size_target * sizeof(idx_ht_16))) == NULL) {
                    *err_flag = 7;
                    goto free_memory;
                }
                memset((idx_ht_16 *)tmp_idx_adj + bitmap_base_size, 0, (bitmap_base_size_target - bitmap_base_size) * sizeof(idx_ht_16));
                idx_adj_head = (idx_ht_16 *)tmp_idx_adj;
            }
            else{
                if((tmp_idx_adj = (idx_ht_8 *)realloc((idx_ht_8 *)idx_adj_head, bitmap_base_size_target * sizeof(idx_ht_8))) == NULL) {
                    *err_flag = 7;
                    goto free_memory;
                }
                memset((idx_ht_8 *)tmp_idx_adj + bitmap_base_size, 0, (bitmap_base_size_target - bitmap_base_size) * sizeof(idx_ht_8));
                idx_adj_head = (idx_ht_8 *)tmp_idx_adj;
            }
            bitmap_base_size = bitmap_base_size_target;
        }
        if((bitmap_head[tmp_quotient].ptr_branch)[tree_index] == NULL) {
            if(((bitmap_head[tmp_quotient].ptr_branch)[tree_index] = (uint8_t *)calloc(BITMAP_BRCH_DTREE, sizeof(uint8_t))) == NULL) {
                *err_flag = 1;
                goto free_memory;
            }
        }
        if(raw_index_range == 32 ) {
            if((((idx_ht_32 *)idx_adj_head)[tmp_quotient].ptr_branch)[tree_index] == NULL) {
                if(((((idx_ht_32 *)idx_adj_head)[tmp_quotient].ptr_branch)[tree_index] = (uint32_t *)calloc(IDX_ADJ_BRCH_SIZE, sizeof(uint32_t))) == NULL){
                    *err_flag = 1;
                    goto free_memory;
                }
            }
        }
        else if(raw_index_range == 16 ) {
            if((((idx_ht_16 *)idx_adj_head)[tmp_quotient].ptr_branch)[tree_index] == NULL) {
                if((((idx_ht_16 *)idx_adj_head)[tmp_quotient].ptr_branch[tree_index] = (uint16_t *)calloc(IDX_ADJ_BRCH_SIZE, sizeof(uint16_t))) == NULL){
                    *err_flag = 1;
                    goto free_memory;
                }
            }
        }
        else{
            if((((idx_ht_8 *)idx_adj_head)[tmp_quotient].ptr_branch)[tree_index] == NULL) {
                if(((((idx_ht_8 *)idx_adj_head)[tmp_quotient].ptr_branch)[tree_index] = (uint8_t *)calloc(IDX_ADJ_BRCH_SIZE, sizeof(uint8_t))) == NULL){
                    *err_flag = 1;
                    goto free_memory;
                }
            }
        }
        if((bitmap_head[tmp_quotient].ptr_branch)[tree_index] != NULL && check_bit(((bitmap_head[tmp_quotient].ptr_branch)[tree_index])[tmp_byte_index], tmp_bit_position)) {
            if(raw_index_range == 32 ) {
                tmp_dup_raw_index = ((((idx_ht_32 *)idx_adj_head)[tmp_quotient].ptr_branch)[tree_index])[tmp_mod];
            }
            else if(raw_index_range == 16 ) {
                tmp_dup_raw_index = ((((idx_ht_16 *)idx_adj_head)[tmp_quotient].ptr_branch)[tree_index])[tmp_mod];
            }
            else{
                tmp_dup_raw_index = ((((idx_ht_8 *)idx_adj_head)[tmp_quotient].ptr_branch)[tree_index])[tmp_mod];
            }
            insert_dup_idx_list(&dup_idx_head_tmp, i, tmp_dup_raw_index);
            continue;
        }
        output_arr[j].out_elem = tmp;
        output_arr[j].raw_index = i;
        j++;
        flip_bit(((bitmap_head[tmp_quotient].ptr_branch)[tree_index])[tmp_byte_index], tmp_bit_position);
        if(raw_index_range == 32 ) {
            (((idx_ht_32 *)idx_adj_head)[tmp_quotient].ptr_branch[tree_index])[tmp_mod] = i;
        }
        else if(raw_index_range == 16 ) {
            ((idx_ht_16 *)idx_adj_head)[tmp_quotient].ptr_branch[tree_index][tmp_mod] = (uint16_t)i;
        }
        else{
            (((idx_ht_8 *)idx_adj_head)[tmp_quotient].ptr_branch[tree_index])[tmp_mod] = (uint8_t)i;
        }
    }
free_memory:
    free_bitmap_dtree(bitmap_head, bitmap_base_size);
    free(bitmap_head);
    if(raw_index_range == 32 ) {
        free_idx_ht_32((idx_ht_32 *)idx_adj_head, bitmap_base_size);
    }
    else if(raw_index_range == 16 ) {
        free_idx_ht_16((idx_ht_16 *)idx_adj_head, bitmap_base_size);
    }
    else{
        free_idx_ht_8((idx_ht_8 *)idx_adj_head, bitmap_base_size);    
    }
    if(*err_flag != 0) {
        free(output_arr);
        return NULL;
    }
    final_output_arr = (out_idx *)realloc(output_arr, j * sizeof(out_idx));
    if(final_output_arr == NULL) {
        free(output_arr);
        *err_flag = 3;
        return NULL;
    }
    *num_elems_out = j;
    *dup_idx_head = dup_idx_head_tmp;
    return final_output_arr;
}

/*
int assemble_h32(int_64bit a) {
    return (((a) >> 8) & (0xFF)) | (((a) >> 16) & (0xFF00)) | (((a) >> 24) & (0xFF0000)) | (((a) >> 32) & (0xFF000000));
}
int assemble_l32(int_64bit a) {
    return ((a) & (0xFF)) | (((a) >> 8) & (0xFF00)) | (((a) >> 16) & (0xFF0000)) | (((a) >> 24) & (0xFF000000));
}

int hash_64_to_32(int_64bit in64) {
    int h32 = assemble_h32(in64);
    int l32 = assemble_l32(in64);
    //printf("%x\t%x\t%lx\t%ld\n", h32, l32, in64, in64);
    int folded = (~h32 ^ l32);
    int rotated = (folded >> 13) | (folded << 19); 
    return (folded ^ rotated);
}

int_64bit* transform_32_to_64_arr(const int *arr_input_32, uint32_t num_elems, const char *option) {
    if(arr_input_32 == NULL) {
        return NULL;
    }
    int_64bit *arr_64 = (int_64bit *)calloc(num_elems, sizeof(int_64bit));
    if(arr_64 == NULL) {
        return NULL;
    }
    srand(time(0));
    for(uint32_t i = 0; i < num_elems; i++) {
        if(option == NULL || strlen(option) == 0) {
            arr_64[i] = (int_64bit)(arr_input_32[i]);
        }
        else{
            if(RAND_MAX == 0x7FFF) {
                arr_64[i] = (((int_64bit)rand()<<48) | ((int_64bit)rand()<<32) | (arr_input_32[i]));
            }
            else{
                arr_64[i] = (((int_64bit)rand()<<32) | (arr_input_32[i]));
            }
        }
        //printf("%ld\t%ld\n", arr_input_32[i], arr_64[i]);
    }
    return arr_64;
}

out_idx_i64* fui_bitmap_dtree_idx_64(const int_64bit *input_arr, const uint32_t num_elems, uint32_t *num_elems_out, int *err_flag, dup_idx_list **dup_idx_head) {
    *err_flag = 0;
    *num_elems_out = 0;
    uint32_t num_elems_out_32 = 0;
    int err_flag_local = 0;
    dup_idx_list *dup_idx_list_32 = NULL;
    uint32_t i = 0;
    int* input_arr_hash = (int *)calloc(num_elems, sizeof(int));
    if(input_arr_hash == NULL) {
        *err_flag = -9;
        return NULL;
    }
    for(i = 0; i < num_elems; i++) {
        input_arr_hash[i] = hash_64_to_32(input_arr[i]);
        printf("%d,,,\n",input_arr_hash[i]);
    }
    out_idx *out_32bit = fui_bitmap_idx_dtree(input_arr_hash, num_elems, &num_elems_out_32, &err_flag_local, &dup_idx_list_32);
    if(out_32bit == NULL) {
        *err_flag = -7;
        free(input_arr_hash);
        return NULL;
    }
    out_idx_i64 *final_output = (out_idx_i64 *)calloc(num_elems_out_32, sizeof(out_idx_i64));
    if(final_output == NULL) {
        *err_flag = -5;
        free(out_32bit);
        free(input_arr_hash);
        return NULL;
    }
    for(i = 0; i < num_elems_out_32; i++) {
        final_output->raw_index = out_32bit->raw_index;
        final_output->out_elem = input_arr[final_output->raw_index];
    }
    *dup_idx_head = dup_idx_list_32;
    *num_elems_out = num_elems_out_32;
    free(out_32bit);
    free(input_arr_hash);
    return final_output;
}*/