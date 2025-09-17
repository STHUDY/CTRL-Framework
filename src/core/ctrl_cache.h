#ifndef __CTRL_CACHE_H
#define __CTRL_CACHE_H

#include "../ctrl_define.h"
#include "ctrl_node.h"

#ifdef __cplusplus
extern "C"
{
#endif

    struct ctrl_cache_base_t
    {
        void *fastData;
    };

    struct ctrl_cache_result_t
    {
        ctrl_bool success;
        void *data;
    };

    struct ctrl_cache_result_t CTRL_Cache_Read_FastData(struct ctrl_cache_base_t *cacheBase);

    ctrl_bool CTRL_Cache_Write_FastData(struct ctrl_cache_base_t *cacheBase, void *data);

    ctrl_bool CTRL_Cache_Clear_FastData(struct ctrl_cache_base_t *cacheBase);

/* ======================================================================
 * 宏定义函数
 * ====================================================================== */
#define CTRL_Cache_Init_CacheBase_(cacheBase) CTRL_Cache_Init_CacheBase(cacheBase)
    static inline ctrl_bool CTRL_Cache_Init_CacheBase(struct ctrl_cache_base_t *cacheBase)
    {
        cacheBase->fastData = ctrl_nullptr;
        return ctrl_true;
    }

#ifdef __cplusplus
}
#endif

#endif
