// ExctCmd.c
#include "IntCan.h"
//#include "CanIf_Type.h"
#include "CanIf.h"
#include "CanTp.h"
//#include "CanTp_Type.h"
#include "CanCom.h"
//#include "Ecu.h"
#include "SysConf.h"
#include "AppRelay.h"
#include "ErrorAlarm.h"
#include "EE.h"
#include "version.h"
#include "SchIf.h"
//#include "EcuDebug.h"
#include "BatterySt.h"

#include "ClockIf.h"

#define GET_HIGH_BYTE(VAL) ((uByte)(VAL >> 8))
#define GET_LOW_BYTE(VAL) ((uByte)(VAL))

volatile uByte gby_i;
uByte gby_length;
volatile uWord gby_rslt[4];
volatile uWord gby_wtmp;   //,gby_wcnt;
//volatile uWord local_num;
//volatile uLWord llgby_wtmp;
//sErr_ByteNType err_type;
uByte gby_data[30];
uByte gby_data_bak[30];
volatile uByte *gby_ptr_by;  //, *gby_ptr_by_a;
sVersion_CodeType  version_code;
sActor_CtrlRelayCmdType ctrl_relay_cmd;

void WR_BCUSysconfProductNo(void)
{
    // BCU sysConf product no
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    gby_rslt[0] = EE_LoadVar(SYSCONF_PRODUCTNO);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    /*if(ERR_OK != gby_rslt[0])
    {
        return;
    }*/ 
    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSCONF_PRODUCTNO, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }

    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSCONF_PRODUCTNO, &CanTpMsgRx.Data[1]);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSCONF_PRODUCTNO);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    ECU_DELAYMS(6);
}

void WR_BCUCellvNum(void)
{
    //BMU number(1byte) & BMU0 Cell number & BMU1- BMU30 cell number
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    gby_rslt[0] = EE_LoadVar(SYSCONF_BMU_NUMBER);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    /* if(ERR_OK != gby_rslt[0])
    {
        return;
    } */
    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSCONF_BMU_NUMBER, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_data_bak[0] = CanTpMsgRx.Data[1];
    if(gby_data_bak[0] > MAX_BMU_NUM)
    {
        gby_data_bak[0] = MAX_BMU_NUM;
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSCONF_BMU_NUMBER, gby_data_bak);
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSCONF_CHIP_NUMBER, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSCONF_BMU_NUMBER);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    
    
    // CHIP NUM (2Byte)
    
    gby_rslt[0] = EE_LoadVar(SYSCONF_CHIP_NUMBER);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);


    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSCONF_CHIP_NUMBER, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[1])
    {
        return;
    }
    gby_data_bak[0] = CanTpMsgRx.Data[2];
    gby_data_bak[1] = CanTpMsgRx.Data[3];
    if((0xFF == gby_data_bak[0])&& (0xFF == gby_data_bak[1]))
    {
        gby_data_bak[0] = gby_data[0];
        gby_data_bak[1] = gby_data[1];
    }

    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSCONF_CHIP_NUMBER, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSCONF_CHIP_NUMBER);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[2])
    {
        return;
    }
    ECU_DELAYMS(6);
    //BMU0 Cell number
 /*   gby_rslt[0] = EE_LoadVar(SYSCONF_BMU0_CELLNUM);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);


    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSCONF_BMU0_CELLNUM, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[1])
    {
        return;
    }
    gby_data_bak[0] = CanTpMsgRx.Data[2];
    gby_data_bak[1] = CanTpMsgRx.Data[3];
    if((0xFF == gby_data_bak[0])&& (0xFF == gby_data_bak[1]))
    {
        gby_data_bak[0] = gby_data[0];
        gby_data_bak[1] = gby_data[1];
    }

    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSCONF_BMU0_CELLNUM, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSCONF_BMU0_CELLNUM);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[2])
    {
        return;
    }
   
  */

    //BMU1- BMU15 cell number
 /*   gby_rslt[0] = EE_LoadVar(SYSCONF_BMU_CELLNUM_G1);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);


    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSCONF_BMU_CELLNUM_G1, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[1])
    {
        return;
    }
    for(gby_i = 0; gby_i < gby_length; gby_i++)
    {
        gby_data_bak[gby_i] = CanTpMsgRx.Data[gby_i+4];

    }

    for(gby_i = 0; gby_i < 15; gby_i++)
    {
        if((0xFF == gby_data_bak[2*gby_i])&& (0xFF == gby_data_bak[2*gby_i+1]))
        {
            gby_data_bak[2*gby_i] = gby_data[2*gby_i];
            gby_data_bak[2*gby_i+1] = gby_data[2*gby_i+1];
        }
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSCONF_BMU_CELLNUM_G1, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSCONF_BMU_CELLNUM_G1);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[2])
    {
        return;
    }
 */
    //BMU16- BMU30 cell number
 /*   gby_rslt[0] = EE_LoadVar(SYSCONF_BMU_CELLNUM_G2);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);


    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSCONF_BMU_CELLNUM_G2, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[1])
    {
        return;
    }
    for(gby_i = 0; gby_i < gby_length; gby_i++)
    {
        gby_data_bak[gby_i] = CanTpMsgRx.Data[gby_i+34];
    }
    for(gby_i = 0; gby_i < 15; gby_i++)
    {
        if((0xFF == gby_data_bak[2*gby_i])&& (0xFF == gby_data_bak[2*gby_i+1]))
        {
            gby_data_bak[2*gby_i] = gby_data[2*gby_i];
            gby_data_bak[2*gby_i+1] = gby_data[2*gby_i+1];
        }
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSCONF_BMU_CELLNUM_G2, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSCONF_BMU_CELLNUM_G2);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);

    ECU_DELAYMS(6);
  */
}

void WR_BCUCellNumPerChip(void)
{
    //BMU0 Cell number per Chip
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    gby_rslt[0] = EE_LoadVar(SYSCONF_CELLNUM_PER_CHIP);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);

    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSCONF_CELLNUM_PER_CHIP, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    for(gby_i = 0; gby_i  < gby_length; gby_i++)
    {
        gby_data_bak[gby_i] = CanTpMsgRx.Data[gby_i+1];
        if(0xFF == gby_data_bak[gby_i])
        {
            gby_data_bak[gby_i] = gby_data[gby_i];
        }
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSCONF_CELLNUM_PER_CHIP, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSCONF_CELLNUM_PER_CHIP);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    ECU_DELAYMS(6);
}

void WR_BCUCellvStartNo(void)
{
    //BMU0 start cell no & // BMU1-BMU30 start cell no
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    //BMU0 start cell no
  /*  gby_rslt[0] = EE_LoadVar(SYSCONF_BMU0_START_CELLNO);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);


    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSCONF_BMU0_START_CELLNO, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[1])
    {
        return;
    }
    gby_data_bak[0] = CanTpMsgRx.Data[1];
    gby_data_bak[1] = CanTpMsgRx.Data[2];
    if((0xFF == gby_data_bak[0])&& (0xFF == gby_data_bak[1]))
    {
        gby_data_bak[0] = gby_data[0];
        gby_data_bak[1] = gby_data[1];
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSCONF_BMU0_START_CELLNO, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSCONF_BMU0_START_CELLNO);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[2])
    {
        return;
    }
    
  */

    //BMU1- BMU15 start cell no  30byte
  /*  gby_rslt[0] = EE_LoadVar(SYSCONF_BMU_START_CELLNO_G1);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);

    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSCONF_BMU_START_CELLNO_G1, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[1])
    {
        return;
    }
    for(gby_i = 0; gby_i < gby_length; gby_i++)
    {
        gby_data_bak[gby_i] = CanTpMsgRx.Data[gby_i+3];

    }
    for(gby_i = 0; gby_i < 15; gby_i++)
    {
        if((0xFF == gby_data_bak[2*gby_i])&& (0xFF == gby_data_bak[2*gby_i+1]))
        {
            gby_data_bak[2*gby_i] = gby_data[2*gby_i];
            gby_data_bak[2*gby_i+1] = gby_data[2*gby_i+1];
        }
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSCONF_BMU_START_CELLNO_G1, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSCONF_BMU_START_CELLNO_G1);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[2])
    {
        return;
    }
  */
    //BMU16- BMU30 start cell no
 /*   gby_rslt[0] = EE_LoadVar(SYSCONF_BMU_START_CELLNO_G2);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);

    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSCONF_BMU_START_CELLNO_G2, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[1])
    {
        return;
    }
    for(gby_i = 0; gby_i < gby_length; gby_i++)
    {
        gby_data_bak[gby_i] = CanTpMsgRx.Data[gby_i+33];
    }
    for(gby_i = 0; gby_i < 15; gby_i++)
    {
        if((0xFF == gby_data_bak[2*gby_i])&& (0xFF == gby_data_bak[2*gby_i+1]))
        {
            gby_data_bak[2*gby_i] = gby_data[2*gby_i];
            gby_data_bak[2*gby_i+1] = gby_data[2*gby_i+1];
        }
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSCONF_BMU_START_CELLNO_G2, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSCONF_BMU_START_CELLNO_G2);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);

    ECU_DELAYMS(6);
  */  
  
}

