#include "fsm_inpfun.h"

void Fsm_Init(FsmManage_Type *ptFsm, FsmState_Type *ptInit_State)
{
    if (ptFsm == NULL || ptInit_State == NULL)
    {
    }
    else
    {
        ptFsm->ptCurrState = ptInit_State;
        ptFsm->ptNextState = NULL;

        if ((ptFsm->ptCurrState != NULL) && (ptFsm->ptCurrState->pfOnEntry != NULL))
        {
            ptFsm->ptCurrState->pfOnEntry();
        }

    }
}

void Fsm_Run(FsmManage_Type *ptFsm)
{
    if (ptFsm == NULL)
    {
    }
    else
    {
        if (ptFsm->ptNextState != NULL)
        {
            if ((ptFsm->ptCurrState != NULL) && (ptFsm->ptCurrState->pfOnExit != NULL))
            {
                ptFsm->ptCurrState->pfOnExit();
            }

            ptFsm->ptCurrState = ptFsm->ptNextState;
            ptFsm->ptNextState = NULL;

            if ((ptFsm->ptCurrState != NULL) && (ptFsm->ptCurrState->pfOnEntry != NULL))
            {
                ptFsm->ptCurrState->pfOnEntry();
            }
        }

        if ((ptFsm->ptCurrState != NULL) && (ptFsm->ptCurrState->pfOnRun != NULL))
        {
            ptFsm->ptCurrState->pfOnRun();
        }
    }
}

void Fsm_Transition(FsmManage_Type *ptFsm, FsmState_Type *ptNext_State)
{
    if (ptFsm == NULL || ptNext_State == NULL)
    {
    }
    else
    {
        if (ptNext_State == ptFsm->ptCurrState)
        {
        }
        else
        {
            ptFsm->ptNextState = ptNext_State;
        }
    }
}


#if 0

/* Test Code */

static FsmManage_Type g_tFsm_Led;

extern FsmState_Type g_tStateA;
extern FsmState_Type g_tStateB;

/*********************************** */
static void StateA_OnEntry(void)
{
    RunWater();
}
static void StateA_OnRun(void)
{
    if (IsWaterEnd)
    {
        All_On;
    }
}

static void StateA_OnExit(void)
{
    StopWater();
}

FsmState_Type g_tStateA =
{
    .u8StateID = 0,
    .pfOnEntry = StateA_OnEntry,
    .pfOnRun = StateA_OnRun,
    .pfOnExit = StateA_OnExit,
};
/*********************************** */
static void StateB_OnEntry(void)
{
    All_Off();
}
static void StateB_OnRun(void)
{
    if (IsButtonPressed)
    {
        Fsm_Transition(&g_tFsm_Led, &g_tStateA);
    }
}

FsmState_Type g_tStateB =
{
    .u8StateID = 1,
    .pfOnEntry = StateB_OnEntry,
    .pfOnRun = StateB_OnRun,
    .pfOnExit = NULL,
};
/*********************************** */
void app_init(void)
{
    Fsm_Init(&g_tFsm_Led, &g_tStateA);
}

void app_loop(void)
{
    Fsm_Run(&g_tFsm_Led);
}

#endif