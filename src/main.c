#include "ctrl_include.h"
#include <windows.h>
#include <stdio.h>

ctrl_attr_ ctrlAttr;

int main()
{

    CTRL_Init_(&ctrlAttr);

    ctrl_interface_id_t pidId = CTRL_PID_Incremental_Create(&ctrlAttr, _CTRL_INTERFACE_ACCURACY_INT, ctrl_nullptr);
    CTRL_PID_Incremental_Init(&ctrlAttr, pidId);
    ctrl_interface_id_t pidId2 = CTRL_PID_Incremental_Create(&ctrlAttr, _CTRL_INTERFACE_ACCURACY_FLOAT, ctrl_nullptr);
    CTRL_PID_Incremental_Init(&ctrlAttr, pidId2);

    ctrl_memory_long_t address = ctrl_none;

    CTRL_Interface_Get_Data_By_FindId(&ctrlAttr, CTRL_PID_INCREMENTA_ID, &address);
    CTRL_Interface_Get_Data_By_FindId(&ctrlAttr, CTRL_PID_INCREMENTA_ID, &address);

    float p = 1;
    CTRL_PID_Incremental_Config(&ctrlAttr, pidId2, _CTRL_PID_INCREMENTAL_CONFIG_KP, &p);
    CTRL_PID_Incremental_Config(&ctrlAttr, pidId2, _CTRL_PID_INCREMENTAL_CONFIG_KI, &p);
    CTRL_PID_Incremental_Config(&ctrlAttr, pidId2, _CTRL_PID_INCREMENTAL_CONFIG_KD, &p);
    CTRL_PID_Incremental_Set(&ctrlAttr, pidId2, _CTRL_PID_INCREMENTAL_AIM_VALUE, &p);

    // CTRL_PID_Incremental_Set(&ctrlAttr, pidId2, _CTRL_PID_INCREMENTAL_CURRENT_VALUE, &p);

    float result = 0;
    CTRL_PID_Incremental_Calc(&ctrlAttr, pidId2, &result);
    CTRL_PID_Incremental_Calc(&ctrlAttr, pidId2, &result);
    CTRL_PID_Incremental_Calc(&ctrlAttr, pidId2, &result);
    CTRL_PID_Incremental_Calc(&ctrlAttr, pidId2, &result);
    CTRL_PID_Incremental_Calc(&ctrlAttr, pidId2, &result);
    CTRL_PID_Incremental_Calc(&ctrlAttr, pidId2, &result);
    CTRL_PID_Incremental_Calc(&ctrlAttr, pidId2, &result);
    CTRL_PID_Incremental_Calc(&ctrlAttr, pidId2, &result);
    CTRL_PID_Incremental_Calc(&ctrlAttr, pidId2, &result);
    CTRL_PID_Incremental_Calc(&ctrlAttr, pidId2, &result);
    CTRL_PID_Incremental_Calc(&ctrlAttr, pidId2, &result);
    CTRL_PID_Incremental_Calc(&ctrlAttr, pidId2, &result);
    CTRL_PID_Incremental_Calc(&ctrlAttr, pidId2, &result);

    float aimvalue = 0;
    float error = -10;
    float add = -1;

    // PID_Position_CalcResult_NowTureValueIncrementalByNowTureValueAndSpeed(&pidMultipleTest, 0, 0, 0, 0);

    while (true)
    {
        ////printf("input aimvalue:");
        ////scanf("%d", &aimvalue);
        ////printf("input error:");
        ////scanf("%f", &error);
        // if (error > 0) {
        //	error = -error;
        // }
        // else {
        //	error = -error;
        // }
        // PID_SetAndSubmit_AimValueAndValue_Interface(&pidMultipleTest, PID_POSITION, error, aimvalue, PID_NOW_VALUE, NULL);
        add += rand() / 1000000.0f;
        error = -add;

        float result = 0;
        CTRL_PID_Incremental_Set(&ctrlAttr, pidId2, _CTRL_PID_INCREMENTAL_AIM_VALUE, &aimvalue);
        CTRL_PID_Incremental_Set(&ctrlAttr, pidId2, _CTRL_PID_INCREMENTAL_CURRENT_VALUE, &error);
        CTRL_PID_Incremental_Calc(&ctrlAttr, pidId2, &result);
        // float result = PID_Incremental_CalcResult_ByNowTureValue(&pidMultipleTest, error, aimvalue);
        printf("result:%f\n", result);

        if (add > 30)
        {
            add = -1;
        }

        // if (result > 0) {
        //	result = -result;
        // }
    }

    // printf("pidId:%d pidId2:%d result:%f\n", pidId, pidId2, result);

    // ctrl_any_t a;
    // float b = 1.1;

    // float* c = (void *)&a;
    // *c = b;

    return 0;
}