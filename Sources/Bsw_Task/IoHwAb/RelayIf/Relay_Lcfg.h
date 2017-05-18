#ifndef RELAYCFG_H
#define RELAYCFG_H

#include "RelayIf_Type.h"

extern sRelayIf_HandleType RelayIf_Handle;  

#define GET_RELAYPROPERTY(RelayIf_Id) \
 (RelayIf_Handle.RelayIf_PropertyPtr + RelayIf_Id)   //get the pointer of property
 
#define GET_RELAYOPERATION(RelayIf_Id) \
 (RelayIf_Handle.RelayIf_OperationPtr + RelayIf_Id)  //get the pointer of operation


#endif /* SPICFG_H */