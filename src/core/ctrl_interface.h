#ifndef __CTRL_INTERFACE_H
#define __CTRL_INTERFACE_H

#include "../ctrl_define.h"
#include "ctrl_node.h"
#include "ctrl_cache.h"

#if CTRL_INTERFACE_TYPE_SIZE_BIT == 64
#define ctrl_interface_long_t ctrl_uint64_t
#define ctrl_interface_id_t ctrl_uint32_t
#elif CTRL_INTERFACE_TYPE_SIZE_BIT == 32
#define ctrl_interface_long_t ctrl_uint32_t
#define ctrl_interface_id_t ctrl_uint16_t
#elif CTRL_INTERFACE_TYPE_SIZE_BIT == 16
#define ctrl_interface_long_t ctrl_uint16_t
#define ctrl_interface_id_t ctrl_uint8_t
#else
#error "CTRL_INTERFACE_TYPE_SIZE_BIT is error, only support 16, 32, 64"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    enum ctrl_interface_accuracy_e
    {
        _CTRL_INTERFACE_ACCURACY_NONE = 0,
        _CTRL_INTERFACE_ACCURACY_AUTO,
        _CTRL_INTERFACE_ACCURACY_INT,
        _CTRL_INTERFACE_ACCURACY_LONG_INT,
        _CTRL_INTERFACE_ACCURACY_FLOAT,
        _CTRL_INTERFACE_ACCURACY_DOUBLE,
        _CTRL_INTERFACE_ACCURACY_LONG_DOUBLE,
        _CTRL_INTERFACE_ACCURACY_THREE_FLOAT_LONG_INT
    };

    struct ctrl_interface_t
    {
        ctrl_interface_id_t id;
        ctrl_interface_id_t findId;
        enum ctrl_interface_accuracy_e accuracy;
        ctrl_node_id_t nodeId;
        ctrl_node_long_t nodeOffset;
        struct ctrl_interface_t *next;
        struct ctrl_interface_t *prev;
    };

    struct ctrl_interface_config_t
    {
        ctrl_bool isUseCache;
        struct ctrl_cache_base_t *cacheBase;
    };

    struct ctrl_interface_base_t
    {
        ctrl_interface_id_t interfaceIndex;
        struct ctrl_interface_t *interfaceListHeader;
        struct ctrl_interface_t *interfaceListTail;
        struct ctrl_interface_config_t interfaceConfig;
        struct ctrl_node_base_t nodeBase;
    };

    struct ctrl_interface_result_t
    {
        ctrl_bool success;
        ctrl_interface_id_t id;
        enum ctrl_interface_accuracy_e accuracy;
    };

    struct ctrl_interface_result_t CTRL_Interface_Create_Interface(struct ctrl_interface_base_t *interfaceBase, enum ctrl_interface_accuracy_e accuracy);
    ctrl_bool CTRL_Interface_Set_FindId_By_Id(struct ctrl_interface_base_t *interfaceBase, ctrl_interface_id_t id, ctrl_interface_id_t findId);
    ctrl_bool CTRL_Interface_Set_Data_By_Id(struct ctrl_interface_base_t *interfaceBase, ctrl_interface_id_t id, void *data);
    void *CTRL_Interface_Get_Data_By_Id(struct ctrl_interface_base_t *interfaceBase, ctrl_interface_id_t id);
    void *CTRL_Interface_Get_Data_By_FindId(struct ctrl_interface_base_t *interfaceBase, ctrl_interface_id_t findId, ctrl_memory_long_t *start);
    ctrl_bool CTRL_Interface_Remove_By_Id(struct ctrl_interface_base_t *interfaceBase, ctrl_interface_id_t id);

    ctrl_bool CTRL_Interface_Clear_Data_By_Id(struct ctrl_interface_base_t *interfaceBase, ctrl_interface_id_t id);
    ctrl_bool CTRL_Interface_Open_Cache(struct ctrl_interface_base_t *interfaceBase);
    ctrl_bool CTRL_Interface_Close_Cache(struct ctrl_interface_base_t *interfaceBase);
    ctrl_bool CTRL_Interface_Destroy_Cache(struct ctrl_interface_base_t *interfaceBase);

/* ======================================================================
 * 宏定义函数
 * ====================================================================== */
#define CTRL_Interface_Init_InterfaceBase_(interfaceBase) CTRL_Interface_Init_InterfaceBase(interfaceBase)
    static inline ctrl_bool CTRL_Interface_Init_InterfaceBase(struct ctrl_interface_base_t *interfaceBase)
    {
        interfaceBase->interfaceIndex = ctrl_none;
        interfaceBase->interfaceListHeader = ctrl_nullptr;
        interfaceBase->interfaceListTail = ctrl_nullptr;
        interfaceBase->interfaceConfig.isUseCache = ctrl_false;
        CTRL_Node_Init_NodeBase_(&interfaceBase->nodeBase);
        return ctrl_true;
    }

/* ======================================================================
 * 类型重命名
 * ====================================================================== */
#define CTRL_Init_(ctrl_attr_) CTRL_Interface_Init_InterfaceBase_(ctrl_attr_)
#define ctrl_attr_ ctrl_attr
    typedef struct ctrl_interface_base_t ctrl_attr;

#ifdef __cplusplus
}
#endif

#endif
