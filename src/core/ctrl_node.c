#include "ctrl_node.h"

struct ctrl_node_t *CTRL_Node_Create_Node__Self(struct ctrl_node_base_t *nodeBase, void *value);
#if CTRL_NODE_CAN_REUSE == 1
struct ctrl_node_t *CTRL_Node_Find_Delete_Node_In_List__Self(struct ctrl_node_base_t *nodeBase);
#endif
ctrl_bool CTRL_Node_Free_Node__Self(struct ctrl_node_base_t *nodeBase, struct ctrl_node_t *node);
ctrl_bool CTRL_Node_Push_In_List_Front_Header__Self(struct ctrl_node_base_t *nodeBase, struct ctrl_node_t *node);
ctrl_bool CTRL_Node_Push_In_List_Back_Tail__Self(struct ctrl_node_base_t *nodeBase, struct ctrl_node_t *node);
ctrl_bool CTRL_Node_Remove_In_List__Self(struct ctrl_node_base_t *nodeBase, struct ctrl_node_t *node);

struct ctrl_node_t *CTRL_Node_Create_Node__Self(struct ctrl_node_base_t *nodeBase, void *value)
{
    struct ctrl_node_t *node = (struct ctrl_node_t *)CTRL_Memory_Malloc(sizeof(struct ctrl_node_t));
    if (node == ctrl_nullptr)
    {
        return ctrl_nullptr;
    }

    if (nodeBase->baseAddress == ctrl_none)
    {
        nodeBase->baseAddress = (ctrl_memory_long_t)node;
        node->offset = 0;
    }
    else
    {
        node->offset = (ctrl_memory_long_t)node - nodeBase->baseAddress;
    }

    node->id = nodeBase->nodeIndex;
    node->value = value;
#if CTRL_NODE_CAN_REUSE == 1
    node->delete = ctrl_false;
#endif
    node->link = ctrl_false;
    node->next = ctrl_nullptr;
    node->prev = ctrl_nullptr;

    ++nodeBase->nodeIndex;
    return node;
}
#if CTRL_NODE_CAN_REUSE == 1
struct ctrl_node_t *CTRL_Node_Find_Delete_Node_In_List__Self(struct ctrl_node_base_t *nodeBase)
{
    struct ctrl_node_t *node = nodeBase->nodeListHeader;
    while (node != ctrl_nullptr)
    {
        if (node->delete == ctrl_true)
        {
            return node;
        }
        node = node->next;
    }

    return ctrl_nullptr;
}

#endif

ctrl_bool CTRL_Node_Free_Node__Self(struct ctrl_node_base_t *nodeBase, struct ctrl_node_t *node)
{
    if (node->link == true)
    {
        CTRL_Node_Remove_In_List__Self(nodeBase, node);
    }
    if (CTRL_Memory_Free(node) == 1)
    {
        return ctrl_false;
    }
    return ctrl_true;
}

ctrl_bool CTRL_Node_Push_In_List_Front_Header__Self(struct ctrl_node_base_t *nodeBase, struct ctrl_node_t *node)
{
    node->next = nodeBase->nodeListHeader;

    if (nodeBase->nodeListHeader != ctrl_nullptr)
        nodeBase->nodeListHeader->prev = node;
    else
        nodeBase->nodeListTail = node;

    nodeBase->nodeListHeader = node;

    ++nodeBase->nodeListCount;
    node->link = true;
    return ctrl_true;
}

ctrl_bool CTRL_Node_Push_In_List_Back_Tail__Self(struct ctrl_node_base_t *nodeBase, struct ctrl_node_t *node)
{
    node->prev = nodeBase->nodeListTail;

    if (nodeBase->nodeListTail != ctrl_nullptr)
        nodeBase->nodeListTail->next = node;
    else
        nodeBase->nodeListHeader = node;

    nodeBase->nodeListTail = node;

    ++nodeBase->nodeListCount;
    node->link = true;
    return ctrl_true;
}

ctrl_bool CTRL_Node_Remove_In_List__Self(struct ctrl_node_base_t *nodeBase, struct ctrl_node_t *node)
{
    if (!node || !node->link)
        return ctrl_false;

    if (node->prev)
        node->prev->next = node->next;
    else
        nodeBase->nodeListHeader = node->next;

    if (node->next)
        node->next->prev = node->prev;
    else
        nodeBase->nodeListTail = node->prev;

    node->next = NULL;
    node->prev = NULL;
    node->link = false;

    if (nodeBase->nodeListCount > 0)
        --nodeBase->nodeListCount;

    return ctrl_true;
}

struct ctrl_node_result_t CTRL_Node_Save_Data(struct ctrl_node_base_t *nodeBase, void *data, ctrl_bool isNeedManage)
{
    struct ctrl_node_result_t result;

#if CTRL_NODE_CAN_REUSE == 1