void WR_BCUTemperNum(void)
{
    //BMU0 temper number & BMU1- BMU30 temper number
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    //BMU0 temper number
 /*
    gby_rslt[0] = EE_LoadVar(SYSCONF_BMU0_TEMPERNUM);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);

    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSCONF_BMU0_TEMPERNUM, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[1])
    {
        return;
    }
    gby_data_bak[0] = CanTpMsgRx.Data[1];
    gby_data_bak[1] = CanTpMsgRx.Data[2];
    if((0xFF == gby_data_bak[0])&& (0xFF == gby_data_bak[1]))
    {
        gby_data_bak[0] = gby_data[0];
        gby_data_bak[1] = gby_data[1];
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSCONF_BMU0_TEMPERNUM, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSCONF_BMU0_TEMPERNUM);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[2])
    {
        return;
    }
 */
 
 
    //BMU1- BMU15 temper number
 /*   gby_rslt[0] = EE_LoadVar(SYSCONF_BMU_TEMPERNUM_G1);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);

    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSCONF_BMU_TEMPERNUM_G1, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[1])
    {
        return;
    }
    for(gby_i = 0; gby_i < gby_length; gby_i++)
    {
        gby_data_bak[gby_i] = CanTpMsgRx.Data[gby_i+3];

    }
    for(gby_i = 0; gby_i < 15; gby_i++)
    {
        if((0xFF == gby_data_bak[2*gby_i])&& (0xFF == gby_data_bak[2*gby_i+1]))
        {
            gby_data_bak[2*gby_i] = gby_data[2*gby_i];
            gby_data_bak[2*gby_i+1] = gby_data[2*gby_i+1];
        }
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSCONF_BMU_TEMPERNUM_G1, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSCONF_BMU_TEMPERNUM_G1);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[2])
    {
        return;
    }
  */
    //BMU16- BMU30 temper number
 /*   gby_rslt[0] = EE_LoadVar(SYSCONF_BMU_TEMPERNUM_G2);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);

    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSCONF_BMU_TEMPERNUM_G2, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[1])
    {
        return;
    }
    for(gby_i = 0; gby_i < gby_length; gby_i++)
    {
        gby_data_bak[gby_i] = CanTpMsgRx.Data[gby_i+33];
    }
    for(gby_i = 0; gby_i < 15; gby_i++)
    {
        if((0xFF == gby_data_bak[2*gby_i])&& (0xFF == gby_data_bak[2*gby_i+1]))
        {
            gby_data_bak[2*gby_i] = gby_data[2*gby_i];
            gby_data_bak[2*gby_i+1] = gby_data[2*gby_i+1];
        }
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSCONF_BMU_TEMPERNUM_G2, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSCONF_BMU_TEMPERNUM_G2);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);

    ECU_DELAYMS(6);
 */   
 
}

void WR_BCUTemperStartNo(void)
{
    //BMU0 temper start n0 & BMU1- BMU30 temper start n0
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    //BMU0 start temper no
  /*  gby_rslt[0] = EE_LoadVar(SYSCONF_BMU0_START_TEMPERNO);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);

    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSCONF_BMU0_START_TEMPERNO, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[1])
    {
        return;
    }
    gby_data_bak[0] = CanTpMsgRx.Data[1];
    gby_data_bak[1] = CanTpMsgRx.Data[2];
    if((0xFF == gby_data_bak[0])&& (0xFF == gby_data_bak[1]))
    {
        gby_data_bak[0] = gby_data[0];
        gby_data_bak[1] = gby_data[1];
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSCONF_BMU0_START_TEMPERNO, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSCONF_BMU0_START_TEMPERNO);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[2])
    {
        return;
    }
 */
    
    //CHIP1-CHIP15 fuse position  30byte
    gby_rslt[0] = EE_LoadVar(SYSCONF_CHIP_FUSE_POS);      //SYSCONF_BMU_START_TEMPERNO_G1
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);

    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSCONF_CHIP_FUSE_POS, gby_data,&gby_length);   //SYSCONF_BMU_START_TEMPERNO_G1
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[1])
    {
        return;
    }
    for(gby_i = 0; gby_i < gby_length; gby_i++)
    {
        gby_data_bak[gby_i] = CanTpMsgRx.Data[gby_i+3];

    }
    for(gby_i = 0; gby_i < 15; gby_i++)
    {
        if((0xFF == gby_data_bak[2*gby_i])&& (0xFF == gby_data_bak[2*gby_i+1]))
        {
            gby_data_bak[2*gby_i] = gby_data[2*gby_i];
            gby_data_bak[2*gby_i+1] = gby_data[2*gby_i+1];
        }
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSCONF_CHIP_FUSE_POS, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSCONF_CHIP_FUSE_POS);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[2])
    {
        return;
    }

    //CHIP1 - CHIP15  TEMPER NUM
   /* gby_rslt[0] = EE_LoadVar(SYSCONF_TEMPERNUM_PER_CHIP);   //   SYSCONF_BMU_START_TEMPERNO_G2
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);


    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSCONF_TEMPERNUM_PER_CHIP, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[1])
    {
        return;
    }
    for(gby_i = 0; gby_i < gby_length; gby_i++)
    {
        gby_data_bak[gby_i] = CanTpMsgRx.Data[gby_i+33];
    }
    for(gby_i = 0; gby_i < 15; gby_i++)
    {
        if((0xFF == gby_data_bak[2*gby_i])&& (0xFF == gby_data_bak[2*gby_i+1]))
        {
            gby_data_bak[2*gby_i] = gby_data[2*gby_i];
            gby_data_bak[2*gby_i+1] = gby_data[2*gby_i+1];
        }
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSCONF_TEMPERNUM_PER_CHIP, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSCONF_TEMPERNUM_PER_CHIP);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
  */
    ECU_DELAYMS(6);
}

void WR_BCUSoc(void)
{
    //BCU SOC   Remian Capacity
    volatile uLWord llwtmp;
    volatile uWord  wtmp;

    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    gby_rslt[0] = EE_LoadVar(SYSTEMST_RATED_ACTUAL_CAPACITY);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    // write soc need read capacity OK!
    if(ERR_OK != gby_rslt[0])
    {

        return;

    } 

    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSTEMST_RATED_ACTUAL_CAPACITY, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    llwtmp = 0;
    llwtmp = gby_data[0];
    llwtmp <<= 8;
    llwtmp += gby_data[1];
    llwtmp <<= 8;
    llwtmp += gby_data[2];
    llwtmp <<= 8;
    llwtmp += gby_data[3];
    wtmp = CanTpMsgRx.Data[1];
    wtmp <<= 8;
    wtmp += CanTpMsgRx.Data[2];
    
    Bat_UpdateSOC(wtmp) ;
    
    llwtmp *= wtmp;
    
    llwtmp /= 1000;
        
/*    
    gby_data_bak[3] = llwtmp;
    llwtmp >>= 8;
    gby_data_bak[2] = llwtmp;
    llwtmp >>= 8;
    gby_data_bak[1] = llwtmp;
    llwtmp >>= 8;
    gby_data_bak[0] = llwtmp;
*/    
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_REMAIN_CAPACITY_BAK_G1, (uByte *)&llwtmp);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSTEMST_REMAIN_CAPACITY_BAK_G1);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    // action result
    ECU_DELAYMS(6);
}

void WR_BCURatesCapActualCap(void)
{      
    uLWord llwtmp;
    uWord  wtmp;
    
    //Rated Capacity & ActualCapatity
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    gby_rslt[0] = EE_LoadVar(SYSTEMST_RATED_ACTUAL_CAPACITY);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    /* if(ERR_OK != gby_rslt[0])

    {

        return;

    } */

    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSTEMST_RATED_ACTUAL_CAPACITY, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    for(gby_i = 0; gby_i  < gby_length; gby_i++)
    {
        gby_data_bak[gby_i] = CanTpMsgRx.Data[gby_i+1];
    }
    if((0xFF == gby_data_bak[0])&&(0xFF == gby_data_bak[1])
            &&(0xFF == gby_data_bak[2])&&(0xFF == gby_data_bak[3]))
    {
        gby_data_bak[0] = gby_data[0];
        gby_data_bak[1] = gby_data[1];
        gby_data_bak[2] = gby_data[2];
        gby_data_bak[3] = gby_data[3];
    }
    if((0xFF == gby_data_bak[4])&&(0xFF == gby_data_bak[5])
            &&(0xFF == gby_data_bak[6])&&(0xFF == gby_data_bak[7]))
    {
        gby_data_bak[4] = gby_data[4];
        gby_data_bak[5] = gby_data[5];
        gby_data_bak[6] = gby_data[6];
        gby_data_bak[7] = gby_data[7];
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_RATED_ACTUAL_CAPACITY, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSTEMST_RATED_ACTUAL_CAPACITY);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);      
    
    ECU_DELAYMS(6);
    
    // re calculate remain capacity
    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSTEMST_RATED_ACTUAL_CAPACITY, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    
    wtmp = Bat_GetSOC();
    llwtmp = *((uLWord *)gby_data);
    
    //calculate remain capacity
    llwtmp *= wtmp;
    
    llwtmp /= 1000;
    
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_REMAIN_CAPACITY_BAK_G1, (uByte*)&llwtmp);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSTEMST_REMAIN_CAPACITY_BAK_G1);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    // action result
    ECU_DELAYMS(6);
    
    
}

