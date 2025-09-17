#ifndef __CTRL_NODE_H
#define __CTRL_NODE_H

#include "../ctrl_define.h"
#include "ctrl_memory.h"

#if CTRL_NODE_TYPE_SIZE_BIT == 64
#define ctrl_node_long_t ctrl_uint64_t
#define ctrl_node_id_t ctrl_uint64_t
#elif CTRL_NODE_TYPE_SIZE_BIT == 32
#define ctrl_node_long_t ctrl_uint32_t
#define ctrl_node_id_t ctrl_uint32_t
#elif CTRL_NODE_TYPE_SIZE_BIT == 16
#define ctrl_node_long_t ctrl_uint16_t
#define ctrl_node_id_t ctrl_uint16_t
#else
#error "CTRL_NODE_TYPE_SIZE_BIT is error, only support 16, 32, 64"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    struct ctrl_node_t
    {
        ctrl_node_id_t id;
        ctrl_node_long_t offset;
        void *value;
#if CTRL_NODE_CAN_REUSE == 1
        ctrl_bool delete;
#endif
        ctrl_bool link;
        struct ctrl_node_t *next;
        struct ctrl_node_t *prev;
    };

    struct ctrl_node_base_t
    {
        ctrl_memory_long_t baseAddress;
        ctrl_node_id_t nodeIndex;

        struct ctrl_node_t *nodeListHeader;
        struct ctrl_node_t *nodeListTail;
        ctrl_node_long_t nodeListCount;
#if CTRL_NODE_CAN_REUSE == 1
        ctrl_node_long_t nodeListDeleteCount;
#endif
    };

    struct ctrl_node_result_t
    {
        ctrl_bool success;
        ctrl_node_id_t nodeId;
        ctrl_node_long_t nodeOffset;
    };

    struct ctrl_node_result_t CTRL_Node_Save_Data(struct ctrl_node_base_t *nodeBase, void *data, ctrl_bool isNeedManage);

    void *CTRL_Node_Read_Data(struct ctrl_node_base_t *nodeBase, ctrl_node_id_t nodeId, ctrl_node_long_t nodeOffset);

    ctrl_bool CTRL_Node_Remove_Data_By_Id(struct ctrl_node_base_t *nodeBase, ctrl_node_id_t nodeId);

    ctrl_bool CTRL_Node_Remove_Data_By_Offset(struct ctrl_node_base_t *nodeBase, ctrl_node_long_t nodeOffset);

    ctrl_bool CTRL_Node_Remove_Data(struct ctrl_node_base_t *nodeBase, ctrl_node_id_t nodeId, ctrl_node_long_t nodeOffset);

    ctrl_bool CTRL_Node_Remove_All_Data(struct ctrl_node_base_t *nodeBase);

    ctrl_bool CTRL_Node_Reset(struct ctrl_node_base_t *nodeBase);

/* ======================================================================
 * 宏定义函数
 * ====================================================================== */
#define CTRL_Node_Init_NodeBase_(nodeBase) CTRL_Node_Init_NodeBase(nodeBase)
    static inline ctrl_bool CTRL_Node_Init_NodeBase(struct ctrl_node_base_t *nodeBase)
    {
        nodeBase->baseAddress = ctrl_none;
        nodeBase->nodeIndex = ctrl_none;
        nodeBase->nodeListHeader = ctrl_nullptr;
        nodeBase->nodeListTail = ctrl_nullptr;
        nodeBase->nodeListCount = ctrl_none;
#if CTRL_NODE_CAN_REUSE == 1
        nodeBase->nodeListDeleteCount = ctrl_none;
#endif
        return ctrl_true;
    }

#ifdef __cplusplus
}
#endif

#endif
