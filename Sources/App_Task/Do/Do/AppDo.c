
#include "Ecu.h"
#include "DoIf.h"
#include "AiIf.h"
#include "AppDo.h"


#define DO_HIHG_LEVEL_VOLT_VALUE 100
#define  DO_MAX_NUM  2


typedef struct
{
    uByte DoId;
    eActor_DoStatusType DoExcuSt;
} sActor_DoExcuxeType;


sActor_DoExcuxeType Actor_DoExcuxe[] =
{
    {(uByte)CPU_DO1_ID,DO_STATUS_LOW},

    {(uByte)CPU_DO2_ID,DO_STATUS_LOW},

};

eActor_DoStatusType Actor_GetDoSt(uByte Do_id)
{
    uWord Do_AdValue;
    eActor_DoStatusType do_st;
    
    do_st = DO_STATUS_RSV;
    Do_AdValue = 0;
    
    switch(Do_id)
    {
       case CPU_DO1_ID: 
       {
          if((uByte)CPU_DO1_ID == Actor_DoExcuxe[Do_id].DoId) 
          {  
              (void)AiIf_MeasureChannel((uByte)AIIF_CPUDO1_AD_NO,&Do_AdValue); //get the AD
          }
          if(Do_AdValue >= DO_HIHG_LEVEL_VOLT_VALUE) 
          {
              do_st = DO_STATUS_HIGH;
          } 
          else 
          {
              do_st = DO_STATUS_LOW;
          }
          break;
       }

      case CPU_DO2_ID: 
      {
          if((uByte)CPU_DO2_ID == Actor_DoExcuxe[Do_id].DoId) 
          {  
              (void)AiIf_MeasureChannel((uByte)AIIF_CPUDO2_AD_NO,&Do_AdValue);
          }
          if(Do_AdValue >= DO_HIHG_LEVEL_VOLT_VALUE) 
          {
              do_st = DO_STATUS_HIGH;
          } 
          else 
          {
              do_st = DO_STATUS_LOW;
          }
          break;
      }
      default:
      {
          break;
      }
    }
    
    return do_st;
}

GeneralReturnType Actor_DoInit(void)
{
    GeneralReturnType rslt;
    uByte i;
    
    rslt = ERR_OK;
    for(i = 0;i < DO_MAX_NUM;i++)
    {
        rslt = DoIf_Ctrl(i,DO_LOW_LEVEL_CMD);
        Actor_DoExcuxe[i].DoExcuSt = Actor_GetDoSt(i);
    }
    return rslt;
}


GeneralReturnType Actor_CtrlDo(uByte Do_id, bool Do_cmd)
{
    GeneralReturnType rslt;
    //uByte io_id;   
    rslt = ERR_OK;
    
    switch(Do_id)
    {
       case CPU_DO1_ID: 
       {
          if((uByte)CPU_DO1_ID == Actor_DoExcuxe[Do_id].DoId) 
          { 
              rslt = DoIf_Ctrl((uByte)DOIF_CPUDO1_NO,Do_cmd); 
              
          }
          break;
       }

      case CPU_DO2_ID: 
      {
          if((uByte)CPU_DO2_ID == Actor_DoExcuxe[Do_id].DoId) 
          {  
              rslt = DoIf_Ctrl((uByte)DOIF_CPUDO2_NO,Do_cmd);
          }
          break;
      }
      default:
      {
          //rslt = ERR_APPDO_ID_OVER_RANGE;
          break;
      }
    }
    return rslt;
}