void WR_BCUCfgVer(void)
{
    //CFG_Version
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    gby_rslt[0] = EE_LoadVar(SYSCONF_CFG_VER);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    /* if(ERR_OK != gby_rslt[0])

    {

        return;

    } */

    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSCONF_CFG_VER, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    for(gby_i = 0; gby_i  < gby_length; gby_i++)
    {
        gby_data_bak[gby_i] = CanTpMsgRx.Data[gby_i+1];
    }
    if((0xFF == gby_data_bak[0])&& (0xFF == gby_data_bak[1])
            &&(0xFF == gby_data_bak[2])&& (0xFF == gby_data_bak[3])
            &&(0xFF == gby_data_bak[4])&& (0xFF == gby_data_bak[5])
            &&(0xFF == gby_data_bak[6])&& (0xFF == gby_data_bak[7]))
    {
        gby_data_bak[0] = gby_data[0];
        gby_data_bak[1] = gby_data[1];
        gby_data_bak[2] = gby_data[2];
        gby_data_bak[3] = gby_data[3];
        gby_data_bak[4] = gby_data[4];
        gby_data_bak[5] = gby_data[5];
        gby_data_bak[6] = gby_data[6];
        gby_data_bak[7] = gby_data[7];
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSCONF_CFG_VER, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSCONF_CFG_VER);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    ECU_DELAYMS(6);
}

void WR_BCUCfgUpdate(void)
{
    //Config update information
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    gby_rslt[0] = EE_LoadVar(SYSCONF_CONFIG_UPDATA_INFO);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);

    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSCONF_CONFIG_UPDATA_INFO, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    for(gby_i = 0; gby_i  < gby_length; gby_i++)
    {
        gby_data_bak[gby_i] = CanTpMsgRx.Data[gby_i+1];
    }
    if((0xFF == gby_data_bak[0])&& (0xFF == gby_data_bak[1])
            &&(0xFF == gby_data_bak[2])&& (0xFF == gby_data_bak[3])
            &&(0xFF == gby_data_bak[4])&& (0xFF == gby_data_bak[5])
            &&(0xFF == gby_data_bak[6])&& (0xFF == gby_data_bak[7]))
    {
        gby_data_bak[0] = gby_data[0];
        gby_data_bak[1] = gby_data[1];
        gby_data_bak[2] = gby_data[2];
        gby_data_bak[3] = gby_data[3];
        gby_data_bak[4] = gby_data[4];
        gby_data_bak[5] = gby_data[5];
        gby_data_bak[6] = gby_data[6];
        gby_data_bak[7] = gby_data[7];
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSCONF_CONFIG_UPDATA_INFO, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSCONF_CONFIG_UPDATA_INFO);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    ECU_DELAYMS(6);
}

void WR_BCUVehiclePlate(void)
{
    //BCU Vehicle plate
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    gby_rslt[0] = EE_LoadVar(SYSTEMST_VEHICLE_PLATE);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    /* if(ERR_OK != gby_rslt[0])

    {

        return;

    } */

    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSTEMST_VEHICLE_PLATE, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    for(gby_i = 0; gby_i  < gby_length; gby_i++)
    {
        gby_data_bak[gby_i] = CanTpMsgRx.Data[gby_i+1];
    }
    if((0xFF == gby_data_bak[0])&& (0xFF == gby_data_bak[1])
            &&(0xFF == gby_data_bak[2])&& (0xFF == gby_data_bak[3])
            &&(0xFF == gby_data_bak[4])&& (0xFF == gby_data_bak[5])
            &&(0xFF == gby_data_bak[6])&& (0xFF == gby_data_bak[7]))
    {
        gby_data_bak[0] = gby_data[0];
        gby_data_bak[1] = gby_data[1];
        gby_data_bak[2] = gby_data[2];
        gby_data_bak[3] = gby_data[3];
        gby_data_bak[4] = gby_data[4];
        gby_data_bak[5] = gby_data[5];
        gby_data_bak[6] = gby_data[6];
        gby_data_bak[7] = gby_data[7];
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_VEHICLE_PLATE, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSTEMST_VEHICLE_PLATE);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    ECU_DELAYMS(6);
}

void WR_BCUSoh(void)
{
    //BCU SOH
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    gby_rslt[0] = EE_LoadVar(SYSTEMST_SOH);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    /* if(ERR_OK != gby_rslt[0])

    {

        return;

    } */

    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSTEMST_SOH, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_data_bak[0] = CanTpMsgRx.Data[1];
    gby_data_bak[1] = CanTpMsgRx.Data[2];
    if((0xFF == gby_data_bak[0]) && (0xFF == gby_data_bak[1]))
    {
        gby_data_bak[0] = gby_data[0];
        gby_data_bak[1] = gby_data[1];
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_SOH, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSTEMST_SOH);
    
    Bat_UpdateSoh(*((uWord *)gby_data_bak));
    
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    ECU_DELAYMS(6);
    CanTpMsgTx.DataLen = 3;
}


void WR_BCUFusePos(void)
{
    //BMU0 FUSE position
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    gby_rslt[0] = EE_LoadVar(SYSCONF_CHIP_FUSE_POS);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);


    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSCONF_CHIP_FUSE_POS, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    for(gby_i = 0; gby_i < gby_length; gby_i++)
    {
        gby_data_bak[gby_i] = CanTpMsgRx.Data[gby_i+1];
        if(0xFF == gby_data_bak[gby_i])
        {
            gby_data_bak[gby_i] = gby_data[gby_i];
        }
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSCONF_CHIP_FUSE_POS, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSCONF_CHIP_FUSE_POS);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    ECU_DELAYMS(6);
    CanTpMsgTx.DataLen = 3;
    
}

void WR_BCUSystemClock(void)
{
    //Read  BMS system clock
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    gby_rslt[0] = ClockIf_WriteTime((sClockIf_TimeType*)&CanTpMsgRx.Data[1]);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
}

