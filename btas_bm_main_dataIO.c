#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "btas.h"
#include "data_io.h"

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
    int with_brute = 0;
    dup_idx_list *dup_idx_list1_1 = NULL, *dup_idx_list1_2 = NULL, *dup_idx_list2_1 = NULL, *dup_idx_list2_2 = NULL;
    if(argc < 3) {
        printf("ERROR: not enough args. USAGE: ./command argv[1] argv[2] argv[3](Optional: brute).\n");
        return 1;
    }
    if(argc > 3 && strcmp(argv[3], "brute") == 0) {
        with_brute = 1;
    }
    uint32_t num_elems, rand_max;
    
    if(string_to_u32_num(argv[1], &num_elems) != 0 || string_to_u32_num(argv[2], &rand_max) != 0) {
        printf("ERROR: arguments illegal. Make sure they are plain positive numbers and < 4,294,967,296.\n");
        return 3;
    }
    printf("INPUT_ELEMS:\t%u\nRANDOM_MAX:\t%u\n\n",num_elems, rand_max);
    
    int32_t *arr_gen = (int32_t *)malloc(sizeof(int32_t) * num_elems);
    int32_t *arr_input = NULL;
    if(arr_gen == NULL) {
        printf("ERROR: Failed to allocate memory for input array.\n");
        return 5;
    }
    int err_flag = 0;
    uint32_t num_elems_out = 0, num_elems_out_idx = 0;
    clock_t start, end;
    int32_t *out_brute_opt = NULL;
    int32_t *out_brute = NULL;
    int32_t *out_ht_dtree = NULL;
    int32_t *out_ht_stree = NULL;
    int32_t *out_ht_dyn = NULL;
    int32_t *out_bit_dyn = NULL;
    int32_t *out_bit_stc = NULL;
    out_idx *out_bit_dyn_idx = NULL;

    generate_random_input_arr(arr_gen, num_elems, rand_max);
    if(export_1d_i32("random.bin", "", arr_gen, num_elems) != 0) {
        printf("ERROR: Failed to export the data to 'random.bin'.\n");
        free(arr_gen);
        return 7;
    }
    if(export_1d_i32("random.csv", "csv", arr_gen, num_elems) != 0) {
        printf("ERROR: Failed to export the data to 'random.csv'.\n");
        free(arr_gen);
        return 9;
    }
    free(arr_gen);

    printf("RANDOM ARRAY INPUT:\n");
    printf("ALGO_TYPE\tTIME_IN_SEC\tUNIQUE_INTEGERS\n");
    
    start = clock();
    arr_input = import_1d_i32("random.bin", "", &num_elems_out, &err_flag);
    out_bit_dyn = fui_bitmap_dyn_stree(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("BTAS_STREE:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);
    free(arr_input);
    free(out_bit_dyn);

    start = clock();
    arr_input = import_1d_i32("random.bin", "", &num_elems_out, &err_flag);
    out_bit_dyn = fui_bitmap_dyn_dtree(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("BTAS_DTREE:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);
    free(arr_input);
    free(out_bit_dyn);

    start = clock();
    arr_input = import_1d_i32("random.bin", "", &num_elems_out, &err_flag);
    out_bit_dyn_idx = fui_bitmap_idx_stree(arr_input, num_elems, &num_elems_out_idx, &err_flag, &dup_idx_list1_1);
    end = clock();
    printf("BTAS_SAIH:\t%lf\t%d\t::::%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out_idx, err_flag);
    free(arr_input);

    start = clock();
    arr_input = import_1d_i32("random.bin", "", &num_elems_out, &err_flag);
    out_bit_dyn_idx = fui_bitmap_idx_dtree(arr_input, num_elems, &num_elems_out_idx, &err_flag, &dup_idx_list1_2);
    end = clock();
    printf("BTAS_DAIH:\t%lf\t%d\t::::%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out_idx, err_flag);
    free(arr_input);

    start = clock();
    arr_input = import_1d_i32("random.bin", "", &num_elems_out, &err_flag);
    out_bit_stc = fui_bitmap_stc_stree(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("BTAS_STC:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);
    free(arr_input);
    free(out_bit_stc);

    /*start = clock();
    out_ht_dtree = fui_htable_dtree(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("HTBL_DTREE:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

    start = clock();
    out_ht_stree = fui_htable_stree(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("HTBL_STREE:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

    start = clock();
    out_ht_dyn = fui_htable_stree_dyn(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("HTBL_STREE_DYN:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

    if(with_brute == 1) {
        start = clock();
        out_brute_opt = fui_brute_opt(arr_input, num_elems, &num_elems_out, &err_flag);
        end = clock();
        printf("BRUTE_OPT:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);
    
        start = clock();
        out_brute = fui_brute(arr_input, num_elems, &num_elems_out, &err_flag);
        end = clock();
        printf("BRUTE_ORIG:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);
    }*/
    
    /*free(out_ht_dtree);
    free(out_ht_stree);
    free(out_ht_dyn);
    if(with_brute == 1) {
        free(out_brute_opt);
        free(out_brute);
    }*/
    print_dup_idx_list(dup_idx_list1_1, 3);
    print_dup_idx_list(dup_idx_list1_2, 3);
    print_out_idx(out_bit_dyn_idx, num_elems_out_idx, 5);
    free_dup_idx_list(dup_idx_list1_1);
    free_dup_idx_list(dup_idx_list1_2);
    return 0;
    
    arr_gen = (int32_t *)malloc(sizeof(int32_t) * num_elems);
    if(arr_gen == NULL) {
        printf("ERROR: Failed to allocate memory for input array.\n");
        return 5;
    }
    generate_growing_arr(arr_gen, num_elems);
    if(export_1d_i32("growing.bin", "", arr_gen, num_elems) != 0) {
        printf("ERROR: Failed to export the data to 'growing.bin'.\n");
        free(arr_gen);
        return 7;
    }
    if(export_1d_i32("growing.csv", "csv", arr_gen, num_elems) != 0) {
        printf("ERROR: Failed to export the data to 'growing.csv'.\n");
        free(arr_gen);
        return 9;
    }
    free(arr_gen);
    
    printf("\nGROWING ARRAY INPUT:\n");
    printf("ALGO_TYPE\tTIME_IN_SEC\tUNIQUE_INTEGERS\n");
    
    start = clock();
    out_bit_dyn = fui_bitmap_dyn_stree(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("BTAS_STREE:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

    start = clock();
    out_bit_dyn = fui_bitmap_dyn_dtree(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("BTAS_DTREE:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

    start = clock();
    out_bit_dyn_idx = fui_bitmap_idx_stree(arr_input, num_elems, &num_elems_out_idx, &err_flag, &dup_idx_list2_1);
    end = clock();
    printf("BTAS_SAIH:\t%lf\t%d\t::::%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out_idx, err_flag);

    start = clock();
    out_bit_dyn_idx = fui_bitmap_idx_dtree(arr_input, num_elems, &num_elems_out_idx, &err_flag, &dup_idx_list2_2);
    end = clock();
    printf("BTAS_DAIH:\t%lf\t%d\t::::%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out_idx, err_flag);

    start = clock();
    out_bit_stc = fui_bitmap_stc_stree(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("BTAS_STC:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

    start = clock();
    out_ht_dtree = fui_htable_dtree(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("HTBL_DTREE:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

    start = clock();
    out_ht_stree= fui_htable_stree(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("HTBL_STREE:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

    start = clock();
    out_ht_dyn = fui_htable_stree_dyn(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("HTBL_STREE_DYN:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

    start = clock();
    out_brute_opt = fui_brute_opt(arr_input, num_elems, &num_elems_out, &err_flag);
    end = clock();
    printf("BRUTE_OPT:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);
    if(with_brute == 1) {
        start = clock();
        out_brute = fui_brute(arr_input, num_elems, &num_elems_out, &err_flag);
        end = clock();
        printf("BRUTE_ORIG:\t%lf\t%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);
        free(out_brute);
    }
    free(out_bit_dyn);
    free(out_bit_stc);
    free(out_ht_dtree);
    free(out_ht_stree);
    free(out_ht_dyn);
    free(out_brute_opt);

    print_dup_idx_list(dup_idx_list2_1, 3);
    print_dup_idx_list(dup_idx_list2_2, 3);
    print_out_idx(out_bit_dyn_idx, num_elems_out_idx, 5);
    free_dup_idx_list(dup_idx_list2_1);
    free_dup_idx_list(dup_idx_list2_2);
    free(out_bit_dyn_idx);
    printf("\nBenchmark done.\n\n");
    
    return 0;
}
