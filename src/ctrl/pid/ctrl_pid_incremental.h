#ifndef __CTRL_PID_INCREMENTAL_H
#define __CTRL_PID_INCREMENTAL_H

#define CTRL_PID_INCREMENTA_ID 1
#define CTRL_PID_INCREMENTA_VERSION "1.0.0"

#include "../../core/ctrl_interface.h"
#include "../../core/ctrl_number.h"

#ifdef __cplusplus
extern "C"
{
#endif

    enum ctrl_pid_incremental_e
    {
        _CTRL_PID_INCREMENTAL_AIM_VALUE = 0,
        _CTRL_PID_INCREMENTAL_CURRENT_VALUE,
        _CTRL_PID_INCREMENTAL_SAMPLE_TICKS,
    };

    enum ctrl_pid_incremental_callback_e
    {
        _CTRL_PID_INCREMENTAL_CALLBACK_BEFORE_CALC = 0,
        _CTRL_PID_INCREMENTAL_CALLBACK_CALC,
        _CTRL_PID_INCREMENTAL_CALLBACK_AFTER_CALC,
    };

    enum ctrl_pid_incremental_calc_e
    {
        _CTRL_PID_INCREMENTAL_CALC_NONE = 0,
        _CTRL_PID_INCREMENTAL_CALC_DISCRET,
        _CTRL_PID_INCREMENTAL_CALC_CONTINUOUS
    };

    enum ctrl_pid_incremental_config_e
    {
        _CTRL_PID_INCREMENTAL_CONFIG_KP = 0,
        _CTRL_PID_INCREMENTAL_CONFIG_KI,
        _CTRL_PID_INCREMENTAL_CONFIG_KD,
    };

    ctrl_bool CTRL_PID_Incremental_Load_DefalutConfig(ctrl_attr *attr, ctrl_interface_id_t id);
    ctrl_interface_id_t CTRL_PID_Incremental_Create(ctrl_attr *attr, enum ctrl_interface_accuracy_e accuracy, ctrl_bool *isSuccess);
    ctrl_bool CTRL_PID_Incremental_Init(ctrl_attr *attr, ctrl_interface_id_t id);
    ctrl_bool CTRL_PID_Incremental_Remove(ctrl_attr *attr, ctrl_interface_id_t id);

    ctrl_bool CTRL_PID_Incremental_Config(ctrl_attr *attr, ctrl_interface_id_t id, enum ctrl_pid_incremental_config_e configChoose, void *valuePtr);
    ctrl_bool CTRL_PID_Incremental_Set(ctrl_attr *attr, ctrl_interface_id_t id, enum ctrl_pid_incremental_e choose, void *valuePtr);

    ctrl_bool CTRL_PID_Incremental_Calc(ctrl_attr *attr, ctrl_interface_id_t id, void *result);

#ifdef __cplusplus
}
#endif

#endif
