// CellvDet.c
#include "CellvDet.h"
#include "EE.h"
#include "ErrorAlarm.h"
#include "SchIf.h"
#include "BatterySt.h"
#include "BalM_Macro.h"
#include "PE_Error.h"

#define CELLVDET_FLAG_ENABLE  1
#define CELLVDET_FLAG_DISABLE 0

#define CELLVDET_FLAG_OK  1
#define CELLVDET_FLAG_FAIL 0

#define VT_POINT  166

const uWord LT_V_T_10K[VT_POINT] =
{  
  988,1013,1039,1066,1093,1121,1150,1180,1211,1243,1276,1310,
  1345,1380,1417,1455,1495,1535,1577,1620,1664,1709,1756,1805,
  1855,1906,1960,2014,2071,2129,2189,2251,2315,2380,2448,2518,
  2590,2665,2742,2821,2902,2986,3073,3163,3255,3350,3448,3549,
  3653,3760,3870,3984,4101,4222,4347,4475,4607,4743,4882,5026,
  5174,5327,5483,5644,5810,5980,6155,6335,6519,6708,6903,7101,
  7306,7516,7730,7950,8175,8405,8641,8881,9127,9378,9635,9896,
  10162,10434,10710,10992,11278,11568,11863,12162,12466,12773,13084,13398,
  13716,14037,14361,14687,15015,15345,15677,16010,16345,16679,17014,17350,
  17684,18018,18351,18682,19012,19340,19665,19987,20306,20622,20934,21241,
  21545,21843,22137,22426,22709,22987,23259,23526,23786,24040,24287,24528,
  24763,24991,25213,25428,25636,25838,26033,26222,26404,26579,26748,26911,
  27068,27218,27363,27502,27635,27762,27884,28000,28112,28218,28320,28416,
  28508,28596,28679,28759,28834,28905,28973,29037,29098,29156

};

typedef struct
{
    uByte cnt;
    uWord TemperDet;
} sCellvDet_TmCntType;

sCellvDet_TmCntType CellvDet_TmCnt;

typedef struct
{
    uWord Busy;    //ltc6804 is busy
    uWord Crc;     //CRC error
    uWord Cfg;     //configuration register error
    uWord Init;    //not been intialized
    uWord Unexp;   //unexpected error
    uWord ReadCellv;
} sCellvDet_CellvErrCntType;

sCellvDet_CellvErrCntType  CellvErrCnt;

uByte Gby_Ic_Num;
uByte gby_Volt_ReadCnt;
uByte gby_Volt_PackErrFlag;    /* record voltage pack detect statue */
uByte gby_Volt_PackErrFlag1;
uWord gwd_Volt_Bak[MAX_CELLV_IC_MODULE_NUM][MAX_CELLV_NUM_PER_IC];
uByte gby_test_flag[MAX_CELLV_IC_MODULE_NUM][MAX_CELLV_NUM_PER_IC];

// for balance circuit check
uByte gby_BalCircuitCheckCnt;
uByte gby_BalCheckStep;
uByte gby_BalFaultCnt;
uWord gwd_BalFaultPos[MAX_CELLV_IC_MODULE_NUM];

uByte gby_OpenWireChkStep;
uWord gwd_OpenWirePos[MAX_CELLV_IC_MODULE_NUM];

uByte Lt_NtcTemperBak[20][3];

uByte Lt_Ntc_Cont[20][3]={0};
uByte RdAuxValue_Err_Cont=0;
uByte gby_ReadCellCnt;

static void CellvDet_InitCellvDet(void);
static void CellvDet_ProcCellvDet(void);
static uByte CellvDet_BalCircuitCheck(void);
static uByte CellvDet_OpenCircuitChk(uByte device_num);
static void CellvDet_ProcTemperDet(void);

#define BAL_TEST
#ifdef BAL_TEST
uLWord glwd_BalTestCnt;
uByte gby_BalFlag;

