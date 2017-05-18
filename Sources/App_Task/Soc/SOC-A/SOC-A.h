// SOC-A.h
#ifndef SOC_A_H
#define SOC_A_H

#include "SOC_Macro.h"

extern void SOC_ModuleInit(void);
extern void SOC_ModuleCountCbk(void);
extern void SOC_ModuleCbk(void);
extern void SOC_EstmatUser(uWord soc_target, uWord TimeInterval);

#endif /* SOC_A_H */