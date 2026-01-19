#include "object_pool.h"

void ObjectPool_Init(PoolManage_Type *ptPool, void *pPoolMemory, uint32_t uBlockSize, uint32_t uTotalBlocks)
{
    POOLASSERT(ptPool != NULL);
    POOLASSERT(pPoolMemory != NULL);
    POOLASSERT(uBlockSize >= sizeof(PoolBlock_Type));
    POOLASSERT(uTotalBlocks > 0);
    
    uint8_t *pblockend;

    uBlockSize = (uBlockSize + 3) & ~3;
    
    ptPool->pPoolMemory = (uint8_t *)pPoolMemory;
    ptPool->uBlockSize = uBlockSize;
    ptPool->uTotalBlocks = uTotalBlocks;

    ptPool->ptFreeList = NULL;
    pblockend = ptPool->pPoolMemory + ((uBlockSize - 1) * uTotalBlocks);
    for (uint8_t i = 0; i < uTotalBlocks; i++)
    {
        PoolBlock_Type *pblock = (PoolBlock_Type *)pblockend;
        pblock->ptNext = ptPool->ptFreeList;
        ptPool->ptFreeList = pblock;
        pblockend -= uBlockSize;
    }
}

void *ObjectPool_Allocate(PoolManage_Type *ptPool)
{
    POOLASSERT(ptPool != NULL);
    
    POOLENTER_CRITICAL();

    if (ptPool->ptFreeList == NULL)
    {
        return NULL;
    }

    PoolBlock_Type *pblock = ptPool->ptFreeList;
#ifdef USED_BLOCK_MAGIC
    pblock->u32Magic = BLOCK_MAGIC_USED;
#endif
    ptPool->ptFreeList = pblock->ptNext;

    POOLEXIT_CRITICAL();

    return (void *)pblock;
}

void ObjectPool_Free(PoolManage_Type *ptPool, void *pBlock)
{
    POOLASSERT(ptPool != NULL);
    POOLASSERT(pBlock != NULL);
#ifdef USED_BLOCK_MAGIC
    POOLASSERT(((PoolBlock_Type *)pBlock)->u32Magic == BLOCK_MAGIC_USED);
#endif

    POOLENTER_CRITICAL();

    PoolBlock_Type *pblock = (PoolBlock_Type *)pBlock;
#ifdef USED_BLOCK_MAGIC
    pblock->u32Magic = BLOCK_MAGIC_FREE;
#endif
    pblock->ptNext = ptPool->ptFreeList;
    ptPool->ptFreeList = pblock;

    POOLEXIT_CRITICAL();
}