static void Cellv_BalTest(void)
{
    uByte i,j;
    
    if(glwd_BalTestCnt >= TM_MS(600000))
    {
        glwd_BalTestCnt = 0;
        if(0 == gby_BalFlag)
        {
            gby_BalFlag = 1;
            for(i = 0;i < Gby_Ic_Num;i++)
            {
               for(j = 0;j < 11;j++)
               {
                   CellvIf_CtrlBalanceSw(i,j,0x11);
                   Bat_UpdateCellBalanceSt(i+1,j+1,0x20);
                   CellvIf_CtrlBalanceSw(i,j+1,0);
                   Bat_UpdateCellBalanceSt(i+1,j+2,0);
                   j++;
               }
            }
            
        }
        else
        {
            gby_BalFlag = 0;
            for(i = 0;i < Gby_Ic_Num;i++)
            {
               for(j = 0;j < 11;j++)
               {
                   CellvIf_CtrlBalanceSw(i,j,0);
                   Bat_UpdateCellBalanceSt(i+1,j+1,0);
                   CellvIf_CtrlBalanceSw(i,j+1,0x11);
                   Bat_UpdateCellBalanceSt(i+1,j+2,0x20);
                   j++;
               }
            }
        }
        CellvIf_ActiveBalSw();
    }
}
#endif
void CellvDetDet_Init(void)
{
    uByte i, j;
    uByte data[8];
    uByte len;
    
    for(i = 0; i < MAX_CELLV_IC_MODULE_NUM; i++)
    {
        // for cell volt 
        gwd_BalFaultPos[i] = 0;
        
        for(j = 0; j < MAX_CELLV_NUM_PER_IC; j++)
        {
            gwd_Volt_Bak[i][j] = 0;
            gby_test_flag[i][j] = 0;
        }        
    }
    
    CellvDet_InitCellvDet();
    CellvDet_TmCnt.cnt = 0;
    gby_ReadCellCnt = 0;
  #ifdef BAL_TEST
    glwd_BalTestCnt = TM_MS(600000);
  #endif
}

void CellvDetDet_MainCountCbk(void)
{
    (CellvDet_TmCnt.cnt < 255) ? (CellvDet_TmCnt.cnt++) : (CellvDet_TmCnt.cnt = 255);
    
    (CellvDet_TmCnt.TemperDet < 65000) ? (CellvDet_TmCnt.TemperDet++) : (CellvDet_TmCnt.TemperDet = 65000);
  #ifdef BAL_TEST
    (glwd_BalTestCnt < 650000) ? (glwd_BalTestCnt++) : (glwd_BalTestCnt = 650000);
  #endif
}

void CellvDet_MainCbk(void)
{
    uByte flg_cellv_det = CELLVDET_FLAG_ENABLE;
    uByte flg_openwire_det = CELLVDET_FLAG_ENABLE;
    uByte bal_check_step, openwire_chk_step;
    static uByte CellVDet_Cnt = 0;
    uByte i,j;
    
 #ifdef BAL_TEST   
    Cellv_BalTest();
 #endif
    
    if(CellvDet_TmCnt.cnt >= TM_MS(20))  //20
    {
        CellvDet_TmCnt.cnt = 0;

        
       /* if(CellVDet_Cnt >= 1)  
        {
            if(gby_BalCircuitCheckCnt >= 1)
            {
                bal_check_step = CellvDet_BalCircuitCheck();
                if(bal_check_step <= 3)
                {
                    flg_cellv_det = CELLVDET_FLAG_DISABLE;
                }
            }
            else
            {
               openwire_chk_step = CellvDet_OpenCircuitChk(Gby_Ic_Num);  
                              
               if(openwire_chk_step >= 14)
               {
                  gby_BalCircuitCheckCnt++;
                  
               }
               else
               {
                  flg_openwire_det = CELLVDET_FLAG_DISABLE;
               } 
            }
        }
        else
        {
           CellVDet_Cnt++; 
        } */  

        if((CELLVDET_FLAG_ENABLE == flg_cellv_det) && (CELLVDET_FLAG_ENABLE == flg_openwire_det))
        {
            CellvDet_ProcCellvDet();

        }   
    }
    
    if(CellvDet_TmCnt.TemperDet >= TM_MS(305))
    {
       CellvDet_TmCnt.TemperDet = 0;
       (void)CellvDet_ProcTemperDet();
    }
}

static void CellvDet_InitCellvDet(void)
{
    uByte i;
    uByte dlen;
    uByte cell_num_per_chip[20];
    uWord hderr_detail = 0;
    GeneralReturnType rslt = 0;

    // get device num from config
    (void)SysConf_GetParaValueFromRam(SYSCONF_CELLNUM_PER_CHIP, &cell_num_per_chip[0], &dlen);

    Gby_Ic_Num = 0;
    for(i = 0; i < MAX_CELLV_IC_MODULE_NUM; i++)
    {
        if(0 != cell_num_per_chip[i])
        {
            Gby_Ic_Num++;
        }
    }
    
    Gby_Ic_Num = Bat_GetBmuNum();
    // init cellv io and register

    rslt |= CellvIf_Init(Gby_Ic_Num);
   

    if(ERR_OK != rslt)
    {
        hderr_detail |= CELLVDET_HDERR_INIT;
    }

    // start convert cellv

    rslt |= CellvIf_StartCnvt(AD_CNVT_MODE_NORMAL);
    
    
    CellvIf_StartAdAxCvnt();

    if(ERR_OK != rslt)
    {
        hderr_detail |= CELLVDET_HDERR_COM;
    }

    if(0 != hderr_detail)
    {
        // update hardware error
        rslt = Err_UpdateHwErrLevel(ERR_HW_DETECT_CELLV, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_DETECT_CELLV, hderr_detail);
    }
}

