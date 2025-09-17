#include "ctrl_pid_incremental.h"
#include "../core/ctrl_memory.h"

#define CTRL_PID_INCREMENTAL_CALC_FORMULA_NONE_(kp, ki, kd, bias, lastOneBias, lastTwoBias) (kp * (bias - lastOneBias) + ki * bias + kd * (bias - 2 * lastOneBias + lastTwoBias))

enum ctrl_pid_incremental_calc_result_e
{
    _CTRL_PID_INCREMENTAL_CALC_RESULT_NONE = 0,
    _CTRL_PID_INCREMENTAL_CALC_RESULT_CONTINUE,
    _CTRL_PID_INCREMENTAL_CALC_RESULT_RETURN
};

struct ctrl_pid_incremental_config_uint32_t
{
    ctrl_bool isSummate;

    ctrl_bool isOpenLimit;
    ctrl_bool isLimitAll;
    ctrl_any_32bit_t limitMin;
    ctrl_any_32bit_t limitMax;

    ctrl_bool isOpenDeadZone;
    ctrl_bool isDeadKeepOut;
    ctrl_bool isDeadZoneAll;
    ctrl_any_32bit_t deadUp;
    ctrl_any_32bit_t deadDown;

    ctrl_bool isUseUnit;
    ctrl_bool isNeedInputUnit;
    ctrl_any_32bit_t unit;
    ctrl_any_32bit_t unitOffset;

    ctrl_bool isUseCallback;
    void (*beforeCallback)(ctrl_uint8_t, ctrl_interface_id_t);
    void (*callback)(ctrl_uint8_t, ctrl_interface_id_t);
    void (*afterCallback)(ctrl_uint8_t, ctrl_interface_id_t);
};

struct ctrl_pid_incremental_uint32_t
{
    ctrl_interface_id_t id;

    ctrl_any_32bit_t kp;
    ctrl_any_32bit_t ki;
    ctrl_any_32bit_t kd;

    struct ctrl_pid_incremental_config_uint32_t config;
    enum ctrl_pid_incremental_calc_e calcStyle;

    ctrl_any_32bit_t aimValue;
    ctrl_any_32bit_t currentValue;
    ctrl_any_32bit_t sampleTicks;

    ctrl_any_32bit_t bias;
    ctrl_any_32bit_t lastOneBias;
    ctrl_any_32bit_t lastTwoBias;

    ctrl_any_32bit_t output;
};

struct ctrl_pid_incremental_base_t
{
    void *pidIncremental;
    enum ctrl_interface_accuracy_e accuracy;
};

ctrl_bool CTRL_PID_Incremental_Load_DefaultConfig_By_PidIncrementalBase_Self(struct ctrl_pid_incremental_base_t *pidIncrementalBase);
ctrl_bool CTRL_PID_Incremental_Set_Config_By_PidIncrementalBase_Tuint32_Self(struct ctrl_pid_incremental_base_t *pidIncrementalBase, enum ctrl_pid_incremental_config_e configChoose, ctrl_any_32bit_t value);
ctrl_bool CTRL_PID_Incremental_Set_Value_By_PidIncrementalBase_Tuint32_Self(struct ctrl_pid_incremental_base_t *pidIncrementalBase, enum ctrl_pid_incremental_e choose, ctrl_any_32bit_t value);

enum ctrl_pid_incremental_calc_result_e CTRL_PID_Incremental_Calc_Before_By_PidIncremental_Tuint32_Int32_Self(struct ctrl_pid_incremental_uint32_t *pidIncremental, ctrl_int32_t *result);
enum ctrl_pid_incremental_calc_result_e CTRL_PID_Incremental_Calc_By_PidIncremental_Tuint32_Int32_Self(struct ctrl_pid_incremental_uint32_t *pidIncremental, ctrl_int32_t *result);
enum ctrl_pid_incremental_calc_result_e CTRL_PID_Incremental_Calc_After_By_PidIncremental_Tuint32_Int32_Self(struct ctrl_pid_incremental_uint32_t *pidIncremental, ctrl_int32_t *result);

