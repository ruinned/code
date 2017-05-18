/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <EcuM.c>
 *  @brief      <EcuM>
 *  
 *  <Compiler: All>    
 *  
 *  @author     <wbn>
 *  @date       <Sep 17, 2014 - 9:44:09 AM>
 */
/*============================================================================*/

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    	<AUTHOR>    	<REVISION LOG>
 *  V1.0.0       Sep 17, 2014    wbn        Initial version
 *
 */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/


/*=======[I N C L U D E S]====================================================*/
#include "SchIf.h"
#include "Mcu.h"


/*=======[V E R S I O N  C H E C K]===========================================*/


/*=======[M A C R O S]========================================================*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/


/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/

uint8 Uds_ProCnt;
void Mcu_PortInit(void)
{
    DDRJ_DDRJ5 = 1;
    PTJ_PTJ5 = 0;
    return;
}
void Mcu_Init(void)
{
    Mcu_PortInit();
    return;
}
void Mcu_PerformReset(void)
{
   // ((void (*)())0x4000)();
   while(1);
    return;
}

void Diag_Init() 
{
  Mcu_Init();
  //ExternFlashIf_Init(); 
  Can_Init(NULL_PTR); 
  CanIfDiag_Init(NULL_PTR);
  CanSM_Init();   
  ComM_Init();
  CanTp_Init();
  Dcm_Init();
  Dem_PreInit();
  Dem_Init(NULL_PTR);
  EcuM_Init();
  (void)ComM_RequestComMode(0,COMM_FULL_COMMUNICATION);    
 // EnableInterrupts;
  CanSM_MainFunction();
  ComM_MainFunction(0u);
  Dem_SetOperationCycleState(DEM_OPCYC_IGNITION, DEM_CYCLE_STATE_START);
  Uds_ProCnt = 0;
}

void Diag_CountCbk(void)
{
    (Uds_ProCnt < 255) ? (Uds_ProCnt++) : (Uds_ProCnt = 255);
 
}

void Diag_Sch(void) 
{
    if(Uds_ProCnt >= TM_MS(10)) 
    {
      Uds_ProCnt = 0;
      Dem_MainFunction(); 
      Dcm_MainFunction(); 
      CanTp_MainFunction();                                                   	
      CanSM_MainFunction();
      ComM_MainFunction(0u);
    }
} 


/*=======[I N T E R N A L F U N C T I O N   I M P L E M E N T A T I O N S]====*/


/*=======[E N D   O F   F I L E]==============================================*/
