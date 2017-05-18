#include "RelayIf_Type.h"

#ifndef ECU_M_IOHWAB_PWMIF
  #include "CPU_RLY_POS.h"
  #include "CPU_RLY_NEG.h"
  #include "CPU_RLY_PRE.h"
  #include "CPU_RLY_CHG.h"
  #include "CPU_RLY_CTRL.h"
  #include "RELAY_POS.h"
  #include "RELAY_NEG.h"
  #include "RELAY_PRE.h"
  #include "RELAY_CHG.h"
  #include "CPU_RLY_FAN.h"
  #include "CPU_RLY_HEAT.h"
  #include "CPU_RLY_OE.h"
  #include "RELAY_FAN.h"
  #include "RELAY_HEAT.h"
#else
  #include "PwmIf.h"
#endif


/*
static GeneralReturnType RelayIf_IcInitCtrl(eRelayIf_IcModeType RelayIf_IcMode) 
{
    return RelayIcDrive_IcInit((uByte)TLE8102_NO,(eRelayIcDrive_ModeType)RelayIf_IcMode);
}

static GeneralReturnType RelayIf_FanSpiCtrl(uByte Spi_Cmd,uWord *Err_CodePtr) 
{
    return RelayIcDrive_IcSpiCtrlOutput((uByte)TLE8102_NO,TLE_IC_CHANNEL1,Spi_Cmd,Err_CodePtr);
}
static GeneralReturnType RelayIf_HeatSpiCtrl(uByte Spi_Cmd,uWord *Err_CodePtr) 
{
    return RelayIcDrive_IcSpiCtrlOutput((uByte)TLE8102_NO,TLE_IC_CHANNEL2,Spi_Cmd,Err_CodePtr);
}

static GeneralReturnType RelayIf_FanDoCtrl(uByte Cmd) 
{
    return RelayIcDrive_IcIoCtrlOutput((uByte)TLE8102_NO,TLE_IC_CHANNEL1,Cmd,NULL);
}
static GeneralReturnType RelayIf_HeatDoCtrl(uByte Cmd) 
{
    return RelayIcDrive_IcIoCtrlOutput((uByte)TLE8102_NO,TLE_IC_CHANNEL2,Cmd,NULL);
}

*/

#ifndef ECU_M_IOHWAB_PWMIF

  static void RelayIf_CpuPosFeedBackDoCtrl(bool *Relay_ValuePtr) 
  {
      //*Relay_ValuePtr = RELAY_POS_GetVal();
  }

  static void RelayIf_CpuNegFeedBackDoCtrl(bool *Relay_ValuePtr) 
  {
      //*Relay_ValuePtr = RELAY_NEG_GetVal();
  }

  static void RelayIf_CpuPreFeedBackDoCtrl(bool *Relay_ValuePtr) 
  {
      //*Relay_ValuePtr = RELAY_PRE_GetVal(); 
  }

  static void RelayIf_CpuChgFeedBackDoCtrl(bool *Relay_ValuePtr) 
  {
      //*Relay_ValuePtr = RELAY_CHG_GetVal();  
  }
  
  static void RelayIf_CpuFanFeedBackDoCtrl(bool *Relay_ValuePtr) 
  {
      //*Relay_ValuePtr = RELAY_FAN_GetVal();  
  }
  
  static void RelayIf_CpuHeatFeedBackDoCtrl(bool *Relay_ValuePtr) 
  {
      //*Relay_ValuePtr = RELAY_HEAT_GetVal();  
  }
  
  
  
#else

  GeneralReturnType PWM_RLY_POS_Operation(void)
  {
     return PwmIf_Start(PWMIF_RELAY_POS_NO);
  }

  GeneralReturnType PWM_RLY_NEG_Operation(void)
  {
     return PwmIf_Start(PWMIF_RELAY_NEG_NO);
  }

  GeneralReturnType PWM_RLY_PRE_Operation(void)
  {
     return PwmIf_Start(PWMIF_RELAY_PRE_NO);
  }

  GeneralReturnType PWM_RLY_CHG_Operation(void)
  {
     return PwmIf_Start(PWMIF_RELAY_CHG_NO);
  }

  GeneralReturnType PWM_RLY_POS_SetRatio(uWord Ratio)
  {
     return PwmIf_SetRatio(PWMIF_RELAY_POS_NO,Ratio);
  }

  GeneralReturnType PWM_RLY_NEG_SetRatio(uWord Ratio)
  {
     return PwmIf_SetRatio(PWMIF_RELAY_NEG_NO,Ratio);
  }

  GeneralReturnType PWM_RLY_PRE_SetRatio(uWord Ratio)
  {
     return PwmIf_SetRatio(PWMIF_RELAY_PRE_NO,Ratio);
  }

  GeneralReturnType PWM_RLY_CHG_SetRatio(uWord Ratio)
  {
     return PwmIf_SetRatio(PWMIF_RELAY_CHG_NO,Ratio);
  }

  GeneralReturnType PWM_RLY_POS_StopCtrl(void)
  {
     return PwmIf_Stop(PWMIF_RELAY_POS_NO);
  }

  GeneralReturnType PWM_RLY_NEG_StopCtrl(void)
  {
     return PwmIf_Stop(PWMIF_RELAY_NEG_NO);
  }

  GeneralReturnType PWM_RLY_PRE_StopCtrl(void)
  {
     return PwmIf_Stop(PWMIF_RELAY_PRE_NO);
  }

  GeneralReturnType PWM_RLY_CHG_StopCtrl(void)
  {
     return PwmIf_Stop(PWMIF_RELAY_CHG_NO);
  }
  