enum ctrl_pid_incremental_calc_result_e CTRL_PID_Incremental_Calc_Before_By_PidIncremental_Tuint32_Float_Self(struct ctrl_pid_incremental_uint32_t *pidIncremental, ctrl_float_t *result);
enum ctrl_pid_incremental_calc_result_e CTRL_PID_Incremental_Calc_By_PidIncremental_Tuint32_Float_Self(struct ctrl_pid_incremental_uint32_t *pidIncremental, ctrl_float_t *result);
enum ctrl_pid_incremental_calc_result_e CTRL_PID_Incremental_Calc_After_By_PidIncremental_Tuint32_Float_Self(struct ctrl_pid_incremental_uint32_t *pidIncremental, ctrl_float_t *result);

ctrl_bool CTRL_PID_Incremental_Load_DefaultConfig_By_PidIncrementalBase_Self(struct ctrl_pid_incremental_base_t *pidIncrementalBase)
{
    switch (pidIncrementalBase->accuracy)
    {
    case _CTRL_INTERFACE_ACCURACY_INT:
    case _CTRL_INTERFACE_ACCURACY_FLOAT:
        struct ctrl_pid_incremental_uint32_t *pidIncrementalUint32 = (struct ctrl_pid_incremental_uint32_t *)pidIncrementalBase->pidIncremental;
        pidIncrementalUint32->config.isSummate = ctrl_true;

        pidIncrementalUint32->config.isOpenLimit = ctrl_false;
        pidIncrementalUint32->config.isLimitAll = ctrl_false;
        pidIncrementalUint32->config.limitMin = ctrl_any_32bit_none;
        pidIncrementalUint32->config.limitMax = ctrl_any_32bit_none;

        pidIncrementalUint32->config.isOpenDeadZone = ctrl_false;
        pidIncrementalUint32->config.isDeadZoneAll = ctrl_false;
        pidIncrementalUint32->config.deadUp = ctrl_any_32bit_none;
        pidIncrementalUint32->config.deadDown = ctrl_any_32bit_none;

        pidIncrementalUint32->config.isUseUnit = ctrl_false;
        pidIncrementalUint32->config.isNeedInputUnit = ctrl_false;
        pidIncrementalUint32->config.unit = ctrl_any_32bit_none;
        pidIncrementalUint32->config.unitOffset = ctrl_any_32bit_none;

        pidIncrementalUint32->config.isUseCallback = ctrl_false;
        pidIncrementalUint32->config.beforeCallback = ctrl_nullptr;
        pidIncrementalUint32->config.callback = ctrl_nullptr;
        pidIncrementalUint32->config.afterCallback = ctrl_nullptr;
        break;

    default:
        break;
    }

    return ctrl_true;
}

ctrl_bool CTRL_PID_Incremental_Set_Config_By_PidIncrementalBase_Tuint32_Self(struct ctrl_pid_incremental_base_t *pidIncrementalBase, enum ctrl_pid_incremental_config_e configChoose, ctrl_any_32bit_t value)
{
    struct ctrl_pid_incremental_uint32_t *pidIncrementalUint32 = (struct ctrl_pid_incremental_uint32_t *)pidIncrementalBase->pidIncremental;
    switch (configChoose)
    {
    case _CTRL_PID_INCREMENTAL_CONFIG_KP:
        pidIncrementalUint32->kp = value;
        break;
    case _CTRL_PID_INCREMENTAL_CONFIG_KI:
        pidIncrementalUint32->ki = value;
        break;
    case _CTRL_PID_INCREMENTAL_CONFIG_KD:
        pidIncrementalUint32->kd = value;
        break;

    default:
        break;
    }

    return ctrl_false;
}

