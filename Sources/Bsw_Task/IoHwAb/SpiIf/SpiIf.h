#ifndef SPIIF_H
#define SPIIF_H

#include "GeneralTypeDef.h"
#include "Ecu.h"
#include "IoIf_Type.h"

//#include "CPU_TLE_CS.h"
//#include "CPU_TLE_SCK.h"
//#include "CPU_TLE_SDI.h"
//#include "CPU_TLE_SDO.h"



#define SPIIF_MAX_NUM  1

typedef enum 
{
   SPIIF_TLE8102_NO,
}eSpiIf_IdNumType;




GeneralReturnType SpiIf_WriteReadByte(uByte SpiIf_Id,uByte cmd,uByte *Rd_DataPtr);
GeneralReturnType SpiIf_WriteByte(uByte SpiIf_Id,uByte cmd);
GeneralReturnType SpiIf_ReadByte(uByte SpiIf_Id,uByte *SpiIf_ValuePtr);

#endif /* SPIIF_H */