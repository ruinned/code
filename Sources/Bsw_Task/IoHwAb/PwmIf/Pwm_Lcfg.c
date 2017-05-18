
#include "PwmIf_Type.h"

#ifdef ECU_M_IOHWAB_PWMIF

#include "Pwm_Relay_Pos.h"
#include "Pwm_Relay_Neg.h"
#include "Pwm_Relay_Pre.h"
#include "Pwm_Relay_Chg.h" 
    
sPwmIf_PropertyType PwmIf_Property[PWMIF_MAX_NUM] = 
{
  {
     PWMIF_RELAY_POS_NO,
     2,
     START_LEVEL_HIGH,
  },
  {
     PWMIF_RELAY_NEG_NO,
     2,
     START_LEVEL_HIGH,
  },
  {
     PWMIF_RELAY_PRE_NO,
     2,
     START_LEVEL_HIGH,
  },
  {
     PWMIF_RELAY_CHG_NO,
     2,
     START_LEVEL_HIGH,
  },  
};

const sPwmIf_OperationType PwmIf_Operation[PWMIF_MAX_NUM] = 
{ 
  {
     Pwm_Relay_Pos_Enable,
     Pwm_Relay_Pos_Disable,
     Pwm_Relay_Pos_SetRatio16,
     Pwm_Relay_Pos_SetValue,
     Pwm_Relay_Pos_ClrValue
  },
  {
     Pwm_Relay_Neg_Enable,
     Pwm_Relay_Neg_Disable,
     Pwm_Relay_Neg_SetRatio16,
     Pwm_Relay_Neg_SetValue,
     Pwm_Relay_Neg_ClrValue
  },
  {
     Pwm_Relay_Pre_Enable,
     Pwm_Relay_Pre_Disable,
     Pwm_Relay_Pre_SetRatio16,
     Pwm_Relay_Pre_SetValue,
     Pwm_Relay_Pre_ClrValue
  },
  {
     Pwm_Relay_Chg_Enable,
     Pwm_Relay_Chg_Disable,
     Pwm_Relay_Chg_SetRatio16,
     Pwm_Relay_Chg_SetValue,
     Pwm_Relay_Chg_ClrValue
  }, 
};

const sPwmIf_HandleType PwmIf_Handle = 
{
   PwmIf_Property,
   PwmIf_Operation
};
#endif



