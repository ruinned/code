
// LvM.h
#ifndef LVM_H
#define LVM_H

#include "Ecu.h"


#define LVM_STATE_ON_THRED     80 //6V
#define LVM_STATE_OFF_THRED    65 //5V

#define LVM_STATE_OFF          0
#define LVM_STATE_ON           1
#define LVM_STATE_INIT         2


#define LOW_VOLT_MANAGE_ENABLE
#define LVM_CAN_ACTIVE_ENABLE

#define POWER_OFF()  (DoIf_Ctrl((uByte)DOIF_CON_NO, (bool)DOIF_CMD_LOW)) // DOIF_CMD_HIGH
#define POWER_ON()  (DoIf_Ctrl((uByte)DOIF_CON_NO, (bool)DOIF_CMD_HIGH))   //DOIF_CMD_LOW


#define POWER_OFF_BMU()  (DoIf_Ctrl((uByte)DOIF_SLAVER_POW_CTRL_NO, (bool)DOIF_CMD_LOW))
#define POWER_ON_BMU()  (DoIf_Ctrl((uByte)DOIF_SLAVER_POW_CTRL_NO, (bool)DOIF_CMD_HIGH))


extern void LvM_Init(void);
extern void LvM_CountCbk(void);
extern void LvM_ProcMain(void);

extern uByte LvM_GetBcuShutDownEn(void);
extern void LvM_UpdateSystemShutDownEn(uByte order);
extern void LvM_UpdateVcuShutDownEn(uByte order);
 

#endif /* LVM_H */