ctrl_bool CTRL_PID_Incremental_Set_Value_By_PidIncrementalBase_Tuint32_Self(struct ctrl_pid_incremental_base_t *pidIncrementalBase, enum ctrl_pid_incremental_e choose, ctrl_any_32bit_t value)
{
    struct ctrl_pid_incremental_uint32_t *pidIncrementalUint32 = (struct ctrl_pid_incremental_uint32_t *)pidIncrementalBase->pidIncremental;
    switch (choose)
    {
    case _CTRL_PID_INCREMENTAL_AIM_VALUE:
        pidIncrementalUint32->aimValue = value;
        break;
    case _CTRL_PID_INCREMENTAL_CURRENT_VALUE:
        pidIncrementalUint32->currentValue = value;
        break;
    case _CTRL_PID_INCREMENTAL_SAMPLE_TICKS:
        pidIncrementalUint32->sampleTicks = value;
        break;

    default:
        break;
    }

    return ctrl_false;
}

enum ctrl_pid_incremental_calc_result_e CTRL_PID_Incremental_Calc_Before_By_PidIncremental_Tuint32_Int32_Self(struct ctrl_pid_incremental_uint32_t *pidIncremental, ctrl_int32_t *result)
{
    struct ctrl_pid_incremental_config_uint32_t config = pidIncremental->config;
    ctrl_int32_t aimValue = CTRL_Number_Get_NumberAny32bit_Int32_(&pidIncremental->aimValue);
    ctrl_int32_t currentValue = CTRL_Number_Get_NumberAny32bit_Int32_(&pidIncremental->currentValue);
    ctrl_int32_t bias = aimValue - currentValue;

    ctrl_int32_t output = CTRL_Number_Get_NumberAny32bit_Int32_(&pidIncremental->output);

    ctrl_int32_t deadDown = CTRL_Number_Get_NumberAny32bit_Int32_(&config.deadDown);
    ctrl_int32_t deadUp = CTRL_Number_Get_NumberAny32bit_Int32_(&config.deadUp);

    pidIncremental->lastTwoBias = pidIncremental->lastOneBias;
    pidIncremental->lastOneBias = pidIncremental->bias;
    CTRL_Number_Set_NumberAny32bit_Int32_(&pidIncremental->bias, &bias);

    if (config.isOpenDeadZone)
    {
        if (config.isDeadZoneAll)
        {
            ctrl_int32_t biasZoneAbs = CTRL_Number_Abs_(aimValue - currentValue);
            if (biasZoneAbs >= CTRL_Number_Abs_(deadDown) && biasZoneAbs <= CTRL_Number_Abs_(deadUp))
            {
                *result = config.isDeadKeepOut ? output : 0;
                return _CTRL_PID_INCREMENTAL_CALC_RESULT_RETURN;
            }
        }
        else
        {
            ctrl_int32_t biasZone = aimValue - currentValue;
            if (biasZone >= deadDown && biasZone <= deadUp)
            {
                *result = config.isDeadKeepOut ? output : 0;
                return _CTRL_PID_INCREMENTAL_CALC_RESULT_RETURN;
            }
        }
    }

    if (config.isUseCallback && config.beforeCallback != ctrl_nullptr)
    {
        config.beforeCallback(CTRL_PID_INCREMENTA_ID, pidIncremental->id);
    }

    return _CTRL_PID_INCREMENTAL_CALC_RESULT_CONTINUE;
}

enum ctrl_pid_incremental_calc_result_e CTRL_PID_Incremental_Calc_By_PidIncremental_Tuint32_Int32_Self(struct ctrl_pid_incremental_uint32_t *pidIncremental, ctrl_int32_t *result)
{
    struct ctrl_pid_incremental_config_uint32_t config = pidIncremental->config;