void WR_BCUThrhdLvl1(void)
{
    //BCU Thrhd lvl1
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    gby_rslt[0] = EE_LoadVar(ERRORALARM_THRHD_LVV1);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    /* if(ERR_OK != gby_rslt[0])

    {

        return;

    } */

    gby_rslt[0] = SysConf_GetParaValueFromRam(ERRORALARM_THRHD_LVV1, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    for(gby_i = 0; gby_i  < gby_length; gby_i++)
    {
        gby_data_bak[gby_i] = CanTpMsgRx.Data[gby_i+1];
    }
    if((0xFF == gby_data_bak[0])&& (0xFF == gby_data_bak[1]))
    {
        gby_data_bak[0] = gby_data[0];
        gby_data_bak[1] = gby_data[1];
    }
    if((0xFF == gby_data_bak[2])&& (0xFF == gby_data_bak[3]))
    {
        gby_data_bak[2] = gby_data[2];
        gby_data_bak[3] = gby_data[3];
    }
    for(gby_i = 0; gby_i < 3; gby_i++)
    {
        if(0xFF == gby_data_bak[gby_i+4])
        {
            gby_data_bak[gby_i+4] = gby_data[gby_i+4];
        }
    }
    for(gby_i = 0; gby_i < 6; gby_i++)
    {
        if((0xFF == gby_data_bak[2*gby_i+7])&& (0xFF == gby_data_bak[2*gby_i+8]))
        {
            gby_data_bak[2*gby_i+7] = gby_data[2*gby_i+7];
            gby_data_bak[2*gby_i+8] = gby_data[2*gby_i+8];
        }
    }
    if(0xFF == gby_data_bak[19])
    {
        gby_data_bak[19] = gby_data[19];
    }
    if((0xFF == gby_data_bak[20])&& (0xFF == gby_data_bak[21]))
    {
        gby_data_bak[20] = gby_data[20];
        gby_data_bak[21] = gby_data[21];
    }
    if((0xFF == gby_data_bak[22])&& (0xFF == gby_data_bak[23]))
    {
        gby_data_bak[22] = gby_data[22];
        gby_data_bak[23] = gby_data[23];
    }
    if(0xFF == gby_data_bak[24])
    {
        gby_data_bak[24] = gby_data[24];
    }
    if(0xFF == gby_data_bak[25])
    {
        gby_data_bak[25] = gby_data[25];
    }
    if((0xFF == gby_data_bak[26])&& (0xFF == gby_data_bak[27]))
    {
        gby_data_bak[26] = gby_data[26];
        gby_data_bak[27] = gby_data[27];
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(ERRORALARM_THRHD_LVV1, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, ERRORALARM_THRHD_LVV1);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    ECU_DELAYMS(6);
}

void WR_BCUThrhdLvl2(void)
{
    //BCU Thrhd lvl2
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    gby_rslt[0] = EE_LoadVar(ERRORALARM_THRHD_LVV2);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    /* if(ERR_OK != gby_rslt[0])

    {

        return;

    } */

    gby_rslt[0] = SysConf_GetParaValueFromRam(ERRORALARM_THRHD_LVV2, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    for(gby_i = 0; gby_i  < gby_length; gby_i++)
    {
        gby_data_bak[gby_i] = CanTpMsgRx.Data[gby_i+1];
    }
    if((0xFF == gby_data_bak[0])&& (0xFF == gby_data_bak[1]))
    {
        gby_data_bak[0] = gby_data[0];
        gby_data_bak[1] = gby_data[1];
    }
    if((0xFF == gby_data_bak[2])&& (0xFF == gby_data_bak[3]))
    {
        gby_data_bak[2] = gby_data[2];
        gby_data_bak[3] = gby_data[3];
    }
    for(gby_i = 0; gby_i < 3; gby_i++)
    {
        if(0xFF == gby_data_bak[gby_i+4])
        {
            gby_data_bak[gby_i+4] = gby_data[gby_i+4];
        }
    }
    for(gby_i = 0; gby_i < 6; gby_i++)
    {
        if((0xFF == gby_data_bak[2*gby_i+7])&& (0xFF == gby_data_bak[2*gby_i+8]))
        {
            gby_data_bak[2*gby_i+7] = gby_data[2*gby_i+7];
            gby_data_bak[2*gby_i+8] = gby_data[2*gby_i+8];
        }
    }
    if(0xFF == gby_data_bak[19])
    {
        gby_data_bak[19] = gby_data[19];
    }
    if((0xFF == gby_data_bak[20])&& (0xFF == gby_data_bak[21]))
    {
        gby_data_bak[20] = gby_data[20];
        gby_data_bak[21] = gby_data[21];
    }
    if((0xFF == gby_data_bak[22])&& (0xFF == gby_data_bak[23]))
    {
        gby_data_bak[22] = gby_data[22];
        gby_data_bak[23] = gby_data[23];
    }
    if(0xFF == gby_data_bak[24])
    {
        gby_data_bak[24] = gby_data[24];
    }
    if(0xFF == gby_data_bak[25])
    {
        gby_data_bak[25] = gby_data[25];
    }
    if((0xFF == gby_data_bak[26])&& (0xFF == gby_data_bak[27]))
    {
        gby_data_bak[26] = gby_data[26];
        gby_data_bak[27] = gby_data[27];
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(ERRORALARM_THRHD_LVV2, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, ERRORALARM_THRHD_LVV2);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    ECU_DELAYMS(6);
}

void WR_BCUThrhdLvl3(void)
{
    //BCU Thrhd lvl3
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    gby_rslt[0] = EE_LoadVar(ERRORALARM_THRHD_LVV3);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    /* if(ERR_OK != gby_rslt[0])

    {

        return;

    } */

    gby_rslt[0] = SysConf_GetParaValueFromRam(ERRORALARM_THRHD_LVV3, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    for(gby_i = 0; gby_i  < gby_length; gby_i++)
    {
        gby_data_bak[gby_i] = CanTpMsgRx.Data[gby_i+1];
    }
    if((0xFF == gby_data_bak[0])&& (0xFF == gby_data_bak[1]))
    {
        gby_data_bak[0] = gby_data[0];
        gby_data_bak[1] = gby_data[1];
    }
    if((0xFF == gby_data_bak[2])&& (0xFF == gby_data_bak[3]))
    {
        gby_data_bak[2] = gby_data[2];
        gby_data_bak[3] = gby_data[3];
    }
    for(gby_i = 0; gby_i < 3; gby_i++)
    {
        if(0xFF == gby_data_bak[gby_i+4])
        {
            gby_data_bak[gby_i+4] = gby_data[gby_i+4];
        }
    }
    for(gby_i = 0; gby_i < 6; gby_i++)
    {
        if((0xFF == gby_data_bak[2*gby_i+7])&& (0xFF == gby_data_bak[2*gby_i+8]))
        {
            gby_data_bak[2*gby_i+7] = gby_data[2*gby_i+7];
            gby_data_bak[2*gby_i+8] = gby_data[2*gby_i+8];
        }
    }
    if(0xFF == gby_data_bak[19])
    {
        gby_data_bak[19] = gby_data[19];
    }
    if((0xFF == gby_data_bak[20])&& (0xFF == gby_data_bak[21]))
    {
        gby_data_bak[20] = gby_data[20];
        gby_data_bak[21] = gby_data[21];
    }
    if((0xFF == gby_data_bak[22])&& (0xFF == gby_data_bak[23]))
    {
        gby_data_bak[22] = gby_data[22];
        gby_data_bak[23] = gby_data[23];
    }
    if(0xFF == gby_data_bak[24])
    {
        gby_data_bak[24] = gby_data[24];
    }
    if(0xFF == gby_data_bak[25])
    {
        gby_data_bak[25] = gby_data[25];
    }
    if((0xFF == gby_data_bak[26])&& (0xFF == gby_data_bak[27]))
    {
        gby_data_bak[26] = gby_data[26];
        gby_data_bak[27] = gby_data[27];
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(ERRORALARM_THRHD_LVV3, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, ERRORALARM_THRHD_LVV3);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    ECU_DELAYMS(6);
}

void WR_BCUHeatControl(void)
{
    //Heat control(fan/warm close&open)
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    gby_rslt[0] = EE_LoadVar(THM_HEAT_MANAGE_PARA);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    /* if(ERR_OK != gby_rslt[0])

    {

        return;

    } */

    gby_rslt[0] = SysConf_GetParaValueFromRam(THM_HEAT_MANAGE_PARA, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    for(gby_i = 0; gby_i  < gby_length; gby_i++)
    {
        gby_data_bak[gby_i] = CanTpMsgRx.Data[gby_i+1];
        if(0xFF == gby_data_bak[gby_i])
        {
            gby_data_bak[gby_i] = gby_data[gby_i];
        }
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(THM_HEAT_MANAGE_PARA, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, THM_HEAT_MANAGE_PARA);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    ECU_DELAYMS(6);
}

void WR_BCUChargeControl(void)
{
    //charge control parameter
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    gby_rslt[0] = EE_LoadVar(SYSTEMST_CHG_CTRL_PARA);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    /* if(ERR_OK != gby_rslt[0])

    {

        return;

    } */

    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSTEMST_CHG_CTRL_PARA, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    for(gby_i = 0; gby_i  < gby_length; gby_i++)
    {
        gby_data_bak[gby_i] = CanTpMsgRx.Data[gby_i+1];
    }
    for(gby_i = 0; gby_i < 3; gby_i++)
    {
        if((0xFF == gby_data_bak[2*gby_i])&& (0xFF == gby_data_bak[2*gby_i+1]))
        {
            gby_data_bak[2*gby_i] = gby_data[2*gby_i];
            gby_data_bak[2*gby_i+1] = gby_data[2*gby_i+1];
        }
    }
    if(0xFF == gby_data_bak[6])
    {
        gby_data_bak[6] = gby_data[6];
    }
    for(gby_i = 0; gby_i < 3; gby_i++)
    {
        if((0xFF == gby_data_bak[2*gby_i+7])&& (0xFF == gby_data_bak[2*gby_i+8]))
        {
            gby_data_bak[2*gby_i+7] = gby_data[2*gby_i+7];
            gby_data_bak[2*gby_i+8] = gby_data[2*gby_i+8];
        }
    }
    if(0xFF == gby_data_bak[13])
    {
        gby_data_bak[13] = gby_data[13];
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_CHG_CTRL_PARA, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSTEMST_CHG_CTRL_PARA);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    ECU_DELAYMS(6);
}


void WR_BCUHistoryCellvInfo(void)
{
    //History Cellv Infomation    14byte
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_HSTY_CELLV_INFO, &CanTpMsgRx.Data[1]);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  EE_SaveVarForCalib(BOTHEE, SYSTEMST_HSTY_CELLV_INFO);
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    }
    ECU_DELAYMS(6);
}

void WR_BCUHistoryTemperInfo(void)
{
    //History Temper Infomation   5byte
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_HSTY_TEMPER_INFO, &CanTpMsgRx.Data[1]);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  EE_SaveVarForCalib(BOTHEE, SYSTEMST_HSTY_TEMPER_INFO);
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    }
    ECU_DELAYMS(6);
}

void WR_BCUHistoryLowTemperArea(void)
{
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    // History low Temper area[0]-[6]     28byte
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_HSTY_TEMPER_AREA_INFO_G1, &CanTpMsgRx.Data[1]);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  EE_SaveVarForCalib(BOTHEE,SYSTEMST_HSTY_TEMPER_AREA_INFO_G1);
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    }

    //History low Temper area[7]-[13]    28byte

    if((ERR_OK == CanTpMsgTx.Data[1])&&(ERR_OK == CanTpMsgTx.Data[2]))
    {
        gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_HSTY_TEMPER_AREA_INFO_G2, &CanTpMsgRx.Data[29]);
        if(ERR_OK == gby_rslt[0])
        {
            gby_rslt[1] =  EE_SaveVarForCalib(BOTHEE,SYSTEMST_HSTY_TEMPER_AREA_INFO_G2);
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        }
    }

    // action result
    ECU_DELAYMS(6);
}

void WR_BCUHistoryHighTemperArea(void)
{
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    // History high Temper area[0]-[6]     28byte
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_HSTY_TEMPER_AREA_INFO_G3, &CanTpMsgRx.Data[1]);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  EE_SaveVarForCalib(BOTHEE,SYSTEMST_HSTY_TEMPER_AREA_INFO_G3);
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    }

    //History high Temper area[7]-[13]    28byte

    if((ERR_OK == CanTpMsgTx.Data[1])&&(ERR_OK == CanTpMsgTx.Data[2]))
    {
        gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_HSTY_TEMPER_AREA_INFO_G4, &CanTpMsgRx.Data[29]);
        if(ERR_OK == gby_rslt[0])
        {
            gby_rslt[1] =  EE_SaveVarForCalib(BOTHEE,SYSTEMST_HSTY_TEMPER_AREA_INFO_G4);
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        }
    }

    // action result
    ECU_DELAYMS(6);
}

void WR_BCUHistoryAvrgTemperArea(void)
{
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    // History avrg Temper area[0]-[6]     28byte
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_HSTY_TEMPER_AREA_INFO_G5, &CanTpMsgRx.Data[1]);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  EE_SaveVarForCalib(BOTHEE,SYSTEMST_HSTY_TEMPER_AREA_INFO_G5);
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    }

    //History avrg Temper area[7]-[13]   28byte

    if((ERR_OK == CanTpMsgTx.Data[1])&&(ERR_OK == CanTpMsgTx.Data[2]))
    {
        gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_HSTY_TEMPER_AREA_INFO_G6, &CanTpMsgRx.Data[29]);
        if(ERR_OK == gby_rslt[0])
        {
            gby_rslt[1] =  EE_SaveVarForCalib(BOTHEE,SYSTEMST_HSTY_TEMPER_AREA_INFO_G6);
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        }
    }

    // action result
    ECU_DELAYMS(6);
}

