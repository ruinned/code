#ifndef RELAYICDRIVECFG_H
#define RELAYICDRIVECFG_H

#include "RelayIcDriveIf_Type.h"

extern const sRelayIcDrive_HandleType RelayIcDrive_Handle;  

#define GET_RELAYICDRIVEPROPERTY(RelayIcDrive_Id) \
 (RelayIcDrive_Handle.RelayIcDrive_PropertyPtr + RelayIcDrive_Id)   //get the pointer of property
 
#define GET_RELAYICDRIVEOPERATION(RelayIcDrive_Id) \
 (RelayIcDrive_Handle.RelayIcDrive_OperationPtr + RelayIcDrive_Id)  //get the pointer of operation


#endif /* RELAYICDRIVECFG_H */