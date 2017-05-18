
#ifndef IOC_CBK_H
#define IOC_CBK_H

#include "IocIf_Type.h"

#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D))
extern GeneralReturnType IocIf_Task(eIocIf_IdNumType IocIf_Id);

#define AC_CP_IOC_TASK() IocIf_Task(IOCIF_AC_CP_NO)   
#endif

#endif /* IOC_CBK_H */