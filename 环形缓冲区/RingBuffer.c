/*
 * @Author: guangnan.li
 * @Date: 2025-12-17 15:50:03
 * @Version: V1.0
 * @LastEditors: guangnan.li
 * @LastEditTime: 2025-12-17 16:02:04
 * @FilePath: \环形缓冲区\RingBuffer.c
 * @Description: 
 * 
 * Copyright (c) 2025 by ${NanNingLiaoWang Ltd.}, All Rights Reserved. 
 */
#include "RingBuffer.h"


/**
 * @breif: 清零初始化
 * @note: 
 * @param [RingBufType] *pRingBuf
 * @return [*]
 */
void RingBuffer_Init(RingBufType *pRingBuf)
{
    pRingBuf->WriteIndex = 0;
    pRingBuf->ReadIndex = 0;
    memset(pRingBuf->Ringbuf, 0, RING_BUFFER_SIZE);
}

/**
 * @breif: 数据写入缓冲区
 * @note: 注意写入写出大小一致，未做安全性检查以及边界处理
 * @param [in] *pRingBuf
 * @param [in] *data
 * @param [in] size
 * @return [*]
 */
void RingBuffer_Write(RingBufType *pRingBuf, uint8_t *data, uint16_t size)
{
    uint16_t write = pRingBuf->WriteIndex;
    uint16_t read  = pRingBuf->ReadIndex;
    uint16_t next = (write + size) & RING_BUFFER_MASK;

    if (next == read)
    {
        /* 覆盖旧数据 */
        read = (read + size) & RING_BUFFER_MASK;
        pRingBuf->ReadIndex = read;
    }

    memcpy(&pRingBuf->Ringbuf[write], data, size);
    pRingBuf->WriteIndex = next;
}

/**
 * @breif: 从缓冲区读取数据
 * @note: 注意写入写出大小一致，未做安全性检查以及边界处理
 * @param [in] *pRingBuf
 * @param [in] *data
 * @param [in] size
 * @return [uint8_t] 0:缓冲区为空  1: 读取成功
 */
uint8_t RingBuffer_Read(RingBufType *pRingBuf, uint8_t *data, uint16_t size)
{
    uint8_t res = 1U;
    uint16_t write = pRingBuf->WriteIndex;
    uint16_t read = pRingBuf->ReadIndex;

    if (write == read)
    {
        res = 0U; /* 缓冲区为空 */
    }

    if (res)
    {
        memcpy(data, &pRingBuf->Ringbuf[read], size);
        read = (read + size) & RING_BUFFER_MASK;
        pRingBuf->ReadIndex = read;
    }

    return res;
}

/**
 * @breif: 获取缓冲区数据大小
 * @note: 
 * @param [RingBufType] *pRingBuf
 * @return [*]
 */
uint16_t RingBuffer_GetDateSize(RingBufType *pRingBuf)
{
    uint16_t write = pRingBuf->WriteIndex;
    uint16_t read = pRingBuf->ReadIndex;
    uint16_t size = 0U;

    if (write >= read)
    {
        size = write - read;
    }
    else
    {
        size = (RING_BUFFER_SIZE - read) + write;
    }

    return size;
}

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