#endif //#ifndef ECU_M_IOHWAB_PWMIF



sRelayIf_PropertyType RelayIf_Property[] = 
{ 
  /*{
     RELAYIF_CPU_RLY_FAN_NO,
     IC_SPI_MODE,
  }, 
  {
     RELAYIF_CPU_RLY_HEAT_NO,
     IC_SPI_MODE,
  },*/
  {
     RELAYIF_CPU_RLY_FAN_NO,
     IO_MODE,
  }, 
  {
     RELAYIF_CPU_RLY_HEAT_NO,
     IO_MODE,
  },
  {
     RELAYIF_CPU_RLY_POS_NO,
     IO_MODE,
  }, 
  {
     RELAYIF_CPU_RLY_NEG_NO,
     IO_MODE,
  },
  {
     RELAYIF_CPU_RLY_PRE_NO,
     IO_MODE,
  },
  {
     RELAYIF_CPU_RLY_CHG_NO,
     IO_MODE,
  }
};

const sRelayIf_OperationType RelayIf_Operation[] = 
{
  /*{
     RelayIf_IcInitCtrl,
     RelayIf_FanSpiCtrl,
     RelayIf_FanDoCtrl,
     NULL,
     NULL,
     NULL,
  },
  {
     RelayIf_IcInitCtrl,
     RelayIf_HeatSpiCtrl,
     RelayIf_HeatDoCtrl,
     NULL,
     NULL,
     NULL,
  },*/
  {
     NULL,
     NULL,
     NULL,
     RELAY_FAN_PutVal,
     CPU_RLY_CTRL_PutVal,
      RelayIf_CpuFanFeedBackDoCtrl,
     CPU_RLY_OE_PutVal,

  },
  {
     NULL,
     NULL,
     NULL, 
     RELAY_HEAT_PutVal,
     CPU_RLY_CTRL_PutVal,
     RelayIf_CpuHeatFeedBackDoCtrl,
     CPU_RLY_OE_PutVal,
     
  },
  {
     NULL,
     NULL,
     NULL,
   #ifndef ECU_M_IOHWAB_PWMIF
     RELAY_POS_PutVal,
     CPU_RLY_CTRL_PutVal,
     RelayIf_CpuPosFeedBackDoCtrl,
     CPU_RLY_OE_PutVal,
   #else
     PWM_RLY_POS_Operation,
     PWM_RLY_POS_SetRatio,
     PWM_RLY_POS_StopCtrl,
   #endif
  },
  {
     NULL,
     NULL,
     NULL,
   #ifndef ECU_M_IOHWAB_PWMIF
     RELAY_NEG_PutVal,
     CPU_RLY_CTRL_PutVal,
     RelayIf_CpuNegFeedBackDoCtrl,
     CPU_RLY_OE_PutVal,
   #else
     PWM_RLY_NEG_Operation,
     PWM_RLY_NEG_SetRatio,
     PWM_RLY_NEG_StopCtrl,
   #endif
  },
  {
     NULL,
     NULL,
     NULL,
   #ifndef ECU_M_IOHWAB_PWMIF
     RELAY_PRE_PutVal,
     CPU_RLY_CTRL_PutVal,
     RelayIf_CpuPreFeedBackDoCtrl,
     CPU_RLY_OE_PutVal, 
   #else
     PWM_RLY_PRE_Operation,
     PWM_RLY_PRE_SetRatio,
     PWM_RLY_PRE_StopCtrl,
   #endif
  },
  {
     NULL,
     NULL,
     NULL,
   #ifndef ECU_M_IOHWAB_PWMIF
     RELAY_CHG_PutVal,
     CPU_RLY_CTRL_PutVal, 
     RelayIf_CpuChgFeedBackDoCtrl,
     CPU_RLY_OE_PutVal,
   #else
     PWM_RLY_CHG_Operation,
     PWM_RLY_CHG_SetRatio,
     PWM_RLY_CHG_StopCtrl,
   #endif
  }
};

sRelayIf_HandleType RelayIf_Handle = 
{
   RelayIf_Property,
   RelayIf_Operation,
};