static void CellvDet_ProcCellvDet(void)
{
    uByte i, j, k, m;
    uByte bmu_no;
    uByte jumperwire_pos[30];
    uByte dlen;
    uByte read_cellv_err;
    uByte cell_num_per_chip[20],cell_num_and_pos_per_chip[30];
    uWord hderr_detail = 0;
    uWord hderr_detail_orin;
    uWord cellv;
    GeneralReturnType rslt = 0;
   
    // get cell num from config
    (void)SysConf_GetParaValueFromRam(SYSCONF_CELLNUM_PER_CHIP, &cell_num_per_chip[0], &dlen);

    hderr_detail_orin = Err_GetHwErrDetail(ERR_HW_DETECT_CELLV);

    /* clear the current pack error flag */
    gby_Volt_PackErrFlag = 0;

    gby_Volt_ReadCnt++;

    if (gby_Volt_ReadCnt > VOLT_RD_CNT)
    {
        gby_Volt_ReadCnt = VOLT_RD_CNT;
    }

    /* read the voltage value */
    
    //CellvIf_WakeIdle();
   
    rslt = CellvIf_RdValue(Gby_Ic_Num, gwd_Volt_Bak);
    
    if (ERR_OK != rslt)
    {
        // unexpected error
        if ((ERR_UNEXPECT == rslt) && (CellvErrCnt.Unexp < CELLVDET_MAX_WORD))
        {
            CellvErrCnt.Unexp++;
        }

        // crc error
        if ((ERR_CRC_CELLV == rslt) && (CellvErrCnt.Crc < CELLVDET_MAX_WORD))
        {
            CellvErrCnt.Crc++;
        }

        // not init error
        if ( (ERR_NOT_INIT == rslt) && (CellvErrCnt.Init < CELLVDET_MAX_WORD) )
        {
            CellvErrCnt.Init++;
        }

        // busy error
        if ( (ERR_BUSY_CELLV == rslt) && (CellvErrCnt.Busy < CELLVDET_MAX_WORD) )
        {
            CellvErrCnt.Busy++;
        }

        hderr_detail |= CELLVDET_HDERR_ADCONVERT;
       gby_Volt_PackErrFlag= hderr_detail;
       gby_Volt_PackErrFlag1 = rslt;
    }
 
   

    /* check whether read voltage fail */
    if (gby_Volt_PackErrFlag != 0)
    {
        // initial voltage detect again
        rslt = 0;
        rslt |= CellvIf_Init(Gby_Ic_Num);
      
        if(ERR_OK != rslt)
        {
            hderr_detail |= CELLVDET_HDERR_INIT;
        }
    }

    /* restart the voltage convert */
    rslt = 0;
    rslt |= CellvIf_StartCnvt(AD_CNVT_MODE_NORMAL);

    if(ERR_OK != rslt)
    {
        hderr_detail |= CELLVDET_HDERR_COM;
    }

    /* update hardware error detail */
    if(gby_Volt_PackErrFlag != 0)
    {
        hderr_detail |= hderr_detail_orin;
    }
    rslt = Err_UpdateHwErrDetail(ERR_HW_DETECT_CELLV, hderr_detail);

    /* check pack work statue */
    read_cellv_err = 0xff;
    read_cellv_err &= gby_Volt_PackErrFlag;

    if (0 != read_cellv_err)
    {
        /* voltage detecting is error */
        CellvErrCnt.ReadCellv++;
        if (CellvErrCnt.ReadCellv > ERR_READCELLV_CNT)
        {
            CellvErrCnt.ReadCellv = ERR_READCELLV_CNT;

            for (i = 0; i < Gby_Ic_Num; i++)
            {
                //if pack is error, the voltage value is cleared to 0V
                if ( (gby_Volt_PackErrFlag1 & (0x01 << i)) != 0 )
                {
                    for (j = 0; j < MAX_CELLV_NUM_PER_IC; j++)
                    {
                        gwd_Volt_Bak[i][j] = 0;
                    }
                }

            }
            rslt = Err_UpdateHwErrLevel(ERR_HW_DETECT_CELLV, ERR_LEVEL_THREE);
        }  
    }
    else
    {
        /* voltage detecting is work well */
        //CellvErrCnt.ReadCellv = 0;
        //rslt = Err_UpdateHwErrLevel(ERR_HW_DETECT_CELLV, ERR_LEVEL_NORMAL);
    }
    
    /* delete volt of fuse and long wire, then update cell voltage */
    k = 1;
    m = 0;
    
 //   rslt = SysConf_GetParaValueFromRam(SYSCONF_BMU_NO, &bmu_no, &dlen);
    rslt = SysConf_GetParaValueFromRam(SYSCONF_CHIP_FUSE_POS, &jumperwire_pos[0], &dlen);
    bmu_no = 1;
    for(i = 0; i < Gby_Ic_Num; i++) 
    {
        cell_num_and_pos_per_chip[i] = (cell_num_per_chip[i] & 0x0f);  //cell_num_per_chip[i] 高4bit--每个通道的温度数， 低4bit--每个通道的电压数
        for(j = 0;j < MAX_JUMPER_WIRE_NUM;j++) 
        {
            if((i + 1) == ((jumperwire_pos[j] & 0xf0) >> 4)) 
            {
                if(cell_num_and_pos_per_chip[i] < MAX_CELLV_NUM_PER_IC) 
                {
                    cell_num_and_pos_per_chip[i]++;   //该通道需要转换的节数(包括保险数量)
                }
            }
        }
    }

    if(Gby_Ic_Num <= 3)
    {
        for(i = 0; i < Gby_Ic_Num; i++)
        {
            k = 1;
            for (j = 0; j < cell_num_and_pos_per_chip[i]; j++)
            { 
                if( ((i + 1) == (jumperwire_pos[m] & 0xf0) >> 4) && ((j + 1) == (jumperwire_pos[m] & 0x0f)))  //jumperwire_pos[] 高4bit--保险所在通道号  低4bit--保险位置 
                {
                    if(m < (MAX_JUMPER_WIRE_NUM - 1))
                    {
                        m++;
                    }
                } 
                else 
                {
                    cellv = gwd_Volt_Bak[i][j] / 10;
                    if((cellv > 4500) || (cellv < 1000))
                    {
                        gby_test_flag[i][j]++;
                        if(gby_test_flag[i][j] > 3)
                        {
                           gby_test_flag[i][j] = 3;
                           (void)Bat_UpdateCellV(i+1, k, cellv);
                        }
                    }
                    else
                    {
                        gby_test_flag[i][j] = 0;
                        (void)Bat_UpdateCellV(i+1, k, cellv);
                    }
                    (void)Bat_UpdateCellV(i+1, k, cellv);  
                    k++;
                }
            }
        }
    }
    else if(Gby_Ic_Num <= 6)
    {
        if(0 == gby_ReadCellCnt)
        {
            gby_ReadCellCnt = 1;
            for(i = 0; i < 3; i++)
            {
                k = 1;
                for (j = 0; j < cell_num_and_pos_per_chip[i]; j++)
                { 
                    if( ((i + 1) == (jumperwire_pos[m] & 0xf0) >> 4) && ((j + 1) == (jumperwire_pos[m] & 0x0f)))  //jumperwire_pos[] 高4bit--保险所在通道号  低4bit--保险位置 
                    {
                        if(m < (MAX_JUMPER_WIRE_NUM - 1))
                        {
                            m++;
                        }
                    } 
                    else 
                    {
                        cellv = gwd_Volt_Bak[i][j] / 10;
                        if((cellv > 4500) || (cellv < 1000))
                        {
                            gby_test_flag[i][j]++;
                            if(gby_test_flag[i][j] > 3)
                            {
                               gby_test_flag[i][j] = 3;
                               (void)Bat_UpdateCellV(i+1, k, cellv);
                            }
                        }
                        else
                        {
                            gby_test_flag[i][j] = 0;
                            (void)Bat_UpdateCellV(i+1, k, cellv);
                        }
                        (void)Bat_UpdateCellV(i+1, k, cellv);  
                        k++;
                    }
                }
            }
        }
        else
        {
            gby_ReadCellCnt = 0;
            for(i = 3; i < Gby_Ic_Num; i++)
            {
                k = 1;
                for (j = 0; j < cell_num_and_pos_per_chip[i]; j++)
                { 
                    if( ((i + 1) == (jumperwire_pos[m] & 0xf0) >> 4) && ((j + 1) == (jumperwire_pos[m] & 0x0f)))  //jumperwire_pos[] 高4bit--保险所在通道号  低4bit--保险位置 
                    {
                        if(m < (MAX_JUMPER_WIRE_NUM - 1))
                        {
                            m++;
                        }
                    } 
                    else 
                    {
                        cellv = gwd_Volt_Bak[i][j] / 10;
                        if((cellv > 4500) || (cellv < 1000))
                        {
                            gby_test_flag[i][j]++;
                            if(gby_test_flag[i][j] > 3)
                            {
                               gby_test_flag[i][j] = 3;
                               (void)Bat_UpdateCellV(i+1, k, cellv);
                            }
                        }
                        else
                        {
                            gby_test_flag[i][j] = 0;
                            (void)Bat_UpdateCellV(i+1, k, cellv);
                        }
                        (void)Bat_UpdateCellV(i+1, k, cellv);  
                        k++;
                    }
                }
            }
        }  
    }
    else
    {
        if(0 == gby_ReadCellCnt)
        {
            gby_ReadCellCnt = 1;
            for(i = 0; i < 3; i++)
            {
                k = 1;
                for (j = 0; j < cell_num_and_pos_per_chip[i]; j++)
                { 
                    if( ((i + 1) == (jumperwire_pos[m] & 0xf0) >> 4) && ((j + 1) == (jumperwire_pos[m] & 0x0f)))  //jumperwire_pos[] 高4bit--保险所在通道号  低4bit--保险位置 
                    {
                        if(m < (MAX_JUMPER_WIRE_NUM - 1))
                        {
                            m++;
                        }
                    } 
                    else 
                    {
                        cellv = gwd_Volt_Bak[i][j] / 10;
                        if((cellv > 4500) || (cellv < 1000))
                        {
                            gby_test_flag[i][j]++;
                            if(gby_test_flag[i][j] > 3)
                            {
                               gby_test_flag[i][j] = 3;
                               (void)Bat_UpdateCellV(i+1, k, cellv);
                            }
                        }
                        else
                        {
                            gby_test_flag[i][j] = 0;
                            (void)Bat_UpdateCellV(i+1, k, cellv);
                        }
                        (void)Bat_UpdateCellV(i+1, k, cellv);  
                        k++;
                    }
                }
            }
        }
        else if(1 == gby_ReadCellCnt)
        {
            gby_ReadCellCnt = 2;
            for(i = 3; i < 6; i++)
            {
                k = 1;
                for (j = 0; j < cell_num_and_pos_per_chip[i]; j++)
                { 
                    if( ((i + 1) == (jumperwire_pos[m] & 0xf0) >> 4) && ((j + 1) == (jumperwire_pos[m] & 0x0f)))  //jumperwire_pos[] 高4bit--保险所在通道号  低4bit--保险位置 
                    {
                        if(m < (MAX_JUMPER_WIRE_NUM - 1))
                        {
                            m++;
                        }
                    } 
                    else 
                    {
                        cellv = gwd_Volt_Bak[i][j] / 10;
                        if((cellv > 4500) || (cellv < 1000))
                        {
                            gby_test_flag[i][j]++;
                            if(gby_test_flag[i][j] > 3)
                            {
                               gby_test_flag[i][j] = 3;
                               (void)Bat_UpdateCellV(i+1, k, cellv);
                            }
                        }
                        else
                        {
                            gby_test_flag[i][j] = 0;
                            (void)Bat_UpdateCellV(i+1, k, cellv);
                        }
                        (void)Bat_UpdateCellV(i+1, k, cellv);  
                        k++;
                    }
                }
            }
        } 
        else 
        {
            gby_ReadCellCnt = 0;
            for(i = 6; i < Gby_Ic_Num; i++)
            {
                k = 1;
                for (j = 0; j < cell_num_and_pos_per_chip[i]; j++)
                { 
                    if( ((i + 1) == (jumperwire_pos[m] & 0xf0) >> 4) && ((j + 1) == (jumperwire_pos[m] & 0x0f)))  //jumperwire_pos[] 高4bit--保险所在通道号  低4bit--保险位置 
                    {
                        if(m < (MAX_JUMPER_WIRE_NUM - 1))
                        {
                            m++;
                        }
                    } 
                    else 
                    {
                        cellv = gwd_Volt_Bak[i][j] / 10;
                        if((cellv > 4500) || (cellv < 1000))
                        {
                            gby_test_flag[i][j]++;
                            if(gby_test_flag[i][j] > 3)
                            {
                               gby_test_flag[i][j] = 3;
                               (void)Bat_UpdateCellV(i+1, k, cellv);
                            }
                        }
                        else
                        {
                            gby_test_flag[i][j] = 0;
                            (void)Bat_UpdateCellV(i+1, k, cellv);
                        }
                        (void)Bat_UpdateCellV(i+1, k, cellv);  
                        k++;
                    }
                }
            }
        } 
    }
    
}
/* end of module voltage measurement */
static void CellvDet_ProcTemperDet(void)
{
   GeneralReturnType rslt = 0;
   uByte i,j,k,t;
   uWord lt_temper_ad[10][6]; 
   uByte lt_temper;
   uWord hderr_detail=0;
   uByte cellt_num_per_chip[20];
   uByte dlen = 0;
   uByte delta_temper;
   uByte total_temper_num;
   
   (void)SysConf_GetParaValueFromRam(SYSCONF_CELLNUM_PER_CHIP, &cellt_num_per_chip[0], &dlen);
    total_temper_num = 0;
    for(i = 0; i < Gby_Ic_Num; i++)
    {
        cellt_num_per_chip[i] = (cellt_num_per_chip[i] & 0xf0) >> 4;
        total_temper_num += cellt_num_per_chip[i];
    }
    rslt = 0;
    
    rslt |= CellvIf_RdAuxValue(Gby_Ic_Num, lt_temper_ad);  //lt_temper_ad
    
    if(ERR_OK == rslt)
    {  
        t=0;
        for(i = 0; i < Gby_Ic_Num; i++)
        {
            t = 0;
            for(j = 0; j < cellt_num_per_chip[i]; j++ )
            {
                if(lt_temper_ad[i][j] >= LT_TEMPER_DET_OPEN_AD_VAL)
                {
                    hderr_detail |= LT_TEMPER_DET_NTC_OPEN_ERR;
                    (void)Bat_UpdateTemper(i+1, t + 1, LT_TEMPER_NTC_OPEN_ERR_VALUE);
                      if(RdAuxValue_Err_Cont<255)
                        RdAuxValue_Err_Cont++;
                    t++;                 
                }
                else if(lt_temper_ad[i][j] <= LT_TEMPER_DET_SHORT_AD_VAL)
                {
                    hderr_detail |= LT_TEMPER_DET_NTC_SHORT_ERR;
                    (void)Bat_UpdateTemper(i+1, t + 1, LT_TEMPER_NTC_SHORT_ERR_VALUE);
                     if(RdAuxValue_Err_Cont<255)
                        RdAuxValue_Err_Cont++; 
                        t++;      
                }
                else 
                {    
                    RdAuxValue_Err_Cont=0;
                    for(k = 0; k < VT_POINT; k++)
                    {
                        if(lt_temper_ad[i][j] <= LT_V_T_10K[k])
                        {
                            lt_temper = VT_POINT - k - 1;
                            break;
                        }
                    }

                    if(VT_POINT == k)
                    {
                        lt_temper = 0;
                    }
                    
                    
                    if(lt_temper >= Lt_NtcTemperBak[i][j])
                    {
                       delta_temper = lt_temper - Lt_NtcTemperBak[i][j];
                    }
                    else
                    {
                       delta_temper = Lt_NtcTemperBak[i][j] - lt_temper;
                    }
                    if((delta_temper <= 10) || (0 == Lt_NtcTemperBak[i][j])||(Lt_Ntc_Cont[i][j]>2))//滤波处理
                    {   
                        Lt_Ntc_Cont[i][j]=0;
                        Lt_NtcTemperBak[i][j] = lt_temper;
                        (void)Bat_UpdateTemper(i+1, t + 1, lt_temper);
                    }
                     else
                    {
                         Lt_Ntc_Cont[i][j]++;
                    }
                    t++;
                }
              
            }
        }
       
    }
    else
    //if(ERR_OK != rslt)
    {       
       for (i = 0; i < Gby_Ic_Num; i++)
       {
            //if pack is error, 
            if ( (rslt & (0x01 << i)) != 0 )
            {
                for (j = 0; j < 3; j++)
                {
                    lt_temper_ad[i][j] = 0;
                }
            }
       }
       
        rslt |= CellvIf_Init(Gby_Ic_Num);    //test
        if(RdAuxValue_Err_Cont<255)
          RdAuxValue_Err_Cont++;
        if(RdAuxValue_Err_Cont>5)
        {  
           (void)Err_UpdateHwErrLevel(ERR_HW_DETECT_TEMPER, ERR_LEVEL_TWO); 
        }
        else
        {
          //(void)Err_UpdateHwErrLevel(ERR_HW_DETECT_TEMPER, ERR_LEVEL_NORMAL);
       }    
    }
    
    (void)Err_UpdateHwErrDetail(ERR_HW_DETECT_TEMPER, hderr_detail);
    rslt = 0;
    CellvIf_StartAdAxCvnt();  
}


