#include "AiIf_Type.h"
#include "AI1.h"
#include "AI2.h"


const sAiIf_PropertyType AiIf_Property[] =  //initial property pointer
{
  {
     AIIF_AI1,
     0,
     AIIF_POW_VOLT_NO,       //in all of the board
     SINGLE,
  },
  {
     AIIF_AI1,
     1,
     AIIF_CPUAI1_NO,      //in all of the board
     SINGLE,
  },
  {
     AIIF_AI1,
     2,
     AIIF_CPUAI_DC_P2_NO,
     SINGLE,
  },
  {
     AIIF_AI1,
     3,
     AIIF_CPUAI_AC_P3_NO,
     SINGLE,
  },
  {
     AIIF_AI1,
     4,
     AIIF_CPUAI2_NO,
     SINGLE,
  },
  {
     AIIF_AI1,
     5,
     AIIF_CPUAI_CAN_INH_NO,
     SINGLE,
  },
  {
     AIIF_AI1,
     6,
     AIIF_CPUAI_OFFBD_CHG_P_NO,
     SINGLE,
  },
  {
     AIIF_AI1,
     7,
     AIIF_CPUAI_ONBD_CHG_P_NO,
     SINGLE,
  },
  {
     AIIF_AI1,
     8,
     AIIF_CPUAI_K15_NO,
     SINGLE,
  },
  {
     AIIF_AI1,
     9,
     AIIF_CPUAI_ALARM_NO,
     SINGLE,
  },
  {
     AIIF_AI1,
     10,
     AIIF_ISO_NEG_NO,
     SINGLE,
  },
  {
     AIIF_AI1,
     11,
     AIIF_ISO_POS_NO,
     SINGLE,
  },
  {
     AIIF_AI1,
     12,
     AIIF_ISO_NEG1_NO,
     SINGLE,
  },
  {
     AIIF_AI1,
     13,
     AIIF_ISO_POS1_NO,
     SINGLE,
  },
  {
     AIIF_AI1,
     14,
     AIIF_SLAVER_POW_AD_NO,
     SINGLE,
  },
  {
     AIIF_AI2,
     0,
     AIIF_CPUDO2_AD_NO,
     SINGLE,
  }, 
  {
     AIIF_AI2,
     1,
     AIIF_CPUDO1_AD_NO,
     SINGLE,
  },
  
};

const sAiIf_OperationType AiIf_Operation[] =  //initial operation pointer
{ 
  {
    AI1_Enable,
    AI1_Disable,
    AI1_MeasureChanWait,
    AI1_GetChanValue,
  },
  {
    AI2_Enable,
    AI2_Disable,
    AI2_MeasureChanWait,
    AI2_GetChanValue,
  }
};

const sAiIf_HandleType AiIf_Handle = 
{ 
   AiIf_Property,
   AiIf_Operation, 
};