/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Rte_Dcm.c>
 *  @brief      <>
 *  
 *  <MCU:MC9S12_XEP100>
 *  
 *  @author     <>
 *  @date       <2017-03-07 10:58:06>
 */
/*============================================================================*/


/****************************** references *********************************/
#include "Rte_Dcm.h"
#include "Dem.h"
#include "Std_ExtendedTypes.h"

#include "BatterySt.h"
#include "BatterySysSt.h"
#include "BmssysSt.h"

/* EEPROM/FLASH */
uint8  Dataf10d[3]  = {24,0,0};    /* 诊断规范版本 */
uint8  Dataf18c[17] = {0x0};    /* ECU序列号 */
uint8  Dataf199[4]  = {0x20,0x17,0x05,0x06};    /* 重编程日期 */
uint8  Dataf187[14] = {0x0};    /* 零件号 */
uint8  Dataf180[17] = {0x0};    /* boot软件标识 */
uint8  Dataf189[17] = {0x0};    /* ECU软件版本号 */
uint8  Dataf17f[17] = {0x0};    /* ECU硬件版本号 */
uint8  Dataf184[10] = {0x0};    /* 指纹数据 */
uint8  Data0201[2]  = {0x0};    /* 重编程次数计数器 */
uint8  Data0200[2]  = {0x0};    /* 重编程成功次数计数器 */
uint8  Data0110[1]  = {0x0};    /* 此处Data0110[1]由用户修改为代表工厂模式的变量 */

/* RAM variables */
uint8  Dataf186[1]  = {0x0};    /* 诊断会话模式 */
uint8  Data1000[1]  = {0x0};    /* 12V电池电压 */
uint8  Data1400[2]  = {0x0};    /* BMS高压电压 */
uint8  Data1404[2]  = {0x0};    /* BMS高压直流电流 */
uint8  Data1409[1]  = {0x0};    /* BMS故障级别 */
uint8  Data0505[1]  = {0x0};    /* BMS状态 */
uint8  Data141e[1]  = {0x0};    /* BMS SOC */
uint8  Data141f[1]  = {0x0};    /* BMS SOH */
uint8  Data3011[1]  = {0x0};    /* BMS慢充充电机充电状态 */
uint8  Data3012[1]  = {0x0};    /* BMS慢充充电机充电故障状态 */
uint8  Data2005[1]  = {0x0};    /* 最大峰值放电功率 */
uint8  Data2006[1]  = {0x0};    /* 最大峰值充电功率 */
uint8  Data2007[1]  = {0x0};    /* 最大连续放电功率 */
uint8  Data2008[1]  = {0x0};    /* 最大连续充电功率 */
uint8  Data2020[2]  = {0x0};    /* 最高单体电压值 */
uint8  Data2021[1]  = {0x0};    /* 最高单体电压编号 */
uint8  Data2022[2]  = {0x0};    /* 最低单体电压值 */
uint8  Data2023[1]  = {0x0};    /* 最低单体电压编号 */
uint8  Data1001[104]= {0x0};    /* 单体电压数据 */
uint8  Data2024[1]  = {0x0};    /* 电池最高温度 */
uint8  Data2025[1]  = {0x0};    /* 电池最高温度编号 */
uint8  Data2026[1]  = {0x0};    /* 电池最低温度 */
uint8  Data2027[1]  = {0x0};    /* 电池最低温度编号 */
uint8  Data2000[24] = {0x0};    /* 电池包温度数据 */
uint8  Data2009[1]  = {0x0};    /* 绝缘电阻值 */
uint8  Data2010[1]  = {0x0};    /* HVIL状态 */
uint8  Data2011[1]  = {0x0};    /* 正主继电器状态 */
uint8  Data2012[1]  = {0x0};    /* 负主继电器状态 */
uint8  Data2013[1]  = {0x0};    /* 预充电继电器状态 */
uint8  Data2014[1]  = {0x0};    /* 慢充充电机继电器状态 */
uint8  Data2015[2]  = {0x0};    /* 最大可允许的充电机充电电压 */
uint8  Data2016[2]  = {0x0};    /* 最大可允许的充电机充电电流 */
uint8  Data2017[1]  = {0x0};    /* BMS充电机控制使能状态 */
uint8  Data2018[1]  = {0x0};    /* BMS emergency线状态 */
uint8  Data2019[1]  = {0x0};    /* PTC继电器状态 */