static uByte CellvDet_BalCircuitCheck(void)   //均衡电路判断
{
    uByte i, j;
    uByte dlen;
    uWord tmp_wd_volt[10][MAX_CELLV_NUM_PER_IC];
    GeneralReturnType rslt = 0;
    static uByte  check_time = 0;
    uWord fault_delta_volt;

    switch (gby_BalCheckStep)
    {
        case 0:
        {
            rslt |= CellvIf_RdValue(Gby_Ic_Num, gwd_Volt_Bak);

            if (ERR_OK == rslt)
            {
                for (i = 0; i < Gby_Ic_Num; i++)
                {
                    for (j = 0; j < MAX_CELLV_NUM_PER_IC; j += 2)
                    {
                        (void)CellvIf_CtrlBalanceSw(i, j, DCH_BALANCE_ON); 
                        (void)CellvIf_CtrlBalanceSw(i, j + 1, DCH_BALANCE_OFF); 
                    }
                }
                (void)CellvIf_ActiveBalSw();
                (void)CellvIf_StartCnvt(AD_CNVT_MODE_BAL_CIRCUIT); 
                gby_BalCheckStep = 1;
            }
            else
            {
                gby_BalCheckStep = 3;  
                (void)CellvIf_StartCnvt(AD_CNVT_MODE_NORMAL);
            }
 
            break;
        }
        case 1:
        {
            rslt = CellvIf_RdValue(Gby_Ic_Num, tmp_wd_volt);
            if (ERR_OK == rslt)
            {
                for (i = 0; i < Gby_Ic_Num; i++)
                {
                    for(j = 0; j < MAX_CELLV_NUM_PER_IC; j += 2)
                    {
                        fault_delta_volt = gwd_Volt_Bak[i][j] / 100;
                        if ( ( (tmp_wd_volt[i][j] + fault_delta_volt) > gwd_Volt_Bak[i][j] )
                             && (gwd_Volt_Bak[i][j] >= BALANCE_CHECK_FAULT_VOLT) )
                        {
                            gwd_BalFaultPos[i] |= (0x0001U << j);
                        }
                        else
                        {
                            gwd_BalFaultPos[i] &= (~(0x0001U << j));
                        }
                    }
                }
            }


            for (i = 0; i < Gby_Ic_Num; i++)
            {
                for (j = 0; j < MAX_CELLV_NUM_PER_IC; j += 2)
                {
                    (void)CellvIf_CtrlBalanceSw(i, j, DCH_BALANCE_OFF); 
                    (void)CellvIf_CtrlBalanceSw(i, j + 1, DCH_BALANCE_ON); 
                }
            }
            (void)CellvIf_ActiveBalSw();
            (void)CellvIf_StartCnvt(AD_CNVT_MODE_BAL_CIRCUIT); 
            gby_BalCheckStep = 2;
            
            break;
        }
        case 2:
        {
            check_time++;
            rslt = CellvIf_RdValue(Gby_Ic_Num, tmp_wd_volt);
            if (ERR_OK == rslt)
            {
                for(i = 0; i < Gby_Ic_Num; i++)
                {
                  for(j = 1; j < MAX_CELLV_NUM_PER_IC; j += 2)
                  {
                      //fault_delta_volt = gwd_Volt_Bak[i][j] / 40;
                      fault_delta_volt = gwd_Volt_Bak[i][j] / 100;
                      if ( ( (tmp_wd_volt[i][j] + fault_delta_volt) > gwd_Volt_Bak[i][j] )
                           && (gwd_Volt_Bak[i][j] >= BALANCE_CHECK_FAULT_VOLT) )
                      {
                          gwd_BalFaultPos[i] |= (0x0001U << j);
                      }
                      else
                      {
                          gwd_BalFaultPos[i] &= (~(0x0001U << j));
                      }
                  }
                }
            }
           
                       
            for(i = 0; i < Gby_Ic_Num; i++)
            {
                for (j = 0; j < MAX_CELLV_NUM_PER_IC; j++)
                {
                    (void)CellvIf_CtrlBalanceSw(i, j, DCH_BALANCE_OFF); 
                }
                
            }
            (void)CellvIf_ActiveBalSw();
            (void)CellvIf_StartCnvt(AD_CNVT_MODE_NORMAL);
            for(i = 0; i < Gby_Ic_Num; i++)
            {
                if(0 != gwd_BalFaultPos[i])
                {
                    gby_BalFaultCnt++;
                    if(1 == gby_BalFaultCnt)
                    {
                        gby_BalCheckStep = 0;
                        break;
                    }
                }
            }
            
            if(gby_BalFaultCnt > 1)
            {
                (void)Err_UpdateHwErrLevel(ERR_HW_BAL_DCH, ERR_LEVEL_TWO); 
                gby_BalCheckStep = 3; 
            }          
            else
            {
                // inclue first ok, second ok and first error, second ok
               if((gby_BalFaultCnt == 0) || ((gby_BalFaultCnt == 1) && (check_time == 2)))   //first normal
               {
                  gby_BalCheckStep = 3; 
               } 
            }
            
            break;
        }
        default:
        {
            gby_BalCheckStep = 4; 
            break;
        }
    }
    
    return gby_BalCheckStep;

}



