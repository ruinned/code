#ifndef ISO_LCFG_H
#define ISO_LCFG_H

#include "GeneralTypeDef.h"

extern  GeneralReturnType  Iso_GetPosAdValue(uWord *Ai_AdcValue);
extern  GeneralReturnType Iso_GetNegAdValue(uWord *Ai_AdcValue);                                        

/* 
extern const sIso_HandleType Iso_Handle;
   
#define GET_ISOPROPERTY(Iso_Id) \
 (Iso_Handle.Iso_PropertyPtr + Iso_Id)  //get the property pointer

#define GET_ISOOPERATION(Iso_Id) \
 (Iso_Handle.Iso_OperationPtr + Iso_Id)  //get the property pointer 
*/

#endif /* ISO_LCFG_H */