void WR_BCUHistoryDetTemperArea(void)
{
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    // History det Temper area[0]-[6]     28byte
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_HSTY_TEMPER_AREA_INFO_G7, &CanTpMsgRx.Data[1]);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  EE_SaveVarForCalib(BOTHEE,SYSTEMST_HSTY_TEMPER_AREA_INFO_G7);
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    }

    //History det Temper area[7]-[13]    28byte

    if((ERR_OK == CanTpMsgTx.Data[1])&&(ERR_OK == CanTpMsgTx.Data[2]))
    {
        gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_HSTY_TEMPER_AREA_INFO_G8, &CanTpMsgRx.Data[29]);
        if(ERR_OK == gby_rslt[0])
        {
            gby_rslt[1] =  EE_SaveVarForCalib(BOTHEE,SYSTEMST_HSTY_TEMPER_AREA_INFO_G8);
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        }
    }

    // action result
    ECU_DELAYMS(6);
}

void WR_BCUHistoryPowerInfo(void)
{
    //History Power Infomation
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_HSTY_POWER_INFO, &CanTpMsgRx.Data[1]);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  EE_SaveVarForCalib(BOTHEE, SYSTEMST_HSTY_POWER_INFO);
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    }

    // action result
    ECU_DELAYMS(6);
}

void WR_BCUHistoryCurrentArea(void)
{
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    //History cur area[0]-[6]     28byte
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_HSTY_CURRENT_AREA_INFO_G1, &CanTpMsgRx.Data[1]);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  EE_SaveVarForCalib(BOTHEE,SYSTEMST_HSTY_CURRENT_AREA_INFO_G1);
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    }

    //History cur Temper area[7]-[13]    28byte

    if((ERR_OK == CanTpMsgTx.Data[1])&&(ERR_OK == CanTpMsgTx.Data[2]))
    {
        gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_HSTY_CURRENT_AREA_INFO_G2, &CanTpMsgRx.Data[29]);
        if(ERR_OK == gby_rslt[0])
        {
            gby_rslt[1] =  EE_SaveVarForCalib(BOTHEE,SYSTEMST_HSTY_CURRENT_AREA_INFO_G2);
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        }
    }

    // action result
    ECU_DELAYMS(6);
}

void WR_BCUHistorySocInfo(void)
{
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    //History SOC area[0]-[6]     28byte
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_HSTY_SOC_AREA_INFO_G1, &CanTpMsgRx.Data[1]);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  EE_SaveVarForCalib(BOTHEE,SYSTEMST_HSTY_SOC_AREA_INFO_G1);
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    }

    //History SOC area[6]-[9]    12byte

    if((ERR_OK == CanTpMsgTx.Data[1])&&(ERR_OK == CanTpMsgTx.Data[2]))
    {
        gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_HSTY_SOC_AREA_INFO_G2, &CanTpMsgRx.Data[29]);
        if(ERR_OK == gby_rslt[0])
        {
            gby_rslt[1] =  EE_SaveVarForCalib(BOTHEE,SYSTEMST_HSTY_SOC_AREA_INFO_G2);
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        }
    }

    // action result
    ECU_DELAYMS(6);
}

void WR_BCUHistoryCapacityInfo(void)
{
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    //total chg&dch AH   8byte
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_TOTAL_CHG_DCH_AH, &CanTpMsgRx.Data[1]);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  EE_SaveVarForCalib(BOTHEE,SYSTEMST_TOTAL_CHG_DCH_AH);
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    }

    //Remian Capatiyt 4byte

    if((ERR_OK == CanTpMsgTx.Data[1])&&(ERR_OK == CanTpMsgTx.Data[2]))
    {
        gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_REMAIN_CAPACITY_BAK_G1, &CanTpMsgRx.Data[9]);
        if(ERR_OK == gby_rslt[0])
        {
            gby_rslt[1] =  EE_SaveVarForCalib(BOTHEE,SYSTEMST_REMAIN_CAPACITY_BAK_G1);
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        }
    }

    // action result
    ECU_DELAYMS(6);
}


void WR_BCUBatPackInfo1(void)
{
    //
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    gby_rslt[0] = EE_LoadVar(SYSTEMST_BAT_PACK_INFO_G1);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    /* if(ERR_OK != gby_rslt[0])

    {

        return;

    } */

    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSTEMST_BAT_PACK_INFO_G1, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    for(gby_i = 0; gby_i  < gby_length; gby_i++)
    {
        gby_data_bak[gby_i] = CanTpMsgRx.Data[gby_i+1];
    }
    if((0xFF == gby_data_bak[0])&& (0xFF == gby_data_bak[1]))
    {
        gby_data_bak[0] = gby_data[0];
        gby_data_bak[1] = gby_data[1];
    }
    if(0xFF == gby_data_bak[2])
    {
        gby_data_bak[2] = gby_data[2];
    }
    if(0xFF == gby_data_bak[3])
    {
        gby_data_bak[3] = gby_data[3];
    }
    if(0xFF == gby_data_bak[4])
    {
        gby_data_bak[4] = gby_data[4];
    }
    if((0xFF == gby_data_bak[5])&& (0xFF == gby_data_bak[6]))
    {
        gby_data_bak[5] = gby_data[5];
        gby_data_bak[6] = gby_data[6];
    }
    if((0xFF == gby_data_bak[7])&& (0xFF == gby_data_bak[8])
            &&(0xFF == gby_data_bak[9])&& (0xFF == gby_data_bak[10]))
    {
        gby_data_bak[7] = gby_data[7];
        gby_data_bak[8] = gby_data[8];
        gby_data_bak[9] = gby_data[9];
        gby_data_bak[10] = gby_data[10];
    }
    if((0xFF == gby_data_bak[11])&& (0xFF == gby_data_bak[12])
            &&(0xFF == gby_data_bak[13])&& (0xFF == gby_data_bak[14]))
    {
        gby_data_bak[11] = gby_data[11];
        gby_data_bak[12] = gby_data[12];
        gby_data_bak[13] = gby_data[13];
        gby_data_bak[14] = gby_data[14];
    }
    if((0xFF == gby_data_bak[15])&& (0xFF == gby_data_bak[16]))
    {
        gby_data_bak[15] = gby_data[15];
        gby_data_bak[16] = gby_data[16];
    }
    if(0xFF == gby_data_bak[17])
    {
        gby_data_bak[17] = gby_data[17];
    }
    if((0xFF == gby_data_bak[18])&& (0xFF == gby_data_bak[19]))
    {
        gby_data_bak[18] = gby_data[18];
        gby_data_bak[19] = gby_data[19];
    }
    if((0xFF == gby_data_bak[20])&& (0xFF == gby_data_bak[21])
            &&(0xFF == gby_data_bak[22])&& (0xFF == gby_data_bak[23])
            &&(0xFF == gby_data_bak[24])&& (0xFF == gby_data_bak[25]))
    {
        gby_data_bak[20] = gby_data[20];
        gby_data_bak[21] = gby_data[21];
        gby_data_bak[22] = gby_data[22];
        gby_data_bak[23] = gby_data[23];
        gby_data_bak[24] = gby_data[24];
        gby_data_bak[25] = gby_data[25];
    }
    if((0xFF == gby_data_bak[26])&& (0xFF == gby_data_bak[27])
            &&(0xFF == gby_data_bak[28])&& (0xFF == gby_data_bak[29]))
    {
        gby_data_bak[26] = gby_data[26];
        gby_data_bak[27] = gby_data[27];
        gby_data_bak[28] = gby_data[28];
        gby_data_bak[29] = gby_data[29];
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_BAT_PACK_INFO_G1, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSTEMST_BAT_PACK_INFO_G1);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    ECU_DELAYMS(6);
}

void WR_BCUBatPackInfo2(void)
{
    //
    //Battery pack information2
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    gby_rslt[0] = EE_LoadVar(SYSTEMST_BAT_PACK_INFO_G2);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    /* if(ERR_OK != gby_rslt[0])

    {

        return;

    } */

    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSTEMST_BAT_PACK_INFO_G2, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    for(gby_i = 0; gby_i  < gby_length; gby_i++)
    {
        gby_data_bak[gby_i] = CanTpMsgRx.Data[gby_i+1];
    }
    if((0xFF == gby_data_bak[0])&& (0xFF == gby_data_bak[1])
            &&(0xFF == gby_data_bak[2])&& (0xFF == gby_data_bak[3]))
    {
        gby_data_bak[0] = gby_data[0];
        gby_data_bak[1] = gby_data[1];
        gby_data_bak[2] = gby_data[2];
        gby_data_bak[3] = gby_data[3];
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_BAT_PACK_INFO_G2, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSTEMST_BAT_PACK_INFO_G2);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    ECU_DELAYMS(6);
}

void WR_BCUBatPackInfo3(void)
{
    //
    //Battery pack information3
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    gby_rslt[0] = EE_LoadVar(SYSTEMST_TOTAL_CHG_DCH_AH);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    /* if(ERR_OK != gby_rslt[0])

    {

        return;

    } */

    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSTEMST_TOTAL_CHG_DCH_AH, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    for(gby_i = 0; gby_i  < gby_length; gby_i++)
    {
        gby_data_bak[gby_i] = CanTpMsgRx.Data[gby_i+1];
    }
    if((0xFF == gby_data_bak[0])&& (0xFF == gby_data_bak[1])
            &&(0xFF == gby_data_bak[2])&& (0xFF == gby_data_bak[3]))
    {
        gby_data_bak[0] = gby_data[0];
        gby_data_bak[1] = gby_data[1];
        gby_data_bak[2] = gby_data[2];
        gby_data_bak[3] = gby_data[3];
    }
    if((0xFF == gby_data_bak[4])&& (0xFF == gby_data_bak[5])
            &&(0xFF == gby_data_bak[6])&& (0xFF == gby_data_bak[7]))
    {
        gby_data_bak[4] = gby_data[4];
        gby_data_bak[5] = gby_data[5];
        gby_data_bak[6] = gby_data[6];
        gby_data_bak[7] = gby_data[7];
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_TOTAL_CHG_DCH_AH, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSTEMST_TOTAL_CHG_DCH_AH);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    ECU_DELAYMS(6);
}