    ctrl_int32_t kp = CTRL_Number_Get_NumberAny32bit_Int32_(&pidIncremental->kp);
    ctrl_int32_t ki = CTRL_Number_Get_NumberAny32bit_Int32_(&pidIncremental->ki);
    ctrl_int32_t kd = CTRL_Number_Get_NumberAny32bit_Int32_(&pidIncremental->kd);
    ctrl_int32_t bias = CTRL_Number_Get_NumberAny32bit_Int32_(&pidIncremental->bias);
    ctrl_int32_t lastOneBias = CTRL_Number_Get_NumberAny32bit_Int32_(&pidIncremental->lastOneBias);
    ctrl_int32_t lastTwoBias = CTRL_Number_Get_NumberAny32bit_Int32_(&pidIncremental->lastTwoBias);
    ctrl_int32_t sampleTicks = CTRL_Number_Get_NumberAny32bit_Int32_(&pidIncremental->sampleTicks);

    switch (pidIncremental->calcStyle)
    {
    case _CTRL_PID_INCREMENTAL_CALC_NONE:
        *result = CTRL_PID_INCREMENTAL_CALC_FORMULA_NONE_(kp, ki, kd, bias, lastOneBias, lastTwoBias);
        break;

    default:
        break;
    }

    if (config.isUseCallback && config.callback != ctrl_nullptr)
    {
        config.callback(CTRL_PID_INCREMENTA_ID, pidIncremental->id);
    }

    return _CTRL_PID_INCREMENTAL_CALC_RESULT_CONTINUE;
}

enum ctrl_pid_incremental_calc_result_e CTRL_PID_Incremental_Calc_After_By_PidIncremental_Tuint32_Int32_Self(struct ctrl_pid_incremental_uint32_t *pidIncremental, ctrl_int32_t *result)
{
    struct ctrl_pid_incremental_config_uint32_t config = pidIncremental->config;

    ctrl_int32_t output = CTRL_Number_Get_NumberAny32bit_Int32_(&pidIncremental->output);
    ctrl_int32_t limitMin = CTRL_Number_Get_NumberAny32bit_Int32_(&config.limitMin);
    ctrl_int32_t limitMax = CTRL_Number_Get_NumberAny32bit_Int32_(&config.limitMax);

    if (config.isSummate)
    {
        output += *result;
    }
    else
    {
        output = *result;
    }

    if (config.isOpenLimit)
    {
        if (config.isLimitAll)
        {
            ctrl_int32_t sign = output > 0 ? 1 : -1;
            ctrl_int32_t outputAbs = CTRL_Number_Abs_(output);
            if (outputAbs < CTRL_Number_Abs_(limitMin))
            {
                output = limitMin * sign;
            }
            else if (outputAbs > CTRL_Number_Abs_(limitMax))
            {
                output = limitMax * sign;
            }
        }
        else
        {
            if (output < limitMin)
            {
                output = limitMin;
            }
            else if (output > limitMax)
            {
                output = limitMax;
            }
        }
    }

    CTRL_Number_Set_NumberAny32bit_Int32_(&pidIncremental->output, &output);

    *result = output;

    if (config.isUseCallback && config.afterCallback != ctrl_nullptr)
    {
        config.afterCallback(CTRL_PID_INCREMENTA_ID, pidIncremental->id);
    }

    return _CTRL_PID_INCREMENTAL_CALC_RESULT_RETURN;
}

enum ctrl_pid_incremental_calc_result_e CTRL_PID_Incremental_Calc_Before_By_PidIncremental_Tuint32_Float_Self(struct ctrl_pid_incremental_uint32_t *pidIncremental, ctrl_float_t *result)
{
    struct ctrl_pid_incremental_config_uint32_t config = pidIncremental->config;
    ctrl_float_t aimValue = CTRL_Number_Get_NumberAny32bit_Float_(&pidIncremental->aimValue);
    ctrl_float_t currentValue = CTRL_Number_Get_NumberAny32bit_Float_(&pidIncremental->currentValue);
    ctrl_float_t bias = aimValue - currentValue;