static uint32 seedData= 0x12345678;

/********************* callback function for ECU Reset********************/
Std_ReturnType Rte_EcuReset(uint8 ResetType, Dcm_NegativeResponseCodeType*UDS_FAR ErrorCode)
{
    if(ResetType == 0x01)   /* HW reset */
    {
        ;      
    } 
    else if(ResetType == 0x03)     /* SW reset */
    {
        ;
    }
    return E_OK;
}


/**********call back functions called by DCM when a service request received******/
Std_ReturnType Rte_Indication1(uint8 SID, uint8 *UDS_FAR RequestData, uint16 DataSize)
{
    return E_OK;
}


/*******function for Protocol Start and stop***********/
Std_ReturnType  StartProtocol(Dcm_ProtocolType  ProtocolID)
{
	  return E_OK;
}

Std_ReturnType  StopProtocol(Dcm_ProtocolType  ProtocolID)
{
	  return E_OK;
}

#include "EE.h"
uint8 Fl_BootMode[4] = {0};
/*************functions for session control service*****************/
Std_ReturnType GetSesChgPermission(Dcm_SesType SesCtrlTypeActive, Dcm_SesType SesCtrlTypeNew)
{
      
      if(((0x03 == SesCtrlTypeActive) || (0x02 == SesCtrlTypeActive)) && (0x02 == SesCtrlTypeNew))
      {

           uint8  flag = 0xB5 ;
               
      	/*接收到编程会话请求*/ 
         /* clear bootloader request flash and app update flag */
         //(void)Eeprom_Erase(FL_BOOT_MODE, 4uL); 
         //(void)Eeprom_Write (FL_BOOT_MODE,1uL,&flag);  
         (void)SysConf_InitParaTable(FL_BOOT_MODE, (uLWord)(&Fl_BootMode), sizeof(Fl_BootMode));
         Fl_BootMode[0] = 0xA5;
         Fl_BootMode[1] = 0xA5;
         Fl_BootMode[2] = 0xA5;
         Fl_BootMode[3] = 0xA5;
         
         (void)EE_SaveVar(CPUEE,FL_BOOT_MODE);
         
         #if 1//0      /* 如果应用回复50 02，则不需要下面这段程序 */
         return E_FORCE_RCRRP;/* 78 pending app 不回复50 02 ，等待boot来回 在boot中要模拟接收10 02 */
         #else
         return E_OK;     /* 应用回复50 02 */
         #endif
            	  	
      }
      
      
    /* 02 DCM_PROGRAMMING_SESSION can't change to 03DCM_EXTENDED_DIAGNOSTIC_SESSION */
    if((0x02 == SesCtrlTypeActive) && (0x03 == SesCtrlTypeNew)) 
    {
       return E_SESSION_NOT_ALLOWED;
    }
    
    /* 01 can't change to 02*/
    if((0x01 == SesCtrlTypeActive) && (0x02 == SesCtrlTypeNew)) 
    {
       return E_SESSION_NOT_ALLOWED;
    }
	  return E_OK;
}


Std_ReturnType ChangeIndication(Dcm_SesType SesCtrlTypeActive, Dcm_SesType SesCtrlTypeNew)
{
	  return E_OK;
}


/*************callback functions for DID services*************/

