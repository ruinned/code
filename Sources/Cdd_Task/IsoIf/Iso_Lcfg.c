//#include "IsoIf_Type.h"
#include "Iso_Lcfg.h"
#include "AiIf.h"                    

 GeneralReturnType Iso_GetPosAdValue(uWord *Ai_AdcValue) 
{
    //return (GeneralReturnType)AiIf_MeasureChannel(AIIF_ISO_POS_NO,Ai_AdcValue);
} 

 GeneralReturnType Iso_GetNegAdValue(uWord *Ai_AdcValue) 
{
   // return (GeneralReturnType)AiIf_MeasureChannel(AIIF_ISO_NEG_NO,Ai_AdcValue);
} 

/*const sIso_PropertyType Iso_Property[] = 
{
   ISO1,
};
 
const  sIso_OperationType Iso_Operation[] =
{ 
  {
     ISO_CTRL_POS_PutVal,
     ISO_CTRL_NEG_PutVal,
     ISO_POW_PutVal,
     Iso_GetPosAdValue,
     Iso_GetNegAdValue,
  }, 
};
  
 
const sIso_HandleType Iso_Handle = 
{
    Iso_Property,
    Iso_Operation,
};
*/

 /* ISO_LCFG_H*/
