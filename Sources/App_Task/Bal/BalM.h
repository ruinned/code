// BalM.h
#ifndef BALM_H
#define BALM_H

#include "BalM_Macro.h"

// include solution headfiles
#ifdef USE_BALM_A
#include"BalM-A.h"
#endif

#ifdef USE_BALM_B
#include"BalM-B.h"
#endif

#ifdef USE_BALM_C
#include"BalM-C.h"
#endif

#ifdef USE_BALM_D
#include"BalM-D.h"
#endif

#ifdef USE_BALM_E
#include"BalM-E.h"
#endif

#ifdef USE_BALM_F
#include"BalM-F.h"
#endif

#ifdef USE_BALM_G
#include"BalM-G.h"
#endif

extern void BalM_Init(void);
extern void BalM_MainCountCbk(void); 
extern void BalM_MainCbk(void);

#endif /* BALM_H */