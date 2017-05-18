#ifndef RELAYIF_TYPE_H
#define RELAYIF_TYPE_H

#include "GeneralTypeDef.h"
#include "RelayIcDriveIf.h"
#include "Ecu.h"
#include "IoIf_Type.h"

#define RELAYIF_MAX_NUM  6

typedef enum 
{
   //RELAYIF_CPU_TLE8102_FAN_NO,  //use the spi mode
  // RELAYIF_CPU_TLE8102_HEAT_NO, //use the spi mode
   RELAYIF_CPU_RLY_FAN_NO,      //use the do mode
   RELAYIF_CPU_RLY_HEAT_NO,     //use the do mode
   RELAYIF_CPU_RLY_POS_NO,
   RELAYIF_CPU_RLY_NEG_NO,
   RELAYIF_CPU_RLY_PRE_NO,
   RELAYIF_CPU_RLY_CHG_NO
}eRelayIf_IdNumType;

typedef enum 
{
   RELAYIF_CMD_LOW,  //the relay command
   RELAYIF_CMD_HIGH
}eRelayIf_CmdType; 

#define CLOSE_RELAY RELAYIF_CMD_HIGH
#define OPEN_RELAY  RELAYIF_CMD_LOW 

typedef enum 
{
   RELAYIF_FEEDBACK_VALUE_LOW,
   RELAYIF_FEEDBACK_VALUE_HIGH
}eRelayIf_FeedBackValueType;  //the feedback value

typedef enum 
{
   RELAYIF_IC_SPI_MODE,
   RELAYIF_IC_IO_MODE,
}eRelayIf_IcModeType;

typedef enum 
{
   IC_SPI_MODE,   //use the ic spi mode
   IC_IO_MODE,  //use the ic io mode
   IO_MODE,    //use the io mode 
}eRelayIf_ModeType;

typedef struct 
{
    const eRelayIf_IdNumType RelayIf_IdNum;
    eRelayIf_ModeType  RelayIf_Mode;
}sRelayIf_PropertyType;

typedef void (* RelayIf_DoCtrlPtrType)(bool Cmd);   //use the io pin
typedef GeneralReturnType (* RelayIf_PwmCtrlPtrType)(void);   
typedef GeneralReturnType (* RelayIf_PwmSetRatioPtrType)(uWord Ratio);
typedef GeneralReturnType (* RelayIf_PwmStopCtrlPtrTyte)(void);

typedef GeneralReturnType (* RelayIf_IcInitPtrType)(eRelayIf_IcModeType Relay_Mode); //init the ic
typedef GeneralReturnType (* RelayIf_IcSpiCtrlPtrType)(uByte Spi_Cmd,uWord *Err_CodePtr);  //use the ic with spi mode
typedef GeneralReturnType (* RelayIf_IcDoCtrlPtrType)(uByte Cmd); //use the ic with io mode
typedef void (* RelayIf_FeedBackPtrType)(bool *Relay_ValuePtr); //feedback value
typedef struct 
{
    RelayIf_IcInitPtrType  RelayIf_IcInitPtr;
    RelayIf_IcSpiCtrlPtrType  RelayIf_IcSpiCtrlPtr;
    RelayIf_IcDoCtrlPtrType RelayIf_IcDoCtrlPtr;
  #ifndef ECU_M_IOHWAB_PWMIF  
    RelayIf_DoCtrlPtrType  RelayIf_DoCtrlPtr;
    RelayIf_DoCtrlPtrType  RelayIf_ClkDoCtrlPtr;
    RelayIf_FeedBackPtrType RelayIf_FeedBackPtr;
    RelayIf_DoCtrlPtrType  RelayIf_OeCtrlPtr;
    
  #else
    RelayIf_PwmCtrlPtrType RelayIf_PwmCtrlPtr;
    RelayIf_PwmSetRatioPtrType RelayIf_PwmSetRatioPtr;
    RelayIf_PwmStopCtrlPtrTyte RelayIf_PwmStopCtrlPtr;
  #endif
}sRelayIf_OperationType; 

typedef struct 
{
    sRelayIf_PropertyType *RelayIf_PropertyPtr;      //the pointer of property
    const sRelayIf_OperationType *RelayIf_OperationPtr;   //the pointer of operation
    //eRelayIf_ModeType RelayIf_IcMode;
}sRelayIf_HandleType;

#endif 
 

