// RunModem.c
#include "RunModem.h"
#include "EcuDebug.h"
#include "BatterySysst.h"



eRunMode_ModeType RumMode_LocalEcuMode;
/*#if((M_BCU_D == ECU_CATEGORY) || (M_BCU_C == ECU_CATEGORY))
eRunMode_ModeType RumMode_OtherEcuMode[MAX_BMU_NUM];
#else
eRunMode_ModeType RumMode_BcuMode;
#endif */

void RumMode_Init(void)
{
    uWord i;  
    RumMode_LocalEcuMode = RUNMODE_DEFAULT;   
/*#if((M_BCU_D == ECU_CATEGORY) || (M_BCU_C == ECU_CATEGORY))    
    RumMode_LocalEcuMode = RUNMODE_DEFAULT;       
   
    for(i = 0; i < MAX_BMU_NUM; i++)
    {
        RumMode_OtherEcuMode[i] = RUNMODE_DEFAULT;    
    } 
#else
   
    RumMode_BcuMode = RUNMODE_DEFAULT    

#endif*/  
    (void)RunModem_SwitchModeTo(RUNMODE_BMS_INIT);
    (void)RunModem_SwitchModeTo(RUNMODE_SYS_WAIT);
}

/* switch mode interface */
#define RUNMODEM_CHANGE_TO(mode) {RumMode_LocalEcuMode = (mode);return ERR_OK;}
GeneralReturnType RunModem_SwitchModeTo(eRunMode_ModeType eMode)
{     
   eRunMode_ModeType last_mode;
   eRunMode_ModeType next_mode;
   
   next_mode = eMode;
   last_mode = RumMode_LocalEcuMode;
   
    BatSys_UpdateRunMode((uByte)eMode);
   RUNMODEM_CHANGE_TO(eMode);
/*   
   // special mode ->
   if(RUNMODE_SHUT_DOWN == next_mode)
   {
     // all -> 12
     RUNMODEM_CHANGE_TO(next_mode);     
   }
  
   if(RUNMODE_FAULT == next_mode)
   {
      if(RUNMODE_SELF_ACTIV != last_mode)
     {
       RUNMODEM_CHANGE_TO(next_mode);     
     }
   }
  
   //  --> RUNMODE_FACTORY rsv
   if(RUNMODE_SYS_MTCE == next_mode)
   {
     RUNMODEM_CHANGE_TO(next_mode);     
   }   
   // end of special mode
   
   //RUNMODE_DEFAULT ->   
   if(RUNMODE_DEFAULT == last_mode)
   {
     // -> RUNMODE_SELF_ACTIV  0 -> 11
     if(RUNMODE_SELF_ACTIV == next_mode)
     {
       RUNMODEM_CHANGE_TO(next_mode);          
     }
     // -> RUNMODE_SELF_ACTIV 0 -> 1 
     if(RUNMODE_BMS_INIT == next_mode)
     {
       RUNMODEM_CHANGE_TO(next_mode);        
     }    
   }
   
   // RUNMODE_BMS_INIT ->
   if(RUNMODE_BMS_INIT == last_mode)
   {        
      // -> RUMMODE_BATSYS_INIT 1 -> 2
      if(RUMMODE_BATSYS_INIT == next_mode)
      {
        RUNMODEM_CHANGE_TO(next_mode);
      }
   }
   
   //  RUMMODE_BATSYS_INIT ->
   if(RUMMODE_BATSYS_INIT == last_mode)
   {           
        // 2 -> 3
        if(RUNMODE_SYS_WAIT == next_mode) 
        {
          RUNMODEM_CHANGE_TO(next_mode);
        }
        
   }
   
   //  RUNMODE_SYS_WAIT ->
   if(RUNMODE_SYS_WAIT == last_mode)
   {   
        // 3 -> 4
        if(RUNMODE_HV_ENABLE == next_mode) 
        {
          RUNMODEM_CHANGE_TO(next_mode);
        }
        
        // 3 -> 9
        if(RUNMODE_SLOW_CHG == next_mode) 
        {
          RUNMODEM_CHANGE_TO(next_mode);
        }
        
        // 3 -> 10
        if(RUNMODE_FAST_CHG == next_mode) 
        {
          RUNMODEM_CHANGE_TO(next_mode);
        }
        
        // 3 -> 7
        if(RUNMODE_DIAG == next_mode)
        {
          RUNMODEM_CHANGE_TO(next_mode);
        }
        
        // 3 -> 8
        if(RUNMODE_CALB == next_mode)
        {
          RUNMODEM_CHANGE_TO(next_mode);
        }
        
        // 3 -> 15
        if(RUNMODE_GATWAY == next_mode)
        {
          RUNMODEM_CHANGE_TO(next_mode);
        }
        
   }
   
   // RUNMODE_HV_ENABLE ->
   if(RUNMODE_HV_ENABLE == last_mode)
   {
     // 4 -> 5
     if(RUNMODE_HV_OUTPUT == next_mode)
     {
       RUNMODEM_CHANGE_TO(next_mode);
     }
     
     // 4 -> 3
     if(RUNMODE_SYS_WAIT == next_mode)
     {
       RUNMODEM_CHANGE_TO(next_mode);
     }
   }
   
   // RUNMODE_HV_OUTPUT ->
   if(RUNMODE_HV_OUTPUT == last_mode)
   {
      // 5 -> 3
      if(RUNMODE_SYS_WAIT == next_mode)
      {
         RUNMODEM_CHANGE_TO(next_mode);
      }
   }
   
   // RUNMODE_FAULT (6) cannot jump out, unless reset 
   
   // RUNMODE_DIAG ->
   if(RUNMODE_DIAG == last_mode)
   {
     // 7 -> 3
     if(RUNMODE_SYS_WAIT == next_mode)
     {
       RUNMODEM_CHANGE_TO(next_mode);
     }
   }
   
   // RUNMODE_CALB ->
   if(RUNMODE_CALB == last_mode)
   {
     // 8 -> 3
     if(RUNMODE_SYS_WAIT == next_mode)
     {
       RUNMODEM_CHANGE_TO(next_mode);
     }
   }
   
   // RUNMODE_SLOW_CHG ->
   if(RUNMODE_SLOW_CHG == last_mode)
   {
     // 9 -> 3
     if(RUNMODE_SYS_WAIT == next_mode)
     {
       RUNMODEM_CHANGE_TO(next_mode);
     }
   }
   
   // RUNMODE_FAST_CHG ->
   if(RUNMODE_FAST_CHG == last_mode)
   {
     // 10 -> 3
     if(RUNMODE_SYS_WAIT == next_mode)
     {
       RUNMODEM_CHANGE_TO(next_mode);
     }
   }
   
   // RUNMODE_SELF_ACTIV (11) cannot jump out, unless reset 
   // RUNMODE_SHUT_DOWN (12) cannot jump out, unless reset 
   
   // RUNMODE_SYS_MTCE ->
   if(RUNMODE_SYS_MTCE == last_mode)
   {
     // 13 -> 3
     if(RUNMODE_SYS_WAIT == next_mode)
     {
       RUNMODEM_CHANGE_TO(next_mode);
     }
   }
   
   // RUNMODE_FACTORY (14) rsv
   
   // RUNMODE_GATWAY ->
   if(RUNMODE_GATWAY == last_mode)
   {
     // 15 -> 3
     if(RUNMODE_SYS_WAIT == next_mode)
     {
       RUNMODEM_CHANGE_TO(next_mode);
     }
   }
*/   
    return ERR_RUNMODE_NOT_ALLOWED;
   
}
GeneralReturnType RunModem_UpdateModeFromCan(uByte bmu_no, eRunMode_ModeType eMode);

void RumMode_MainProcess(void)
{        
    BatSys_UpdateRunMode((uByte)RumMode_LocalEcuMode);  
}

static void RunMode_SynOtherEcuMod(void)
{
  // will implement at next version v1.1
}