Std_ReturnType Rte_DidReadData_f10d(uint8*UDS_FAR data)      /* 诊断规范版本 */
{
    uint8  i;

    /*Dem_SetEventStatus(6,DEM_EVENT_STATUS_FAILED);
    Dem_SetEventStatus(7,DEM_EVENT_STATUS_FAILED); */
            
    for(i = 0; i < 3; i++)
    {
        data[i] = Dataf10d[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_f18c(uint8*UDS_FAR data)       /* ECU序列号 */
{
    uint8  i;

    for(i = 0; i < 17; i++)
    {
        data[i] = Dataf18c[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_f199(uint8*UDS_FAR data)       /* 重编程日期 */
{
    uint8  i;
    /*Dem_SetEventStatus(6,DEM_EVENT_STATUS_PASSED);
    Dem_SetEventStatus(7,DEM_EVENT_STATUS_PASSED); */
    for(i = 0; i < 4; i++)
    {
        data[i] = Dataf199[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_f187(uint8*UDS_FAR data)        /* 零件号 */
{
    uint8  i;

    for(i = 0; i < 14; i++)
    {
        data[i] = Dataf187[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_f180(uint8*UDS_FAR data)     /* boot软件标识 */
{
    uint8  i;

    for(i = 0; i < 17; i++)
    {
        data[i] = Dataf180[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_f189(uint8*UDS_FAR data)     /* ECU软件版本号 */
{
    uint8  i;

    for(i = 0; i < 17; i++)
    {
        data[i] = Dataf189[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_f17f(uint8*UDS_FAR data)     /* ECU硬件版本号 */
{
    uint8  i;

    for(i = 0; i < 17; i++)
    {
        data[i] = Dataf17f[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_f184(uint8*UDS_FAR data)       /* 指纹数据 */
{
    uint8  i;

    for(i = 0; i < 10; i++)
    {
        data[i] = Dataf184[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_0201(uint8*UDS_FAR data)        /* 重编程次数计数器 */
{
    uint8  i;
    /*Dem_SetOperationCycleState(DEM_OPCYC_IGNITION, DEM_CYCLE_STATE_END);
    Dem_Shutdown(); */
    for(i = 0; i < 2; i++)
    {
        data[i] = Data0201[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_0200(uint8*UDS_FAR data)        /* 重编程成功次数计数器 */
{
    uint8  i;

    for(i = 0; i < 2; i++)
    {
        data[i] = Data0200[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_0110(uint8*UDS_FAR data)    /* 此处Data0110[1]由用户修改为代表工厂模式的变量 */
{
    uint8  i;

    for(i = 0; i < 1; i++)
    {
        data[i] = Data0110[i];
    }

		return E_OK;
}

extern uint8 DslInternal_GetSesCtrlType(void);
Std_ReturnType Rte_DidReadData_f186(uint8*UDS_FAR data)   /* 诊断会话模式 */
{
    uint8  i;
    Dataf186[0] = DslInternal_GetSesCtrlType();
    for(i = 0; i < 1; i++)
    {
        data[i] = Dataf186[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_1000(uint8*UDS_FAR data)  /* 12V电池电压 */
{
    uint8  i;
    uWord  lwd_sumv;

    lwd_sumv = BmsSys_GetK30Volt();
    Data1000[0] = (uint8)lwd_sumv;
    
    for(i = 0; i < 1; i++)
    {
        data[i] = Data1000[i];
    }

	  return E_OK;
}


Std_ReturnType Rte_DidReadData_1400(uint8*UDS_FAR data)  /* BMS高压电压 */
{
    uint8  i;
    uWord  lwd_sumv;
    
    lwd_sumv = Bat_GetSumV(INT_SUMV);
    Data1400[0] = (lwd_sumv>>8)&0xff; 
    Data1400[1] = (uint8)(lwd_sumv&0xff); 

    for(i = 0; i < 2; i++)
    {
        data[i] = Data1400[i];
    }

	  return E_OK;
}


Std_ReturnType Rte_DidReadData_1404(uint8*UDS_FAR data)  /* BMS高压直流电流 */
{
    uint8  i;
    uWord  lwd_cur;
    
    lwd_cur = Bat_GetCurrent(INSCUR);
    Data1404[0] = (lwd_cur>>8)&0xff; 
    Data1404[1] = (uint8)(lwd_cur&0xff); 

    for(i = 0; i < 2; i++)
    {
        data[i] = Data1404[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_1409(uint8*UDS_FAR data)   /* BMS故障级别 */
{
    uint8  i;

    for(i = 0; i < 1; i++)
    {
        data[i] = Data1409[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_0505(uint8*UDS_FAR data)     /* BMS状态 */
{
    uint8  i;

    for(i = 0; i < 1; i++)
    {
        data[i] = Data0505[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_141e(uint8*UDS_FAR data)     /* BMS SOC */
{
    uint8  i;
    uWord  lwd_soc;
    
    lwd_soc = Bat_GetSOC();
    lwd_soc /=4; //0.4%
    Data141e[0] = (uByte)lwd_soc;  //soc 
    

    for(i = 0; i < 1; i++)
    {
        data[i] = Data141e[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_141f(uint8*UDS_FAR data)     /* BMS SOH */
{
    uint8  i;
    uWord  lwd_soh;
    
    lwd_soh = Bat_GetSoh();
    Data141f[0] = (lwd_soh>>8)&0xff; 
    Data141f[0] = (uint8)(lwd_soh&0xff); 

    for(i = 0; i < 1; i++)
    {
        data[i] = Data141f[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_3011(uint8*UDS_FAR data)      /* BMS慢充充电机充电状态 */
{
    uint8  i;

    for(i = 0; i < 1; i++)
    {
        data[i] = Data3011[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_3012(uint8*UDS_FAR data)      /* BMS慢充充电机充电故障状态 */
{
    uint8  i;

    for(i = 0; i < 1; i++)
    {
        data[i] = Data3012[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_2005(uint8*UDS_FAR data)        /* 最大峰值放电功率 */
{
    uint8  i;
    uWord  lwd_temp;
    
    lwd_temp = Bat_GetMaxPulseDchPower();
    Data2005[0] = (lwd_temp>>8)&0xff; 
    Data2005[1] = (uint8)(lwd_temp&0xff);

    for(i = 0; i < 1; i++)
    {
        data[i] = Data2005[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_2006(uint8*UDS_FAR data)        /* 最大峰值充电功率 */
{
    uint8  i;
    

    for(i = 0; i < 1; i++)
    {
        data[i] = Data2006[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_2007(uint8*UDS_FAR data)        /* 最大连续放电功率 */
{
    uint8  i;
    uWord  lwd_temp;
    
    lwd_temp = Bat_GetMaxPermitDchPower();
    Data2007[0] = (lwd_temp>>8)&0xff; 
    Data2007[1] = (uint8)(lwd_temp&0xff);

    for(i = 0; i < 1; i++)
    {
        data[i] = Data2007[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_2008(uint8*UDS_FAR data)         /* 最大连续充电功率 */
{
    uint8  i;
    

    for(i = 0; i < 1; i++)
    {
        data[i] = Data2008[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_2020(uint8*UDS_FAR data)          /* 最高单体电压值 */
{
    uint8  i;
    uWord  lwd_temp;
    
    lwd_temp = Bat_GetMaxV();
    Data2020[0] = (lwd_temp>>8)&0xff; 
    Data2020[1] = (uint8)(lwd_temp&0xff);

    for(i = 0; i < 2; i++)
    {
        data[i] = Data2020[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_2021(uint8*UDS_FAR data)         /* 最高单体电压编号 */
{
    uint8  i;
    Word  lwd_temp;
    
    lwd_temp = Bat_GetMaxVBmuNo();
    Data2021[0] = (lwd_temp>>8)&0xff; 
    Data2021[0] = (uint8)(lwd_temp&0xff);

    for(i = 0; i < 1; i++)
    {
        data[i] = Data2021[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_2022(uint8*UDS_FAR data)         /* 最低单体电压值 */
{
    uint8  i;
    uWord  lwd_temp;
    
    lwd_temp = Bat_GetMinV();
    lwd_temp /= 50;
    Data2022[0] = (lwd_temp>>8)&0xff; 
    Data2022[1] = (uint8)(lwd_temp&0xff);
    
    for(i = 0; i < 2; i++)
    {
        data[i] = Data2022[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_2023(uint8*UDS_FAR data)          /* 最低单体电压编号 */
{
    uint8  i;
    uWord  lwd_temp;
    
    lwd_temp = Bat_GetMinVBmuNo();
    Data2023[0] = (lwd_temp>>8)&0xff; 
    Data2023[0] = (uint8)(lwd_temp&0xff);

    for(i = 0; i < 1; i++)
    {
        data[i] = Data2023[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_1001(uint8*UDS_FAR data)            /* 单体电压数据 */
{
    uint8  i,j;
    uByte bmu_num;
    uWord wcnt;
    uWord wtmp;
    uLWord lwtmp;
    
    bmu_num = Bat_GetBmuNum();
    for(i = 1,wcnt = 0; i <= 8; i++)
    {
       wtmp = Bat_GetPackSerialNum(i);
       for(j = 0; j < wtmp; j++)
      {
          lwtmp = Bat_GetCellV(i, (j + 1));

          /* cell volt */
          //Data1001[wcnt++] = (uByte)((lwtmp & 0xff00) >> 8);
          Data1001[wcnt++] = (uByte)(lwtmp & 0x00ff);
      }
    }
    

    for(i = 0; i < 104; i++)
    {
        data[i] = Data1001[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_2024(uint8*UDS_FAR data)            /* 电池最高温度 */
{
    uint8  i;
    
    Data2024[0] = Bat_GetMaxT();

    for(i = 0; i < 1; i++)
    {
        data[i] = Data2024[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_2025(uint8*UDS_FAR data)          /* 电池最高温度编号 */
{
    uint8  i;
    uWord  lwd_temp;
    
    lwd_temp = Bat_GetMaxTTotalNo();
    Data2025[0] = (lwd_temp>>8)&0xff; 
    Data2025[0] = (uint8)(lwd_temp&0xff);

    for(i = 0; i < 1; i++)
    {
        data[i] = Data2025[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_2026(uint8*UDS_FAR data)          /* 电池最低温度 */
{
    uint8  i;
    
    Data2026[0] = Bat_GetMinT();

    for(i = 0; i < 1; i++)
    {
        data[i] = Data2026[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_2027(uint8*UDS_FAR data)          /* 电池最低温度编号 */
{
    uint8  i;
    uWord  lwd_temp;
    
    lwd_temp = Bat_GetMinTTotalNo();
    Data2027[0] = (lwd_temp>>8)&0xff; 
    Data2027[0] = (uint8)(lwd_temp&0xff);

    for(i = 0; i < 1; i++)
    {
        data[i] = Data2027[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_2000(uint8*UDS_FAR data)          /* 电池包温度数据 */
{
    uint8  i,j;
    uByte bmu_num;
    uWord wcnt;
    uWord wtmp;
    uLWord lwtmp;
    
    bmu_num = Bat_GetBmuNum();
    for(i = 1,wcnt = 0; i <= 8; i++)
    {
       wtmp = Bat_GetPackTemperNum(i);
       for(j = 0; j < wtmp; j++)
      {
          Data2000[wcnt++] = Bat_GetTemper(i, (j + 1));
      }
    }

    for(i = 0; i < 24; i++)
    {
        data[i] = Data2000[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_2009(uint8*UDS_FAR data)            /* 绝缘电阻值 */
{
    uint8  i;

    for(i = 0; i < 1; i++)
    {
        data[i] = Data2009[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_2010(uint8*UDS_FAR data)           /* HVIL状态 */
{
    uint8  i;

    for(i = 0; i < 1; i++)
    {
        data[i] = Data2010[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_2011(uint8*UDS_FAR data)          /* 正主继电器状态 */
{
    uint8  i;
    
    Data2011[0] = BatSys_GetPosRlySt();

    for(i = 0; i < 1; i++)
    {
        data[i] = Data2011[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_2012(uint8*UDS_FAR data)           /* 负主继电器状态 */
{
    uint8  i;
    
    Data2012[0] = BatSys_GetNegRlySt();

    for(i = 0; i < 1; i++)
    {
        data[i] = Data2012[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_2013(uint8*UDS_FAR data)           /* 预充电继电器状态 */
{
    uint8  i;
    
    Data2013[0] = BatSys_GetPreRlySt();

    for(i = 0; i < 1; i++)
    {
        data[i] = Data2013[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_2014(uint8*UDS_FAR data)           /* 慢充充电机继电器状态 */
{
    uint8  i;
    
    Data2013[0] = BatSys_GetChgRlySt();

    for(i = 0; i < 1; i++)
    {
        data[i] = Data2014[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_2015(uint8*UDS_FAR data)            /* 最大可允许的充电机充电电压 */
{
    uint8  i;

    for(i = 0; i < 2; i++)
    {
        data[i] = Data2015[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_2016(uint8*UDS_FAR data)          /* 最大可允许的充电机充电电流 */
{
    uint8  i;

    for(i = 0; i < 2; i++)
    {
        data[i] = Data2016[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_2017(uint8*UDS_FAR data)           /* BMS充电机控制使能状态 */
{
    uint8  i;

    for(i = 0; i < 1; i++)
    {
        data[i] = Data2017[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_2018(uint8*UDS_FAR data)           /* BMS emergency线状态 */
{
    uint8  i;

    for(i = 0; i < 1; i++)
    {
        data[i] = Data2018[i];
    }

	  return E_OK;
}

Std_ReturnType Rte_DidReadData_2019(uint8*UDS_FAR data)            /* PTC继电器状态 */
{
    uint8  i;

    for(i = 0; i < 1; i++)
    {
        data[i] = Data2019[i];
    }

	  return E_OK;
}



Std_ReturnType Rte_DidWriteData_f199(uint8*UDS_FAR data, uint16 dataLength, Dcm_NegativeResponseCodeType*UDS_FAR ErrorCode)         /* 重编程日期 */
{
    uint8  i;

    for(i = 0; i < 4; i++)
    {
        Dataf199[i] = data[i];
    }
    
		return E_OK;
}

Std_ReturnType Rte_DidWriteData_f184(uint8*UDS_FAR data, uint16 dataLength, Dcm_NegativeResponseCodeType*UDS_FAR ErrorCode)          /* 指纹数据 */
{
    uint8  i;

    for(i = 0; i < 10; i++)
    {
        Dataf184[i] = data[i];
    }
    
		return E_OK;
}

Std_ReturnType Rte_DidWriteData_0110(uint8*UDS_FAR data, uint16 dataLength, Dcm_NegativeResponseCodeType*UDS_FAR ErrorCode)         /* 此处Data0110[1]由用户修改为代表工厂模式的变量 */
{
    uint8  i;

    for(i = 0; i < 1; i++)
    {
        Data0110[i] = data[i];
    }
    
		return E_OK;	
}

Std_ReturnType Rte_DidReturnControlToEcu_d001(uint8 *UDS_FAR ControlOptionRecord,  uint8 *UDS_FAR ControlEnableMaskRecord, uint8 *UDS_FAR ControlStatusRecord, Dcm_NegativeResponseCodeType  *UDS_FAR ErrorCode)
{
    return E_OK;
}


Std_ReturnType Rte_DidShortTermAdjustment_d001(uint8 *UDS_FAR ControlOptionRecord, uint8 *UDS_FAR ControlEnableMaskRecord, uint8 *UDS_FAR ControlStatusRecord, Dcm_NegativeResponseCodeType  *UDS_FAR ErrorCode)
{
		return E_OK;
}


Std_ReturnType Rte_DidFreezeCurrentState_d001(uint8 *UDS_FAR ControlOptionRecord, uint8 *UDS_FAR ControlEnableMaskRecord, uint8 *UDS_FAR ControlStatusRecord, Dcm_NegativeResponseCodeType  *UDS_FAR ErrorCode)
{
    return E_OK;
}


Std_ReturnType Rte_ResetToDefault_d001(uint8 *UDS_FAR ControlOptionRecord, uint8 *UDS_FAR ControlEnableMaskRecord, uint8 *UDS_FAR ControlStatusRecord, Dcm_NegativeResponseCodeType  *UDS_FAR ErrorCode)
{
    return E_OK;
}

/******callback functions for Routine control**********/

Std_ReturnType StartRoutine_0203(uint8*UDS_FAR InBuffer, uint8*UDS_FAR OutBuffer, Dcm_NegativeResponseCodeType*UDS_FAR ErrorCode)
{
	return E_OK;
}

Std_ReturnType RequestResults_060E(uint8*UDS_FAR OutBuffer, Dcm_NegativeResponseCodeType *UDS_FAR ErrorCode)
{
    *OutBuffer = 0x01;
    return E_OK;
}


Std_ReturnType StartRoutine_060E(uint8*UDS_FAR InBuffer, uint8*UDS_FAR OutBuffer, Dcm_NegativeResponseCodeType*UDS_FAR ErrorCode)
{  
    *OutBuffer = *InBuffer;
    return E_OK;
}


Std_ReturnType StopRoutine_060E(uint8*UDS_FAR InBuffer, uint8*UDS_FAR OutBuffer, Dcm_NegativeResponseCodeType*UDS_FAR ErrorCode)
{
	return E_OK;
}


/*******function for security access***********/
Std_ReturnType  Rte_GetSeed1(uint8 *UDS_FAR SecurityAccessRecord,uint8 *UDS_FAR Seed,Dcm_NegativeResponseCodeType *UDS_FAR ErrorCode)
{
  	Seed[0] = (uint8)(seedData >> 24);
  	Seed[1] = (uint8)(seedData >> 16);
  	Seed[2] = (uint8)(seedData >> 8);
  	Seed[3] = ((uint8)(seedData));
    return E_OK;
}

Std_ReturnType  Rte_GetSeed2(uint8 *UDS_FAR SecurityAccessRecord,uint8 *UDS_FAR Seed,Dcm_NegativeResponseCodeType *UDS_FAR ErrorCode)
{
  	Seed[0] = (uint8)(seedData >> 24);
  	Seed[1] = (uint8)(seedData >> 16);
  	Seed[2] = (uint8)(seedData >> 8);
  	Seed[3] = ((uint8)(seedData));
    return E_OK;
}

Std_ReturnType  Rte_GetSeed9(uint8 *UDS_FAR SecurityAccessRecord,uint8 *UDS_FAR Seed,Dcm_NegativeResponseCodeType *UDS_FAR ErrorCode)
{
  	Seed[0] = (uint8)(seedData >> 24);
  	Seed[1] = (uint8)(seedData >> 16);
  	Seed[2] = (uint8)(seedData >> 8);
  	Seed[3] = ((uint8)(seedData));
    return E_OK;
}


Std_ReturnType  Rte_CompareKey1(uint8 *UDS_FAR key)
{
    uint8 data0;
    uint8 data1;
    uint8 data2;
    uint8 data3;    
    uint32 lock = 0;
    
    lock = (seedData^0xA5CEFDB6UL)+0xA5CEFDB6UL;
    
    data0 = (uint8)(lock >> 24);
    data1 = (uint8)(lock >> 16);
    data2 = (uint8)(lock >> 8);
    data3 = (uint8)(lock);
    
    if((data0 != key[0]) ||(data1 != key[1])||(data2 != key[2])||(data3 != key[3]))
    {
        return E_COMPARE_KEY_FAILED;
    }
    return E_OK;
}

Std_ReturnType  Rte_CompareKey2(uint8 *UDS_FAR key)
{
    uint8 data0;
    uint8 data1;
    uint8 data2;
    uint8 data3;    
    uint32 lock = 0;
    
    lock = (seedData^0xA5CEFDB6UL)+0xA5CEFDB6UL;
    
    data0 = (uint8)(lock >> 24);
    data1 = (uint8)(lock >> 16);
    data2 = (uint8)(lock >> 8);
    data3 = (uint8)(lock);
    
    if((data0 != key[0]) ||(data1 != key[1])||(data2 != key[2])||(data3 != key[3]))
    {
        return E_COMPARE_KEY_FAILED;
    }
    return E_OK;
}

Std_ReturnType  Rte_CompareKey9(uint8 *UDS_FAR key)
{
    uint8 data0;
    uint8 data1;
    uint8 data2;
    uint8 data3;    
    uint32 lock = 0;
    
    lock = (seedData^0xA5CEFDB6UL)+0xA5CEFDB6UL;
    
    data0 = (uint8)(lock >> 24);
    data1 = (uint8)(lock >> 16);
    data2 = (uint8)(lock >> 8);
    data3 = (uint8)(lock);
    
    if((data0 != key[0]) ||(data1 != key[1])||(data2 != key[2])||(data3 != key[3]))
    {
        return E_COMPARE_KEY_FAILED;
    }
    return E_OK;
}

#if (STD_ON == DCM_FACTORY_MODE_ENABLED)
/********************************************
Initial value: 0xFF
On the production line in factory: 0xFE-0x10
Development phase: 0x0F-0x01
At the end of line in factory: 0x00
**********************************************/
Std_ReturnType Get_FactoryMode(void)
{

    if(Data0110[0] == 0x00)
    {
       return E_NOT_OK;
    }
    else
    {
       return E_OK; /*factory mode is active*/
    }
}


#endif

