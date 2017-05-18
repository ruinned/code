#ifndef SPICFG_H
#define SPICFG_H

#include "SpiIf_Type.h"

extern const sSpiIf_HandleType SpiIf_Handle;  

#define GET_SPIPROPERTY(SpiIf_Id) \
 (SpiIf_Handle.SpiIf_PropertyPtr + SpiIf_Id)   //get the pointer of property
 
#define GET_SPIOPERATION(Spi_Id) \
 (SpiIf_Handle.SpiIf_OperationPtr + SpiIf_Id)  //get the pointer of operation


#endif /* SPICFG_H */