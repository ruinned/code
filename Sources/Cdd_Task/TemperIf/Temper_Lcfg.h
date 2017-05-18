#ifndef TEMPERCFG_H
#define TEMPERCFG_H

#include "TemperIf_Type.h"

#if ((ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R) || (ECU_CATEGORY == M_BCU_C))
extern const sTemperIf_HandleType TemperIf_Handle; 
  
#define GET_TEMPERPROPERTY(TemperIf_Id) \
 (TemperIf_Handle.TemperIf_PropertyPtr + TemperIf_Id)    //get the pointer of property

#define GET_TEMPEROPERATION(TemperIf_Id) \
(TemperIf_Handle.TemperIf_OperationPtr + TemperIf_Id)  //get the pointer of operation

#endif //#if ((ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R) || (ECU_CATEGORY == M_BCU_C))
#endif /* TEMPERCFG_H */