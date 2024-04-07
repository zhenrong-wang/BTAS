#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

extern "C" {
#include "filter_uniq_ints.h"
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
    int *out = NULL, *out_naive = NULL, *out_ht = NULL, *out_ht_new = NULL, *out_ht_dyn = NULL;
    generate_random_input_arr(arr_input,num_elems,rand_max);

    printf("RANDOM ARRAY INPUT:\n");
    printf("ALGO_TYPE\tTIME_IN_SEC\tUNIQUE_INTEGERS\n");
    start = clock();
    out = filter_unique_elems_naive_improved(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("NAIVE_ALGO_NEW:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

    start = clock();
    out_naive = filter_unique_elems_naive(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("NAIVE_ALGO:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

    start = clock();
    out_ht = filter_unique_elems_ht(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("HASH_ALGO:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

    start = clock();
    out_ht_new = filter_unique_elems_ht_new(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("HASH_ALGO_NEW:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

    start = clock();
    out_ht_dyn = filter_unique_elems_ht_dyn(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("HASH_ALGO_DYN:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

    printf("COMPARISON:\t%d\t%d\t%d\t%d\n\n", compare_arr(out, out_naive, num_elems_out), compare_arr(out, out_ht, num_elems_out), compare_arr(out, out_ht_new, num_elems_out), compare_arr(out, out_ht_dyn, num_elems_out));
    free(out);
    free(out_naive);
    free(out_ht);
    free(out_ht_new);
    free(out_ht_dyn);

    generate_growing_arr(arr_input,num_elems);
    printf("GROWING ARRAY INPUT:\n");
    printf("ALGO_TYPE\tTIME_IN_SEC\tUNIQUE_INTEGERS\n");
    start = clock();
    out = filter_unique_elems_naive_improved(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("NAIVE_ALGO_NEW:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

    start = clock();
    out_naive = filter_unique_elems_naive(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("NAIVE_ALGO:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

    start = clock();
    out_ht = filter_unique_elems_ht(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("HASH_ALGO:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

    start = clock();
    out_ht_new = filter_unique_elems_ht_new(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("HASH_ALGO_NEW:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

    start = clock();
    out_ht_dyn = filter_unique_elems_ht_dyn(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("HASH_ALGO_DYN:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

    printf("COMPARISON:\t%d\t%d\t%d\t%d\n\n", compare_arr(out, out_naive, num_elems_out), compare_arr(out, out_ht, num_elems_out), compare_arr(out, out_ht_new, num_elems_out), compare_arr(out, out_ht_dyn, num_elems_out));
    free(out);
    free(out_naive);
    free(out_ht);
    free(out_ht_new);
    free(out_ht_dyn);

    free(arr_input);
    return 0;
}