#ifndef XGATE_REALTIME_SRV_H
#define XGATE_REALTIME_SRV_H
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"
#include "Xgate_RealTime.h" 
#include "Xgate_Config.h"
#include "ClockIf.h"

#ifdef XGATE_ENABLE_REALTIME
//


#pragma push
#pragma DATA_SEG XGATE_DATA 
#pragma CODE_SEG XGATE_CODE
extern void Xgate_RdRealTimeSrv(void);

#pragma pop


#endif // #ifdef XGATE_ENABLE_REALTIME
#endif // XGATE_REALTIME_SRV_H