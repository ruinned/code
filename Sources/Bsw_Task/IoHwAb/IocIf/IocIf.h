#ifndef IOCIF_H
#define IOCIF_H

#include "GeneralTypeDef.h"
#include "Ecu.h"
#include "IoIf_Type.h"

#include "AC_CP_IOC.h"

//#include "Ioc_Lcfg.h"

#define  IOCIF_MAX_NUM  1

typedef enum 
{
   IOCIF_AC_CP_NO,
}eIocIf_IdNumType; 


//extern GeneralReturnType IocIf_Task(eIocIf_IdNumType IocIf_Id);

#define AC_CP_IOC_TASK() IocIf_Task(IOCIF_AC_CP_NO)   


GeneralReturnType IocIf_Init(void);
GeneralReturnType IocIf_GetIocValue(uByte IocIf_Id,uWord *IocIf_PeriodValuePtr,uByte *IocIf_RatioValuePtr);
GeneralReturnType IocIf_Enable(uByte IocIf_Id);
GeneralReturnType IocIf_Disable(uByte IocIf_Id);
GeneralReturnType IocIf_EnableEvent(uByte IocIf_Id);
GeneralReturnType IocIf_DisableEvent(uByte IocIf_Id);
void IocIf_Task(uByte IocIf_Id);


#endif /* IOCIF_H */