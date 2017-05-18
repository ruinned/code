#ifndef RELAYICDRIVEIF_H
#define RELAYICDRIVEIF_H

#include "RelayIcDrive_Lcfg.h"

/* command configration for TLE8102SG */
#define RELAY_IC_NC                        0x00   //000 00000b  // invalid command

#define RELAY_IC_DIAG                  0x20   //001 00000b  // diag SPI communication

#define RELAY_IC_OUT_CONFIG                0x40   //010 00000b  // output configaration
#define RELAY_IC_OUT_CONFIG_CH2_CURR_LIM1      0x10   //000 10000b
#define RELAY_IC_OUT_CONFIG_CH1_CURR_LIM1      0x08   //000 01000b
#define RELAY_IC_OUT_CONFIG_O_TEMP_LATCH       0x04   //000 00100b
#define RELAY_IC_OUT_CONFIG_CH1_SLE2           0x02   //000 00010b
#define RELAY_IC_OUT_CONFIG_CH1_SLE1           0x01   //000 00001b

#define RELAY_IC_IO_CTRL_CONFIG            0x60   //011 00000b  // io control config
#define RELAY_IC_CTRL_CONFIG_DIA_OUT_IO        0x00   //000 00000b
#define RELAY_IC_CTRL_CONFIG_DIA_OUT_SPI       0x10   //000 10000b
#define RELAY_IC_CTRL_CONFIG_BOL_INPUT         0x00   //000 00000b
#define RELAY_IC_CTRL_CONFIG_BOL_INPUT_OR_SPI  0x04   //000 00100b
#define RELAY_IC_CTRL_CONFIG_BOL_INPUT_AND_SPI 0x08   //000 01000b
#define RELAY_IC_CTRL_CONFIG_BOL_SPI           0x0C   //000 01100b
#define RELAY_IC_CTRL_CONFIG_BOL_IO            0x00   //000 00000b
#define RELAY_IC_CTRL_CONFIG_SENS_GFIO         0x00   //000 00000b
#define RELAY_IC_CTRL_CONFIG_SENS_CH1          0x01   //000 00001b
#define RELAY_IC_CTRL_CONFIG_SENS_CH2          0x02   //000 00002b
#define RELAY_IC_CTRL_CONFIG_SENS_CH1_AND_CH2  0x03   //000 00003b

#define RELAY_IC_RESET_REG                 0x80   //100 00000b  // reset regsiter

#define RELAY_IC_SLEEP                     0xA0   //101 00000b  // goto sleep mode

#define RELAY_IC_WAKE_UP                   0xC0   //110 00000b  // wakeup from sleep

#define RELAY_IC_OUTPUT_CTRL               0xE0   //111 00000b  // output control for SPI
#define RELAY_IC_CTRL_CH2_OFF_CH1_OFF          0xE0   //111 00000b  
#define RELAY_IC_CTRL_CH2_OFF_CH1_ON           0xE8   //111 01000b
#define RELAY_IC_CTRL_CH2_ON_CH1_OFF           0xF0   //111 10000b
#define RELAY_IC_CTRL_CH2_ON_CH1_ON            0xF8   //111 11000b
#define RELAY_IC_CTRL_CH1_ON                   0x08   //000 01000b
#define RELAY_IC_CTRL_CH2_ON                   0x10   //000 10000b


// diagnostic the tle8102 fault state
#define RELAY_IC_FAULT              0x0001
#define RELAY_IC_OVER_TEMP_ERR      0x0002
#define RELAY_IC_CH_OVER_TEMP_ERR      0x0004

#define RELAY_IC_CH1_IS_NORMAL         0x0000
#define RELAY_IC_CH1_SHORT_GND_ERR     0x0010
#define RELAY_IC_CH1_OPEN_LOAD_ERR     0x0020 
#define RELAY_IC_CH1_UNDER_CURR_ERR    0x0030 
#define RELAY_IC_CH1_OVER_LOAD_ERR     0x0040
#define RELAY_IC_CH1_OVER_TEMPER_ERR   0x0050

#define RELAY_IC_CH2_IS_NORMAL         0x0000
#define RELAY_IC_CH2_SHORT_GND_ERR     0x0100
#define RELAY_IC_CH2_OPEN_LOAD_ERR     0x0200 
#define RELAY_IC_CH2_UNDER_CURR_ERR    0x0300 
#define RELAY_IC_CH2_OVER_LOAD_ERR     0x0400
#define RELAY_IC_CH2_OVER_TEMPER_ERR   0x0500


#define SPIRELAY_PARA_ERR              0x1000

// SPI/IO control the output 
//#define RELAY_IC_CH1                   0x01
//#define RELAY_IC_CH2                   0x02

#define RELAY_IC_CH_ON                 0x01
#define RELAY_IC_CH_OFF                0x00

GeneralReturnType RelayIcDrive_IcInit(uByte RelayIcDrive_Id,eRelayIcDrive_ModeType Relay_IcMode);
GeneralReturnType RelayIcDrive_IcDiagIoMode(uByte RelayIcDrive_Id);
GeneralReturnType RelayIcDrive_IcDiagSpiMode(uByte RelayIcDrive_Id);
GeneralReturnType RelayIcDrive_IcDiag(uByte RelayIcDrive_Id,uWord *Err_CodePtr);
GeneralReturnType RelayIcDrive_IcSpiCtrlOutput(uByte RelayIcDrive_Id,eRelayIcDrive_ChannelType channle, uByte cmd, uWord *Err_CodePtr);
GeneralReturnType RelayIcDrive_IcIoCtrlOutput(uByte RelayIcDrive_Id,eRelayIcDrive_ChannelType channle, uByte cmd, uWord *Err_CodePtr);
GeneralReturnType RelayIcDrive_IcResetRegister(uByte RelayIcDrive_Id);
GeneralReturnType RelayIcDrive_IcGotoSleep(uByte RelayIcDrive_Id);
GeneralReturnType RelayIcDrive_IcWakeUp(uByte RelayIcDrive_Id);
#endif /* RELAYICDRIVEIF_H */