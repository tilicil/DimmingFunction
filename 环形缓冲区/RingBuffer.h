#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

#include "stdint.h"
#include "string.h"

#ifndef RING_BUFFER_SIZE
#define RING_BUFFER_SIZE 32   /*必须是2的幂次方，方便优化*/
#endif

#ifndef RING_BUFFER_MASK
#define RING_BUFFER_MASK (RING_BUFFER_SIZE - 1)
#endif

typedef struct {
    volatile uint16_t WriteIndex;
    volatile uint16_t ReadIndex;
    uint8_t Ringbuf[RING_BUFFER_SIZE];
} RingBufType;

/**
 * @breif: 清零初始化
 * @note: 
 * @param [RingBufType] *pRingBuf
 * @return [*]
 */
void RingBuffer_Init(RingBufType *pRingBuf);

/**
 * @breif: 数据写入缓冲区,数据满时覆盖已有数据
 * @note: 注意写入写出大小一致，未做安全性检查以及边界处理
 * @param [in] *pRingBuf
 * @param [in] *data
 * @param [in] size
 * @return [*]
 */
void RingBuffer_Write(RingBufType *pRingBuf, uint8_t *data, uint16_t size);

/**
 * @breif: 从缓冲区读取数据
 * @note: 注意写入写出大小一致，未做安全性检查以及边界处理
 * @param [in] *pRingBuf
 * @param [in] *data
 * @param [in] size
 * @return [uint8_t] 0:缓冲区为空  1: 读取成功
 */
uint8_t RingBuffer_Read(RingBufType *pRingBuf, uint8_t *data, uint16_t size);

/**
 * @breif: 获取缓冲区数据大小
 * @note: 
 * @param [RingBufType] *pRingBuf
 * @return [*]
 */
uint16_t RingBuffer_GetDateSize(RingBufType *pRingBuf);

/**
 * @breif: 清空缓冲区
 * @note: 
 * @param [RingBufType] *pRingBuf
 * @return [*]
 */
void RingBuffer_Clear(RingBufType *pRingBuf)
{
    pRingBuf->WriteIndex = pRingBuf->ReadIndex;
}

#endif 