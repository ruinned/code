
#ifndef AIIF_TYPE_H
#define AIIF_TYPE_H

#include "GeneralTypeDef.h"
#include "Ecu.h"
#include "IoIf_Type.h"

#define ENABLE 1
#define DISABLE 0

#define AIIF_MAX_CHANNEL_NUM  17
#define AIIF_MAX_AI_NUM    2

typedef enum 
{
   AIIF_AI1,
   AIIF_AI2,
}eAiIf_AiIdNumType;  //the AI num

typedef enum 
{
   AIIF_POW_VOLT_NO,  /* in all of the boards */
   AIIF_CPUAI1_NO,   /* in all of the boards */
   AIIF_CPUAI_DC_P2_NO,
   AIIF_CPUAI_AC_P3_NO,
   AIIF_CPUAI2_NO,
   AIIF_CPUAI_CAN_INH_NO,
   AIIF_CPUAI_OFFBD_CHG_P_NO,
   AIIF_CPUAI_ONBD_CHG_P_NO,
   AIIF_CPUAI_K15_NO,
   AIIF_CPUAI_ALARM_NO,
   AIIF_ISO_NEG_NO,
   AIIF_ISO_POS_NO,
   AIIF_ISO_NEG1_NO,
   AIIF_ISO_POS1_NO,
   AIIF_SLAVER_POW_AD_NO,
   AIIF_CPUDO2_AD_NO, //just in the M_BCU_C and M_BCU_D boards
   AIIF_CPUDO1_AD_NO, /* in all of the board */
   
}eAiIf_ChannelIdNumType;

typedef enum 
{
   SINGLE,
   MEASURE
}eAiIf_MeasureTypeType;

typedef struct 
{
    const eAiIf_AiIdNumType AiIf_AiIdNum;   //the property of Ai number 
    const uByte AiIf_AdcChannelIdNum;  //the channel number in one of the ADC
    const eAiIf_ChannelIdNumType AiIf_ChannelIdNum;  //the channel number in all of the ADC
    const eAiIf_MeasureTypeType AiIf_MeasureType; //the measure mode
}sAiIf_PropertyType;

typedef uByte (*AiIf_EnableTypePtr)(void);   //enalbe adc module
typedef uByte (*AiIf_DisableTypePtr)(void);   //disable adc module
typedef uByte (*AiIf_MeasureChanWaitTypePtr)(uByte Channel); //start the channel measure
typedef uByte (*AiIf_GetChanValueTypePtr)(uByte Channel,void* Value);  //get the channel ad value

typedef struct 
{
    AiIf_EnableTypePtr  AiIf_EnablePtr; 
    AiIf_DisableTypePtr AiIf_DisablePtr;
    AiIf_MeasureChanWaitTypePtr AiIf_MeasureChanWaitPtr;
    AiIf_GetChanValueTypePtr AiIf_GetChanValuePtr;
}sAiIf_OperationType;

typedef struct 
{
    const sAiIf_PropertyType *AiIf_PropertyPtr;   //property pointer  
    const sAiIf_OperationType *AiIf_OperationPtr;  //operation pointer
}sAiIf_HandleType;

#endif 