
#include "DoIf_Type.h"

#include "CPUDO1.h"
#include "CON1.h"
#include "CPU_CHG_S.h"
#include "CPU_CHG_P.h"
#include "Slaver_Pow_Ctrl.h"
#include "CPUDO2.h"
#include "ISO_CTRL_POS.h"
#include "ISO_CTRL_NEG.h"

    
sDoIf_PropertyType DoIf_Property[DOIF_MAX_NUM] =  //initial the do property
{ 
  {
     DOIF_CPUDO1_NO,
     DOIF_ABNORMAL_LOGIC_TYPE,//DOIF_NORMAL_LOGIC_TYPE,
     DOIF_CMD_LOW,
  },
  {
     DOIF_CON_NO,
     DOIF_NORMAL_LOGIC_TYPE,
     DOIF_CMD_LOW,
  },
  {
     DOIF_CPU_CHG_S_NO,
     DOIF_NORMAL_LOGIC_TYPE,
     DOIF_CMD_LOW,
  },
  {
     DOIF_CPU_CHG_P_NO,
     DOIF_NORMAL_LOGIC_TYPE,
     DOIF_CMD_LOW,
  },
  {
     DOIF_SLAVER_POW_CTRL_NO,
     DOIF_NORMAL_LOGIC_TYPE,
     DOIF_CMD_LOW,
  },
  {
     DOIF_CPUDO2_NO,
     DOIF_ABNORMAL_LOGIC_TYPE,//DOIF_NORMAL_LOGIC_TYPE,
     DOIF_CMD_LOW,
  },
  {
     DOIF_ISO_CTRL_POS_NO,
     DOIF_NORMAL_LOGIC_TYPE,
     DOIF_CMD_LOW,
  },
  {
     DOIF_ISO_CTRL_NEG_NO,
     DOIF_NORMAL_LOGIC_TYPE,
     DOIF_CMD_LOW,
  },
};

const sDoIf_OperationType DoIf_Operation[DOIF_MAX_NUM] =  //initial the do operation pointer
{ 
   CPUDO1_PutVal,
   CON1_PutVal,
   CPU_CHG_S_PutVal,
   CPU_CHG_P_PutVal,
   Slaver_Pow_Ctrl_PutVal, 
   CPUDO2_PutVal,
   ISO_CTRL_POS_PutVal,
   ISO_CTRL_NEG_PutVal,
};

const sDoIf_HandleType DoIf_Handle = 
{
   DoIf_Property,
   DoIf_Operation
};




