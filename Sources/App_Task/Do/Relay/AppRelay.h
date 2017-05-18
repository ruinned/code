// AppRelay.h
#ifndef APPRELAY_H
#define APPRELAY_H

#include "GeneralTypeDef.h"
#include "Ecu.h"
                                         

typedef enum 
{
   IMMED_RELAY_CMD,
   DELAY_RELAY_CMD,
   TEST_RELAY_CMD,
}eActor_RelayCmdType;

typedef struct
{
  Bool Bcmd; 
  eActor_RelayCmdType CmdType; //relay command type ,include immediate cotrol,delay control and relay test
  uLWord DuratonTime; // 1ms unit
  uLWord Frequence; // 1Hz unit
  uByte Dute;  // 1% unit
}sActor_CtrlRelayCmdType;
#define RELAY_CLOSE_CMD TRUE
#define RELAY_OPEN_CMD  FALSE

typedef enum
{
  RELAY_OPENED,
  RELAY_CLOSED,
  RELAY_ERROR,
  RELAY_RSV
}eActor_RelayStatusType;

#define CPU_FAN_ID 0
#define CPU_HEAT_ID 1
#define POS_RELAY_ID 2
#define NEG_RELAY_ID 3
#define PRE_RELAY_ID 4
#define CHG_RELAY_ID 5
#define BCU_CTRL_BMU_FAN_ID   6
#define BCU_CTRL_BMU_HEAT_ID  7


extern GeneralReturnType Actor_RelayInit(void);
extern GeneralReturnType Actor_CtrlRelay(uByte relay_id, sActor_CtrlRelayCmdType relay_cmd);
extern eActor_RelayStatusType Actor_GetRelaySt(uByte relay_id);
extern void Actor_RelayMain(void);

#endif