    ctrl_float_t output = CTRL_Number_Get_NumberAny32bit_Float_(&pidIncremental->output);

    ctrl_float_t deadDown = CTRL_Number_Get_NumberAny32bit_Float_(&config.deadDown);
    ctrl_float_t deadUp = CTRL_Number_Get_NumberAny32bit_Float_(&config.deadUp);

    pidIncremental->lastTwoBias = pidIncremental->lastOneBias;
    pidIncremental->lastOneBias = pidIncremental->bias;
    CTRL_Number_Set_NumberAny32bit_Float_(&pidIncremental->bias, &bias);

    if (config.isOpenDeadZone)
    {
        if (config.isDeadZoneAll)
        {
            ctrl_float_t biasZoneAbs = CTRL_Number_Abs_(aimValue - currentValue);
            if (biasZoneAbs >= CTRL_Number_Abs_(deadDown) && biasZoneAbs <= CTRL_Number_Abs_(deadUp))
            {
                *result = config.isDeadKeepOut ? output : 0;
                return _CTRL_PID_INCREMENTAL_CALC_RESULT_RETURN;
            }
        }
        else
        {
            ctrl_float_t biasZone = aimValue - currentValue;
            if (biasZone >= deadDown && biasZone <= deadUp)
            {
                *result = config.isDeadKeepOut ? output : 0;
                return _CTRL_PID_INCREMENTAL_CALC_RESULT_RETURN;
            }
        }
    }

    if (config.isUseCallback && config.beforeCallback != ctrl_nullptr)
    {
        config.beforeCallback(CTRL_PID_INCREMENTA_ID, pidIncremental->id);
    }

    return _CTRL_PID_INCREMENTAL_CALC_RESULT_CONTINUE;
}

enum ctrl_pid_incremental_calc_result_e CTRL_PID_Incremental_Calc_By_PidIncremental_Tuint32_Float_Self(struct ctrl_pid_incremental_uint32_t *pidIncremental, ctrl_float_t *result)
{
    ctrl_float_t kp = CTRL_Number_Get_NumberAny32bit_Float_(&pidIncremental->kp);
    ctrl_float_t ki = CTRL_Number_Get_NumberAny32bit_Float_(&pidIncremental->ki);
    ctrl_float_t kd = CTRL_Number_Get_NumberAny32bit_Float_(&pidIncremental->kd);
    ctrl_float_t bias = CTRL_Number_Get_NumberAny32bit_Float_(&pidIncremental->bias);
    ctrl_float_t lastOneBias = CTRL_Number_Get_NumberAny32bit_Float_(&pidIncremental->lastOneBias);
    ctrl_float_t lastTwoBias = CTRL_Number_Get_NumberAny32bit_Float_(&pidIncremental->lastTwoBias);
    ctrl_float_t sampleTicks = CTRL_Number_Get_NumberAny32bit_Float_(&pidIncremental->sampleTicks);

    switch (pidIncremental->calcStyle)
    {
    case _CTRL_PID_INCREMENTAL_CALC_NONE:
        *result = CTRL_PID_INCREMENTAL_CALC_FORMULA_NONE_(kp, ki, kd, bias, lastOneBias, lastTwoBias);
        break;

    default:
        break;
    }

    return _CTRL_PID_INCREMENTAL_CALC_RESULT_CONTINUE;
}

enum ctrl_pid_incremental_calc_result_e CTRL_PID_Incremental_Calc_After_By_PidIncremental_Tuint32_Float_Self(struct ctrl_pid_incremental_uint32_t *pidIncremental, ctrl_float_t *result)
{
    struct ctrl_pid_incremental_config_uint32_t config = pidIncremental->config;

    ctrl_float_t output = CTRL_Number_Get_NumberAny32bit_Float_(&pidIncremental->output);
    ctrl_float_t limitMin = CTRL_Number_Get_NumberAny32bit_Float_(&config.limitMin);
    ctrl_float_t limitMax = CTRL_Number_Get_NumberAny32bit_Float_(&config.limitMax);

