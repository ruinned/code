// Xgate_AcCpIoc.h
// modify base on AC CP_IOC.h, jusd use for xgate

#ifndef XGATE_AC_CP_IOC_SRV_H
#define XGATE_AC_CP_IOC_SRV_H

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"
#include "Xgate_Config.h"
#include "Xgate_AcCpIoc.h"  

#ifdef XGATE_ENABLE_ACCP

#pragma push
#pragma DATA_SEG XGATE_DATA 
#pragma CODE_SEG XGATE_CODE  

extern interrupt void Xgate_AcCpIsr(void);
extern void Xgate_AcCpCalVal(void);

#pragma pop

#endif /* ifndef __XGATE_AC_CP_IOC */

#endif