    if (isNeedManage)
    {
        if (nodeBase->nodeListDeleteCount != 0)
        {
            struct ctrl_node_t *deleteNode = CTRL_Node_Find_Delete_Node_In_List__Self(nodeBase);
            if (deleteNode != ctrl_nullptr)
            {
                deleteNode->value = data;
                result.success = ctrl_true;
                result.nodeId = deleteNode->id;
                result.nodeOffset = deleteNode->offset;
                deleteNode->delete = ctrl_false;
                --nodeBase->nodeListDeleteCount;
                return result;
            }
            else
            {
                nodeBase->nodeListDeleteCount = 0;
            }
        }
    }

#endif

    struct ctrl_node_t *tmpNode = CTRL_Node_Create_Node__Self(nodeBase, data);

    if (tmpNode == ctrl_nullptr)
    {
        result.success = ctrl_false;
        return result;
    }

    result.success = ctrl_true;
    result.nodeId = tmpNode->id;
    result.nodeOffset = tmpNode->offset;

    if (isNeedManage)
    {
        CTRL_Node_Push_In_List_Back_Tail__Self(nodeBase, tmpNode);
    }

    return result;
}

void *CTRL_Node_Read_Data(struct ctrl_node_base_t *nodeBase, ctrl_node_id_t nodeId, ctrl_node_long_t nodeOffset)
{
    struct ctrl_node_t *node = (struct ctrl_node_t *)(nodeBase->baseAddress + nodeOffset);
#if CTRL_NODE_CAN_REUSE == 1
    if (node->delete == true)
    {
        return ctrl_nullptr;
    }
#endif
    return node->value;
}

ctrl_bool CTRL_Node_Remove_Data_By_Id(struct ctrl_node_base_t *nodeBase, ctrl_node_id_t nodeId)
{
    struct ctrl_node_t *node = nodeBase->nodeListHeader;
    while (node != ctrl_nullptr)
    {
        if (node->id == nodeId)
        {
#if CTRL_NODE_CAN_REUSE == 1
            node->delete = ctrl_true;
            ++nodeBase->nodeListDeleteCount;
#else
            CTRL_Node_Free_Node__Self(nodeBase, node);
#endif
            return ctrl_true;
        }
        node = node->next;
    }

    return ctrl_false;
}

ctrl_bool CTRL_Node_Remove_Data_By_Offset(struct ctrl_node_base_t *nodeBase, ctrl_node_long_t nodeOffset)
{
    struct ctrl_node_t *node = nodeBase->nodeListHeader;
    while (node != ctrl_nullptr)
    {
        if (node->offset == nodeOffset)
        {
#if CTRL_NODE_CAN_REUSE == 1
            node->delete = ctrl_true;
            ++nodeBase->nodeListDeleteCount;
#else
            CTRL_Node_Free_Node__Self(nodeBase, node);
#endif
            return ctrl_true;
        }
        node = node->next;
    }

    return ctrl_false;
}

ctrl_bool CTRL_Node_Remove_Data(struct ctrl_node_base_t *nodeBase, ctrl_node_id_t nodeId, ctrl_node_long_t nodeOffset)
{
    struct ctrl_node_t *node = (struct ctrl_node_t *)(nodeBase->baseAddress + nodeOffset);
#if CTRL_NODE_CAN_REUSE == 1
    node->delete = ctrl_true;
    ++nodeBase->nodeListDeleteCount;
    return ctrl_true;
#else
    return CTRL_Node_Free_Node__Self(nodeBase, node);
#endif
}

ctrl_bool CTRL_Node_Remove_All_Data(struct ctrl_node_base_t *nodeBase)
{
    struct ctrl_node_t *node = NULL;
    while (node != ctrl_nullptr)
    {
#if CTRL_NODE_CAN_REUSE == 1
        node->delete = ctrl_true;
        ++nodeBase->nodeListDeleteCount;
#else
        CTRL_Node_Free_Node__Self(nodeBase, node);
#endif
        node = node->next;
    }
    return ctrl_true;
}

ctrl_bool CTRL_Node_Reset(struct ctrl_node_base_t *nodeBase)
{
    struct ctrl_node_t *node = NULL;
    while (node != ctrl_nullptr)
    {
        CTRL_Node_Free_Node__Self(nodeBase, node);
    }
    nodeBase->baseAddress = ctrl_none;
    nodeBase->nodeIndex = 0;

    nodeBase->nodeListHeader = ctrl_nullptr;
    nodeBase->nodeListTail = ctrl_nullptr;
    nodeBase->nodeListCount = 0;
#if CTRL_NODE_CAN_REUSE == 1
    nodeBase->nodeListDeleteCount = 0;
#endif
    return ctrl_true;
}
