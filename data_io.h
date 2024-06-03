/**
 * 
 * This code is distributed under the license: MIT License
 * Originally written by Zhenrong WANG
 * mailto: zhenrongwang@live.com 
 * GitHub: https://github.com/zhenrong-wang
 * 
 */

#ifndef DATA_IO
#define DATA_IO
#include <stdint.h>

#define TXT_READ_BLOCK 1048576

int export_1d_u32(const char *target_file, const char* type, uint32_t *array, uint64_t num_elems);
uint32_t* import_1d_u32(const char *source_file, const char* type, uint64_t *num_elems_read, int *err_flag);

#endif