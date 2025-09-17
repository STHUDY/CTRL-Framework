#ifndef __CTRL_DEFINE_H
#define __CTRL_DEFINE_H

/*==============================================================================
 * CTRL 版本信息
 * =============================================================================
 */
#define CTRL_VERSION "1.0.0"

/*==============================================================================
 * CTRL 内存设置
 * =============================================================================
 */
/**
 * CPU 位数
 */
#define CTRL_CPU_BIT 64
/**
 * CPU 架构
 * 0 => i386
 * 1 => x86_64
 */
#define CTRL_CPU_FRAME 0

/*==============================================================================
 * CTRL 内存设置
 * =============================================================================
 */
#define CTRL_MEMORY_TYPE_SIZE_BIT 64
/**
 * 部分单片机不支持堆内存，请选择自带库处理方式或者自定义
 * 内存处理方式（0：自带库处理，1：标准库处理，2：自定义处理-[请转到ctrl_memory.c中进行自定义处理]）
 *
 */
#define CTRL_MEMORY_HANDLE_STYLE 1
/**
 * 内存大小
 * 当且仅当CTRL_MEMORY_HANDLE_STYLE为0时，CTRL内存大小才会生效
 */
#define CTRL_MAX_MEMORY_MAP_SIZE 4096

/*=============================================================================
 * CTRL 节点设置
 *=============================================================================
 */
#define CTRL_NODE_TYPE_SIZE_BIT 64
/**
 * 开启节点复用可以减少因内存分配所引起的性能损耗，但也会占用更多内存
 * 1：开启节点复用
 * 0：关闭节点复用
 */
#define CTRL_NODE_CAN_REUSE 1 // 是否开启节点重用

/*=============================================================================
 * CTRL 接口设置
 *=============================================================================
 */
#define CTRL_INTERFACE_TYPE_SIZE_BIT 64

/*=============================================================================
 * CTRL 数据设置
 *=============================================================================
 */
#define CTRL_NUMBER_TYPE_SIZE_BIT 64

/**
 * 使用标准数学库
 * 0: 使用自带的数学库（快速计算）
 * 1: 使用自带的数学库（准确计算）
 * 2: 使用标准库math.h（快速计算）
 * 3：使用标准库math.h（准确计算）
 */
#define CTRL_NUMBER_USE_MATH_INCLUDE 1

/** ----------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */

#if CTRL_MEMORY_HANDLE_STYLE == 1
#include <malloc.h>
#undef CTRL_MAX_MEMORY_MAP_SIZE
#else if CTRL_MEMORY_HANDLE_STYLE == 2
#undef CTRL_MAX_MEMORY_MAP_SIZE
#endif

#if CTRL_NUMBER_USE_MATH_INCLUDE == 2 || CTRL_NUMBER_USE_MATH_INCLUDE == 3
#include <math.h>
#endif

#ifndef _STDINT

#ifndef uint8_t
#define uint8_t unsigned char
#endif

#ifndef int8_t
#define int8_t signed char
#endif

#ifndef uint16_t
#define uint16_t unsigned short
#endif

#ifndef int16_t
#define int16_t signed short
#endif

#ifndef uint32_t
#define uint32_t unsigned int
#endif

#ifndef int32_t
#define int32_t signed int
#endif

#ifndef uint64_t
#define uint64_t unsigned long long
#endif

#ifndef int64_t
#define int64_t signed long long
#endif

#ifndef float_t
#define float_t float
#endif

#ifndef double_t
#define double_t double
#endif

#ifndef long_double_t
#define long_double_t long double
#endif

#endif

#ifndef _INC_STDDEF

#ifndef NULL
#define NULL ((void *)0)
#endif

#endif

#ifndef _STDBOOL

#ifndef bool
#define bool uint8_t
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#endif

#define ctrl_int8_t int8_t
#define ctrl_uint8_t uint8_t
#define ctrl_int16_t int16_t
#define ctrl_uint16_t uint16_t
#define ctrl_int32_t int32_t
#define ctrl_uint32_t uint32_t
#define ctrl_int64_t int64_t
#define ctrl_uint64_t uint64_t
#define ctrl_float_t float_t
#define ctrl_double_t double_t
#define ctrl_long_double_t long_double_t
#define ctrl_nullptr NULL
#define ctrl_none 0
#define ctrl_bool bool
#define ctrl_true true
#define ctrl_false false

#endif
