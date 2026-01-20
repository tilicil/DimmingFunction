#ifndef _OBJECT_POOL_H_
#define _OBJECT_POOL_H_

#include <stdint.h>
#include <stddef.h>

#define USED_BLOCK_MAGIC         /*使用魔数检查内存块状态，不允许连续释放两次*/

#ifdef USED_BLOCK_MAGIC

#define BLOCK_MAGIC_FREE 0xDEADBEEF
#define BLOCK_MAGIC_USED 0xCAFEBABE
typedef struct PoolBlock_Type
{
    uint32_t u32Magic;             /* Magic number for debugging */
    struct PoolBlock_Type *ptNext; /*4Byte in 32bitMcu*/
} PoolBlock_Type;

#else

typedef struct PoolBlock_Type
{
    struct PoolBlock_Type *ptNext; /*4Byte in 32bitMcu*/
} PoolBlock_Type;

#endif

typedef struct
{
    PoolBlock_Type *ptFreeList; /* Pointer to the first free block */
    uint32_t uBlockSize;        /* Size of each block */
    uint32_t uTotalBlocks;      /* Total number of blocks */
    uint8_t *pPoolMemory;       /* Pointer to the pool memory */
} PoolManage_Type;

#define POOLASSERT(x) \
    if ((x) == 0)     \
    {                 \
        for (;;)      \
            ;         \
    }

#define POOLENTER_CRITICAL()   /* Implement platform-specific critical section entry */
#define POOLEXIT_CRITICAL()    /* Implement platform-specific critical section exit */


/************************************************************************************************************* */
void ObjectPool_Init(PoolManage_Type *ptPool, void *pPoolMemory, uint32_t uBlockSize, uint32_t uTotalBlocks);
void *ObjectPool_Allocate(PoolManage_Type *ptPool);
void ObjectPool_Free(PoolManage_Type *ptPool, void *pBlock);


#endif