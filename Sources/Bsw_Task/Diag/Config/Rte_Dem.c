/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Rte_Dem.c>
 *  @brief      <>
 *  
 *  <MCU:MC9S12_XEP100>
 *  
 *  @author     <>
 *  @date       <2017-02-25 10:48:10>
 */
/*============================================================================*/


/****************************** references *********************************/
#include "Rte_Dem.h"
#include "Dem_Dcm.h"
#include "Std_ExtendedTypes.h"

#include "BatterySt.h"
#include "BatterySysSt.h"
#include "BmssysSt.h"

uint8 Data2101[20]  = {0x0};    /* FF */
uint8 ExtendData01[2]  = {0x0};   /* Extended data */
uint8 DemOccurrenceCounter;
uint8 DemAgingCounter;

/*********callback function for Communication Control 0x85 Service*********/
void Rte_EnableAllDtcsRecord(void)
{
   /*The update of the DTC status bit information shall continue once a ControlDTCSetting request is performed
     with sub-function set to on or a session layer timeout occurs (server transitions to defaultSession. */
    (void)Dem_EnableDTCStorage(DEM_DTC_GROUP_ALL_DTCS, DEM_DTC_KIND_ALL_DTCS);
}

/*********callback function for Communication Control 0x28 Service*********/
Std_ReturnType  Rte_CommunicaitonControl(uint8  controlType,uint8 communicationType)
{
#if 0
    switch(controlType)
    {
        case 0x00:
            if(0x01 == communicationType)
            {
                Com_IpduGroupStart(0,TRUE);
                Com_IpduGroupStart(1,TRUE);
            }
            else if(0x02 == communicationType)
            {
                CanNm_EnableCommunication(0);
            }
            else if(0x03 == communicationType)
            {
                Com_IpduGroupStart(0,TRUE);
                Com_IpduGroupStart(1,TRUE);
                CanNm_EnableCommunication(0);
            }
            break;
        case 0x01:
            if(0x01 == communicationType)
            {
                Com_IpduGroupStart(0,TRUE);
                Com_IpduGroupStop(1);
            }
            break;
        case 0x02:
            if(0x01 == communicationType)
            {
                Com_IpduGroupStart(1,TRUE);
                Com_IpduGroupStop(0);
            }
            break;
        case 0x03:
            if(0x01 == communicationType)
            {
                Com_IpduGroupStop(0);
                Com_IpduGroupStop(1);
            }
            else if(0x02 == communicationType)
            {
                CanNm_DisableCommunication(0);
            }
            else if(0x03 == communicationType)
            {
                Com_IpduGroupStop(0);
                Com_IpduGroupStop(1);
                CanNm_DisableCommunication(0);
            }
            break;
        default:
            break;
    }
#endif
    return E_OK;
}

Std_ReturnType Rte_DidReadData_2101(uint8*UDS_FAR Buffer, uint16 BufSize)
{
    uint8  i, index = 0;
    uWord  lwd_temp;
    
    Data2101[index++] = 0;
    
    lwd_temp = Bat_GetSumV(INT_SUMV);
    Data2101[index++] = (lwd_temp>>8)&0xff; 
    Data2101[index++] = (uint8)(lwd_temp&0xff);
    
    lwd_temp = Bat_GetCurrent(INSCUR);
    Data2101[index++] = (lwd_temp>>8)&0xff; 
    Data2101[index++] = (uint8)(lwd_temp&0xff);
    
    Data2101[index++] = Bat_GetMaxT();
    
    Data2101[index++] = Bat_GetMinT();
    
    lwd_temp = Bat_GetSOC();
    lwd_temp /=4; //0.4%
    Data2101[index++] = (uByte)lwd_temp;  //soc 
    
    lwd_temp = Bat_GetSoh();
    Data2101[index++] = (uint8)(lwd_temp&0xff);
    
    lwd_temp = Bat_GetMinV();
    lwd_temp /= 50; 
    Data2101[index++] = (uint8)(lwd_temp&0xff);
    
    lwd_temp = Bat_GetMaxV();
    lwd_temp /= 50; 
    Data2101[index++] = (uint8)(lwd_temp&0xff); 
    
    Data2101[index++] = 0;
    
    lwd_temp = BmsSys_GetK30Volt();
    Data2101[index++] = (uint8)lwd_temp;
    
    Data2101[index++] = 0;
    
    Data2101[index++] = 0;
    
    
    

    for(i = 0; i < 20; i++)
    {
        Buffer[i] = Data2101[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadExtendData_01(uint8*UDS_FAR Buffer, uint16 BufSize)
{
    uint8  i;
    
    ExtendData01[0] = DemOccurrenceCounter;
    ExtendData01[1] = DemAgingCounter;


    for(i = 0; i < 2; i++)
    {
        Buffer[i] = ExtendData01[i];
    }

	  return E_OK;
}


/*************callback functions for DID services*************/



