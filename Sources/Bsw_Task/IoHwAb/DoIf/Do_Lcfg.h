#ifndef IOCFG_H
#define IOCFG_H

#include "DoIf_Type.h"

extern const sDoIf_HandleType DoIf_Handle; 
  
#define GET_DOPROPERTY(DoIf_Id) \
 (DoIf_Handle.DoIf_PropertyPtr + DoIf_Id)    //get the pointer of property

#define GET_DOOPERATION(DoIf_Id) \
(DoIf_Handle.DoIf_OperationPtr + DoIf_Id)  //get the pointer of operation

#endif /* IOCFG_H */