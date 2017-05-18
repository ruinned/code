#ifndef IOCCFG_H
#define IOCCFG_H

#include "IocIf_Type.h"
#if ((ECU_CATEGORY == M_BCU_C) || (ECU_CATEGORY == M_BCU_D))
extern const sIocIf_HandleType IocIf_Handle; 
  
#define GET_IOCPROPERTY(IocIf_Id) \
 (IocIf_Handle.IocIf_PropertyPtr + IocIf_Id)    //get the pointer of property

#define GET_IOCOPERATION(IocIf_Id) \
(IocIf_Handle.IocIf_OperationPtr + IocIf_Id)  //get the pointer of operation
#endif
#endif /* IOCCFG_H */