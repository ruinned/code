#ifndef PWMIF_H
#define PWMIF_H

#include "Pwm_Lcfg.h"
#ifdef ECU_M_IOHWAB_PWMIF
GeneralReturnType PwmIf_Start(uByte PwmIf_Id);
GeneralReturnType PwmIf_Stop(uByte PwmIf_Id);
GeneralReturnType PwmIf_SetRatio(uByte PwmIf_Id,uWord Ratio);
GeneralReturnType PwmIf_SetStartLevel(uByte PwmIf_Id,bool StartLevel);
#endif
#endif /* PWMIF_H */