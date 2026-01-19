#ifndef __ANIME_TASK_H
#define __ANIME_TASK_H

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* 动画模式枚举 */
typedef enum
{
    ANIME_MODE_ADB_OFF = 0,
    ANIME_MODE_ADB_ON,
    ANIME_MODE_ADB_WEL,
    ANIME_MODE_ADB_FRW,
    ANIME_MODE_MAX
} Anime_Mode_t;

/*功能裁剪*/
// #define ANIME_TASK_RELOAD_ENABLE        /*动画重载功能*/
// #define ANIME_TASK_DELAY_ENABLE         /*动画延时功能*/

/* 任务时间相关定义 */
#define ANIME_TASK_TIMERCNT         (20U)      /* 任务轮询时间(ms) */
#define ANIME_TASK_TIMEREXPIERED    (ANIME_TASK_TIMERCNT)      /* 任务超时执行时间(ms) */
#define ANIME_TASK_ARR_LEN          (24)      /* 动画单帧数组长度 */

/* 返回值定义 */
#define ANIME_RET_OK                (0U)
#define ANIME_RET_ERR_PARAM         (1U)
#define ANIME_RET_ERR_MODE          (2U)

/*****************************************************************************/
/* 函数指针类型定义 */
typedef void (*Anime_ArrExecute_Func)(uint16_t const *inputarr);

/* 目标动画信息结构体 */
typedef struct
{
    uint16_t u16target_delay;
    uint16_t u16target_steps;
    uint16_t const (*pu16target_arr)[ANIME_TASK_ARR_LEN];  /* 修正为uint16_t类型 */
    uint8_t  u8target_mode;
    uint8_t  u8target_reload_times;
} Anime_Target_Info_t;

typedef struct
{
    uint16_t u16run_delaycnt;
    uint16_t u16run_step_index;
    uint16_t u16run_timercnt;
    uint8_t  u8run_last_mode;
    uint8_t  u8run_curr_mode;
    uint8_t  u8run_reload_cnt;
}Anime_Running_Info_t;

typedef struct
{
    Anime_Mode_t tinput_mode;
    Anime_ArrExecute_Func tinput_func;
    bool  bfeedback_runtoend;
}Anime_Input_Info_t;

/*********************************************************************************/
extern Anime_Input_Info_t tAnime_Info_In;
extern const Anime_Target_Info_t tAnime_Target_Info[ANIME_MODE_MAX];

// /**
//  * @brief 动画数组执行函数(用户需要实现)
//  * @param[in] input_arr 输入的动画数组指针
//  * @return 无
//  */
// extern void Anime_Arr_Execute(uint16_t const *input_arr);

/**
 * @brief 初始化动画输入信息
 * @param[out] ptinput_info 输入信息结构体指针
 * @param[in]  mode 动画模式
 * @param[in]  func 动画执行回调函数
 * @return uint8_t ANIME_RET_OK:成功, ANIME_RET_ERR_PARAM:参数错误
 */
extern uint8_t Anime_InputInfo_Init(Anime_Input_Info_t *ptinput_info, uint8_t mode, void (*func)(uint16_t const *));

/**
 * @brief 动画任务运行主函数(使用内部静态运行信息)
 * @param[in,out] ptinput_info 输入信息结构体指针
 * @param[in]     ptarget_info 目标动画信息数组指针
 * @return 无
 */
extern void Anime_Task_Run(Anime_Input_Info_t *ptinput_info, Anime_Target_Info_t const *ptarget_info);

#endif
