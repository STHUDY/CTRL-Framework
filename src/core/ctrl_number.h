#ifndef __CTRL_NUMBER_H
#define __CTRL_NUMBER_H

#include "../ctrl_define.h"

#if CTRL_NUMBER_TYPE_SIZE_BIT == 64
#define ctrl_number_long_t ctrl_uint64_t
#elif CTRL_NUMBER_TYPE_SIZE_BIT == 32
#define ctrl_number_long_t ctrl_uint32_t
#elif CTRL_NUMBER_TYPE_SIZE_BIT == 16
#define ctrl_number_long_t ctrl_uint16_t
#else
#error "CTRL_NODE_TYPE_SIZE_BIT is error, only support 16, 32, 64"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct ctrl_number_any_t
    {
        uint8_t byte1;
        uint8_t byte2;
        uint8_t byte3;
        uint8_t byte4;
        uint8_t byte5;
        uint8_t byte6;
        uint8_t byte7;
        uint8_t byte8;
#if CTRL_CPU_BIT == 64
        uint8_t byte9;
        uint8_t byte10;
        uint8_t byte11;
        uint8_t byte12;
#if CTRL_CPU_FRAME == 1
        uint8_t byte13;
        uint8_t byte14;
        uint8_t byte15;
        uint8_t byte16;
#endif

#endif
    } ctrl_any_t;

#define ctrl_any ctrl_any_t

#if CTRL_CPU_BIT == 64

#if CTRL_CPU_FRAME == 1
#define ctrl_any_none ((ctrl_any_t){0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})
#else
#define ctrl_any_none ((ctrl_any_t){0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})
#endif

#else

#define ctrl_any_none ((ctrl_any_t){0, 0, 0, 0, 0, 0, 0, 0})

#endif

#define ctrl_any_64bit_t ctrl_uint64_t
#define ctrl_any_64bit_none 0

#define ctrl_any_32bit_t ctrl_uint32_t
#define ctrl_any_32bit_none 0

    struct ctrl_number_float_long_int_t
    {
        ctrl_bool sign;
        ctrl_uint8_t accuracy;
        ctrl_number_long_t integer;
        ctrl_number_long_t decimal;
    };

    /**
     * 整数型浮点数乘法
     * 此函数实现比较抽象，可能会造成大量性能损耗请注意
     */
    struct ctrl_number_float_long_int_t CTRL_Number_Float_Int_Multiply(struct ctrl_number_float_long_int_t a, struct ctrl_number_float_long_int_t b);

    /**
     * 整数型浮点数除法
     * 此函数实现比较抽象，可能会造成大量性能损耗请注意
     */
    struct ctrl_number_float_long_int_t CTRL_Number_Float_Int_Divide(struct ctrl_number_float_long_int_t a, struct ctrl_number_float_long_int_t b);

    /**
     * 计算整数型的幂
     */
    ctrl_number_long_t CTRL_Number_Pow_Number_Long_Int(ctrl_number_long_t base, ctrl_number_long_t exponent);

/* ======================================================================
 * 宏定义函数
 * ====================================================================== */
#if CTRL_NUMBER_USE_MATH_INCLUDE % 2 == 1

#define CTRL_Float_Int_To_Float_(number) CTRL_Number_Float_Int_To_Float(number)
#define CTRL_Float_Int_To_Double_(number) CTRL_Number_Float_Int_To_Double(number)
    static inline ctrl_float_t CTRL_Number_Float_Int_To_Float(struct ctrl_number_float_long_int_t number)
    {
        ctrl_float_t decimal = (ctrl_float_t)number.decimal;
        for (int i = 0; i < number.accuracy; i++)
        {
            decimal /= 10.0f;
        }
        return (int8_t)(((!!number.sign) << 1) - 1) * ((ctrl_float_t)(number.integer) + decimal);
    }

    static inline ctrl_double_t CTRL_Number_Float_Int_To_Double(struct ctrl_number_float_long_int_t number)
    {
        ctrl_double_t decimal = (ctrl_double_t)number.decimal;
        for (int i = 0; i < number.accuracy; i++)
        {
            decimal /= 10.0;
        }
        return (int8_t)(((!!number.sign) << 1) - 1) * ((ctrl_double_t)(number.integer) + decimal);
    }

#else
#define CTRL_Float_Int_To_Float_(number) ((int8_t)(((!!number.sign) << 1) - 1) * ((ctrl_float_t)(number.integer) + number.decimal / (ctrl_float_t)CTRL_Number_Pow_Number_Long_Int(10, number.accuracy)))
#define CTRL_Float_Int_To_Double_(number) ((int8_t)(((!!number.sign) << 1) - 1) * ((ctrl_double_t)(number.integer) + number.decimal / (ctrl_double_t)CTRL_Number_Pow_Number_Long_Int(10, number.accuracy)))
#endif

#define CTRL_Number_Set_NumberAny32bit_Int32_(numberAny32bit, value) (*numberAny32bit = *(ctrl_any_32bit_t *)value)
#define CTRL_Number_Set_NumberAny32bit_Float_(numberAny32bit, value) (*numberAny32bit = *(ctrl_any_32bit_t *)value)

#define CTRL_Number_Get_NumberAny32bit_Int32_(numberAny32bit) (*(ctrl_int32_t *)numberAny32bit)
#define CTRL_Number_Get_NumberAny32bit_Float_(numberAny32bit) (*(ctrl_float_t *)numberAny32bit)

#define CTRL_Number_To_NumberAny32bit_(number) (*(ctrl_any_32bit_t *)number)

#define CTRL_Number_Abs_(number) (number >= 0 ? number : -number)

#ifdef __cplusplus
}
#endif

#endif
