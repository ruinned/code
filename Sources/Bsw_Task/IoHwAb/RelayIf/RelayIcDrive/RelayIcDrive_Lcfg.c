#include "RelayIf_Type.h"

#include "CPU_RLY_FAN.h"
#include "CPU_RLY_HEAT.h"

//#include "CPU_TLE_CO1.h"

static GeneralReturnType RelayIcDrive_IcSpiCtrl(uByte Spi_Cmd,uByte *Rd_DataPtr) 
{
    return SpiIf_WriteReadByte((uByte)SPIIF_TLE8102_NO,Spi_Cmd,Rd_DataPtr);
}

static bool RelayIcDrive_IcReadStatus(void) 
{
    //return CPU_TLE_CO1_GetVal(); 
}

sRelayIcDrive_PropertyType RelayIcDrive_Property[] = 
{ 
    {
       TLE8102_NO,
       TLE_IC_SPI_MODE,
    }
};

const sRelayIcDrive_OperationType RelayIcDrive_Operation[RELAYICDRIVE_MAX_NUM] = 
{
    {
       RelayIcDrive_IcSpiCtrl,
       CPU_RLY_FAN_PutVal,
       CPU_RLY_HEAT_PutVal,
       RelayIcDrive_IcReadStatus,
    }
};

const sRelayIcDrive_HandleType RelayIcDrive_Handle = 
{
    RelayIcDrive_Property,
    RelayIcDrive_Operation
};

