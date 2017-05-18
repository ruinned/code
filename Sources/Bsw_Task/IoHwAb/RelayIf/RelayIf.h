#ifndef RELAYIF_H
#define RELAYIF_H

#include "Relay_Lcfg.h"

GeneralReturnType RelayIf_IcChangeMode(uByte RelayIf_Id,uByte Mode);
GeneralReturnType RelayIf_Ctrl(uByte RelayIf_Id,uByte cmd);
#ifndef ECU_M_IOHWAB_PWMIF
GeneralReturnType RelayIf_FeedBack(uByte RelayIf_Id,uByte *Relay_ValuePtr);
#endif

#endif /* RELAYIF_H */