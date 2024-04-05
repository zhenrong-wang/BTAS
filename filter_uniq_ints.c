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
 * @brief Convert a string to a posivie number
 *  The string cannot contain characters other than 0~9
 * 
 * @returns
 *  -1 if illegal chars found or NULL pointer
 *  A positive number if succeeded
 */
int convert_string_to_positive_num(const char* string) {
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
int* filter_unique_elems(const int *input_arr, const unsigned int num_elems, unsigned int *num_elems_out, int *err_flag){
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

int compare_arr(int *arr_a, int *arr_b, unsigned int num_elems) {
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


/**
 * @brief
 *  usage: ./command argv[1] argv[2]
 *  argv[1] indicates the size (number of elems) of the input array
 *  argv[2] indicates the maximun of the random number generated
 *      NOTE: argv[2] doesn't affect the Round 2 of benchmark
 * 
 * @returns
 *  0 if everything goes well
 *  
 */
int main(int argc, char** argv) {
    if(argc < 3) {
        printf("ERROR: not enough args. USAGE: ./command argv[1] argv[2].\n");
        return 1;
    }

    int num_elems = convert_string_to_positive_num(argv[1]), rand_max = convert_string_to_positive_num(argv[2]);
    printf("INPUT_ELEMS:\t%d\nRANDOM_MAX:\t%d\n\n",num_elems, rand_max);
    if(num_elems < 0 || rand_max < 0) {
        printf("ERROR: arguments illegal. Make sure they are plain positive numbers.\n");
        return 3;
    }

    int *arr_input = (int *)malloc(sizeof(int) * num_elems);
    if(arr_input == NULL) {
        printf("ERROR: Failed to allocate memory for input array.\n");
        return 5;
    }
    int err_flag = 0;
    unsigned int num_elems_out = 0;
    clock_t start, end;
    int *out = NULL, *out_naive = NULL, *out_ht = NULL;
    generate_random_input_arr(arr_input,num_elems,rand_max);

    printf("RANDOM ARRAY INPUT:\n");
    printf("ALGO_TYPE\tTIME_IN_SEC\tUNIQUE_INTEGERS%d\n");
    start = clock();
    out = filter_unique_elems(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("NEW_ALGO:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);
    
    start = clock();
    out_naive = filter_unique_elems_naive(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("NAIVE_ALGO:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

    start = clock();
    out_ht = filter_unique_elems_ht(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("HASH_ALGO:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

    printf("COMPARE:\t%d\t%d\n\n", compare_arr(out, out_naive, num_elems_out), compare_arr(out, out_ht, num_elems_out));
    free(out);
    free(out_naive);
    free(out_ht);

    generate_growing_arr(arr_input,num_elems);
    printf("GROWING ARRAY INPUT:\n");
    printf("ALGO_TYPE\tTIME_IN_SEC\tUNIQUE_INTEGERS%d\n");
    start = clock();
    out = filter_unique_elems(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("NEW_ALGO:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);
    
    start = clock();
    out_naive = filter_unique_elems_naive(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("NAIVE_ALGO:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

    start = clock();
    out_ht = filter_unique_elems_ht(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("HASH_ALGO:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

    printf("COMPARE:\t%d\t%d\n\n", compare_arr(out, out_naive, num_elems_out), compare_arr(out, out_ht, num_elems_out));
    free(out);
    free(out_ht);
    
    free(arr_input);
    return 0;
}
