#include "ctrl_cache.h"

struct ctrl_cache_result_t CTRL_Cache_Read_FastData(struct ctrl_cache_base_t *cacheBase)
{
    struct ctrl_cache_result_t result = {false, ctrl_nullptr};
    if (cacheBase->fastData == ctrl_nullptr)
    {
        return result;
    }
    result.data = cacheBase->fastData;
    result.success = true;
    return result;
}

ctrl_bool CTRL_Cache_Write_FastData(struct ctrl_cache_base_t *cacheBase, void *data)
{
    cacheBase->fastData = data;
    return true;
}

ctrl_bool CTRL_Cache_Clear_FastData(struct ctrl_cache_base_t *cacheBase)
{
    cacheBase->fastData = ctrl_nullptr;
    return ctrl_true;
}