void WR_BCUBatPackInfo4(void)
{
    //
    //Battery pack information4
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    gby_rslt[0] = EE_LoadVar(SYSTEMST_BAT_PACK_INFO_G4);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    /* if(ERR_OK != gby_rslt[0])

    {

        return;

    } */

    gby_rslt[0] = SysConf_GetParaValueFromRam(SYSTEMST_BAT_PACK_INFO_G4, gby_data,&gby_length);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    for(gby_i = 0; gby_i  < gby_length; gby_i++)
    {
        gby_data_bak[gby_i] = CanTpMsgRx.Data[gby_i+1];
    }
    for(gby_i = 0; gby_i  < 3; gby_i++)
    {
        if((0xFF == gby_data_bak[2*gby_i]) && (0xFF == gby_data_bak[2*gby_i+1]))
        {
            gby_data_bak[2*gby_i] = gby_data[2*gby_i];
            gby_data_bak[2*gby_i+1] = gby_data[2*gby_i+1];
        }
    }
    gby_rslt[0] =  SysConf_UpdataParaToRam(SYSTEMST_BAT_PACK_INFO_G4, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, SYSTEMST_BAT_PACK_INFO_G4);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    ECU_DELAYMS(6);
}


void RD_BCUSysconfProductNo(void)
{
    // BCU sysConf product no
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_rslt[0] =  EE_LoadVar(SYSCONF_PRODUCTNO);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSCONF_PRODUCTNO, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i < gby_length; gby_i++)
            {
                CanTpMsgTx.Data[gby_i + 3] = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
        }
    }
    else
    {

        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
    }
}

void RD_BCUCellvNum(void)
{
    //BMU number & BMU0 Cell number & BMU1- BMU30 cell number
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_ptr_by = &(CanTpMsgTx.Data[3]);

    // BMU number
    gby_rslt[0] =  EE_LoadVar(SYSCONF_BMU_NUMBER);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSCONF_BMU_NUMBER, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }

    //BMU0 Cell number
  /*  gby_rslt[0] =  EE_LoadVar(SYSCONF_BMU0_CELLNUM);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSCONF_BMU0_CELLNUM, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            for(gby_i = 0; gby_i < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    } */

    //BMU1- BMU15 cell number
 /*   gby_rslt[0] = EE_LoadVar(SYSCONF_BMU_CELLNUM_G1);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSCONF_BMU_CELLNUM_G1, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {

            for(gby_i = 0; gby_i < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }
 */
    //BMU16- BMU30 cell number
 /*   gby_rslt[0] = SysConf_GetParaValueFromRam(SYSCONF_BMU_CELLNUM_G2, gby_data,&gby_length);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSCONF_BMU_CELLNUM_G2, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }
 */  
  
}

void RD_BCUCellNumPerChip(void)
{
    //BMU0 Cell number per Chip
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_rslt[0] =  EE_LoadVar(SYSCONF_CELLNUM_PER_CHIP);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSCONF_CELLNUM_PER_CHIP, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i < gby_length; gby_i++)
            {
                CanTpMsgTx.Data[gby_i + 3] = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
        }
    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
    }
}

void RD_BCUCellvStartNo(void)
{
    //BMU0 start cell no & // BMU1-BMU30 start cell no

    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_ptr_by = &(CanTpMsgTx.Data[3]);
    // BMU0 start cell no
 /*
    gby_rslt[0] =  EE_LoadVar(SYSCONF_BMU0_START_CELLNO);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSCONF_BMU0_START_CELLNO, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {

            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }
    
 */

    // BMU1-BMU15 start cell no
 /*   gby_rslt[0] =  EE_LoadVar(SYSCONF_BMU_START_CELLNO_G1);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSCONF_BMU_START_CELLNO_G1, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {

            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }
  */
    // BMU16-BMU30 start cell no
  /*  gby_rslt[0] = EE_LoadVar(SYSCONF_BMU_START_CELLNO_G2);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSCONF_BMU_START_CELLNO_G2, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }
  */
    
}

void RD_BCUTemperNum(void)
{
    //BMU0 temper number & BMU1- BMU30 temper number
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_ptr_by = &(CanTpMsgTx.Data[3]);

    //BMU0 temper number
/*    gby_rslt[0] =  EE_LoadVar(SYSCONF_BMU0_TEMPERNUM);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSCONF_BMU0_TEMPERNUM, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {

            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }
    
 */

    //BMU1- BMU15 temper number
 /*   gby_rslt[0] =  EE_LoadVar(SYSCONF_BMU_TEMPERNUM_G1);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSCONF_BMU_TEMPERNUM_G1, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {

            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }
 */
    //BMU16- BMU30 temper number
 /*   gby_rslt[0] = EE_LoadVar(SYSCONF_BMU_TEMPERNUM_G2);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSCONF_BMU_TEMPERNUM_G2, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }
  */
    
}

void RD_BCUTemperStartNo(void)
{
    //BMU0 temper start n0 & BMU1- BMU30 temper start n0
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_ptr_by = &(CanTpMsgTx.Data[3]);

    //BMU0 temper start n0
  /*  gby_rslt[0] =  EE_LoadVar(SYSCONF_BMU0_START_TEMPERNO);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSCONF_BMU0_START_TEMPERNO, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {

            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }
    
  */

    //BMU1- BMU15 temper start n0
    gby_rslt[0] =  EE_LoadVar(SYSCONF_CHIP_FUSE_POS);     //SYSCONF_BMU_START_TEMPERNO_G1
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSCONF_CHIP_FUSE_POS, gby_data,&gby_length); //SYSCONF_BMU_START_TEMPERNO_G1
        if(ERR_OK == gby_rslt[1])
        {

            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }

    //BMU16- BMU30 temper start n0
  /*  gby_rslt[0] = EE_LoadVar(SYSCONF_TEMPERNUM_PER_CHIP);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSCONF_TEMPERNUM_PER_CHIP, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }
   */
}

void RD_BCUSoc(void)
{
    //BCU SOC
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.Data[1] = 0 ;
    CanTpMsgTx.Data[2] = 0 ;
    CanTpMsgTx.DataLen = 3;
    gby_wtmp = Bat_GetSOC();
    CanTpMsgTx.Data[3] = (uByte)(gby_wtmp>>8);
    CanTpMsgTx.Data[4] = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
}

void RD_BCURatesCapActualCap(void)
{
    //Rated Capacity & ActualCapatity
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;

    gby_rslt[0] =  EE_LoadVar(SYSTEMST_RATED_ACTUAL_CAPACITY);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_RATED_ACTUAL_CAPACITY, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                CanTpMsgTx.Data[gby_i + 3] = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
        }
    }
    else
    {

        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
    }
}

void RD_BCUCfgVer(void)
{
    //CFG_Version
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_rslt[0] =  EE_LoadVar(SYSCONF_CFG_VER);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSCONF_CFG_VER, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                CanTpMsgTx.Data[gby_i + 3] = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
        }
    }
    else
    {

        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
    }
}

void RD_BCUCfgUpdate(void)
{
    //Config update information
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;

    gby_rslt[0] =  EE_LoadVar(SYSCONF_CONFIG_UPDATA_INFO);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSCONF_CONFIG_UPDATA_INFO, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                CanTpMsgTx.Data[gby_i + 3] = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
        }
    }
    else
    {

        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
    }
}

void RD_BCUVehiclePlate(void)
{
    // BCU Vehicle plate
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;

    gby_rslt[0] =  EE_LoadVar(SYSTEMST_VEHICLE_PLATE);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_VEHICLE_PLATE, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                CanTpMsgTx.Data[gby_i + 3] = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
        }
    }
    else
    {

        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
    }
}

void RD_BCUSoh(void)
{
    // BCU SOH
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;

    gby_rslt[0] =  EE_LoadVar(SYSTEMST_SOH);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_SOH, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                CanTpMsgTx.Data[gby_i + 3] = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
        }
    }
    else
    {

        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
    }
}


void RD_BCUFusePos(void)
{
    // BCU fuse position
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
  
    gby_rslt[0] =  EE_LoadVar(SYSCONF_CHIP_FUSE_POS);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSCONF_CHIP_FUSE_POS, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                CanTpMsgTx.Data[gby_i + 3] = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
        }
    }
    else
    {

        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
    }
    
}

void RD_BCUSystemClock(void)
{
    // Read  BMS system clock
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    gby_rslt[0] =  ClockIf_ReadTime((sClockIf_TimeType*)&CanTpMsgTx.Data[3]);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    // action result
    CanTpMsgTx.DataLen = 10;
}

void RD_BCUThrhdLvl1(void)
{
    //BCU Thrhd lvl1
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_rslt[0] =  EE_LoadVar(ERRORALARM_THRHD_LVV1);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(ERRORALARM_THRHD_LVV1, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                CanTpMsgTx.Data[gby_i + 3] = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
        }
    }
    else
    {

        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
    }
}

