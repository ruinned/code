// SOF-B.h
#ifndef SOF_B_H
#define SOF_B_H

#include "SOF_Macro.h" 


extern void SOF_ModuleInit(void);
extern void SOF_ModuleCountCbk(void);
extern void SOF_ModuleCbk(void);
extern void SOF_ModifyUser(uByte type, uByte ratio);
extern void SOF_SetTargetUser(uByte type, uWord value);
 
#endif /* SOF_B_H */