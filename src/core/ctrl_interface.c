#include "ctrl_interface.h"
#include "ctrl_memory.h"

struct ctrl_interface_t *CTRL_Interface_Find_Interface_By_Id_Self(struct ctrl_interface_base_t *interfaceBase, ctrl_interface_id_t id);
ctrl_bool CTRL_Interface_Push_In_List_Back_Tail__Self(struct ctrl_interface_base_t *interfaceBase, struct ctrl_interface_t *interface);

struct ctrl_interface_t *CTRL_Interface_Find_Interface_By_Id_Self(struct ctrl_interface_base_t *interfaceBase, ctrl_interface_id_t id)
{
    struct ctrl_interface_t *interface = interfaceBase->interfaceListHeader;
    while (interface != ctrl_nullptr)
    {
        if (interface->id == id)
        {
            return interface;
        }
        interface = interface->next;
    }
    return ctrl_nullptr;
}

ctrl_bool CTRL_Interface_Push_In_List_Back_Tail__Self(struct ctrl_interface_base_t *interfaceBase, struct ctrl_interface_t *interface)
{
    interface->prev = interfaceBase->interfaceListTail;

    if (interfaceBase->interfaceListTail != ctrl_nullptr)
        interfaceBase->interfaceListTail->next = interface;
    else
        interfaceBase->interfaceListHeader = interface;

    interfaceBase->interfaceListTail = interface;

    return ctrl_true;
}

struct ctrl_interface_result_t CTRL_Interface_Create_Interface(struct ctrl_interface_base_t *interfaceBase, enum ctrl_interface_accuracy_e accuracy)
{
    struct ctrl_interface_result_t result = {ctrl_false, ctrl_none};

    struct ctrl_interface_t *interface = (struct ctrl_interface_t *)CTRL_Memory_Malloc(sizeof(struct ctrl_interface_t));
    if (interface == ctrl_nullptr)
    {
        return result;
    }
    // struct ctrl_node_result_t nodeResult = CTRL_Node_Save_Data(&interfaceBase->nodeBase, (void *)interface, ctrl_true);
    // if (nodeResult.success == ctrl_false)
    // {
    //     CTRL_Memory_Free(interface);
    //     return result;
    // }

    interface->id = interfaceBase->interfaceIndex;
    interface->findId = 0;
    // interface->nodeId = nodeResult.nodeId;
    // interface->nodeOffset = nodeResult.nodeOffset;
    interface->nodeId = ctrl_none;
    interface->nodeOffset = ctrl_none;
    interface->accuracy = accuracy;
    interface->next = ctrl_nullptr;
    interface->prev = ctrl_nullptr;
    CTRL_Interface_Push_In_List_Back_Tail__Self(interfaceBase, interface);
    interfaceBase->interfaceIndex += 1;

    result.success = ctrl_true;
    result.id = interface->id;
    result.accuracy = interface->accuracy;
    return result;
}

ctrl_bool CTRL_Interface_Set_FindId_By_Id(struct ctrl_interface_base_t *interfaceBase, ctrl_interface_id_t id, ctrl_interface_id_t findId)
{
    struct ctrl_interface_t *interface = ctrl_nullptr;
    if (interfaceBase->interfaceConfig.isUseCache == true)
    {
        struct ctrl_cache_result_t cacheResult = CTRL_Cache_Read_FastData(interfaceBase->interfaceConfig.cacheBase);
        if (cacheResult.success == ctrl_true)
            interface = (struct ctrl_interface_t *)cacheResult.data;
        if (cacheResult.success != ctrl_true || interface->id != id)
            interface = CTRL_Interface_Find_Interface_By_Id_Self(interfaceBase, id);
    }
    else
    {
        interface = CTRL_Interface_Find_Interface_By_Id_Self(interfaceBase, id);
    }
    if (interface == ctrl_nullptr)
    {
        return ctrl_false;
    }

    interface->findId = findId;

    return ctrl_true;
}

ctrl_bool CTRL_Interface_Set_Data_By_Id(struct ctrl_interface_base_t *interfaceBase, ctrl_interface_id_t id, void *data)
{
    struct ctrl_interface_t *interface = ctrl_nullptr;
    if (interfaceBase->interfaceConfig.isUseCache == true)
    {
        struct ctrl_cache_result_t cacheResult = CTRL_Cache_Read_FastData(interfaceBase->interfaceConfig.cacheBase);
        if (cacheResult.success == ctrl_true)
            interface = (struct ctrl_interface_t *)cacheResult.data;
        if (cacheResult.success != ctrl_true || interface->id != id)
            interface = CTRL_Interface_Find_Interface_By_Id_Self(interfaceBase, id);
    }
    else
    {
        interface = CTRL_Interface_Find_Interface_By_Id_Self(interfaceBase, id);
    }
    if (interface == ctrl_nullptr)
    {
        return ctrl_false;
    }

    struct ctrl_node_result_t nodeResult = CTRL_Node_Save_Data(&interfaceBase->nodeBase, data, ctrl_true);

    if (nodeResult.success == ctrl_true)
    {
        interface->nodeId = nodeResult.nodeId;
        interface->nodeOffset = nodeResult.nodeOffset;
    }

    return ctrl_true;
}

