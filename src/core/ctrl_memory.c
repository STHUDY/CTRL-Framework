#include "ctrl_memory.h"

#if CTRL_MEMORY_HANDLE_STYLE == 0

struct MEMORY_NODE
{
    void *ptr;
    ctrl_memory_long_t use_size;
    struct MEMORY_NODE *next;
};

const ctrl_memory_byte_t __Memory_Node_Size = sizeof(struct MEMORY_NODE);
static ctrl_memory_long_t __Use_Heap_Size = 0;
static ctrl_memory_byte_t __Heap_Size_Map[CTRL_MAX_MEMORY_SIZE];
static struct MEMORY_NODE *__Head_Node = ctrl_nullptr;
static struct MEMORY_NODE *__End_Node = ctrl_nullptr;

ctrl_memory_byte_t Add_Node(struct MEMORY_NODE *node)
{
    if (__Head_Node == ctrl_nullptr)
    {
        __Head_Node = node;
        __End_Node = node;
    }
    else
    {
        __End_Node->next = node;
        __End_Node = node;
    }
    return 0;
}

ctrl_memory_byte_t *Malloc_GetCanArea(ctrl_memory_long_t size)
{
    struct MEMORY_NODE *node = __Head_Node;
    ctrl_memory_byte_t *start = __Heap_Size_Map;
    ctrl_memory_byte_t *result = ctrl_nullptr;
    ctrl_memory_long_t minDistense = CTRL_MAX_MEMORY_SIZE;

    while (node != ctrl_nullptr)
    {
        ctrl_memory_byte_t *node_end = (ctrl_memory_byte_t *)node + node->use_size + __Memory_Node_Size;
        if (node_end <= start)
        {
            node = node->next;
            continue;
        }

        ctrl_memory_long_t distense = node_end - start;
        if (distense >= size && distense < minDistense)
        {
            minDistense = distense;
            result = start;
        }

        start = node_end;
        node = node->next;
    }

    if (result == ctrl_nullptr && start + size <= __Heap_Size_Map + CTRL_MAX_MEMORY_SIZE)
    {
        result = start;
    }

    return result;
}

void *CTRL_Memory_Malloc(ctrl_memory_long_t size)
{
    if (__Use_Heap_Size + size + __Memory_Node_Size > CTRL_MAX_MEMORY_SIZE)
    {
        return ctrl_nullptr;
    }

    ctrl_memory_byte_t *ptr = Malloc_GetCanArea(size + __Memory_Node_Size);
    if (ptr == ctrl_nullptr)
    {
        return ctrl_nullptr;
    }

    struct MEMORY_NODE *newNode = (struct MEMORY_NODE *)ptr;
    void *result = ptr + __Memory_Node_Size;

    newNode->ptr = result;
    newNode->use_size = size;
    newNode->next = ctrl_nullptr;

    Add_Node(newNode);
    __Use_Heap_Size += size + __Memory_Node_Size;

    return result;
}

ctrl_memory_byte_t CTRL_Memory_Free(void *ptr)
{
    struct MEMORY_NODE *node = __Head_Node;
    struct MEMORY_NODE **prevNext = &__Head_Node;

    while (node != ctrl_nullptr && node->ptr != ptr)
    {
        prevNext = &(node->next);
        node = node->next;
    }

    if (node == ctrl_nullptr)
    {
        return 1; // Ptr not found
    }

    *prevNext = node->next;
    if (node == __End_Node)
    {
        __End_Node = node->next ? node->next : __Head_Node;
    }

    __Use_Heap_Size -= node->use_size + __Memory_Node_Size;
    return 0;
}

ctrl_memory_byte_t CTRL_Memory_Clear(void *ptr, ctrl_memory_long_t size)
{
    struct MEMORY_NODE *node = __Head_Node;

    while (node != ctrl_nullptr && node->ptr != ptr)
    {
        node = node->next;
    }

    if (node == ctrl_nullptr)
    {
        return 1; // Memory block not found
    }

    // Manually clear the memory block
    ctrl_memory_byte_t *tmp = (ctrl_memory_byte_t *)node->ptr;
    ctrl_memory_long_t i = 0;

    if (node->use_size != size)
    {
        return 1;
    }

    for (i = 0; i < node->use_size; i++)
    {
        tmp[i] = 0;
    }

    return 0;
}

#elif CTRL_MEMORY_HANDLE_STYLE == 1
void *CTRL_Memory_Malloc(ctrl_memory_long_t size)
{
    return malloc(size);
}

ctrl_memory_byte_t CTRL_Memory_Free(void *ptr)
{
    free(ptr);
    return 0;
}

ctrl_memory_byte_t CTRL_Memory_Clear(void *ptr, ctrl_memory_long_t size)
{
    if (ptr == ctrl_nullptr)
    {
        return 1;
    }

    for (ctrl_memory_long_t i = 0; i < size; i++)
    {
        ((ctrl_memory_byte_t *)ptr)[i] = 0;
    }

    return 1;
}

#elif CTRL_MEMORY_HANDLE_STYLE == 2

// 请在这里写自定义内存申请函数

void *CTRL_Memory_Malloc(ctrl_memory_long_t size)
{
    return ctrl_nullptr;
}

ctrl_memory_byte_t CTRL_Memory_Free(void *ptr)
{
    return 0;
}

ctrl_memory_byte_t CTRL_Memory_Clear(void *ptr, ctrl_memory_long_t size)
{
    return 0;
}

#else
#error "CTRL_MEMORY_HANDLE_STYLE must be 0, 1 or 2"
#endif