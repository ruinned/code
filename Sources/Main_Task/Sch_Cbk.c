// Sch_Cbk.c
#include "Sch_Include.h"

//#include "CellvIf.h"

#include "Timer.h"

#include "Mcu.h"

#include "OS.h"

#ifdef __DEBUG_TASK
void Debug_InitTask(void);
#endif



/*
 *************************** Product Code Begin ************************************
 */
 
void Sch_Init(void);

extern volatile bool Timer_EnEvent;           /* Enable/Disable events */

void Sch_Init(void)
{
   Timer_EnableEvent();
   (void)Timer_Enable();
   __EI();
} 
void Sch_CounterCallback(void)
{
    // place timer count in here
    Wdgm_CountCbk();  
    EE_MainCountCbk();
    Err_Count();
    Bat_CountCbk();     
    IntCan_CountCbk();
    SupplyPowDet_CountCbk();
    LvM_CountCbk();
    BalM_MainCountCbk();
    ThM_MainCountCbk();    
    
    CurDet_Count();
    SumvDet_MainCountCbk();  
    ChgCan_CountCbk(); 
    SOC_MainCountCbk();  

    IsoDet_MainCountCbk();
    DataStore_MainCountCbk(); 
    CarCan_CountCbk();
    ChgM_MainCountCbk();
    SOF_MainCountCbk();
    HvM_MainCountCbk();
    SOH_MainCountCbk();
    IntRes_MainCountCbk();
    Diag_CountCbk();
    
    CellvDetDet_MainCountCbk(); 
     
    Prj_MainCounterCbk(); 
    
}



void Sch_TimerCallback(void)
{
    // place timer tasks in here
    // call cycle : 5ms 
            
   ChgCan_MainSendMsg();
     
   CarCan_MainSendMsg();
     
   Actor_RelayMain(); 
   
    
}

void Sch_MainInit(void)
{
    // place product initial in here   
   #ifndef __DEBUG_DISABLE_PRODUCT_TASK 
      
    //Prj_PreMainInit();
    LvM_Init();
    Wdgm_Init();          
    EE_Init(); 
    Err_Init();
    SumvDet_Init();
    SysConf_Init();
    SupplyPowDet_Init();
    Actor_RelayInit(); 
    Bat_StatusInit();   
    Bms_SysStInit();
    BatSys_StInit();
    IntCan_Init(); 
    DataStore_Init();       
    BalM_Init();
    ThM_Init();
    
    CellvDetDet_Init();
          
    CurDet_Init();
    
    ChgInfo_Init();
    ChgCan_Init();
    ChgM_Init();
    SOC_Init(); 

    IsoDet_Init();
    ClockIf_Init();
    CarCan_Init();       
    
    SOF_Init();
    SOH_Init();
    IntRes_Init();
    IocIf_Init(); 

    RumMode_Init();
      
   // Prj_PosMainInit();
    Diag_Init();
   
#ifdef ECU_M_ENABLE_XGATE   
    // setup xgate
    Xgate_SetupAndStart();
#endif   
 
    Sch_Init(); 
   #endif /* End of  product task initial, DON't PLACE Product code below */  
   /* NOTE: HERE NO CODE!!! */
   
   /* Use for debug, do not touch */ 
   #ifdef __DEBUG_TASK
   Debug_InitTask();
   #endif     
}

void Sch_MainCallback(void)
{
    /* Add Other task here */  

    EE_MainCbk();
    Err_Main();

    Bat_Main();
    
    IntCan_MainProcess();
   
    IntCan_MainReceiveMsg();
    LvM_ProcMain();

    SupplyPowDet_ProcMain();
    BalM_MainCbk();
    ThM_MainCbk(); 
      
    if(0 == IntCan_GetTestModeSt())
    {
        SumvDet_MainCbk();
        CurDet_Main();
        IsoDet_MainCbk(); 
        
        CellvDet_MainCbk();  
    }
    ChgM_MainCbk();
    ChgCan_MainLoadData();
    ChgCan_MainReceiveMsg();
    SOC_MainCbk(); 

    DataStore_MainCbk();  
     
    CarCan_ProMain();
    CarCan_MainReceiveMsg();
    SOF_MainCbk();
    HvM_MainCbk();
    SOH_MainCbk();
    IntRes_MainCbk();  
    Diag_Sch();    
 
    //Prj_MainProCbk();  
 
    /* Wdg Task */
    Wdgm_MainTrig();       
}


/*
 *************************** Product Code End ************************************
 */
 
/*
  
 */

/*
 *************************** Debug Code Begin************************************
 */

/*
 * If you need test, please open __DEBUG __DEBUG_TASK and __DEBUG_DISABLE_PRODUCT_TASK
 * ,then place test task below,
 */ 
#ifdef __DEBUG_TASK
#include "CurDet.h"
#include "EE.h"
void Debug_InitTask(void)
{  /* This function equal  Sch_MainInit() for debug */
    /* add Test Code here */
    uByte  data[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    uByte  data_bak[16],len;
    Wdgm_Init();     
    SysConf_Init(); 
    EE_Init();   
   // SysConf_UpdataParaToRam(SYSCONF_PRODUCTNO,data);
   // SysConf_GetParaValueFromRam(SYSCONF_PRODUCTNO,data_bak,&len);
    Bat_StatusInit(); 
  //  Bat_UpdateSumV(INT_SUMV,0x1234); 
     CurDet_Init();
    Sch_Init();
}
void Debug_MainTask(void)
{   /* This function equal Sch_MainCallback() for debug */      

    Wdgm_MainTrig();     
    #ifdef __DEBUG_TEST_MAIN_FUNC_TIME
    Wdgm_Single_trig();
    #endif  
    
     
}

void Debug_TimerTask(void)
{  /* This function equal Sch_CounterCallback() and  Sch_TimerCallback() for debug */
    CurDet_Count();
    
    Wdgm_CountCbk(); 
}
#endif

/*
 *************************** Debug Code End************************************
 */
 
 
 