void RD_BCUThrhdLvl2(void)
{
    //BCU Thrhd lvl2
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_rslt[0] =  EE_LoadVar(ERRORALARM_THRHD_LVV2);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(ERRORALARM_THRHD_LVV2, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                CanTpMsgTx.Data[gby_i + 3] = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
        }
    }
    else
    {

        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
    }
}
void RD_BCUThrhdLvl3(void)
{
    //BCU Thrhd lvl3
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_rslt[0] =  EE_LoadVar(ERRORALARM_THRHD_LVV3);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(ERRORALARM_THRHD_LVV3, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                CanTpMsgTx.Data[gby_i + 3] = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
        }
    }
    else
    {

        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
    }
}

void RD_BCUHeatControl(void)
{
    //Heat control(fan/warm close&open)
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_rslt[0] =  EE_LoadVar(THM_HEAT_MANAGE_PARA);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(THM_HEAT_MANAGE_PARA, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                CanTpMsgTx.Data[gby_i + 3] = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
        }
    }
    else
    {

        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
    }
}

void RD_BCUChargeControl(void)
{
    //charge control parameter
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_rslt[0] =  EE_LoadVar(SYSTEMST_CHG_CTRL_PARA);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_CHG_CTRL_PARA, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                CanTpMsgTx.Data[gby_i + 3] = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
        }
    }
    else
    {

        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
    }

}


void RD_BCUHistoryCellvInfo(void)
{
    //History Cellv Infomation    14byte
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_rslt[0] =  EE_LoadVar(SYSTEMST_HSTY_CELLV_INFO);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_HSTY_CELLV_INFO, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                CanTpMsgTx.Data[gby_i + 3] = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
        }
    }
    else
    {

        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
    }
}

void RD_BCUHistoryTemperInfo(void)
{
    //History Temper Infomation   5byte
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_rslt[0] =  EE_LoadVar(SYSTEMST_HSTY_TEMPER_INFO);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_HSTY_TEMPER_INFO, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                CanTpMsgTx.Data[gby_i + 3] = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
        }
    }
    else
    {

        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
    }
}

void RD_BCUHistoryLowTemperArea(void)
{
    //History low Temper area[0]-[15]
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_ptr_by = &(CanTpMsgTx.Data[3]);
    //History low Temper area[0]-[7]     30byte
    gby_rslt[0] =  EE_LoadVar(SYSTEMST_HSTY_TEMPER_AREA_INFO_G1);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_HSTY_TEMPER_AREA_INFO_G1, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {

            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }

    //History low Temper area[8]-[15]     30byte
    gby_rslt[0] = EE_LoadVar(SYSTEMST_HSTY_TEMPER_AREA_INFO_G2);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_HSTY_TEMPER_AREA_INFO_G2, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }
}

void RD_BCUHistoryHighTemperArea(void)
{
    //History high Temper area[0]-[15]
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_ptr_by = &(CanTpMsgTx.Data[3]);
    //History high Temper area[0]-[7]     30byte
    gby_rslt[0] =  EE_LoadVar(SYSTEMST_HSTY_TEMPER_AREA_INFO_G3);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_HSTY_TEMPER_AREA_INFO_G3, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {

            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }

    //History high Temper area[8]-[15]     30byte
    gby_rslt[0] = EE_LoadVar(SYSTEMST_HSTY_TEMPER_AREA_INFO_G4);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_HSTY_TEMPER_AREA_INFO_G4, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }
}
void RD_BCUHistoryAvrgTemperArea(void)
{
    //History avrg Temper area[0]-[15]
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_ptr_by = &(CanTpMsgTx.Data[3]);
    //History avrg Temper area[0]-[7]     30byte
    gby_rslt[0] =  EE_LoadVar(SYSTEMST_HSTY_TEMPER_AREA_INFO_G5);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_HSTY_TEMPER_AREA_INFO_G5, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {

            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }

    //History avrg Temper area[8]-[15]     30byte
    gby_rslt[0] = EE_LoadVar(SYSTEMST_HSTY_TEMPER_AREA_INFO_G6);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_HSTY_TEMPER_AREA_INFO_G6, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }
}
void RD_BCUHistoryDetTemperArea(void)
{
    //History det Temper area[1]-[15]
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_ptr_by = &(CanTpMsgTx.Data[3]);
    //History det Temper area[0]-[7]     30byte
    gby_rslt[0] =  EE_LoadVar(SYSTEMST_HSTY_TEMPER_AREA_INFO_G7);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_HSTY_TEMPER_AREA_INFO_G7, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {

            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }

    //History det Temper area[8]-[15]     30byte
    gby_rslt[0] = EE_LoadVar(SYSTEMST_HSTY_TEMPER_AREA_INFO_G8);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_HSTY_TEMPER_AREA_INFO_G8, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }
}
void RD_BCUHistoryPowerInfo(void)
{
    //History power Infomation  8byte
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_rslt[0] =  EE_LoadVar(SYSTEMST_HSTY_POWER_INFO);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_HSTY_POWER_INFO, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                CanTpMsgTx.Data[gby_i + 3] = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
        }
    }
    else
    {

        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
    }
}
void RD_BCUHistoryCurrentArea(void)
{
    //History current area[0]-[15]
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_ptr_by = &(CanTpMsgTx.Data[3]);
    //History current area[0]-[7]     30byte
    gby_rslt[0] =  EE_LoadVar(SYSTEMST_HSTY_CURRENT_AREA_INFO_G1);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_HSTY_CURRENT_AREA_INFO_G1, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {

            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }

    //History current Temper area[8]-[15]     30byte
    gby_rslt[0] = EE_LoadVar(SYSTEMST_HSTY_CURRENT_AREA_INFO_G2);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_HSTY_CURRENT_AREA_INFO_G2, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }
}
void RD_BCUHistorySocInfo(void)
{
    //History SOC Temper area[8]-[9]
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_ptr_by = &(CanTpMsgTx.Data[3]);
    //History SOC area[0]-[7]     30byte
    gby_rslt[0] =  EE_LoadVar(SYSTEMST_HSTY_SOC_AREA_INFO_G1);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_HSTY_SOC_AREA_INFO_G1, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {

            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }

    //History SOC  area[8]-[9]     8byte
    gby_rslt[0] = EE_LoadVar(SYSTEMST_HSTY_SOC_AREA_INFO_G2);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_HSTY_SOC_AREA_INFO_G2, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }
}
void RD_BCUHistoryCapacityInfo(void)
{
    //total chg&dch AH & Remian Capacity 4byte
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_ptr_by = &(CanTpMsgTx.Data[3]);
    //total chg&dch AH   8byte
    gby_rslt[0] =  EE_LoadVar(SYSTEMST_TOTAL_CHG_DCH_AH);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_TOTAL_CHG_DCH_AH, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {

            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }

    //Remian Capacity 4byte
    gby_rslt[0] = EE_LoadVar(SYSTEMST_REMAIN_CAPACITY_BAK_G1);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_REMAIN_CAPACITY_BAK_G1, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                *(gby_ptr_by++) = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            return;
        }

    }
    else
    {
        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
        return;
    }
}
void RD_BCUBatPackInfo1(void)
{
    //bat pack information1
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_rslt[0] =  EE_LoadVar(SYSTEMST_BAT_PACK_INFO_G1);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_BAT_PACK_INFO_G1, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                CanTpMsgTx.Data[gby_i + 3] = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
        }
    }
    else
    {

        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
    }
}
void RD_BCUBatPackInfo2(void)
{
    //bat pack information2
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_rslt[0] =  EE_LoadVar(SYSTEMST_BAT_PACK_INFO_G2);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_BAT_PACK_INFO_G2, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                CanTpMsgTx.Data[gby_i + 3] = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
        }
    }
    else
    {

        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
    }
}
void RD_BCUBatPackInfo3(void)
{
    //bat pack information3
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_rslt[0] =  EE_LoadVar(SYSTEMST_TOTAL_CHG_DCH_AH);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_TOTAL_CHG_DCH_AH, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                CanTpMsgTx.Data[gby_i + 3] = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
        }
    }
    else
    {

        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
    }
}
void RD_BCUBatPackInfo4(void)
{
    //bat pack information4
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_rslt[0] =  EE_LoadVar(SYSTEMST_BAT_PACK_INFO_G4);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_BAT_PACK_INFO_G4, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                CanTpMsgTx.Data[gby_i + 3] = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
        }
    }
    else
    {

        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
    }
}


void RD_BCUBatsysInfo1(void)
{
//current/temper sensr type
}

void RD_BCUBatsysInfo2(void)
{
    //Positive/Negative/Precharge/Charge  contactor life
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_rslt[0] =  EE_LoadVar(SYSTEMST_CONTACTOR_LIFE);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_CONTACTOR_LIFE, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                CanTpMsgTx.Data[gby_i + 3] = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
        }
    }
    else
    {

        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
    }
}


void RD_BCUBatsysInfo3(void)
{
    //Vehicle VIN
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_rslt[0] =  EE_LoadVar(SYSTEMST_VEHICLE_VIN);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(SYSTEMST_VEHICLE_VIN, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                CanTpMsgTx.Data[gby_i + 3] = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
        }
    }
    else
    {

        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
    }
}



void RD_HDErrInfo1(void)
{
    //hard err
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.Data[1] = 0;
    CanTpMsgTx.Data[2] = 0;
    CanTpMsgTx.DataLen += 3;
    gby_ptr_by = &(CanTpMsgTx.Data[3]);
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_DETECT_CELLV);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_DETECT_TEMPER);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_DETECT_CUR);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_DETECT_ISO);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_DETECT_SUMV_IN);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_DETECT_SUMV_OUT);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_CLOCK_EXT);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_DETECT_WH);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_BAL_CHG);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_BAL_DCH);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
}

