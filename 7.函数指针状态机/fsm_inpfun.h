#ifndef _FSM_INPFUN_H
#define _FSM_INPFUN_H_

#include <stdint.h>
#include <stddef.h>
    
typedef struct
{
    uint8_t u8StateID;
    void (*pfOnEntry)(void);
    void (*pfOnRun)(void);
    void (*pfOnExit)(void);
} FsmState_Type;

typedef struct
{
    FsmState_Type *ptCurrState;
    FsmState_Type *ptNextState;
} FsmManage_Type;


#endif