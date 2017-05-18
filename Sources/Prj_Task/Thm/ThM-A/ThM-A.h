// ThM.h
#ifndef THM_A_H
#define THM_A_H

#include "GeneralTypeDef.h"
#include "Ecu.h"
//#include "Version_Type.h" 
#include "ThM_Macro.h" 


extern void ThM_ModuleInit(void);
extern void ThM_ModuleCountCbk(void);
extern void ThM_ModuleCbk(void);
extern void ThM_TestFan(uByte fan_on_off, uWord fan_speed_ctrl);

#endif /* THM_A_H */