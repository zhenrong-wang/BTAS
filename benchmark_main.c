#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "btas.h"
#include "data_io.h"

/**
 * @brief
 *  usage: ./command argv[1] argv[2] CMD_FLAGS
 * 
 * @param [in]
 *  argv[1] indicates the size (number of elems) of the input array
 *  argv[2] indicates the maximun of the random number generated
 *      NOTE: argv[2] doesn't affect the Round 2 of benchmark
 *  CMD_FLAGS: 
 *    --brute  : Execute brute algorithms (might cause OOM if the dataset is large!)
 *    --fio-bin: Execute the file I/O benchmark with binary reading
 *    --fio-csv: Execute the file I/O benchmark with csv reading
 *      NOTE: By default, the CMD_FLAGS are off, meaning that the brute algos would 
 *            not be executed, and no file I/O triggered.
 * 
 * @returns
 *   0 : if everything goes well
 *   1 : if command args not enough
 *   3 : illegal arguments found
 *   5 : Failed to allocate memory for input array
 *   7 : Failed to write data to the binary file
 *   9 : Failed to write data to the csv files
 *  
 */
int main(int argc, char** argv) {
    
    int with_brute = 0, with_fio = 0, with_count = 0;
    uint32_t rand_max;
    dup_idx_list *dup_idx_list1 = NULL;
    dup_idx_list *dup_idx_list2 = NULL;
    char data_file_bin[512] = "", data_file_csv[512] = "";
    int err_flag = 0;
    uint64_t num_elems = 0, num_elems_out = 0, num_elems_read = 0, num_elems_out_idx = 0, uniq_count = 0;
    clock_t start, end;
    uint32_t *out_brute_opt = NULL;
    uint32_t *out_brute = NULL;
    uint32_t *out_ht_stree = NULL;
    uint32_t *out_ht_dyn = NULL;
    uint32_t *out_bit_dyn = NULL;
    uint32_t *out_bit_stc = NULL;
    out_idx *out_bit_dyn_idx = NULL;
    uint32_t *arr_input = NULL;

    if(argc < 3) {
        printf("ERROR: not enough args. USAGE: ./command argv[1] argv[2] CMD_FLAGS \n");
        return 1;
    }
    if(cmd_flag_parser(argc, argv, "--brute") == 0) {
        with_brute = 1;
    }
    if(cmd_flag_parser(argc, argv, "--fio-bin") == 0) {
        with_fio = 1;
    }
    else {
        if(cmd_flag_parser(argc, argv, "--fio-csv") == 0) {
            with_fio = 2;
        }
    }
    if(cmd_flag_parser(argc, argv, "--count") == 0) {
        with_count = 1;
    }
    if(string_to_u64_num(argv[1], &num_elems) != 0 || string_to_u32_num(argv[2], &rand_max) != 0) {
        printf("ERROR: arguments illegal. Make sure they are plain positive numbers and < 4,294,967,296.\n");
        return 3;
    }
    printf("INPUT_ELEMS:\t%lu\nRANDOM_MAX:\t%u\n\n",num_elems, rand_max);
    
    uint32_t *arr_gen = (uint32_t *)malloc(sizeof(uint32_t) * num_elems);
    if(arr_gen == NULL) {
        printf("ERROR: Failed to allocate memory for input array.\n");
        return 5;
    }
    
    printf("Generating a random array for benchmarking ...\n");
    generate_random_input_arr(arr_gen, num_elems, rand_max);
    if(with_fio != 0) {
        printf("Writing data to files ...\n");
        snprintf(data_file_bin, 512, "random_%s_%s.bin", argv[1], argv[2]);
        if(export_1d_u32(data_file_bin, "", arr_gen, num_elems) != 0) {
            printf("ERROR: Failed to export the data to 'random_..._....bin'.\n");
            free(arr_gen);
            return 7;
        }
        snprintf(data_file_csv, 512, "random_%s_%s.csv", argv[1], argv[2]);
        if(export_1d_u32(data_file_csv, "csv", arr_gen, num_elems) != 0) {
            printf("ERROR: Failed to export the data to 'random_..._....csv'.\n");
            free(arr_gen);
            return 9;
        }
        free(arr_gen);
        printf("The binary and csv data files generated.\n\n");
    }

    printf("RANDOM ARRAY INPUT:\n");
    printf("ALGO_TYPE\t\tTIME_IN_SEC\tUNIQUE_INTEGERS\n");

    if(with_fio == 0) {
        start = clock();
        out_bit_dyn = fui_bitmap_dyn(arr_gen, num_elems, &num_elems_out, &err_flag);
        end = clock();
        free(out_bit_dyn);
        printf("BTAS_DYN_NOF_EXPORT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

        if(with_count == 1) {
            start = clock();
            uniq_count = fui_bitmap_dyn_count(arr_gen, num_elems, &err_flag);
            end = clock();
            printf("BTAS_DYN_NOF_COUNT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, uniq_count);
        }
    }
    else {
        if(with_fio == 1) {
            start = clock();
            arr_input = import_1d_u32(data_file_bin, "", &num_elems_read, &err_flag);
        }
        else {
            start = clock();
            arr_input = import_1d_u32(data_file_csv, "csv", &num_elems_read, &err_flag);
        }
        out_bit_dyn = fui_bitmap_dyn(arr_input, num_elems_read, &num_elems_out, &err_flag);
        end = clock();
        free(arr_input);
        free(out_bit_dyn);
        printf("BTAS_DYN_FIO_EXPORT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

        if(with_count == 1) {
            if(with_fio == 1) {
                start = clock();
                arr_input = import_1d_u32(data_file_bin, "", &num_elems_read, &err_flag);
            }
            else {
                start = clock();
                arr_input = import_1d_u32(data_file_csv, "csv", &num_elems_read, &err_flag);
            }
            uniq_count = fui_bitmap_dyn_count(arr_input, num_elems_read, &err_flag);
            end = clock();
            free(arr_input);
            printf("BTAS_DYN_FIO_COUNT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, uniq_count);
        }
    }
    
    if(with_fio == 0) {
        start = clock();
        out_bit_dyn_idx = fui_bitmap_idx(arr_gen, num_elems, &num_elems_out_idx, &err_flag, &dup_idx_list1);
        end = clock();
        printf("BTAS_IDX_NOF_EXPORT:\t%lf\t%ld\t::::%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out_idx, err_flag);
    }
    else {
        if(with_fio == 1) {
            start = clock();
            arr_input = import_1d_u32(data_file_bin, "", &num_elems_read, &err_flag);
        }
        else {
            start = clock();
            arr_input = import_1d_u32(data_file_csv, "csv", &num_elems_read, &err_flag);
        }
        out_bit_dyn_idx = fui_bitmap_idx(arr_input, num_elems_read, &num_elems_out_idx, &err_flag, &dup_idx_list1);
        end = clock();
        free(arr_input);
        printf("BTAS_IDX_FIO_EXPORT:\t%lf\t%ld\t::::%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out_idx, err_flag);
    }

    if(with_fio == 0) {
        start = clock();
        out_bit_stc = fui_bitmap_stc(arr_gen, num_elems, &num_elems_out, &err_flag);
        end = clock();
        free(out_bit_stc);
        printf("BTAS_STC_NOF_EXPORT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

        if(with_count == 1) {
            start = clock();
            uniq_count = fui_bitmap_stc_count(arr_gen, num_elems, &err_flag);
            end = clock();
            printf("BTAS_STC_NOF_COUNT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, uniq_count);
        }
    }
    else {
        if(with_fio == 1) {
            start = clock();
            arr_input = import_1d_u32(data_file_bin, "", &num_elems_read, &err_flag);
        }
        else {
            start = clock();
            arr_input = import_1d_u32(data_file_csv, "csv", &num_elems_read, &err_flag);
        }
        out_bit_stc = fui_bitmap_stc(arr_input, num_elems_read, &num_elems_out, &err_flag);
        end = clock();
        free(arr_input);
        free(out_bit_stc);
        printf("BTAS_STC_FIO_EXPORT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

        if(with_count == 1) {
            if(with_fio == 1) {
                start = clock();
                arr_input = import_1d_u32(data_file_bin, "", &num_elems_read, &err_flag);
            }
            else {
                start = clock();
                arr_input = import_1d_u32(data_file_csv, "csv", &num_elems_read, &err_flag);
            }
            uniq_count = fui_bitmap_stc_count(arr_input, num_elems_read, &err_flag);
            end = clock();
            free(arr_input);
            printf("BTAS_STC_FIO_COUNT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, uniq_count);
        }
    }

    if(with_fio == 0) {
        start = clock();
        out_ht_stree = fui_htable(arr_gen, num_elems, &num_elems_out, &err_flag);
        end = clock();
        free(out_ht_stree);
        printf("HTBL_NOF_EXPORT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

        if(with_count == 1) {
            start = clock();
            uniq_count = fui_htable_count(arr_gen, num_elems, &err_flag);
            end = clock();
            printf("HTBL_NOF_COUNT :\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, uniq_count);
        }
    }
    else {
        if(with_fio == 1) {
            start = clock();
            arr_input = import_1d_u32(data_file_bin, "", &num_elems_read, &err_flag);
        }
        else {
            start = clock();
            arr_input = import_1d_u32(data_file_csv, "csv", &num_elems_read, &err_flag);
        }
        out_ht_stree = fui_htable(arr_input, num_elems_read, &num_elems_out, &err_flag);
        end = clock();
        free(arr_input);
        free(out_ht_stree);
        printf("HTBL_FIO_EXPORT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

        if(with_count == 1) {
            if(with_fio == 1) {
                start = clock();
                arr_input = import_1d_u32(data_file_bin, "", &num_elems_read, &err_flag);
            }
            else {
                start = clock();
                arr_input = import_1d_u32(data_file_csv, "csv", &num_elems_read, &err_flag);
            }
            uniq_count = fui_htable_count(arr_input, num_elems_read, &err_flag);
            end = clock();
            free(arr_input);
            printf("HTBL_FIO_COUNT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, uniq_count);
        }
    }

    if(with_fio == 0) {
        start = clock();
        out_ht_dyn = fui_htable_dyn(arr_gen, num_elems, &num_elems_out, &err_flag);
        end = clock();
        free(out_ht_dyn);
        printf("HTBL_DYN_NOF_EXPORT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

        if(with_count == 1) {
            start = clock();
            uniq_count = fui_htable_dyn_count(arr_gen, num_elems, &err_flag);
            end = clock();
            printf("HTBL_DYN_NOF_COUNT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, uniq_count);
        }
    }
    else {
        if(with_fio == 1) {
            start = clock();
            arr_input = import_1d_u32(data_file_bin, "", &num_elems_read, &err_flag);
        }
        else {
            start = clock();
            arr_input = import_1d_u32(data_file_csv, "csv", &num_elems_read, &err_flag);
        }
        out_ht_dyn = fui_htable_dyn(arr_input, num_elems_read, &num_elems_out, &err_flag);
        end = clock();
        free(arr_input);
        free(out_ht_dyn);
        printf("HTBL_DYN_FIO_EXPORT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

        if(with_count == 1) {
            if(with_fio == 1) {
                start = clock();
                arr_input = import_1d_u32(data_file_bin, "", &num_elems_read, &err_flag);
            }
            else {
                start = clock();
                arr_input = import_1d_u32(data_file_csv, "csv", &num_elems_read, &err_flag);
            }
            uniq_count = fui_htable_dyn_count(arr_input, num_elems_read, &err_flag);
            end = clock();
            free(arr_input);
            printf("HTBL_DYN_FIO_COUNT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, uniq_count);
        }
    }
    
    if(with_brute == 1) {
        if(with_fio == 0) {
            start = clock();
            out_brute_opt = fui_brute_opt(arr_gen, num_elems, &num_elems_out, &err_flag);
            end = clock();
            free(out_brute_opt);
            printf("BRUTE_OPT_NOF_EXPORT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

            if(with_count == 1) {
                start = clock();
                uniq_count = fui_brute_opt_count(arr_gen, num_elems, &err_flag);
                end = clock();
                printf("BRUTE_OPT_NOF_COUNT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, uniq_count);
            }
        }
        else {
            if(with_fio == 1) {
                start = clock();
                arr_input = import_1d_u32(data_file_bin, "", &num_elems_read, &err_flag);
            }
            else {
                start = clock();
                arr_input = import_1d_u32(data_file_csv, "csv", &num_elems_read, &err_flag);
            }
            out_brute_opt = fui_brute_opt(arr_input, num_elems_read, &num_elems_out, &err_flag);
            end = clock();
            free(arr_input);
            free(out_brute_opt);
            printf("BRUTE_OPT_FIO_EXPORT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

            if(with_count == 1) {
                if(with_fio == 1) {
                    start = clock();
                    arr_input = import_1d_u32(data_file_bin, "", &num_elems_read, &err_flag);
                }
                else {
                    start = clock();
                    arr_input = import_1d_u32(data_file_csv, "csv", &num_elems_read, &err_flag);
                }
                uniq_count = fui_brute_opt_count(arr_input, num_elems_read, &err_flag);
                end = clock();
                free(arr_input);
                printf("BRUTE_OPT_FIO_COUNT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);
            }
        }
        
        if(with_fio == 0) {
            start = clock();
            out_brute = fui_brute(arr_gen, num_elems, &num_elems_out, &err_flag);
            end = clock();
            free(out_brute);
            printf("BRUTE_ORIG_NOF_EXPORT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

            if(with_count == 1) {
                start = clock();
                out_brute = fui_brute(arr_gen, num_elems, &num_elems_out, &err_flag);
                end = clock();
                free(out_brute);
                printf("BRUTE_ORIG_NOF_COUNT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);
            }
        }
        else {
            if(with_fio == 1) {
                start = clock();
                arr_input = import_1d_u32(data_file_bin, "", &num_elems_read, &err_flag);
            }
            else {
                start = clock();
                arr_input = import_1d_u32(data_file_csv, "csv", &num_elems_read, &err_flag);
            }
            out_brute = fui_brute(arr_input, num_elems_read, &num_elems_out, &err_flag);
            end = clock();
            free(arr_input);
            free(out_brute);
            printf("BRUTE_ORIG_FIO_EXPORT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

            if(with_count == 1) {
                if(with_fio == 1) {
                    start = clock();
                    arr_input = import_1d_u32(data_file_bin, "", &num_elems_read, &err_flag);
                }
                else {
                    start = clock();
                    arr_input = import_1d_u32(data_file_csv, "csv", &num_elems_read, &err_flag);
                }
                uniq_count = fui_brute_count(arr_input, num_elems_read, &err_flag);
                end = clock();
                free(arr_input);
                printf("BRUTE_ORIG_FIO_COUNT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, uniq_count);
            }
        }
    }
    print_dup_idx_list(dup_idx_list1, 3);
    print_out_idx(out_bit_dyn_idx, num_elems_out_idx, 5);
    free(out_bit_dyn_idx);
    free_dup_idx_list(dup_idx_list1);
    
    arr_gen = (uint32_t *)malloc(sizeof(uint32_t) * num_elems);
    if(arr_gen == NULL) {
        printf("ERROR: Failed to allocate memory for input array.\n");
        return 5;
    }

    printf("Generating a random array for benchmarking ...\n");
    generate_growing_arr(arr_gen, num_elems);
    if(with_fio != 0) {
        printf("Writing data to files ...\n");
        snprintf(data_file_bin, 512, "growing_%s_%s.bin", argv[1], argv[2]);
        if(export_1d_u32(data_file_bin, "", arr_gen, num_elems) != 0) {
            printf("ERROR: Failed to export the data to 'growing_..._....bin'.\n");
            free(arr_gen);
            return 7;
        }
        snprintf(data_file_csv, 512, "growing_%s_%s.csv", argv[1], argv[2]);
        if(export_1d_u32(data_file_csv, "csv", arr_gen, num_elems) != 0) {
            printf("ERROR: Failed to export the data to 'growing_..._....csv'.\n");
            free(arr_gen);
            return 9;
        }
        free(arr_gen);
        printf("The binary and csv data files generated.\n\n");
    }
     
    printf("GROWING ARRAY INPUT:\n");
    printf("ALGO_TYPE\t\tTIME_IN_SEC\tUNIQUE_INTEGERS\n");

    if(with_fio == 0) {
        start = clock();
        out_bit_dyn = fui_bitmap_dyn(arr_gen, num_elems, &num_elems_out, &err_flag);
        end = clock();
        free(out_bit_dyn);
        printf("BTAS_DYN_NOF_EXPORT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

        if(with_count == 1) {
            start = clock();
            uniq_count = fui_bitmap_dyn_count(arr_gen, num_elems, &err_flag);
            end = clock();
            printf("BTAS_DYN_NOF_COUNT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, uniq_count);
        }
    }
    else {
        if(with_fio == 1) {
            start = clock();
            arr_input = import_1d_u32(data_file_bin, "", &num_elems_read, &err_flag);
        }
        else {
            start = clock();
            arr_input = import_1d_u32(data_file_csv, "csv", &num_elems_read, &err_flag);
        }
        out_bit_dyn = fui_bitmap_dyn(arr_input, num_elems_read, &num_elems_out, &err_flag);
        end = clock();
        free(arr_input);
        free(out_bit_dyn);
        printf("BTAS_DYN_FIO_EXPORT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

        if(with_count == 1) {
            if(with_fio == 1) {
                start = clock();
                arr_input = import_1d_u32(data_file_bin, "", &num_elems_read, &err_flag);
            }
            else {
                start = clock();
                arr_input = import_1d_u32(data_file_csv, "csv", &num_elems_read, &err_flag);
            }
            uniq_count = fui_bitmap_dyn_count(arr_input, num_elems_read, &err_flag);
            end = clock();
            free(arr_input);
            printf("BTAS_DYN_FIO_COUNT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, uniq_count);
        }
    }
    
    if(with_fio == 0) {
        start = clock();
        out_bit_dyn_idx = fui_bitmap_idx(arr_gen, num_elems, &num_elems_out_idx, &err_flag, &dup_idx_list2);
        end = clock();
        printf("BTAS_IDX_NOF_EXPORT:\t%lf\t%ld\t::::%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out_idx, err_flag);
    }
    else {
        if(with_fio == 1) {
            start = clock();
            arr_input = import_1d_u32(data_file_bin, "", &num_elems_read, &err_flag);
        }
        else {
            start = clock();
            arr_input = import_1d_u32(data_file_csv, "csv", &num_elems_read, &err_flag);
        }
        out_bit_dyn_idx = fui_bitmap_idx(arr_input, num_elems_read, &num_elems_out_idx, &err_flag, &dup_idx_list2);
        end = clock();
        free(arr_input);
        printf("BTAS_IDX_FIO_EXPORT:\t%lf\t%ld\t::::%d\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out_idx, err_flag);
    }

    if(with_fio == 0) {
        start = clock();
        out_bit_stc = fui_bitmap_stc(arr_gen, num_elems, &num_elems_out, &err_flag);
        end = clock();
        free(out_bit_stc);
        printf("BTAS_STC_NOF_EXPORT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

        if(with_count == 1) {
            start = clock();
            uniq_count = fui_bitmap_stc_count(arr_gen, num_elems, &err_flag);
            end = clock();
            printf("BTAS_STC_NOF_COUNT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, uniq_count);
        }
    }
    else {
        if(with_fio == 1) {
            start = clock();
            arr_input = import_1d_u32(data_file_bin, "", &num_elems_read, &err_flag);
        }
        else {
            start = clock();
            arr_input = import_1d_u32(data_file_csv, "csv", &num_elems_read, &err_flag);
        }
        out_bit_stc = fui_bitmap_stc(arr_input, num_elems_read, &num_elems_out, &err_flag);
        end = clock();
        free(arr_input);
        free(out_bit_stc);
        printf("BTAS_STC_FIO_EXPORT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

        if(with_count == 1) {
            if(with_fio == 1) {
                start = clock();
                arr_input = import_1d_u32(data_file_bin, "", &num_elems_read, &err_flag);
            }
            else {
                start = clock();
                arr_input = import_1d_u32(data_file_csv, "csv", &num_elems_read, &err_flag);
            }
            uniq_count = fui_bitmap_stc_count(arr_input, num_elems_read, &err_flag);
            end = clock();
            free(arr_input);
            printf("BTAS_STC_FIO_COUNT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, uniq_count);
        }
    }
    
    if(with_fio == 0) {
        start = clock();
        out_ht_stree = fui_htable(arr_gen, num_elems, &num_elems_out, &err_flag);
        end = clock();
        free(out_ht_stree);
        printf("HTBL_NOF_EXPORT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

        if(with_count == 1) {
            start = clock();
            uniq_count = fui_htable_count(arr_gen, num_elems, &err_flag);
            end = clock();
            printf("HTBL_NOF_COUNT :\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, uniq_count);
        }
    }
    else {
        if(with_fio == 1) {
            start = clock();
            arr_input = import_1d_u32(data_file_bin, "", &num_elems_read, &err_flag);
        }
        else {
            start = clock();
            arr_input = import_1d_u32(data_file_csv, "csv", &num_elems_read, &err_flag);
        }
        out_ht_stree = fui_htable(arr_input, num_elems_read, &num_elems_out, &err_flag);
        end = clock();
        free(arr_input);
        free(out_ht_stree);
        printf("HTBL_FIO_EXPORT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

        if(with_count == 1) {
            if(with_fio == 1) {
                start = clock();
                arr_input = import_1d_u32(data_file_bin, "", &num_elems_read, &err_flag);
            }
            else {
                start = clock();
                arr_input = import_1d_u32(data_file_csv, "csv", &num_elems_read, &err_flag);
            }
            uniq_count = fui_htable_count(arr_input, num_elems_read, &err_flag);
            end = clock();
            free(arr_input);
            printf("HTBL_FIO_COUNT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, uniq_count);
        }
    }

    if(with_fio == 0) {
        start = clock();
        out_ht_dyn = fui_htable_dyn(arr_gen, num_elems, &num_elems_out, &err_flag);
        end = clock();
        free(out_ht_dyn);
        printf("HTBL_DYN_NOF_EXPORT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

        if(with_count == 1) {
            start = clock();
            uniq_count = fui_htable_dyn_count(arr_gen, num_elems, &err_flag);
            end = clock();
            printf("HTBL_DYN_NOF_COUNT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, uniq_count);
        }
    }
    else {
        if(with_fio == 1) {
            start = clock();
            arr_input = import_1d_u32(data_file_bin, "", &num_elems_read, &err_flag);
        }
        else {
            start = clock();
            arr_input = import_1d_u32(data_file_csv, "csv", &num_elems_read, &err_flag);
        }
        out_ht_dyn = fui_htable_dyn(arr_input, num_elems_read, &num_elems_out, &err_flag);
        end = clock();
        free(arr_input);
        free(out_ht_dyn);
        printf("HTBL_DYN_FIO_EXPORT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

        if(with_count == 1) {
            if(with_fio == 1) {
                start = clock();
                arr_input = import_1d_u32(data_file_bin, "", &num_elems_read, &err_flag);
            }
            else {
                start = clock();
                arr_input = import_1d_u32(data_file_csv, "csv", &num_elems_read, &err_flag);
            }
            uniq_count = fui_htable_dyn_count(arr_input, num_elems_read, &err_flag);
            end = clock();
            free(arr_input);
            printf("HTBL_DYN_FIO_COUNT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, uniq_count);
        }
    }
    
    if(with_brute == 1) {
        if(with_fio == 0) {
            start = clock();
            out_brute_opt = fui_brute_opt(arr_gen, num_elems, &num_elems_out, &err_flag);
            end = clock();
            free(out_brute_opt);
            printf("BRUTE_OPT_NOF_EXPORT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

            if(with_count == 1) {
                start = clock();
                uniq_count = fui_brute_opt_count(arr_gen, num_elems, &err_flag);
                end = clock();
                printf("BRUTE_OPT_NOF_COUNT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, uniq_count);
            }
        }
        else {
            if(with_fio == 1) {
                start = clock();
                arr_input = import_1d_u32(data_file_bin, "", &num_elems_read, &err_flag);
            }
            else {
                start = clock();
                arr_input = import_1d_u32(data_file_csv, "csv", &num_elems_read, &err_flag);
            }
            out_brute_opt = fui_brute_opt(arr_input, num_elems_read, &num_elems_out, &err_flag);
            end = clock();
            free(arr_input);
            free(out_brute_opt);
            printf("BRUTE_OPT_FIO_EXPORT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);

            if(with_count == 1) {
                if(with_fio == 1) {
                    start = clock();
                    arr_input = import_1d_u32(data_file_bin, "", &num_elems_read, &err_flag);
                }
                else {
                    start = clock();
                    arr_input = import_1d_u32(data_file_csv, "csv", &num_elems_read, &err_flag);
                }
                uniq_count = fui_brute_opt_count(arr_input, num_elems_read, &err_flag);
                end = clock();
                free(arr_input);
                printf("BRUTE_OPT_FIO_COUNT:\t%lf\t%ld\n", (double)(end - start)/CLOCKS_PER_SEC, num_elems_out);
            }
        }
    }
    print_dup_idx_list(dup_idx_list2, 3);
    print_out_idx(out_bit_dyn_idx, num_elems_out_idx, 5);
    free_dup_idx_list(dup_idx_list2);
    free(out_bit_dyn_idx);
    printf("\nBenchmark done.\n\n");
    return 0;
}
