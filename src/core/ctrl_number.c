#include "ctrl_number.h"
struct ctrl_number_float_long_int_t CTRL_Number_Float_Int_Multiply(struct ctrl_number_float_long_int_t a, struct ctrl_number_float_long_int_t b)
{
    ctrl_number_long_t accuracy = a.accuracy > b.accuracy ? a.accuracy : b.accuracy;
    ctrl_uint8_t accuracy_cut = a.accuracy - b.accuracy;
    ctrl_bool sign = a.sign == b.sign ? true : false;

    ctrl_number_long_t cut = 1;
    ctrl_number_long_t i;
    for (i = 0; i < accuracy; i++)
    {
        cut *= 10;
    }

    if (accuracy_cut > 0)
    {
        for (i = 0; i < accuracy_cut; i++)
        {
            b.decimal *= 10;
        }
    }
    else if (accuracy_cut < 0)
    {
        accuracy_cut = -accuracy_cut;
        for (i = 0; i < accuracy_cut; i++)
        {
            a.decimal *= 10;
        }
    }

    ctrl_number_long_t cutAccuracy = 1;

    if (a.decimal != 0 && b.decimal != 0)
    {
        while (true)
        {
            ctrl_number_long_t aD = a.decimal / 10;
            ctrl_number_long_t bD = b.decimal / 10;
            if (a.decimal % 10 != 0 || b.decimal % 10 != 0)
            {
                break;
            }
            a.decimal = aD;
            b.decimal = bD;
            cutAccuracy *= 10;
        }
    }
    
    struct ctrl_number_float_long_int_t result;
    ctrl_number_long_t integerCut = (b.integer * cut);
    ctrl_number_long_t divCut = cutAccuracy * cutAccuracy;
    result.accuracy = accuracy;
    result.sign = sign;
    if (cut > divCut)
    {
        divCut = (cut / divCut);
        result.decimal = (a.decimal * (integerCut / cutAccuracy) + a.decimal * b.decimal) / divCut;
    }
    else
    {
        divCut = divCut / cut;
        if (divCut == 0)
        {
            divCut = 1;
        }
        result.decimal = (a.decimal * (integerCut / cutAccuracy) + a.decimal * b.decimal) * divCut;
    }
    result.integer = (a.integer * integerCut + a.integer * (b.decimal * cutAccuracy));

    result.decimal = (result.integer % cut) + result.decimal;
    result.integer = result.integer / cut + result.decimal / cut;
    result.decimal = result.decimal % cut;

    return result;
}

struct ctrl_number_float_long_int_t CTRL_Number_Float_Int_Divide(struct ctrl_number_float_long_int_t a, struct ctrl_number_float_long_int_t b)
{
    ctrl_number_long_t accuracy = a.accuracy > b.accuracy ? a.accuracy : b.accuracy;
    ctrl_uint8_t accuracy_cut = a.accuracy - b.accuracy;
    ctrl_bool sign = a.sign == b.sign ? true : false;

    ctrl_number_long_t cut = 1;
    ctrl_number_long_t i;
    for (i = 0; i < accuracy; i++)
    {
        cut *= 10;
    }
    if (accuracy_cut > 0)
    {
        for (i = 0; i < accuracy_cut; i++)
        {
            b.decimal *= 10;
        }
    }
    else if (accuracy_cut < 0)
    {
        accuracy_cut = -accuracy_cut;
        for (i = 0; i < accuracy_cut; i++)
        {
            a.decimal *= 10;
        }
    }

    struct ctrl_number_float_long_int_t result;
    result.accuracy = accuracy;
    result.sign = sign;

    ctrl_number_long_t aDivLongInt = a.integer * cut * cut + a.decimal;
    ctrl_number_long_t bDivLongInt = b.integer * cut + b.decimal;

    ctrl_number_long_t divResult = aDivLongInt / bDivLongInt;

    result.integer = divResult / cut;
    result.decimal = divResult % cut;
    return result;
}

ctrl_number_long_t CTRL_Number_Pow_Number_Long_Int(ctrl_number_long_t base, ctrl_number_long_t exponent)
{
#if CTRL_NUMBER_USE_MATH_INCLUDE == 0
    ctrl_number_long_t result = 1;

    if (exponent < 0)
    {
        return 0;
    }

    while (exponent > 0)
    {
        if (exponent % 2 == 1)
        {
            result *= base;
        }
        base *= base;
        exponent /= 2;
    }

    return result;
#elif CTRL_NUMBER_USE_MATH_INCLUDE == 1
    ctrl_number_long_t result = 1;
    for (ctrl_number_long_t i = 0; i < exponent; i++)
    {
        result *= base;
    }
    return result;
#elif CTRL_NUMBER_USE_MATH_INCLUDE == 2
    return (ctrl_number_long_t)pow((ctrl_double_t)base, (ctrl_double_t)exponent);
#elif CTRL_NUMBER_USE_MATH_INCLUDE == 3
    return (ctrl_number_long_t)powl((ctrl_long_double_t)base, (ctrl_long_double_t)exponent);
#endif
}