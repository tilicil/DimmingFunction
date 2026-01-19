#ifndef _FSM_INTABLE_
#define _FSM_INTABLE_

#include <stdint.h>
#include <stddef.h>

typedef enum
{
    FSM_STATE_IDLE,
    FSM_STATE_WAIT_OK,
    FSM_STATE_WAIT_DATA,
    FSM_STATE_ERROR,
    FSM_STATE_MAX
} FsmState_Type;

typedef enum
{
    FSM_EVENT_START,
    FSM_EVENT_OK_RECEIVED,
    FSM_EVENT_DATA_RECEIVED,
    FSM_EVENT_ERROR_OCCURRED,
    FSM_EVENT_MAX
} FsmEvent_Type;

typedef struct
{
    FsmState_Type tCurrState;
    FsmEvent_Type tEvent;
    FsmState_Type tNextState;
    void (*pfAction)(void);
} FsmTransition_Type;

#endif