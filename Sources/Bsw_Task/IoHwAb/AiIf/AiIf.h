#ifndef AIIF_H
#define AIIF_H

#include "GeneralTypeDef.h"
#include "Ecu.h"
#include "IoIf_Type.h"
#include "Ai1.h"
#include "Ai2.h"


#define  CPU_ADC_ACCURACY 4096      // 10bit AD

#define  ADC_VOLT_CRITERION  5000    // 5V

#define ENABLE 1
#define DISABLE 0

#define AIIF_MAX_CHANNEL_NUM  21
#define AIIF_MAX_AI_NUM    2

//通道号与PE生成时选择的顺序有关，禁止改动 AN0~AN15--AI1  AN16~AN23---AI2//
typedef enum 
{
   AIIF_CHG_U2_AD_NO,
   AIIF_CPUAI_DC_P2_NO,
   AIIF_CPUAI2_NO,
   AIIF_POW_VOLT_NO,
   AIIF_CPUAI_OFFBD_CHG_P_NO,
   AIIF_CPUAI_K15_NO,
   AIIF_CPUNTC4_AD_NO,
   AIIF_CPUNTC2_AD_NO,
   AIIF_SLAVER_POW_AD_NO,
   AIIF_CPUAI_AC_P3_NO,
   AIIF_CPUAI1_NO,
   AIIF_CPUAI_CAN_INH_NO,
   AIIF_CPUAI_ONBD_CHG_P_NO,
   AIIF_CPUAI_ALARM_NO,
   AIIF_CPUNTC3_AD_NO,
   AIIF_CPUNTC1_AD_NO,
   AIIF_CPUDO2_AD_NO,
   AIIF_CPUDO1_AD_NO,
   AIIF_CPU_CUR1_AD_NO,
   AIIF_CPU_CUR2_AD_NO,
   AIIF_CPU_CPUAI3_NO  
}eAiIf_ChannelIdNumType;



GeneralReturnType AiIf_MeasureChannel(uByte AiIf_ChannelId,uWord *AiIf_AdValuePtr); 

#endif /* AIIF_H */