



#ifndef GBCHG_IFDET_H
#define GBCHG_IFDET_H

#include "GeneralTypeDef.h"
//#include "AiIf_Type.h"
#include "AiIf.h"


#define GAIN_VOLT 110

                     
 extern void GbChgDet_Ai(eAiIf_ChannelIdNumType Ch, uWord VoltGain, uWord *VoltPtr);
 extern void GbChgDet_DutyCalc(uByte *DutyPtr);


#endif  /*GBCHG_IFDET_H */