void *CTRL_Interface_Get_Data_By_Id(struct ctrl_interface_base_t *interfaceBase, ctrl_interface_id_t id)
{
    struct ctrl_interface_t *interface = ctrl_nullptr;
    if (interfaceBase->interfaceConfig.isUseCache == true)
    {
        struct ctrl_cache_result_t cacheResult = CTRL_Cache_Read_FastData(interfaceBase->interfaceConfig.cacheBase);
        if (cacheResult.success == ctrl_true)
            interface = (struct ctrl_interface_t *)cacheResult.data;
        if (cacheResult.success != ctrl_true || interface->id != id)
            interface = CTRL_Interface_Find_Interface_By_Id_Self(interfaceBase, id);
    }
    else
    {
        interface = CTRL_Interface_Find_Interface_By_Id_Self(interfaceBase, id);
    }

    return CTRL_Node_Read_Data(&interfaceBase->nodeBase, interface->nodeId, interface->nodeOffset);
}

void *CTRL_Interface_Get_Data_By_FindId(struct ctrl_interface_base_t *interfaceBase, ctrl_interface_id_t findId, ctrl_memory_long_t *start)
{
    struct ctrl_interface_t *interface = interfaceBase->interfaceListHeader;
    if (start != ctrl_nullptr && *start != ctrl_none)
    {
        interface = ((struct ctrl_interface_t *)(*start))->next;
    }
    while (interface != ctrl_nullptr)
    {
        if (interface->findId == findId)
        {
            if (start != ctrl_nullptr)
            {
                *start = (ctrl_memory_long_t)interface;
            }
            return CTRL_Node_Read_Data(&interfaceBase->nodeBase, interface->nodeId, interface->nodeOffset);
        }
        interface = interface->next;
    }

    return ctrl_nullptr;
}

ctrl_bool CTRL_Interface_Remove_By_Id(struct ctrl_interface_base_t *interfaceBase, ctrl_interface_id_t id)
{
    struct ctrl_interface_t *interface = ctrl_nullptr;
    if (interfaceBase->interfaceConfig.isUseCache == true)
    {
        struct ctrl_cache_result_t cacheResult = CTRL_Cache_Read_FastData(interfaceBase->interfaceConfig.cacheBase);
        if (cacheResult.success == ctrl_true)
        {
            interface = (struct ctrl_interface_t *)cacheResult.data;
            CTRL_Cache_Clear_FastData(interfaceBase->interfaceConfig.cacheBase);
        }
        if (cacheResult.success != ctrl_true || interface->id != id)
            interface = CTRL_Interface_Find_Interface_By_Id_Self(interfaceBase, id);
    }
    else
    {
        interface = CTRL_Interface_Find_Interface_By_Id_Self(interfaceBase, id);
    }
    if (interface == ctrl_nullptr)
    {
        return ctrl_false;
    }
    return ctrl_true;
}

ctrl_bool CTRL_Interface_Clear_Data_By_Id(struct ctrl_interface_base_t *interfaceBase, ctrl_interface_id_t id)
{
    struct ctrl_interface_t *interface = ctrl_nullptr;
    if (interfaceBase->interfaceConfig.isUseCache == true)
    {
        struct ctrl_cache_result_t cacheResult = CTRL_Cache_Read_FastData(interfaceBase->interfaceConfig.cacheBase);
        if (cacheResult.success == ctrl_true)
            interface = (struct ctrl_interface_t *)cacheResult.data;
        if (cacheResult.success != ctrl_true || interface->id != id)
            interface = CTRL_Interface_Find_Interface_By_Id_Self(interfaceBase, id);
    }
    else
    {
        interface = CTRL_Interface_Find_Interface_By_Id_Self(interfaceBase, id);
    }
    if (interface == ctrl_nullptr)
    {
        return ctrl_false;
    }

    interface->nodeId = ctrl_none;
    interface->nodeOffset = ctrl_none;

    if (CTRL_Node_Remove_Data(&interfaceBase->nodeBase, interface->nodeId, interface->nodeOffset) != ctrl_true)
    {
        return ctrl_false;
    }

    if (CTRL_Memory_Free(interface) != 0)
    {
        return ctrl_false;
    }

    return ctrl_true;
}

ctrl_bool CTRL_Interface_Open_Cache(struct ctrl_interface_base_t *interfaceBase)
{
    interfaceBase->interfaceConfig.isUseCache = true;
    if (interfaceBase->interfaceConfig.cacheBase == ctrl_nullptr)
    {
        interfaceBase->interfaceConfig.cacheBase = (struct ctrl_cache_base_t *)CTRL_Memory_Malloc(sizeof(struct ctrl_cache_base_t));
        if (interfaceBase->interfaceConfig.cacheBase == ctrl_nullptr)
        {
            interfaceBase->interfaceConfig.isUseCache = false;
            return ctrl_false;
        }
    }
    CTRL_Cache_Init_CacheBase_(interfaceBase->interfaceConfig.cacheBase);
    return ctrl_true;
}

ctrl_bool CTRL_Interface_Close_Cache(struct ctrl_interface_base_t *interfaceBase)
{
    interfaceBase->interfaceConfig.isUseCache = false;
    return ctrl_true;
}

ctrl_bool CTRL_Interface_Destroy_Cache(struct ctrl_interface_base_t *interfaceBase)
{
    interfaceBase->interfaceConfig.isUseCache = false;
    if (interfaceBase->interfaceConfig.cacheBase != ctrl_nullptr)
    {
        if (CTRL_Memory_Free(interfaceBase->interfaceConfig.cacheBase) != 0)
        {
            return ctrl_false;
        }
    }
    return ctrl_true;
}