static uByte CellvDet_OpenCircuitChk(uByte device_num)//开路判断
{
   static uWord Cellv1[10][12], Cellv2[10][12];
   uByte i,j,m,dlen;
   uByte JumperWirePos[30];
   uByte CellNumPerChip[20];
   uByte cell_offset[20];
   
   (void)SysConf_GetParaValueFromRam(SYSCONF_CELLNUM_PER_CHIP, &CellNumPerChip[0], &dlen);
   (void)SysConf_GetParaValueFromRam(SYSCONF_CHIP_FUSE_POS, &JumperWirePos[0], &dlen);
   
  // CellNumPerChip[0] = 12;  //for test
  // CellNumPerChip[1] = 10;  //for test
   
    m = 0;
    for(i = 0; i < Gby_Ic_Num; i++) 
    {
        cell_offset[i] = 0;
        for(j = 0; j < MAX_CELLV_NUM_PER_IC; j++) 
        {
            if(((i + 1) == ((JumperWirePos[m] & 0xf0) >> 4))
               && (((j + 1) == (JumperWirePos[m] & 0x0f)) ) ) 
            {
                if(m < (MAX_JUMPER_WIRE_NUM - 1))
                {
                    m++;
                    cell_offset[i]++;
                }
            }
        }
    }
   
   switch(gby_OpenWireChkStep)
   {
      case 0:
      {
          (void)CellvIf_StartCnvt(AD_CNVT_MODE_OPEN_CIRCUIT_PU);
          gby_OpenWireChkStep++;
          break;
      }
      case 1:
      {    (void)CellvIf_RdValue(device_num, Cellv1);
          (void)CellvIf_StartCnvt(AD_CNVT_MODE_OPEN_CIRCUIT_PU);
          gby_OpenWireChkStep++;
          break;
      }
      
       case 2:
      {    (void)CellvIf_RdValue(device_num, Cellv1);
          (void)CellvIf_StartCnvt(AD_CNVT_MODE_OPEN_CIRCUIT_PU);
          gby_OpenWireChkStep++;
          break;
      }
      
        
      
      case 3:
      {
          (void)CellvIf_RdValue(device_num, Cellv1);
          (void)CellvIf_StartCnvt(AD_CNVT_MODE_OPEN_CIRCUIT_PD);
          gby_OpenWireChkStep++;
          break;
      }
     
       case 4:
      {   (void)CellvIf_RdValue(device_num, Cellv2);
          (void)CellvIf_StartCnvt(AD_CNVT_MODE_OPEN_CIRCUIT_PD);
          gby_OpenWireChkStep++;
          break;
      }
           case 5:
      {   (void)CellvIf_RdValue(device_num, Cellv2);
          (void)CellvIf_StartCnvt(AD_CNVT_MODE_OPEN_CIRCUIT_PD);
          gby_OpenWireChkStep++;
          break;
      }
      case 6:
      {
          (void)CellvIf_RdValue(device_num, Cellv2);
          for(i = 1; i < device_num; i++)
          {        
             for(j = 0; j < (CellNumPerChip[i] + cell_offset[i] - 1); j++)
             {
               if(((Cellv2[i][j + 1] > Cellv1[i][j + 1] + 3000))
                && (Cellv2[i][0] < 0xFFFF))
               {
                  gwd_OpenWirePos[i] |= (uWord)(0x0001U << j);
               }
               if(Cellv1[i][0] == 0xFFFF)
               {
                  gwd_OpenWirePos[i] |= 0x1000;
               }
               if((Cellv2[i][CellNumPerChip[i] + cell_offset[i] - 1] <= 100) && (Cellv1[i][0] < 0xFFFF))
               {
                  gwd_OpenWirePos[i] |= (uWord)(0x0001U << (CellNumPerChip[i] + cell_offset[i] - 1));
               }
             }  
          } 
          (void)CellvIf_StartCnvt(AD_CNVT_MODE_NORMAL);
          gby_OpenWireChkStep++;
          break;
      }
      default:
      {
         gby_OpenWireChkStep = 14;
         break;
      }
      
   }
   
   return gby_OpenWireChkStep;
   
}


