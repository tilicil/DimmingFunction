/*
 * @Author: guangnan.li
 * @Date: 2024-05-30 10:15:03
 * @Version: V1.0
 * @LastEditors: guangnan.li
 * @LastEditTime: 2025-12-17 15:03:46
 * @FilePath: \动画运行任务函数\animetask.c
 * @Description: 
 * 
 * Copyright (c) 2024 by ${NanNingLiaoWang Ltd.}, All Rights Reserved. 
 */

#include "animetask.h"

/* 
 * @note 单帧全灭,用于ADB关闭状态
 */
static uint16_t const Anime_ADB_Off_Arr[1][ANIME_TASK_ARR_LEN] = 
{
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

/* 
 * @note 单帧全亮,用于ADB开启状态
 */
static uint16_t const Anime_ADB_On_Arr[1][ANIME_TASK_ARR_LEN] = 
{
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

/* 
 * @note 
 */
static uint16_t const Anime_ADB_Wel_Arr[51][ANIME_TASK_ARR_LEN] = 
{
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

/* 
 * @note 
 */
static uint16_t const Anime_ADB_Frw_Arr[51][ANIME_TASK_ARR_LEN] = 
{
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

/* 目标动画信息表 */
Anime_Target_Info_t const tAnime_Target_Info[ANIME_MODE_MAX] = 
{
    {
        .u8target_mode = ANIME_MODE_ADB_OFF,
        .u16target_delay = 0U,
        .u16target_steps = 1U,
        .u8target_reload_times = 0U,
        .pu16target_arr = Anime_ADB_Off_Arr,
    },
    {
        .u8target_mode = ANIME_MODE_ADB_ON,
        .u16target_delay = 0U,
        .u16target_steps = 1U,
        .u8target_reload_times = 0U,
        .pu16target_arr = Anime_ADB_On_Arr,    
    },
    {
        .u8target_mode = ANIME_MODE_ADB_WEL,
        .u16target_delay = 0U,
        .u16target_steps = 51U,
        .u8target_reload_times = 0U,
        .pu16target_arr = Anime_ADB_Wel_Arr,
    },
    {
        .u8target_mode = ANIME_MODE_ADB_FRW,
        .u16target_delay = 0U,
        .u16target_steps = 51U,
        .u8target_reload_times = 0U,
        .pu16target_arr = Anime_ADB_Frw_Arr,
    },
};
Anime_Input_Info_t tAnime_Info_In;
static Anime_Running_Info_t tAnime_Running_Info = {0U};  /* 动画运行信息(内部静态变量) */

// /**
//  * @brief 动画数组执行函数(用户需要实现)
//  * @param[in] input_arr 输入的动画数组指针
//  * @return 无
//  */
// void Anime_Arr_Execute(uint16_t const *input_arr)
// {
//     /* 用户需要在此实现具体的动画执行逻辑 */
//     (void)input_arr;  /* 避免未使用参数警告 */
// }

/**
 * @brief 初始化动画输入信息
 * @param[out] ptinput_info 输入信息结构体指针
 * @param[in]  mode 动画模式
 * @param[in]  func 动画执行回调函数
 * @return uint8_t ANIME_RET_OK:成功, ANIME_RET_ERR_PARAM:参数错误
 */
uint8_t Anime_InputInfo_Init(Anime_Input_Info_t *ptinput_info, uint8_t mode, void (*func)(uint16_t const *))
{
    uint8_t res;
    
    if (ptinput_info == NULL)
    {
        res = ANIME_RET_ERR_PARAM;
    }
    else
    {
        ptinput_info->tinput_mode = (Anime_Mode_t)mode;
        ptinput_info->bfeedback_runtoend = true;
        
        if (func != NULL)
        {
            ptinput_info->tinput_func = func;
            res = ANIME_RET_OK;
        }
        else
        {
            res = ANIME_RET_ERR_PARAM;
        }
    }

    return res;
}

/**
 * @brief 动画任务运行主函数(使用内部静态运行信息)
 * @param[in,out] ptinput_info 输入信息结构体指针
 * @param[in]     ptarget_info 目标动画信息数组指针
 * @return 无
 */
void Anime_Task_Run(Anime_Input_Info_t *ptinput_info, Anime_Target_Info_t const *ptarget_info)
{
    bool process_pass = false;
    uint8_t i = 0U;
    uint8_t matched_index = 0U;  /* 保存匹配的索引 */

    /* 指针校验 */
    if ((ptinput_info != NULL) && (ptarget_info != NULL))
    {
        /* 检查输入动画模式是否匹配 */
        for (i = 0U; i < (uint8_t)ANIME_MODE_MAX; i++)
        {
            if ((uint8_t)ptinput_info->tinput_mode == ptarget_info[i].u8target_mode)
            {
                tAnime_Running_Info.u8run_curr_mode = (uint8_t)ptinput_info->tinput_mode;  /* 传递输入模式 */
                tAnime_Running_Info.u8run_reload_cnt = 0U;
                matched_index = i;  /* 保存匹配的索引 */
                process_pass = true;
                break;
            }
        }
    }

    if (process_pass)
    {
        tAnime_Running_Info.u16run_timercnt += ANIME_TASK_TIMERCNT;  /* 定时器计数累加 */

        /* 模式变化,新状态打断旧状态 */
        if (tAnime_Running_Info.u8run_last_mode != tAnime_Running_Info.u8run_curr_mode)
        {
            tAnime_Running_Info.u16run_step_index = 0U;   /* 重置动画索引 */
            tAnime_Running_Info.u16run_delaycnt = 0U;     /* 重置延时 */
            tAnime_Running_Info.u8run_last_mode = tAnime_Running_Info.u8run_curr_mode;
            ptinput_info->bfeedback_runtoend = false;
        }

#ifdef ANIME_TASK_DELAY_ENABLE
        tAnime_Running_Info.u16run_delaycnt += ANIME_TASK_TIMERCNT;

        if (tAnime_Running_Info.u16run_delaycnt < ptarget_info[matched_index].u16target_delay)
        {
            /* 延时未到，不执行动画 */
        }
        else
#endif
        {   /* 达到对应延时时间 */
            tAnime_Running_Info.u16run_delaycnt = ptarget_info[matched_index].u16target_delay;  /* 保持延时到达时间 */
            
            /* 检查函数指针有效性后再调用 */
            if (ptinput_info->tinput_func != NULL)
            {
                ptinput_info->tinput_func(ptarget_info[matched_index].pu16target_arr[tAnime_Running_Info.u16run_step_index]);  /* 执行动画数组 */
            }
            
            /* 定时器超时进入 */
            if (tAnime_Running_Info.u16run_timercnt >= ANIME_TASK_TIMEREXPIERED)
            {
                tAnime_Running_Info.u16run_timercnt -= ANIME_TASK_TIMEREXPIERED;
                
                /* 动画未结束 */
                if (tAnime_Running_Info.u16run_step_index < (ptarget_info[matched_index].u16target_steps - 1U))
                {
                    tAnime_Running_Info.u16run_step_index++;  /* 动画步骤累加 */
                    ptinput_info->bfeedback_runtoend = false;
                }
#ifdef ANIME_TASK_RELOAD_ENABLE
                else if (tAnime_Running_Info.u8run_reload_cnt < ptarget_info[matched_index].u8target_reload_times)
                {
                    tAnime_Running_Info.u8run_reload_cnt++;
                    tAnime_Running_Info.u16run_step_index = 0U;  /* 动画结束进入循环 */
                    ptinput_info->bfeedback_runtoend = false;
                }
#endif
                else
                {
                    tAnime_Running_Info.u8run_reload_cnt = ptarget_info[matched_index].u8target_reload_times;
                    tAnime_Running_Info.u16run_step_index = ptarget_info[matched_index].u16target_steps - 1U;  /* 动画结束保持最后状态 */
                    ptinput_info->bfeedback_runtoend = true;
                }
            }
        }
    }
}