    if (config.isSummate)
    {
        output += *result;
    }
    else
    {
        output = *result;
    }

    if (config.isOpenLimit)
    {
        if (config.isLimitAll)
        {
            ctrl_float_t sign = output > 0 ? 1.0f : -1.0f;
            ctrl_float_t outputAbs = CTRL_Number_Abs_(output);
            if (outputAbs < CTRL_Number_Abs_(limitMin))
            {
                output = limitMin * sign;
            }
            else if (outputAbs > CTRL_Number_Abs_(limitMax))
            {
                output = limitMax * sign;
            }
        }
        else
        {
            if (output < limitMin)
            {
                output = limitMin;
            }
            else if (output > limitMax)
            {
                output = limitMax;
            }
        }
    }

    CTRL_Number_Set_NumberAny32bit_Float_(&pidIncremental->output, &output);

    *result = output;

    if (config.isUseCallback && config.afterCallback != ctrl_nullptr)
    {
        config.afterCallback(CTRL_PID_INCREMENTA_ID, pidIncremental->id);
    }

    return _CTRL_PID_INCREMENTAL_CALC_RESULT_RETURN;
}

ctrl_bool CTRL_PID_Incremental_Load_DefalutConfig(ctrl_attr *attr, ctrl_interface_id_t id)
{
    struct ctrl_pid_incremental_base_t *pidIncrementalBase = (struct ctrl_pid_incremental_base_t *)CTRL_Interface_Get_Data_By_Id(attr, id);
    if (pidIncrementalBase == ctrl_nullptr)
    {
        return ctrl_false;
    }

    CTRL_PID_Incremental_Load_DefaultConfig_By_PidIncrementalBase_Self(pidIncrementalBase);

    return ctrl_true;
}

ctrl_interface_id_t CTRL_PID_Incremental_Create(ctrl_attr *attr, enum ctrl_interface_accuracy_e accuracy, ctrl_bool *isSuccess)
{
    struct ctrl_interface_result_t result = CTRL_Interface_Create_Interface(attr, accuracy);
    if (result.success == ctrl_false)
    {
        if (isSuccess != ctrl_nullptr)
        {
            *isSuccess = ctrl_false;
        }
        return ctrl_none;
    }

    struct ctrl_pid_incremental_base_t *pidIncrementalBase = (struct ctrl_pid_incremental_base_t *)CTRL_Memory_Malloc(sizeof(struct ctrl_pid_incremental_base_t));

    if (pidIncrementalBase == ctrl_nullptr)
    {
        if (isSuccess != ctrl_nullptr)
        {
            *isSuccess = ctrl_false;
        }

        CTRL_Interface_Remove_By_Id(attr, result.id);

        return ctrl_none;
    }

    struct ctrl_pid_incremental_uint32_t *pidIncrementalUint32 = (struct ctrl_pid_incremental_uint32_t *)CTRL_Memory_Malloc(sizeof(struct ctrl_pid_incremental_uint32_t));

    if (pidIncrementalUint32 == ctrl_nullptr)
    {
        if (isSuccess != ctrl_nullptr)
        {
            *isSuccess = ctrl_false;
        }

        CTRL_Memory_Free(pidIncrementalBase);

        CTRL_Interface_Remove_By_Id(attr, result.id);

        return ctrl_none;
    }

    pidIncrementalBase->accuracy = accuracy;
    pidIncrementalBase->pidIncremental = (void *)pidIncrementalUint32;

    if (CTRL_Interface_Set_Data_By_Id(attr, result.id, pidIncrementalBase) == ctrl_false)
    {
        if (isSuccess != ctrl_nullptr)
        {
            *isSuccess = ctrl_false;
        }

        CTRL_Memory_Free(pidIncrementalBase);

        CTRL_Memory_Free(pidIncrementalUint32);

        CTRL_Interface_Remove_By_Id(attr, result.id);

        return ctrl_none;
    }

    if (CTRL_Interface_Set_FindId_By_Id(attr, result.id, CTRL_PID_INCREMENTA_ID) == ctrl_false)
    {
        if (isSuccess != ctrl_nullptr)
        {
            *isSuccess = ctrl_false;
        }

        CTRL_Memory_Free(pidIncrementalBase);

        CTRL_Memory_Free(pidIncrementalUint32);

        CTRL_Interface_Remove_By_Id(attr, result.id);
    }

    pidIncrementalUint32->id = result.id;

    if (isSuccess != ctrl_nullptr)
    {
        *isSuccess = ctrl_true;
    }

    return result.id;
}

