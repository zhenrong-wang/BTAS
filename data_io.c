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
#include "data_io.h"

int export_1d_u32(const char *target_file, const char* type, uint32_t *array, uint64_t num_elems) {
    if(array == NULL || num_elems == 0) {
        return -3;
    }
    int file_type_flag = 0; /* 0 - bin, 1 - csv */
    FILE* file_p = NULL;
    if(type != NULL && strcmp(type, "csv") == 0) {
        file_type_flag = 1;
    }
    if(file_type_flag == 0) {
        file_p = fopen(target_file, "wb+");
    }
    else {
        file_p = fopen(target_file, "w+");
    }
    if(file_p == NULL) {
        return -1;
    }
    if(file_type_flag == 0) {
        size_t elem_written = fwrite(array, sizeof(int32_t), num_elems, file_p);
        fclose(file_p);
        if(elem_written != num_elems) {
            return 1;
        }
        return 0;
    }
    else {
        for(uint64_t i = 0; i < num_elems; i++) {
            fprintf(file_p, "%d\n", array[i]);
        }
        fclose(file_p);
        return 0;
    }
}

uint32_t* import_1d_u32(const char *source_file, const char* type, uint64_t *num_elems_read, int *err_flag) {
    *err_flag = 0;
    *num_elems_read = 0;
    int file_type_flag = 0; /* 0 - bin, 1 - csv */
    FILE* file_p = NULL;
    if(type != NULL && strcmp(type, "csv") == 0) {
        file_type_flag = 1;
    }

    if(file_type_flag == 0) {
        file_p = fopen(source_file, "rb");
    }
    else {
        file_p = fopen(source_file, "r");
    }
    if(file_p == NULL) {
        *err_flag = -1;
        return NULL;
    }

    if(file_type_flag == 0) {
        fseek(file_p, 0, SEEK_END);
        long file_size = ftell(file_p);
        rewind(file_p);
        size_t num_elems_total = file_size / sizeof(uint32_t);
        uint32_t *array = (uint32_t *)calloc(num_elems_total, sizeof(uint32_t));
        if(array == NULL) {
            *err_flag = 1;
            fclose(file_p);
            return NULL;
        }
        size_t num_elems = fread(array, sizeof(uint32_t), num_elems_total, file_p);
        *num_elems_read = num_elems;
        if(num_elems != num_elems_total) {
            *err_flag = 3;
            free(array);
            fclose(file_p);
            return NULL;
        }
        fclose(file_p);
        return array;
    }
    else{
        size_t i = 0, blocks = 1;
        char ch;
        uint32_t *array_tmp = NULL;
        uint32_t *array = (uint32_t *)calloc(TXT_READ_BLOCK, sizeof(uint32_t));
        if(array == NULL) {
            *err_flag = 1;
            fclose(file_p);
            return NULL;
        }
        while(!feof(file_p)){
            int scanf_res = fscanf(file_p, "%d%c", array + i, &ch);
            if(scanf_res < 2) {
                if(ferror(file_p)){
                    *err_flag = 5;
                    free(array);
                    fclose(file_p);
                    return NULL;
                }
            }
            if(scanf_res > 0){
                i++;
            }
            if(i % TXT_READ_BLOCK == 0) {
                blocks++;
                array_tmp = (uint32_t *)realloc(array, blocks * TXT_READ_BLOCK * sizeof(uint32_t));
                if(array_tmp == NULL) {
                    *err_flag = 7;
                    free(array);
                    fclose(file_p);
                    return NULL;
                }
                memset(array_tmp + (blocks - 1) * TXT_READ_BLOCK, 0, TXT_READ_BLOCK * sizeof(uint32_t));
                array = array_tmp;
            }
        }
        fclose(file_p); 
        *num_elems_read = i;
        array_tmp = (uint32_t *)realloc(array, i * sizeof(uint32_t));
        if(array_tmp == NULL) {
            return array;
        }
        else{
            return array_tmp;
        }
    }
}