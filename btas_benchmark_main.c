#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "btas.h"

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
    int *arr_input = (int *)malloc(sizeof(int) * num_elems);
    if(arr_input == NULL) {
        printf("ERROR: Failed to allocate memory for input array.\n");
        return 5;
    }
    int err_flag = 0;
    uint32_t num_elems_out = 0, num_elems_out_idx = 0;
    clock_t start, end;

    int *out_brute_opt = NULL;
    int *out_brute = NULL;
    int *out_ht_dtree = NULL;
    int *out_ht_stree = NULL;
    int *out_ht_dyn = NULL;
    int *out_bit_dyn = NULL;
    int *out_bit_stc = NULL;
    out_idx *out_bit_dyn_idx = NULL;

    generate_random_input_arr(arr_input, num_elems, rand_max);
    //print_arr(arr_input, num_elems, 100);
    
  /**
   * Please uncomment the FILE I/O part if you'd like to write the arr_input
   * to a file.
   * 
    FILE* file_p=fopen("random.csv","wb+");
    if(file_p == NULL){
        free(arr_input);
        return 7;
    }
    for(uint32_t i = 0; i < num_elems; i++) {
        fprintf(file_p, "%d\n", arr_input[i]);
    }
    fclose(file_p);
   */
    printf("RANDOM ARRAY INPUT:\n");
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
    out_bit_dyn_idx = fui_bitmap_idx_stree(arr_input, num_elems, &num_elems_out_idx, &err_flag, &dup_idx_list1_1);
    end = clock();
    printf("BTAS_SAIH:\t%lf\t%d\t::::%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out_idx, err_flag);

    start = clock();
    out_bit_dyn_idx = fui_bitmap_idx_dtree(arr_input, num_elems, &num_elems_out_idx, &err_flag, &dup_idx_list1_2);
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
    }

    free(out_bit_dyn);
    free(out_bit_stc);
    free(out_ht_dtree);
    free(out_ht_stree);
    free(out_ht_dyn);
    if(with_brute == 1) {
        free(out_brute_opt);
        free(out_brute);
    }
    print_dup_idx_list(dup_idx_list1_1, 3);
    print_dup_idx_list(dup_idx_list1_2, 3);
    print_out_idx(out_bit_dyn_idx, num_elems_out_idx, 5);
    free_dup_idx_list(dup_idx_list1_1);
    free_dup_idx_list(dup_idx_list1_2);
    free(out_bit_dyn_idx);

    memset(arr_input, 0, num_elems);
    generate_growing_arr(arr_input, num_elems);

  /**
   * Please uncomment the FILE I/O part if you'd like to write the arr_input
   * to a file.
   * 
    file_p=fopen("growing.csv","wb+");
    if(file_p == NULL){
        free(arr_input);
        return 7;
    }
    for(uint32_t i = 0; i < num_elems; i++) {
        fprintf(file_p, "%d\n", arr_input[i]);
    }
    fclose(file_p);
   */
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
    
    free(arr_input);
    return 0;
}
