// AppDo.h
#ifndef APPDO_H
#define APPDO_H

#include "GeneralTypeDef.h"
#include "Ecu.h"

#define DO_HIGH_LEVEL_CMD TRUE
#define DO_LOW_LEVEL_CMD  FALSE

typedef enum
{
  DO_STATUS_LOW,
  DO_STATUS_HIGH,
  DO_STATUS_ERR,
  DO_STATUS_RSV,
}eActor_DoStatusType;
 

#define CPU_DO1_ID 0

#define CPU_DO2_ID 1


extern GeneralReturnType Actor_DoInit(void);
extern GeneralReturnType Actor_CtrlDo(uByte Do_id, bool Do_cmd);
extern eActor_DoStatusType Actor_GetDoSt(uByte Do_id);

#endif