#ifndef RELAYICDRIVEIF_TYPE_H
#define RELAYICDRIVEIF_TYPE_H

#include "GeneralTypeDef.h"
#include "SpiIf.h"
#include "IoIf_Type.h"

#define RELAYICDRIVE_MAX_NUM 1

typedef enum 
{
   TLE8102_NO
}eRelayIcDrive_IdNumType;

typedef enum 
{
   TLE_IC_SPI_MODE,
   TLE_IC_IO_MODE,
}eRelayIcDrive_ModeType;

typedef enum 
{   
   TLE_IC_CHANNEL1 = 1,
   TLE_IC_CHANNEL2
}eRelayIcDrive_ChannelType;

typedef struct 
{
    const eRelayIcDrive_IdNumType RelayIcDrive_IdNum;
    eRelayIcDrive_ModeType  RelayIcDrive_Mode;
}sRelayIcDrive_PropertyType;

typedef GeneralReturnType (* RelayIcDrive_IcSpiCtrlPtrType)(uByte Spi_cmd,uByte *Rd_DataPtr);
typedef void (* RelayIcDrive_IcDoCtrlPtrType)(bool Relay_Cmd);
typedef bool (* RelayIcDrive_IcReadStatusPtrType)(void);

typedef struct                           
{
    RelayIcDrive_IcSpiCtrlPtrType  RelayIcDrive_IcSpiCtrlPtr;
    RelayIcDrive_IcDoCtrlPtrType  RelayIcDrive_IcCh1DoCtrlPtr;
    RelayIcDrive_IcDoCtrlPtrType  RelayIcDrive_IcCh2DoCtrlPtr;
    RelayIcDrive_IcReadStatusPtrType  RelayIcDrive_IcReadStatusPtr;
}sRelayIcDrive_OperationType; 


typedef struct 
{
    const sRelayIcDrive_PropertyType *RelayIcDrive_PropertyPtr;      //the pointer of property
    const sRelayIcDrive_OperationType *RelayIcDrive_OperationPtr;   //the pointer of operation
}sRelayIcDrive_HandleType;

#endif 
 