ctrl_bool CTRL_PID_Incremental_Init(ctrl_attr *attr, ctrl_interface_id_t id)
{
    struct ctrl_pid_incremental_base_t *pidIncrementalBase = CTRL_Interface_Get_Data_By_Id(attr, id);
    if (pidIncrementalBase == ctrl_nullptr)
    {
        return ctrl_false;
    }

    CTRL_PID_Incremental_Load_DefaultConfig_By_PidIncrementalBase_Self(pidIncrementalBase);

    switch (pidIncrementalBase->accuracy)
    {
    case _CTRL_INTERFACE_ACCURACY_INT:
    case _CTRL_INTERFACE_ACCURACY_FLOAT:
        struct ctrl_pid_incremental_uint32_t *pidIncrementalUint32 = (struct ctrl_pid_incremental_uint32_t *)pidIncrementalBase->pidIncremental;
        pidIncrementalUint32->bias = ctrl_any_32bit_none;
        pidIncrementalUint32->kd = ctrl_any_32bit_none;
        pidIncrementalUint32->ki = ctrl_any_32bit_none;
        pidIncrementalUint32->kp = ctrl_any_32bit_none;
        pidIncrementalUint32->lastOneBias = ctrl_any_32bit_none;
        pidIncrementalUint32->lastTwoBias = ctrl_any_32bit_none;
        pidIncrementalUint32->output = ctrl_any_32bit_none;
        pidIncrementalUint32->aimValue = ctrl_any_32bit_none;
        pidIncrementalUint32->currentValue = ctrl_any_32bit_none;
        pidIncrementalUint32->calcStyle = _CTRL_PID_INCREMENTAL_CALC_NONE;
        pidIncrementalUint32->sampleTicks = ctrl_any_32bit_none;
        break;

    default:
        break;
    }

    return ctrl_true;
}

ctrl_bool CTRL_PID_Incremental_Remove(ctrl_attr *attr, ctrl_interface_id_t id)
{
    struct ctrl_pid_incremental_base_t *pidIncrementalBase = CTRL_Interface_Get_Data_By_Id(attr, id);
    if (pidIncrementalBase == ctrl_nullptr)
    {
        return ctrl_false;
    }

    if (CTRL_Memory_Free(pidIncrementalBase) != 0)
    {
        return ctrl_false;
    }

    if (CTRL_Memory_Free(pidIncrementalBase->pidIncremental) != 0)
    {
        return ctrl_false;
    }

    if (CTRL_Interface_Remove_By_Id(attr, id) != ctrl_true)
    {
        return ctrl_false;
    }
    return ctrl_true;
}

ctrl_bool CTRL_PID_Incremental_Config(ctrl_attr *attr, ctrl_interface_id_t id, enum ctrl_pid_incremental_config_e configChoose, void *valuePtr)
{
    struct ctrl_pid_incremental_base_t *pidIncrementalBase = (struct ctrl_pid_incremental_base_t *)CTRL_Interface_Get_Data_By_Id(attr, id);
    if (pidIncrementalBase == NULL)
    {
        return ctrl_false;
    }

    switch (pidIncrementalBase->accuracy)
    {
    case _CTRL_INTERFACE_ACCURACY_INT:
    case _CTRL_INTERFACE_ACCURACY_FLOAT:
        CTRL_PID_Incremental_Set_Config_By_PidIncrementalBase_Tuint32_Self(pidIncrementalBase, configChoose, CTRL_Number_To_NumberAny32bit_(valuePtr));
        break;

    default:
        break;
    }

    return ctrl_true;
}

