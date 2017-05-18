#ifndef TEMPERIF_H
#define TEMPERIF_H

#include "Temper_Lcfg.h"

#define MAX_B20_NUM 8

#if ((ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R) || (ECU_CATEGORY == M_BCU_C))
extern GeneralReturnType TemperIf_18b20GetTemperAd(eTemperIf_IdNumType TemperIf_Id,uByte *T18B20AdPtr,uWord *Temper_AdValuePtr);
extern GeneralReturnType TemperIf_NtcGetTemperAd(eTemperIf_IdNumType TemperIf_Id,uWord *Temper_AdValuePtr); 
extern void TemperIf_Init(void);
extern GeneralReturnType TemperIf_StartCnvt(eTemperIf_IdNumType TemperIf_Id);
extern void TemperIf_Srch18B20Rom(uByte *T18b20RomPtr,uByte *T18b20RomNumPtr);
extern GeneralReturnType TemperIf_HwErrDiag(uWord * HwErrPtr);
#endif //#if ((ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R) || (ECU_CATEGORY == M_BCU_C))
#endif /* TEMPERIF_H */