void RD_HDErrInfo2(void)
{
    //hard err
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.Data[1] = 0;
    CanTpMsgTx.Data[2] = 0;
    CanTpMsgTx.DataLen += 3;
    gby_ptr_by = &(CanTpMsgTx.Data[3]);
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_EE_CPU);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_EE_EXT);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_FLASH_EXT);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_FLASH_CPU);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_CPU_GENERAL);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
}

void RD_HDErrInfo3(void)
{
    //hard err
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.Data[1] = 0;
    CanTpMsgTx.Data[2] = 0;
    CanTpMsgTx.DataLen += 3;
    gby_ptr_by = &(CanTpMsgTx.Data[3]);
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_CAN4);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_CAN0);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_CAN1);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_CAN2);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_CAN3);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
}

void RD_HDErrInfo4(void)
{
    //hard err
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.Data[1] = 0;
    CanTpMsgTx.Data[2] = 0;
    CanTpMsgTx.DataLen += 3;
    gby_ptr_by = &(CanTpMsgTx.Data[3]);
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_SOFTWARE);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
}

void RD_HDErrInfo5(void)
{
    //hard err
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.Data[1] = 0;
    CanTpMsgTx.Data[2] = 0;
    CanTpMsgTx.DataLen += 3;
    gby_ptr_by = &(CanTpMsgTx.Data[3]);
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_RS232);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_RS485);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_ETHERNET);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
}

void RD_HDErrInfo6(void)
{
    //hard err
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.Data[1] = 0;
    CanTpMsgTx.Data[2] = 0;
    CanTpMsgTx.DataLen += 3;
    gby_ptr_by = &(CanTpMsgTx.Data[3]);
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_DI);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_DO);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_AI);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_AO);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
}

void RD_HDErrInfo7(void)
{
    //hard err
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.Data[1] = 0;
    CanTpMsgTx.Data[2] = 0;
    CanTpMsgTx.DataLen += 3;
    gby_ptr_by = &(CanTpMsgTx.Data[3]);
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_PRE_CHG);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
    gby_wtmp = Err_GetHwErrDetail(ERR_HW_RELAY_DRIVER);
    *(gby_ptr_by++) = (uByte)(gby_wtmp>>8);
    *(gby_ptr_by++) = (uByte)(gby_wtmp&0x00FF);
    CanTpMsgTx.DataLen += 2;
}

void RD_GetProjectVer(void)
{
    //Get Soft  version
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_ptr_by = &(CanTpMsgTx.Data[3]);
    gby_rslt[0] = Version_GetProjectVer((sVersion_CodeType*)gby_ptr_by);
    CanTpMsgTx.DataLen += sizeof(sVersion_CodeType);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    CanTpMsgTx.DataLen += 2;
    if(gby_rslt[0])
    {
        CanTpMsgTx.DataLen = 3;
    }
}

void RD_GetPlatVer(void)
{
    //Get plat version
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_ptr_by = &(CanTpMsgTx.Data[3]);
    gby_rslt[0] = Version_GetPlatformVer((sVersion_CodeType*)gby_ptr_by);
    CanTpMsgTx.DataLen += sizeof(sVersion_CodeType);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    CanTpMsgTx.DataLen += 2;
    if(gby_rslt[0])
    {
        CanTpMsgTx.DataLen = 3;
    }
}


void RD_GetModuleVer(void)
{
    uWord  wtmp, wcnt;  
    ModuleID_Type module_id[VERSION_MODULE_NUMBER];
    uWord ModuleNum = 0;
    uWord  rslt[4];
    volatile uByte *ptr_by;

    //Get Module Version
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.Data[3] = CanTpMsgRx.Data[1];
    CanTpMsgTx.Data[4] = CanTpMsgRx.Data[2];
    CanTpMsgTx.DataLen += 3;
    wtmp = CanTpMsgTx.Data[3] ;
    wtmp = (wtmp<<8)+CanTpMsgTx.Data[4] ;
    if(0 == wtmp)
    {
        // search module Id
        rslt[0] = Version_SearchModule(module_id,&ModuleNum);
        CanTpMsgTx.Data[5] = GET_HIGH_BYTE(ModuleNum);
        CanTpMsgTx.Data[6] = GET_LOW_BYTE(ModuleNum);
        CanTpMsgTx.DataLen += 2;
        // return valid module id to monitor
        ptr_by = &(CanTpMsgTx.Data[7]);
        for(wcnt = 0; wcnt < ModuleNum; wcnt++)
        {
            *(ptr_by++) =  module_id[wcnt];
        }
        CanTpMsgTx.DataLen += ModuleNum;
    }
    else
    {
        ptr_by = &(CanTpMsgTx.Data[5]);
        rslt[0] = Version_GetModuleVer( wtmp,(sVersion_CodeType*)ptr_by);
        CanTpMsgTx.DataLen += sizeof(sVersion_CodeType);
    }
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(rslt[0]);
    CanTpMsgTx.DataLen += 2;
    if(rslt[0])
    {
        CanTpMsgTx.DataLen = 5;
    }
}

void RD_GetHardwareVer(void)
{
    //Get hardware Version
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_ptr_by = &(CanTpMsgTx.Data[3]);
    gby_rslt[0] = Version_GetPlatformHwVer((sVersion_CodeType*)gby_ptr_by);
    CanTpMsgTx.DataLen += sizeof(sVersion_CodeType);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    CanTpMsgTx.DataLen += 2;
    if(gby_rslt[0])
    {
        CanTpMsgTx.DataLen = 3;
    }
}


void WR_QrCodeInfo(void)
{
    
    uByte i;
    //BCU Thrhd lvl1
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    gby_rslt[0] = EE_LoadVar(QR_CODE_INFOR);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    /* if(ERR_OK != gby_rslt[0])

    {

        return;

    } */

    gby_rslt[0] = SysConf_GetParaValueFromRam(QR_CODE_INFOR, gby_data,&gby_length);  //QR_CODE_INFOR
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);                                     
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    for(gby_i = 0; gby_i  < gby_length; gby_i++)
    {
        gby_data_bak[gby_i] = CanTpMsgRx.Data[gby_i+1];
    }
    
    if( (0xFF == gby_data_bak[0]) && (0xFF == gby_data_bak[1]) && (0xFF == gby_data_bak[2]) && (0xFF == gby_data_bak[3])
      &&(0xFF == gby_data_bak[4]) && (0xFF == gby_data_bak[5]) && (0xFF == gby_data_bak[6]) && (0xFF == gby_data_bak[7]))
    {
        for( i= 0;i < 8;i++)
        {
            gby_data_bak[i] = gby_data[i];
        }
    }
    
    if( (0xFF == gby_data_bak[8]) && (0xFF == gby_data_bak[9]) && (0xFF == gby_data_bak[10]) && (0xFF == gby_data_bak[11]) && (0xFF == gby_data_bak[12]) 
      &&(0xFF == gby_data_bak[13]) && (0xFF == gby_data_bak[14]) && (0xFF == gby_data_bak[15]) && (0xFF == gby_data_bak[16]) && (0xFF == gby_data_bak[17]) 
      &&(0xFF == gby_data_bak[18]) && (0xFF == gby_data_bak[19]) && (0xFF == gby_data_bak[20]) && (0xFF == gby_data_bak[21]) && (0xFF == gby_data_bak[22]))
    {
        for(i = 8;i < 23;i++)
        {
            gby_data_bak[i] = gby_data[i];
        }
    }
    
    
    if( (0xFF == gby_data_bak[23]) && (0xFF == gby_data_bak[24]) && (0xFF == gby_data_bak[25]) && (0xFF == gby_data_bak[26])
      &&(0xFF == gby_data_bak[27]) && (0xFF == gby_data_bak[28]) && (0xFF == gby_data_bak[29]))
    {
        for( i= 23;i < 30;i++)
        {
            gby_data_bak[i] = gby_data[i];
        }
    }
    

    gby_rslt[0] =  SysConf_UpdataParaToRam(QR_CODE_INFOR, gby_data_bak);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    if(ERR_OK != gby_rslt[0])
    {
        return;
    }
    gby_rslt[0] =  EE_SaveVarForCalib(BOTHEE, QR_CODE_INFOR);
    CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
    CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
    ECU_DELAYMS(6);

}


void RD_QrCodeInfo(void)
{
    
    //BCU Thrhd lvl1
    CanTpMsgTx.DataLen = 0;
    CanTpMsgTx.Data[0] = CanTpMsgRx.Data[0];
    CanTpMsgTx.DataLen += 1;
    gby_rslt[0] =  EE_LoadVar(QR_CODE_INFOR);
    if(ERR_OK == gby_rslt[0])
    {
        gby_rslt[1] =  SysConf_GetParaValueFromRam(QR_CODE_INFOR, gby_data,&gby_length);
        if(ERR_OK == gby_rslt[1])
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
            for(gby_i = 0; gby_i  < gby_length; gby_i++)
            {
                CanTpMsgTx.Data[gby_i + 3] = gby_data[gby_i];
            }
            CanTpMsgTx.DataLen += gby_length;
        }
        else
        {
            CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[1]);
            CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[1]);
            CanTpMsgTx.DataLen += 2;
        }
    }
    else
    {

        CanTpMsgTx.Data[1] = GET_HIGH_BYTE(gby_rslt[0]);
        CanTpMsgTx.Data[2] = GET_LOW_BYTE(gby_rslt[0]);
        CanTpMsgTx.DataLen += 2;
    }

}