ctrl_bool CTRL_PID_Incremental_Set(ctrl_attr *attr, ctrl_interface_id_t id, enum ctrl_pid_incremental_e choose, void *valuePtr)
{
    struct ctrl_pid_incremental_base_t *pidIncrementalBase = (struct ctrl_pid_incremental_base_t *)CTRL_Interface_Get_Data_By_Id(attr, id);
    if (pidIncrementalBase == NULL)
    {
        return ctrl_false;
    }

    switch (pidIncrementalBase->accuracy)
    {
    case _CTRL_INTERFACE_ACCURACY_INT:
    case _CTRL_INTERFACE_ACCURACY_FLOAT:
        CTRL_PID_Incremental_Set_Value_By_PidIncrementalBase_Tuint32_Self(pidIncrementalBase, choose, CTRL_Number_To_NumberAny32bit_(valuePtr));
        break;

    default:
        break;
    }

    return ctrl_true;
}

ctrl_bool CTRL_PID_Incremental_Calc(ctrl_attr *attr, ctrl_interface_id_t id, void *result)
{
    struct ctrl_pid_incremental_base_t *pidIncrementalBase = (struct ctrl_pid_incremental_base_t *)CTRL_Interface_Get_Data_By_Id(attr, id);
    if (pidIncrementalBase == NULL)
    {
        return ctrl_false;
    }

    enum ctrl_pid_incremental_calc_result_e calcResult;

    switch (pidIncrementalBase->accuracy)
    {
    case _CTRL_INTERFACE_ACCURACY_INT:
    {
        ctrl_int32_t calcReturn = ctrl_none;
        calcResult = CTRL_PID_Incremental_Calc_Before_By_PidIncremental_Tuint32_Int32_Self(pidIncrementalBase->pidIncremental, &calcReturn);
        if (calcResult == _CTRL_PID_INCREMENTAL_CALC_RESULT_CONTINUE)
            calcResult = CTRL_PID_Incremental_Calc_By_PidIncremental_Tuint32_Int32_Self(pidIncrementalBase->pidIncremental, &calcReturn);
        if (calcResult == _CTRL_PID_INCREMENTAL_CALC_RESULT_CONTINUE)
            calcResult = CTRL_PID_Incremental_Calc_After_By_PidIncremental_Tuint32_Int32_Self(pidIncrementalBase->pidIncremental, &calcReturn);
        if (result != ctrl_nullptr)
            *(ctrl_int32_t *)result = calcReturn;
        break;
    }
    case _CTRL_INTERFACE_ACCURACY_FLOAT:
    {
        ctrl_float_t calcReturn = ctrl_none;
        calcResult = CTRL_PID_Incremental_Calc_Before_By_PidIncremental_Tuint32_Float_Self(pidIncrementalBase->pidIncremental, &calcReturn);
        if (calcResult == _CTRL_PID_INCREMENTAL_CALC_RESULT_CONTINUE)
            calcResult = CTRL_PID_Incremental_Calc_By_PidIncremental_Tuint32_Float_Self(pidIncrementalBase->pidIncremental, &calcReturn);
        if (calcResult == _CTRL_PID_INCREMENTAL_CALC_RESULT_CONTINUE)
            calcResult = CTRL_PID_Incremental_Calc_After_By_PidIncremental_Tuint32_Float_Self(pidIncrementalBase->pidIncremental, &calcReturn);
        if (result != ctrl_nullptr)
            *(ctrl_float_t *)result = calcReturn;
        break;
    }

    default:
        break;
    }

    return ctrl_true;
}
