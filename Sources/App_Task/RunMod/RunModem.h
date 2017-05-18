// RunModem.h
#ifndef RUNMODEM_H
#define RUNMODEM_H

#include "Ecu.h" 
#include "EcuConf.h"
#include "GeneralTypeDef.h"

typedef enum
{
    RUNMODE_DEFAULT,       // 0
    RUNMODE_BMS_INIT,      // 1
    RUMMODE_BATSYS_INIT,   // 2
    RUNMODE_SYS_WAIT,      // 3
    RUNMODE_HV_ENABLE,     // 4
    RUNMODE_HV_OUTPUT,     // 5
    RUNMODE_FAULT,         // 6          
    RUNMODE_DIAG,          // 7
    RUNMODE_CALB,          // 8
    RUNMODE_SLOW_CHG,      // 9
    RUNMODE_FAST_CHG,      // 10
    RUNMODE_SELF_ACTIV,     // 11
    RUNMODE_SHUT_DOWN,      // 12
    RUNMODE_SYS_MTCE,       // 13
    RUNMODE_FACTORY,        // 14
    RUNMODE_GATWAY         // 15
}eRunMode_ModeType;

// For runmode 0x45C0~0x45DF
#define ERR_RUNMODE_NOT_ALLOWED   0x45C0 // ERR_MANAGE_RUNMODE_BASE

void RumMode_Init(void);
GeneralReturnType RunModem_SwitchModeTo(eRunMode_ModeType eMode);

#endif /* RUNMODEM_H */