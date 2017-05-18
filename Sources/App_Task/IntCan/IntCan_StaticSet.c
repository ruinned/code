
#include "CanIf_Type.h"
#include "Ecu.h" 
#include "EcuConf.h"
#include "GeneralTypeDef.h"
#include "IntCan_StaticSet.h"
#include "ErrorMacro.h"

#define FLAG_ENABLE 1
#define FLAG_DISABLE 0


const sIntCan_StaticSetType IntCan_StaticSet = 
{

   INT_CAN_BAUDRATE,
   AH_SELF_DEFINE_EN,
   AH_SELF_DEFINE,
   SUMV_TYPE,
   TEST_FRAME1_CYCLE,
   TEST_FRAME2_CYCLE,
   SELF_RECEIVE_MSG_EN,
   SELF_RECV_MSG0_ID,
   SELF_RECV_MSG1_ID,
   SELF_RECV_MSG2_ID,
   ERR_CNT_CAN4_L3
   
};


