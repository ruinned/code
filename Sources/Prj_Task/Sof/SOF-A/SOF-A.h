// SOF.h
#ifndef SOF_A_H
#define SOF_A_H

#include "SOF_Macro.h" 

extern void SOF_ModuleInit(void);
extern void SOF_ModuleCountCbk(void);
extern void SOF_ModuleCbk(void);
extern void SOF_ModifyUser(uByte type, uByte ratio);
extern void SOF_SetTargetUser(uByte type, uWord value);
 

#endif /* SOF_H */