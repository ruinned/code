

#ifndef DOIF_TYPE_H
#define DOIF_TYPE_H

#include "GeneralTypeDef.h"
#include "Ecu.h"
#include "IoIf_Type.h"

#define DOIF_MAX_NUM  8
 
typedef enum 
{
   DOIF_CPUDO1_NO,
   DOIF_CON_NO,
   DOIF_CPU_CHG_S_NO,
   DOIF_CPU_CHG_P_NO,
   DOIF_SLAVER_POW_CTRL_NO,
   DOIF_CPUDO2_NO,
   DOIF_ISO_CTRL_POS_NO,
   DOIF_ISO_CTRL_NEG_NO,

}eDoIf_IdNumType; 

typedef enum 
{
  DOIF_NORMAL_LOGIC_TYPE,
  DOIF_ABNORMAL_LOGIC_TYPE
}eDoIf_LogicTypeType;


typedef enum 
{
   DOIF_CMD_LOW,   /* DO output value low level */
   DOIF_CMD_HIGH   /* DO output value high level */
}eDoIf_CmdType;

typedef void (* DoIf_PutValPtrType)(bool Val);

typedef struct 
{
    const eDoIf_IdNumType DoIf_IdNum;  //the property of do number
    const eDoIf_LogicTypeType DoIf_LogicType; //the logic type
    eDoIf_CmdType DoIf_Cmd;         //the property of do initial value
}sDoIf_PropertyType;  //do property

typedef struct 
{
    const DoIf_PutValPtrType  DoIf_PutValPtr; //the pointer of function to operate DO
}sDoIf_OperationType;

typedef struct 
{
    sDoIf_PropertyType *DoIf_PropertyPtr;  //the pointer of property
    const sDoIf_OperationType *DoIf_OperationPtr;  //the pointer of operation
}sDoIf_HandleType;

#endif 
 

