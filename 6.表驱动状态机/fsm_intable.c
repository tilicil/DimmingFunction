#include "fsm_intable.h"

static void Action_SendCmd(void);
static void Action_ProcessData(void);
static void Action_HandleError(void);
static void Action_Reset(void);

#if 0
static const FsmTransition_Type tFsm_Table[] = {
    /*当前状态       +      触发事件    =   下个状态     +  执行动作*/
    {FSM_STATE_IDLE, FSM_EVENT_START, FSM_STATE_WAIT_OK, Action_SendCmd},
    {FSM_STATE_WAIT_OK, FSM_EVENT_OK_RECEIVED, FSM_STATE_WAIT_DATA, Action_Reset},
    {FSM_STATE_WAIT_DATA, FSM_EVENT_DATA_RECEIVED, FSM_STATE_IDLE, Action_ProcessData},
    {FSM_STATE_WAIT_DATA, FSM_EVENT_ERROR_OCCURRED, FSM_STATE_ERROR, Action_HandleError},
    {FSM_STATE_ERROR, FSM_EVENT_START, FSM_STATE_WAIT_OK, Action_SendCmd},
};

#define FSM_TABLE_SIZE (sizeof(tFsm_Table) / sizeof(FsmTransition_Type))

/**************************************************************************** */
static FsmState_Type g_CurrState = FSM_STATE_IDLE;

void Fsm_HandleEvent(FsmEvent_Type tEvent)
{
    for (uint8_t i = 0; i < FSM_TABLE_SIZE; i++)
    {
        if (tFsm_Table[i].tCurrState == g_CurrState && 
            tFsm_Table[i].tEvent == tEvent)
        {
            g_CurrState = tFsm_Table[i].tNextState;
            if (tFsm_Table[i].pfAction != NULL)
            {
                tFsm_Table[i].pfAction();
            }
            return;
        }
    }
    /* No valid transition found, remain in current state */
}

FsmState_Type Get_CurrentState(void)
{
    return g_CurrState;
}

#else

/*table[当前状态][事件] = 下一状态*/
static const uint8_t Fsm_State_Table[FSM_STATE_MAX][FSM_EVENT_MAX] =
{

    [FSM_STATE_IDLE] = {FSM_STATE_WAIT_OK, FSM_STATE_IDLE, FSM_STATE_IDLE, FSM_STATE_IDLE},
    [FSM_STATE_WAIT_OK] = {FSM_STATE_WAIT_OK, FSM_STATE_WAIT_DATA, FSM_STATE_WAIT_OK, FSM_STATE_WAIT_OK},
    [FSM_STATE_WAIT_DATA] = {FSM_STATE_WAIT_DATA, FSM_STATE_WAIT_DATA, FSM_STATE_IDLE, FSM_STATE_ERROR},
    [FSM_STATE_ERROR] = {FSM_STATE_WAIT_OK, FSM_STATE_ERROR, FSM_STATE_ERROR, FSM_STATE_ERROR},
};

/*action_table[当前状态][事件] = 下一个动作*/
static void (* const Fsm_Action_Table[FSM_STATE_MAX][FSM_EVENT_MAX])(void) = 
{
    [FSM_STATE_IDLE] = {Action_SendCmd, NULL, NULL, NULL},
    [FSM_STATE_WAIT_OK] = {NULL, Action_Reset, NULL, NULL},
    [FSM_STATE_WAIT_DATA] = {NULL, NULL, Action_ProcessData, Action_HandleError},
    [FSM_STATE_ERROR] = {Action_SendCmd, NULL, NULL, NULL},
};

/**************************************************************************** */
static FsmState_Type g_CurrState = FSM_STATE_IDLE;
void Fsm_HandleEvent(FsmEvent_Type tEvent)
{
    if (tEvent >= FSM_EVENT_MAX)
    {
    }
    else
    {
        FsmState_Type tNextState = Fsm_State_Table[g_CurrState][tEvent];
        void (*pfAction)(void) = Fsm_Action_Table[g_CurrState][tEvent];

        g_CurrState = tNextState;

        if (pfAction != NULL)
        {
            pfAction();
        }
    }
}

FsmState_Type Get_CurrentState(void)
{
    return g_CurrState;
}

#endif