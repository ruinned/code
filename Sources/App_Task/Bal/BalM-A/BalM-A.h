// BalM.h
#ifndef BALM_A_H
#define BALM_A_H

#include "GeneralTypeDef.h"
#include "BalM_Macro.h"



extern void BalM_ModuleInit(void);
extern void BalM_ModuleCountCbk(void);
extern void BalM_ModuleCbk(void);
extern void BalM_UpdateDataFromCan(uByte bmu_no, uByte cmd, uByte *p_data, uByte len);

#endif /* BALM_A_H */