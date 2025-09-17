#ifndef __CTRL_MEMORY_H
#define __CTRL_MEMORY_H

#include "../ctrl_define.h"

#if CTRL_MEMORY_HANDLE_STYLE == 0

#ifndef CTRL_MAX_MEMORY_MAP_SIZE
#define CTRL_MAX_MEMORY_SIZE 4096
#else
#define CTRL_MAX_MEMORY_SIZE CTRL_MAX_MEMORY_MAP_SIZE
#endif

#endif

#ifndef ctrl_memory_byte_t
#define ctrl_memory_byte_t uint8_t
#endif

#ifndef ctrl_memory_long_t

#if CTRL_MEMORY_TYPE_SIZE_BIT == 64
#define ctrl_memory_long_t ctrl_uint64_t
#elif CTRL_MEMORY_TYPE_SIZE_BIT == 32
#define ctrl_memory_long_t ctrl_uint32_t
#elif CTRL_MEMORY_TYPE_SIZE_BIT == 16
#define ctrl_memory_long_t ctrl_uint16_t
#else
#error "CTRL_MEMORY_TYPE_SIZE_BIT is error, only support 16, 32, 64"
#endif

#endif

#ifdef __cplusplus
extern "C"
{
#endif

    /**********************
     * 函数功能：申请内存
     * 参数[size]：申请内存大小
     * 返回值：内存地址
     **********************/
    void *CTRL_Memory_Malloc(ctrl_memory_long_t size);

    /**********************
     * 函数功能：释放内存
     * 参数[ptr]：内存指针
     * 返回值：0是成功，1是失败
     **********************/
    ctrl_memory_byte_t CTRL_Memory_Free(void *ptr);

    /**********************
     * 函数功能：将某块内存初始化
     * 参数[ptr]：内存指针
     * 返回值：0是成功，1是失败
     **********************/
    ctrl_memory_byte_t CTRL_Memory_Clear(void *ptr, ctrl_memory_long_t size);

#ifdef __cplusplus
}
#endif

#endif
