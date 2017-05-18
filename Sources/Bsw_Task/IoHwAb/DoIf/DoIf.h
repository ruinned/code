#ifndef DOIF_H
#define DOIF_H

#include "GeneralTypeDef.h"
#include "Ecu.h"
#include "IoIf_Type.h"

#include "CPUDO1.h"
#include "CON1.h"
#include "CPU_CHG_S.h"
#include "CPU_CHG_P.h"
#include "Slaver_Pow_Ctrl.h"
#include "CPUDO2.h"
#include "ISO_CTRL_POS.h"
#include "ISO_CTRL_NEG.h"
#include "Lock_Lock.h"
#include "Lock_UnLock.h"

//#include "Do_Lcfg.h"


#define DOIF_MAX_NUM  10
typedef enum 
{
   DOIF_CPUDO1_NO,
   DOIF_CON_NO,
   DOIF_CPU_CHG_S_NO,
   DOIF_CPU_CHG_P_NO,
   DOIF_SLAVER_POW_CTRL_NO,
   DOIF_CPUDO2_NO,
   DOIF_ISO_CTRL_POS_NO,
   DOIF_ISO_CTRL_NEG_NO,
   DOIF_LOCK_LOCK_NO,
   DOIF_LOCK_UNLOCK_NO
}eDoIf_IdNumType; 


typedef enum 
{
   DOIF_CMD_LOW,   /* DO output value low level */
   DOIF_CMD_HIGH   /* DO output value high level */
}eDoIf_CmdType;






GeneralReturnType DoIf_Init(uByte DoIf_Id);
GeneralReturnType DoIf_Ctrl(uByte DoIf_Id,bool DoIf_Cmd);


#endif /* DOIF_H */