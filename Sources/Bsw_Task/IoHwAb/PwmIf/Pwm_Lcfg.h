#ifndef PWMCFG_H
#define PWMCFG_H

#include "PwmIf_Type.h"

#ifdef ECU_M_IOHWAB_PWMIF

extern const sPwmIf_HandleType PwmIf_Handle; 
  
#define GET_PWMPROPERTY(PwmIf_Id) \
 (PwmIf_Handle.PwmIf_PropertyPtr + PwmIf_Id)    //get the pointer of property

#define GET_PWMOPERATION(PwmIf_Id) \
(PwmIf_Handle.PwmIf_OperationPtr + PwmIf_Id)   //get the pointer of operation

#endif
#endif /* IOCFG_H */