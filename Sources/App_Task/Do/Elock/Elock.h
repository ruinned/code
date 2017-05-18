#ifndef APPELOCK_H
#define APPELOCK_H
#include "Ecu.h"
#include "Prj_Define.h"


#define  ST_UNLOCK  0
#define  ST_LOCK    1
#define  ST_DEFAULT 2
#define  ST_ERR 3


extern void Elock_UpdateCmd(uByte Cmd);
extern void Elock_Init(void);
extern void Elock_TimeCntCbk(void);
extern void Elock_Main(void); 
extern uByte Elock_GetHdErr(void);
extern void Elock_UpdateMainSartFlag(uByte flag);
extern void Elock_TurnRight(void);
extern void Elock_TurnLeft(void);
extern void Elock_TurnOff(void);
extern uWord Elock_GetAiVolt(void);
extern uByte Elock_GetTemp(uByte Id);
extern uByte Eclock_GetElockSt(void);
extern uByte Eclock_GetMotoSt(void);

#endif