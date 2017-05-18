// SysConf.c
#include "Ecu.h"
#include "SysConf.h"
#include "EE.h"
#include "ErrorAlarm.h"

#if(ECU_RAM_USE_MODE)      
#ifdef ECU_RAM_USE_GPGAE
#pragma DATA_SEG __GPAGE_SEG PAGED_RAM 
#endif
#ifdef ECU_RAM_USE_RPGAE
#pragma DATA_SEG __RPAGE_SEG PAGED_RAM 
#endif      
#else
#pragma DATA_SEG DEFAULT 
#endif
#define SYS_MAX_PARA_LENGTH 30

/*
 * Project parameter  M_BCU_C ,M_BCU_D
 */ 
/* 0 */uByte gby_para_ProductNO[16]; /* product NO., 16 bytes */
/* 1 */uByte gby_para_CfgVer[8]= {0}; /* config version, 8 bytes */
/* 2 */uByte gby_para_CfgSt=0;     /* config status, 1bytes */
/* 3 */sSysConf_MonitorOperationInfo  ConfigUpdataInfo; /* Config update information, 8 bytes */
/* 4 */sSysConf_MonitorOperationInfo  ProgramUpdataInfo; /* Program update information, 8 bytes */
/* 5 */uByte gby_para_RsvFor_0_4[10]; /* Reserve for para0 to para4, 10 bytes */

/* 6 */uByte gby_para_BmuNumber;     /* bmu number, 1 byte */
/* 7 */uByte gby_para_ChipNumber;//gwd_para_Bmu0CellNum;   /* BCU_C(Bmu0) cellnum 2 bytes */
/* 8 */uWord gwd_para_RsvBak_1;//gwd_para_Bmu0StartCellNO; /* BCU_C(Bmu0) Start Cell NO. , 2bytes*/
/* 9 */uWord gwd_para_RsvBak_2; //gwd_para_Bmu0TemperNum; /* Temperature Number of BCU_C(Bmu0),2 bytes */
/* 10 */uWord gwd_para_RsvBak_3;  /* BCU_C(Bmu0) Start Temperature NO. , 2bytes*/
/* 11 */uByte gwd_para_RsvBak_4[10]; /* Reserve for para6 to para10, 11 bytes */   // use for fuse pos  10byte
       
/* 12 */uWord gwd_para_RsvBak_5[15]; /* Bmu cell number, from bmu1 to bmu15, 30 bytes*/
/* 13 */uWord gwd_para_RsvBak_6[15]; /* Bmu cell number, from bmu16 to bmu30, 30 bytes*/
/* 14 */uWord gwd_para_RsvBak_7[15]; /*  Bmu start cell NO., from bmu1 to bmu15, 30 bytes*/
/* 15 */uWord gwd_para_RsvBak_8[15]; /*  Bmu start cell NO., from bmu16 to bmu30, 30 bytes*/
/* 16 */uWord gwd_para_RsvBak_9[15]; /* Temperature Number of Bmu, from from bmu1 to bmu15,30 bytes  */
/* 17 */uWord gwd_para_RsvBak_10[15]; /* Temperature Number of Bmu, from from bmu16 to bmu30,30 bytes  */
/* 18 */uByte gby_para_chip_fuse_pos[30]; /*  Bmu start Temperature NO., from bmu1 to bmu15, 30 bytes*/
/* 19 */uWord gwd_para_RsvBak_11[15]; /*  Bmu start Temperature NO., from bmu16 to bmu30, 30 bytes*/
/* 20 */uByte gby_para_CellNumPerChip[20];  /* Cell number per Chip in BCU_C(Bmu0), 20 bytes, max Channel is 20,  120/12 = 10 reserve 10 channel for extern  */
/* 21 */uByte gby_para_RsvFor_12_20[8]; /* Reserve for para12 to para20, 8 bytes */
/*145 */uByte qr_cd_infor[30];

/*
 * Other parameter define in other module, and must use SysConf_Init() initial Table
 */

/*
 * Other parameter define in other module, and must use SysConf_Init() initial Table
 */

sSysConf_ParameterType ParameterIndexTable[] =
{
   
    /*
     * |Paremeter NO. | Parameter Ram Address | Parameter Length |
     */
    /* 0 */ {SYSCONF_PRODUCTNO, gby_para_ProductNO,16},
    /* 1 */ {SYSCONF_CFG_VER, gby_para_CfgVer,8},
    /* 2 */ {SYSCONF_CONFIG_ST, &gby_para_CfgSt,1},
    /* 3 */ {SYSCONF_CONFIG_UPDATA_INFO, &ConfigUpdataInfo,8},
    /* 4 */ {SYSCONF_PROGRAM_UPDATE_INFO, &ProgramUpdataInfo,8},
    /* 5 */ {SYSCONF_RSV_FOR_0_4, gby_para_RsvFor_0_4,10},
    
    /* 6 */ {SYSCONF_BMU_NUMBER,&gby_para_BmuNumber,1},
    /* 7 */ {SYSCONF_CHIP_NUMBER,&gby_para_ChipNumber,1},
    /* 8 */ {SYSCONF_RSVBAK_1,&gwd_para_RsvBak_1,2},
    /* 9 */ {SYSCONF_RSVBAK_2,&gwd_para_RsvBak_2,2},
    /* 10 */{SYSCONF_RSVBAK_3,&gwd_para_RsvBak_3,2},
    /* 11 */{SYSCONF_RSVBAK_4,gwd_para_RsvBak_4,10},
    
    /* 12 */{SYSCONF_RSVBAK_5,gwd_para_RsvBak_5,30},
    /* 13 */{SYSCONF_RSVBAK_6,gwd_para_RsvBak_6,30},
    /* 14 */{SYSCONF_RSVBAK_7,gwd_para_RsvBak_7,30},
    /* 15 */{SYSCONF_RSVBAK_8,gwd_para_RsvBak_8,30},
    /* 16 */{SYSCONF_RSVBAK_9,gwd_para_RsvBak_9,30},
    /* 17 */{SYSCONF_RSVBAK_10,gwd_para_RsvBak_10,30},   
    /* 18 */{SYSCONF_CHIP_FUSE_POS,gby_para_chip_fuse_pos,30},    //  SYSCONF_BMU_START_TEMPERNO_G1
    /* 19 */{SYSCONF_RSVBAK_11,gwd_para_RsvBak_11,30},
    /* 20 */{SYSCONF_CELLNUM_PER_CHIP,gby_para_CellNumPerChip,20},
    /* 21 */{SYSCONF_RSV_FOR_12_20,gby_para_RsvFor_12_20,8},
    
    
    /* 22 */{ERRORALARM_THRHD_LVV1,NULL,28},   
    /* 23 */{ERRORALARM_RSV_FOR_21_21,NULL,30},
    /* 24 */{ERRORALARM_THRHD_LVV2,NULL,28},     
    /* 25 */{ERRORALARM_RSV_FOR_23_23,NULL, 30},
    /* 26 */{ERRORALARM_THRHD_LVV3,NULL,28,}, 
    /* 27 */{ERRORALARM_RSV_FOR_22_26,NULL,30},
  
    /* 28 */{THM_HEAT_MANAGE_PARA,NULL,7},
    /* 29 */{THM_RSV_FOR_28_28, NULL,21}, 
  
    /* 30 */{SYSTEMST_CHG_CTRL_PARA,NULL,14},
    /* 31 */{SYSTEMST_RSV_FOR_30_30, NULL,14},
  
    /* 32 */{CURDET_CURRENT_CALIB_PARA,NULL,30},
    /* 33 */{SUMVDET_SUMVIN_CALIB_PARA,NULL,6},
    /* 34 */{SUMVDET_SUMVOUT_CALIB_PARA,NULL,6}, 
    /* 35 */{SUMVDET_SUMVINBAK_CALIB_PARA,NULL,6}, 
    /* 36 */{SUMVDET_SUMVOUTBAK_CALIB_PARA, NULL,6},  
    /* 37 */{ISODET_ISO_CALIB_PARA,NULL,6},
  
    /* 38 */{SYSTEMST_BAT_PACK_INFO_G1,NULL,30},
    /* 39 */{SYSTEMST_BAT_PACK_INFO_G2,NULL, 4},
    /* 40 */{SYSTEMST_RSV_FOR_38_39,NULL, 24}, 
  
    /* 41 */{SYSTEMST_BAT_PACK_INFO_G3,NULL,8},
    /* 42 */{SYSTEMST_BAT_PACK_INFO_G4,NULL,6},
    /* 43 */{SYSTEMST_RSV_FOR_41_42,NULL,4},
  
    /* 44 */{SYSTEMST_HSTY_CHG_DCH_INFO,NULL,6},
    /* 45 */{SYSTEMST_HSTY_INRES_INFO,NULL,4},
    /* 46 */{SYSTEMST_HSTY_CELLV_INFO, NULL,14}, 
    /* 47 */{SYSTEMST_HSTY_TEMPER_INFO,NULL, 5},
    /* 48 */{SYSTEMST_HSTY_POWER_INFO,NULL, 8},
    /* 49 */{SYSTEMST_RSV_FOR_44_48,NULL, 20},
  
    /* 50 */{SYSTEMST_HSTY_TEMPER_AREA_INFO_G1,NULL, 28},
    /* 51 */{SYSTEMST_HSTY_TEMPER_AREA_INFO_G2,NULL, 28},
    /* 52 */{SYSTEMST_HSTY_TEMPER_AREA_INFO_G3,NULL, 28},
    /* 53 */{SYSTEMST_HSTY_TEMPER_AREA_INFO_G4,NULL, 28},
    /* 54 */{SYSTEMST_HSTY_TEMPER_AREA_INFO_G5,NULL, 28},
    /* 55 */{SYSTEMST_HSTY_TEMPER_AREA_INFO_G6,NULL, 28},
    /* 56 */{SYSTEMST_HSTY_TEMPER_AREA_INFO_G7,NULL, 28},
    /* 57 */{SYSTEMST_HSTY_TEMPER_AREA_INFO_G8,NULL, 28},
    /* 58 */{SYSTEMST_HSTY_CURRENT_AREA_INFO_G1,NULL,28},
    /* 59 */{SYSTEMST_HSTY_CURRENT_AREA_INFO_G2,NULL,28},
    /* 60 */{SYSTEMST_HSTY_SOC_AREA_INFO_G1,NULL, 28},
    /* 61 */{SYSTEMST_HSTY_SOC_AREA_INFO_G2,NULL, 12},
    /* 62 */{SYSTEMST_RSV_FOR_50_61,NULL, 16},
  
    /* 63 */{EE_RECOGNIZE,NULL, 2},
    /* 64 */{SYSTEMST_REMAIN_CAPACITY_BAK_G1,NULL,4}, 
    /* 65 */{SYSTEMST_REMAIN_CAPACITY_BAK_G2,NULL,4},
    /* 66 */{SYSTEMST_REMAIN_CAPACITY_BAK_G3,NULL,4}, 
    /* 67 */{SYSTEMST_REMAIN_CAPACITY_BAK_G4,NULL,4},
    /* 68 */{SYSTEMST_REMAIN_CAPACITY_BAK_G5,NULL,4},
    /* 69 */{SYSTEMST_REMAIN_CAPACITY_BAK_G6,NULL,4}, 
    /* 70 */{SYSTEMST_REMAIN_AH_CNT_BAK_G1,NULL,4},
    /* 71 */{SYSTEMST_REMAIN_AH_CNT_BAK_G2,NULL,4},
    /* 72 */{SYSTEMST_REMAIN_AH_CNT_BAK_G3,NULL,4},
    /* 73 */{SYSTEMST_REMAIN_AH_CNT_BAK_G4,NULL,4},
    /* 74 */{SYSTEMST_REMAIN_AH_CNT_BAK_G5,NULL,4},
    /* 75 */{SYSTEMST_REMAIN_AH_CNT_BAK_G6,NULL,4},
    /* 76 */{SYSTEMST_RATED_ACTUAL_CAPACITY,NULL,8},
    /* 77 */{SYSTEMST_SOH,NULL,2},
    /* 78 */{SYSTEMST_TOTAL_CHG_DCH_AH,NULL,8},
    /* 79 */{SYSTEMST_SOH_CTRL_PARA, NULL,17},
    /* 80 */{SYSTEMST_SUM_INRES,NULL, 26},
    /* 81 */{SYSTEMST_POWEROFF_RESET_INFO,NULL,9}, 
    /* 82 */{SYSTEMST_BMS_TOTAL_RUNTIME,NULL, 4},
    /* 83 */{SYSTEMST_CONTACTOR_LIFE,NULL, 12},
    /* 84 */{SYSTEMST_REMAIN_MILEAGE,NULL, 2},
    /* 85 */{SYSTEMST_DATA_RECORD_PAGE,NULL,7},
            {BALM_CALC_BAL_MODE,NULL,1}, 
    /* 86 */{SYSTEMST_RSV_FOR_87,NULL, 14},
    /* 87 */{SYSTEMST_RSV_FOR_88,NULL,19},
  
    /* 88 */{SYSTEMST_VEHICLE_VIN,NULL, 17},
    /* 89 */{SYSTEMST_VEHICLE_PLATE,NULL,8},
    /* 90 */{SYSTEMST_RSV_FOR_88_89,NULL,30},  
  
    /* 91 */{SYSTEMST_BAL_TIME_1_15,     NULL,30}, 
    /* 92 */{SYSTEMST_BAL_TIME_16_30,    NULL,30},  
    /* 93 */{SYSTEMST_BAL_TIME_31_45,    NULL,30}, 
    /* 94 */{SYSTEMST_BAL_TIME_46_60,    NULL,30}, 
    /* 95 */{SYSTEMST_BAL_TIME_61_75,    NULL,30}, 
    /* 96 */{SYSTEMST_BAL_TIME_76_90,    NULL,30}, 
    /* 97 */{SYSTEMST_BAL_TIME_91_105,   NULL,30},  
    /* 98 */{SYSTEMST_BAL_TIME_106_120,  NULL,30}, 
    /* 99 */{SYSTEMST_BAL_TIME_121_135,  NULL,30}, 
    /* 100 */{SYSTEMST_BAL_TIME_136_150, NULL,30}, 
    /* 101 */{SYSTEMST_BAL_TIME_151_165, NULL,30}, 
    /* 102 */{SYSTEMST_BAL_TIME_166_180, NULL,30}, 
    /* 103 */{SYSTEMST_BAL_TIME_181_195, NULL,30}, 
    /* 104 */{SYSTEMST_BAL_TIME_196_210, NULL,30}, 
    /* 105 */{SYSTEMST_BAL_TIME_211_225, NULL,30}, 
    /* 106 */{SYSTEMST_BAL_TIME_226_240, NULL,30}, 
    /* 107 */{SYSTEMST_BAL_TIME_241_255, NULL,30}, 
    /* 108 */{SYSTEMST_BAL_TIME_256_270, NULL,30},   
    /* 109 */{SYSTEMST_BAL_TIME_271_285, NULL,30}, 
    /* 110 */{SYSTEMST_BAL_TIME_286_300, NULL,30}, 
     
    /* 111 */{SYSTEMST_TOTAL_CHG_BAL_TIME_1_20,    NULL,30}, 
    /* 112 */{SYSTEMST_TOTAL_CHG_BAL_TIME_21_40,   NULL,30}, 
    /* 113 */{SYSTEMST_TOTAL_CHG_BAL_TIME_41_60,   NULL,30}, 
    /* 114 */{SYSTEMST_TOTAL_CHG_BAL_TIME_61_80,   NULL,30}, 
    /* 115 */{SYSTEMST_TOTAL_CHG_BAL_TIME_81_100,  NULL,30}, 
    /* 116 */{SYSTEMST_TOTAL_CHG_BAL_TIME_101_120, NULL,30}, 
    /* 117 */{SYSTEMST_TOTAL_CHG_BAL_TIME_121_140, NULL,30}, 
    /* 118 */{SYSTEMST_TOTAL_CHG_BAL_TIME_141_160, NULL,30}, 
    /* 119 */{SYSTEMST_TOTAL_CHG_BAL_TIME_161_180, NULL,30}, 
    /* 120 */{SYSTEMST_TOTAL_CHG_BAL_TIME_181_200, NULL,30}, 
    /* 121 */{SYSTEMST_TOTAL_CHG_BAL_TIME_201_220, NULL,30}, 
    /* 122 */{SYSTEMST_TOTAL_CHG_BAL_TIME_221_240, NULL,30}, 
    /* 123 */{SYSTEMST_TOTAL_CHG_BAL_TIME_241_260, NULL,30}, 
    /* 124 */{SYSTEMST_TOTAL_CHG_BAL_TIME_261_280, NULL,30}, 
    /* 125 */{SYSTEMST_TOTAL_CHG_BAL_TIME_281_300, NULL,30}, 
    
    /* 126 */{SYSTEMST_TOTAL_DCH_BAL_TIME_1_20,    NULL,30}, 
    /* 127 */{SYSTEMST_TOTAL_DCH_BAL_TIME_21_40,   NULL,30}, 
    /* 128 */{SYSTEMST_TOTAL_DCH_BAL_TIME_41_60,   NULL,30}, 
    /* 129 */{SYSTEMST_TOTAL_DCH_BAL_TIME_61_80,   NULL,30}, 
    /* 130 */{SYSTEMST_TOTAL_DCH_BAL_TIME_81_100,  NULL,30}, 
    /* 131 */{SYSTEMST_TOTAL_DCH_BAL_TIME_101_120, NULL,30}, 
    /* 132 */{SYSTEMST_TOTAL_DCH_BAL_TIME_121_140, NULL,30}, 
    /* 133 */{SYSTEMST_TOTAL_DCH_BAL_TIME_141_160, NULL,30}, 
    /* 134 */{SYSTEMST_TOTAL_DCH_BAL_TIME_161_180, NULL,30}, 
    /* 135 */{SYSTEMST_TOTAL_DCH_BAL_TIME_181_200, NULL,30}, 
    /* 136 */{SYSTEMST_TOTAL_DCH_BAL_TIME_201_220, NULL,30}, 
    /* 137 */{SYSTEMST_TOTAL_DCH_BAL_TIME_221_240, NULL,30}, 
    /* 138 */{SYSTEMST_TOTAL_DCH_BAL_TIME_241_260, NULL,30}, 
    /* 138 */{SYSTEMST_TOTAL_DCH_BAL_TIME_261_280, NULL,30}, 
    /* 140 */{SYSTEMST_TOTAL_DCH_BAL_TIME_281_300, NULL,30}, 
   
    /* 141 */{TEMPERDET_BMU0_18B20ROM_G1, NULL,30}, 
    /* 142 */{TEMPERDET_BMU0_18B20ROM_G2, NULL,30}, 
    /* 143 */{TEMPERDET_BMU0_18B20ROM_G3, NULL,30}, 
    /* 144 */{TEMPERDET_BMU0_18B20ROM_G4, NULL,30}, 
    /* 145 */{QR_CODE_INFOR, qr_cd_infor,30},            //SYSTEMST_RSV_FOR1 
    /* 146 */{SUMVDET_SUMVINBAK1_CALIB_PARA, NULL,6},   
    /* 147 */{SUMVDET_SUMVOUTBAK1_CALIB_PARA, NULL,6}, 
    /* 148 */{FL_BOOT_MODE, NULL,4},

};



const sSysConf_ParameterAddressType ParameterAddrTable[] =
{

    /*
     * |Paremeter NO. | Max Permit Length |Parameter Bakup Number | Relative Address (CPU,EXT )|
     */
    /* 0 */ {SYSCONF_PRODUCTNO, 16, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0000,0x0000}},
    /* 1 */ {SYSCONF_CFG_VER, 8, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0012,0x0012}},
    /* 2 */ {SYSCONF_CONFIG_ST, 1, SYSCONF_BAKUP_ONE,SYSCONF_BAKUP_ONE, {0x001C,0x001C}},
    /* 3 */ {SYSCONF_CONFIG_UPDATA_INFO, 8, SYSCONF_BAKUP_ONE,SYSCONF_BAKUP_ONE, {0x0020,0x0020}},
    /* 4 */ {SYSCONF_PROGRAM_UPDATE_INFO, 8, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x002A,0x002A}},
    /* 5 */ {SYSCONF_RSV_FOR_0_4, 10, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0034,0x0034}},

    /* 6 */ {SYSCONF_BMU_NUMBER, 1, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0040,0x0040}},
    /* 7 */ {SYSCONF_CHIP_NUMBER,1, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0043,0x0043}},
    /* 8 */ {SYSCONF_RSVBAK_1,2, SYSCONF_BAKUP_ONE,SYSCONF_BAKUP_ONE, {0x0047,0x0047}},
    /* 9 */ {SYSCONF_RSVBAK_2,2, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x004B,0x004B}}, 
    /* 10 */{SYSCONF_RSVBAK_3,2, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x004F,0x004F}},
    /* 11 */{SYSCONF_RSVBAK_4,10, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0053,0x0053}},
    
    /* 12 */{SYSCONF_RSVBAK_5,30, SYSCONF_BAKUP_ONE,SYSCONF_BAKUP_ONE, {0x0060,0x0060}},
    /* 13 */{SYSCONF_RSVBAK_6,30, SYSCONF_BAKUP_ONE,SYSCONF_BAKUP_ONE, {0x0080,0x0080}},
    /* 14 */{SYSCONF_RSVBAK_7,30, SYSCONF_BAKUP_ONE,SYSCONF_BAKUP_ONE, {0x00A0,0x00A0}},
    /* 15 */{SYSCONF_RSVBAK_8,30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x00C0,0x00C0}},   
    /* 16 */{SYSCONF_RSVBAK_9,30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x00E0,0x00E0}},
    /* 17 */{SYSCONF_RSVBAK_10,30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0100,0x0100}},
    /* 18 */{SYSCONF_CHIP_FUSE_POS,30, SYSCONF_BAKUP_ONE,SYSCONF_BAKUP_ONE, {0x0120,0x0120}},
    /* 19 */{SYSCONF_RSVBAK_11,30, SYSCONF_BAKUP_ONE,SYSCONF_BAKUP_ONE, {0x0140,0x0140}},
    /* 20 */{SYSCONF_CELLNUM_PER_CHIP,20, SYSCONF_BAKUP_ONE,SYSCONF_BAKUP_ONE, {0x0160,0x0160}},
    /* 21 */{SYSCONF_RSV_FOR_12_20,8, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0176,0x0176}},

    /* 22 */{ERRORALARM_THRHD_LVV1,28, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0180,0x0180}},    
    /* 23 */{ERRORALARM_RSV_FOR_21_21,30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x01A0,0x01A0}},   
    /* 24 */{ERRORALARM_THRHD_LVV2,28, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x01C0,0x01C0}},      
    /* 25 */{ERRORALARM_RSV_FOR_23_23, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x01E0,0x01E0}},  
    /* 26 */{ERRORALARM_THRHD_LVV3,28, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0200,0x0200}},  
    /* 27 */{ERRORALARM_RSV_FOR_22_26,30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0220,0x0220}},  
  
    /* 28 */{THM_HEAT_MANAGE_PARA,7, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0240,0x0240}},  
    /* 29 */{THM_RSV_FOR_28_28, 21, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0249,0x0249}},  
  
    /* 30 */{SYSTEMST_CHG_CTRL_PARA,14, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0260,0x0260}},  
    /* 31 */{SYSTEMST_RSV_FOR_30_30, 14, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0270,0x0270}},  
  
    /* 32 */{CURDET_CURRENT_CALIB_PARA,30,SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0280,0x0280}},  
    /* 33 */{SUMVDET_SUMVIN_CALIB_PARA,6, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x02A0,0x02A0}},  
    /* 34 */{SUMVDET_SUMVOUT_CALIB_PARA,6, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x02A8,0x02A8}},  
    /* 35 */{SUMVDET_SUMVINBAK_CALIB_PARA,6, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x02B0,0x02B0}},  
    /* 36 */{SUMVDET_SUMVOUTBAK_CALIB_PARA, 6, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x02B8,0x02B8}},  
    /* 37 */{ISODET_ISO_CALIB_PARA,6, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x02C0,0x02C0}},  
    
  
    /* 38 */{SYSTEMST_BAT_PACK_INFO_G1,30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x02E0,0x02E0}}, 
    /* 39 */{SYSTEMST_BAT_PACK_INFO_G2, 4, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0300,0x0300}}, 
    /* 40 */{SYSTEMST_RSV_FOR_38_39, 24, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0306,0x0306}}, 
  
    /* 41 */{SYSTEMST_BAT_PACK_INFO_G3,8, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0C00,0x0C00}}, 
    /* 42 */{SYSTEMST_BAT_PACK_INFO_G4,6, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0C0A,0x0C0A}}, 
    /* 43 */{SYSTEMST_RSV_FOR_41_42,4, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0C12,0x0C12}}, 
  
    /* 44 */{SYSTEMST_HSTY_CHG_DCH_INFO,6, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0C18,0x0C18}}, 
    /* 45 */{SYSTEMST_HSTY_INRES_INFO,4, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0C20,0x0C20}}, 
    /* 46 */{SYSTEMST_HSTY_CELLV_INFO, 14, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0C26,0x0C26}}, 
    /* 47 */{SYSTEMST_HSTY_TEMPER_INFO, 5, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0C36,0x0C36}}, 
    /* 48 */{SYSTEMST_HSTY_POWER_INFO, 8, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0C40,0x0C40}},   
    /* 49 */{SYSTEMST_RSV_FOR_44_48, 20, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0C4A,0x0C4A}}, 
  
    /* 50 */{SYSTEMST_HSTY_TEMPER_AREA_INFO_G1, 28, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0320,0x0320}}, 
    /* 51 */{SYSTEMST_HSTY_TEMPER_AREA_INFO_G2, 28, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0340,0x0340}}, 
    /* 52 */{SYSTEMST_HSTY_TEMPER_AREA_INFO_G3, 28, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0360,0x0360}}, 
    /* 53 */{SYSTEMST_HSTY_TEMPER_AREA_INFO_G4, 28, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0380,0x0380}}, 
    /* 54 */{SYSTEMST_HSTY_TEMPER_AREA_INFO_G5, 28, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x03A0,0x03A0}}, 
    /* 55 */{SYSTEMST_HSTY_TEMPER_AREA_INFO_G6, 28, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x03C0,0x03C0}}, 
    /* 56 */{SYSTEMST_HSTY_TEMPER_AREA_INFO_G7, 28, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x03E0,0x03E0}}, 
    /* 57 */{SYSTEMST_HSTY_TEMPER_AREA_INFO_G8, 28, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0400,0x0400}},  
    /* 58 */{SYSTEMST_HSTY_CURRENT_AREA_INFO_G1,28, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0420,0x0420}}, 
    /* 59 */{SYSTEMST_HSTY_CURRENT_AREA_INFO_G2,28, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0440,0x0440}}, 
    /* 60 */{SYSTEMST_HSTY_SOC_AREA_INFO_G1, 28, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0460,0x0460}}, 
    /* 61 */{SYSTEMST_HSTY_SOC_AREA_INFO_G2, 12, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0480,0x0480}},
    /* 62 */{SYSTEMST_RSV_FOR_50_61, 16, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x048E,0x048E}}, 
  
    /* 63 */{EE_RECOGNIZE, 2, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0C60,0x0C60}}, 
    /* 64 */{SYSTEMST_REMAIN_CAPACITY_BAK_G1,4, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0C64,0x0C64}},  
    /* 65 */{SYSTEMST_REMAIN_CAPACITY_BAK_G2,4, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0C6A,0x0C6A}},  
    /* 66 */{SYSTEMST_REMAIN_CAPACITY_BAK_G3,4, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0C70,0x0C70}},  
    /* 67 */{SYSTEMST_REMAIN_CAPACITY_BAK_G4,4, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0C76,0x0C76}},  
    /* 68 */{SYSTEMST_REMAIN_CAPACITY_BAK_G5,4, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0C80,0x0C80}},  
    /* 69 */{SYSTEMST_REMAIN_CAPACITY_BAK_G6,4, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0C86,0x0C86}},    
    /* 70 */{SYSTEMST_REMAIN_AH_CNT_BAK_G1,4, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0C8C,0x0C8C}},  
    /* 71 */{SYSTEMST_REMAIN_AH_CNT_BAK_G2,4, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0C92,0x0C92}}, 
    /* 72 */{SYSTEMST_REMAIN_AH_CNT_BAK_G3,4, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0C98,0x0C98}}, 
    /* 73 */{SYSTEMST_REMAIN_AH_CNT_BAK_G4,4, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0CA0,0x0CA0}}, 
    /* 74 */{SYSTEMST_REMAIN_AH_CNT_BAK_G5,4, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0CA6,0x0CA6}}, 
    /* 75 */{SYSTEMST_REMAIN_AH_CNT_BAK_G6,4, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0CAC,0x0CAC}}, 
    /* 76 */{SYSTEMST_RATED_ACTUAL_CAPACITY,8, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0CB2,0x0CB2}},
    /* 77 */{SYSTEMST_SOH, 2, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0CBC,0x0CBC}},  
    /* 78 */{SYSTEMST_TOTAL_CHG_DCH_AH,8, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0CC0,0x0CC0}},  
    /* 79 */{SYSTEMST_SOH_CTRL_PARA, 17, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0CCA,0x0CCA}}, 
    /* 80 */{SYSTEMST_SUM_INRES, 26, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0CE0,0x0CE0}}, 
    /* 81 */{SYSTEMST_POWEROFF_RESET_INFO,9, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0D00,0x0D00}},  
    /* 82 */{SYSTEMST_BMS_TOTAL_RUNTIME, 4, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0D0B,0x0D0B}}, 
    /* 83 */{SYSTEMST_CONTACTOR_LIFE, 12, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0D11,0x0D11}}, 
    /* 84 */{SYSTEMST_REMAIN_MILEAGE, 2, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0D20,0x0D20}}, 
    /* 85 */{SYSTEMST_DATA_RECORD_PAGE,7, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0D24,0x0D24}}, 
            {BALM_CALC_BAL_MODE,1, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0D2D,0x0D2D}}, 
    /* 86 */{SYSTEMST_RSV_FOR_87,  14, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0D30,0x0D30}},  
    /* 87 */{SYSTEMST_RSV_FOR_88, 19, SYSCONF_BAKUP_THREE, SYSCONF_BAKUP_ONE,{0x0D40,0x0D40}}, 
  
    /* 88 */{SYSTEMST_VEHICLE_VIN, 17, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x04A0,0x04A0}}, 
    /* 89 */{SYSTEMST_VEHICLE_PLATE,8, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x04B3,0x04B3}},  
    /* 90 */{SYSTEMST_RSV_FOR_88_89,30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x04C0,0x04C0}},  
  
    /* 91 */{SYSTEMST_BAL_TIME_1_15,     30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x04E0,0x04E0}}, 
    /* 92 */{SYSTEMST_BAL_TIME_16_30,    30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0500,0x0500}},  
    /* 93 */{SYSTEMST_BAL_TIME_31_45,    30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0520,0x0520}}, 
    /* 94 */{SYSTEMST_BAL_TIME_46_60,    30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0540,0x0540}}, 
    /* 95 */{SYSTEMST_BAL_TIME_61_75,    30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0560,0x0560}}, 
    /* 96 */{SYSTEMST_BAL_TIME_76_90,    30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0580,0x0580}},
    /* 97 */{SYSTEMST_BAL_TIME_91_105,   30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x05A0,0x05A0}},  
    /* 98 */{SYSTEMST_BAL_TIME_106_120,  30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x05C0,0x05C0}}, 
    /* 99 */{SYSTEMST_BAL_TIME_121_135,  30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x05E0,0x05E0}},
    /* 100 */{SYSTEMST_BAL_TIME_136_150, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0600,0x0600}}, 
    /* 101 */{SYSTEMST_BAL_TIME_151_165, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0620,0x0620}}, 
    /* 102 */{SYSTEMST_BAL_TIME_166_180, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0640,0x0640}}, 
    /* 103 */{SYSTEMST_BAL_TIME_181_195, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0660,0x0660}}, 
    /* 104 */{SYSTEMST_BAL_TIME_196_210, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0680,0x0680}}, 
    /* 105 */{SYSTEMST_BAL_TIME_211_225, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x06A0,0x06A0}}, 
    /* 106 */{SYSTEMST_BAL_TIME_226_240, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x06C0,0x06C0}}, 
    /* 107 */{SYSTEMST_BAL_TIME_241_255, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x06E0,0x06E0}},  
    /* 108 */{SYSTEMST_BAL_TIME_256_270, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0700,0x0700}},  
    /* 109 */{SYSTEMST_BAL_TIME_271_285, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0720,0x0720}},  
    /* 110 */{SYSTEMST_BAL_TIME_286_300, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0740,0x0740}},
     
    /* 111 */{SYSTEMST_TOTAL_CHG_BAL_TIME_1_20,    30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0760,0x0760}},
    /* 112 */{SYSTEMST_TOTAL_CHG_BAL_TIME_21_40,   30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0780,0x0780}},
    /* 113 */{SYSTEMST_TOTAL_CHG_BAL_TIME_41_60,   30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x07A0,0x07A0}},
    /* 114 */{SYSTEMST_TOTAL_CHG_BAL_TIME_61_80,   30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x07C0,0x07C0}},
    /* 115 */{SYSTEMST_TOTAL_CHG_BAL_TIME_81_100,  30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x07E0,0x07E0}},
    /* 116 */{SYSTEMST_TOTAL_CHG_BAL_TIME_101_120, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0800,0x0800}},
    /* 117 */{SYSTEMST_TOTAL_CHG_BAL_TIME_121_140, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0820,0x0820}},
    /* 118 */{SYSTEMST_TOTAL_CHG_BAL_TIME_141_160, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0840,0x0840}},
    /* 119 */{SYSTEMST_TOTAL_CHG_BAL_TIME_161_180, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0860,0x0860}},
    /* 120 */{SYSTEMST_TOTAL_CHG_BAL_TIME_181_200, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0880,0x0880}},
    /* 121 */{SYSTEMST_TOTAL_CHG_BAL_TIME_201_220, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x08A0,0x08A0}},
    /* 122 */{SYSTEMST_TOTAL_CHG_BAL_TIME_221_240, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x08C0,0x08C0}},
    /* 123 */{SYSTEMST_TOTAL_CHG_BAL_TIME_241_260, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x08E0,0x08E0}},
    /* 124 */{SYSTEMST_TOTAL_CHG_BAL_TIME_261_280, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0900,0x0900}},
    /* 125 */{SYSTEMST_TOTAL_CHG_BAL_TIME_281_300, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0920,0x0920}},
    
    /* 126 */{SYSTEMST_TOTAL_DCH_BAL_TIME_1_20,    30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0940,0x0940}},
    /* 127 */{SYSTEMST_TOTAL_DCH_BAL_TIME_21_40,   30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0960,0x0960}},
    /* 128*/{SYSTEMST_TOTAL_DCH_BAL_TIME_41_60,    30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0980,0x0980}},
    /* 129 */{SYSTEMST_TOTAL_DCH_BAL_TIME_61_80,   30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x09A0,0x09A0}},
    /* 130 */{SYSTEMST_TOTAL_DCH_BAL_TIME_81_100,  30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x09C0,0x09C0}},
    /* 131 */{SYSTEMST_TOTAL_DCH_BAL_TIME_101_120, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x09E0,0x09E0}},
    /* 132 */{SYSTEMST_TOTAL_DCH_BAL_TIME_121_140, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0A00,0x0A00}},
    /* 133 */{SYSTEMST_TOTAL_DCH_BAL_TIME_141_160, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0A20,0x0A20}},
    /* 134 */{SYSTEMST_TOTAL_DCH_BAL_TIME_161_180, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0A40,0x0A40}},
    /* 135 */{SYSTEMST_TOTAL_DCH_BAL_TIME_181_200, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0A60,0x0A60}},
    /* 136 */{SYSTEMST_TOTAL_DCH_BAL_TIME_201_220, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0A80,0x0A80}},
    /* 137 */{SYSTEMST_TOTAL_DCH_BAL_TIME_221_240, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0AA0,0x0AA0}},
    /* 138 */{SYSTEMST_TOTAL_DCH_BAL_TIME_241_260, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0AC0,0x0AC0}},
    /* 139 */{SYSTEMST_TOTAL_DCH_BAL_TIME_261_280, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0AE0,0x0AE0}},
    /* 140 */{SYSTEMST_TOTAL_DCH_BAL_TIME_281_300, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0B00,0x0B00}},
   
    /* 141 */{TEMPERDET_BMU0_18B20ROM_G1, 27, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0B20,0x0B20}},
    /* 142 */{TEMPERDET_BMU0_18B20ROM_G2, 27, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0B40,0x0B40}},
    /* 143 */{TEMPERDET_BMU0_18B20ROM_G3, 27, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0B60,0x0B60}},
    /* 144 */{TEMPERDET_BMU0_18B20ROM_G4, 27, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0B80,0x0B80}},
    /* 145 */{QR_CODE_INFOR, 30, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0BA0,0x0BA0}},  //QR_CODE_INFOR
    /* 146 */{SUMVDET_SUMVINBAK1_CALIB_PARA, 6, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0BC0,0x0BC0}},
    /* 147 */{SUMVDET_SUMVOUTBAK1_CALIB_PARA, 6, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0BE0,0x0BE0}},
    /* 148 */{FL_BOOT_MODE, 4, SYSCONF_BAKUP_ONE, SYSCONF_BAKUP_ONE,{0x0C00,0x0C00}},
      
};                                                                             
#define SYSCONF_DEAL_PARA_FROM_EE(para, max)  (((para) > (max))? ((para) = (max)):((para) = (para)))
void SysConf_Init(void)
{
    uByte i,btmp;
    GeneralReturnType rslt;
// initial parameter value

    for(i = 0; i < 16; i++)
    {
        gby_para_ProductNO[i] = 0;
    }

    for(i = 0; i < 8; i++)
    {
        gby_para_CfgVer[i] = 0;
    }
    
    gby_para_CfgSt=0;
    
    ConfigUpdataInfo.User = 0;
    for(i = 0; i < 6; i++)
    {
        ConfigUpdataInfo.Date[i]  = 0;
    }

    ProgramUpdataInfo.User = 0;
    for(i = 0; i < 6; i++)
    {
        ProgramUpdataInfo.Date[i]  = 0;
    }
    
    for(i = 0; i < 10; i++)
    {
        gby_para_RsvFor_0_4[i] = 0;
    }
    
    gby_para_BmuNumber = 0;
    
    //gwd_para_Bmu0CellNum = 0;
    gby_para_ChipNumber =0;
    
    //gwd_para_Bmu0StartCellNO = 0;
    gwd_para_RsvBak_1=0;
    
    //gwd_para_Bmu0TemperNum = 0;
    gwd_para_RsvBak_2=0;
    
    //gwd_para_Bmu0StartTemperNO = 0;
    gwd_para_RsvBak_3=0;
    
    for(i = 0; i < 10; i++)
    {
        gwd_para_RsvBak_4[i] = 0;
    }
      
    for(i = 0; i < 15; i++)
    {
        gwd_para_RsvBak_5[i] = 0;
        gwd_para_RsvBak_6[i] = 0;
    }
  
    for(i = 0; i < 15; i++)
    {
        gwd_para_RsvBak_7[i] = 0;
        gwd_para_RsvBak_8[i] = 0;
    }
    
    for(i = 0; i < 15; i++)
    {
        gwd_para_RsvBak_9[i] = 0;
        gwd_para_RsvBak_10[i] = 0;
        gwd_para_RsvBak_11[i] = 0;
    }
    

    
    for(i = 0; i < 20; i++)
    {
        gby_para_CellNumPerChip[i] = 0;
    }
    
    for(i = 0; i < 8; i++)
    {
        gby_para_RsvFor_12_20[i] = 0;
    }
    
    for(i = 0; i < 30; i++)
    {
        qr_cd_infor[i]=0;
        gby_para_chip_fuse_pos[i]=0;
    }

    /* load parameter from EE */
    rslt = EE_LoadVar(SYSCONF_PRODUCTNO);    
    rslt = EE_LoadVar(SYSCONF_CFG_VER);
    rslt = EE_LoadVar(SYSCONF_CONFIG_ST);
    rslt = EE_LoadVar(SYSCONF_CONFIG_UPDATA_INFO);
    rslt = EE_LoadVar(SYSCONF_PROGRAM_UPDATE_INFO);
    
    //rslt = EE_LoadVar(SYSCONF_RSV_FOR_0_4);
   
    rslt = EE_LoadVar(SYSCONF_BMU_NUMBER);
    //rslt = 1; //test
    if(rslt)
    {
        gby_para_BmuNumber = 1;
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_READ);
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_EXT, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_EXT, EE_HDERR_READ);
    }
    SYSCONF_DEAL_PARA_FROM_EE(gby_para_BmuNumber,MAX_BMU_NUM);
    
    rslt = EE_LoadVar(SYSCONF_CELLNUM_PER_CHIP);

    if(rslt)
    {
        for(i = 0;i < 20;i++)
        {
            gby_para_CellNumPerChip[i] = 0x3c;
        }
        
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_READ);
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_EXT, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_EXT, EE_HDERR_READ);
    }
    for(i = 0;i < 20;i++)
    {
        btmp = gby_para_CellNumPerChip[i] & 0x0f;
        SYSCONF_DEAL_PARA_FROM_EE(btmp,12);
        gby_para_CellNumPerChip[i] &= 0xf0;
        gby_para_CellNumPerChip[i] |= btmp;
        btmp = (gby_para_CellNumPerChip[i] & 0xf0) >> 4;
        SYSCONF_DEAL_PARA_FROM_EE(btmp,3);
        gby_para_CellNumPerChip[i] &= 0x0f;
        gby_para_CellNumPerChip[i] |= (btmp << 4);
    }

   /* rslt = EE_LoadVar(SYSCONF_CHIP_NUMBER);
    //rslt = 1; //test
    if(rslt)
    {
        gby_para_ChipNumber = 1;
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_READ);
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_EXT, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_EXT, EE_HDERR_READ);
    }
    SYSCONF_DEAL_PARA_FROM_EE(gby_para_ChipNumber,MAX_CHIP_NUM); */
    
    gby_para_ChipNumber = gby_para_BmuNumber;
    
    rslt = EE_LoadVar(SYSCONF_CHIP_FUSE_POS);     //SYSCONF_BMU_START_TEMPERNO_G1
    if(rslt)
    {
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_READ);
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_EXT, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_EXT, EE_HDERR_READ);
    }
    for(i = 0; i < 30; i++)
    {
        btmp = gby_para_chip_fuse_pos[i] & 0x0f;
        SYSCONF_DEAL_PARA_FROM_EE(btmp,12);
        gby_para_chip_fuse_pos[i] &= 0xf0;
        gby_para_chip_fuse_pos[i] |= btmp;
        btmp = (gby_para_chip_fuse_pos[i] & 0xf0) >> 4;
        SYSCONF_DEAL_PARA_FROM_EE(btmp,10);
        gby_para_chip_fuse_pos[i] &= 0x0f;
        gby_para_chip_fuse_pos[i] |= (btmp << 4);
    }
    
   /* 
    rslt = EE_LoadVar(SYSCONF_TEMPERNUM_PER_CHIP);    
    if(rslt)
    {
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_READ);
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_EXT, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_EXT, EE_HDERR_READ);
    }
    for(i = 0; i < 15; i++)
    {
      SYSCONF_DEAL_PARA_FROM_EE(gwd_para_TempNumPerChip[i],30);   //  MAX_TEMPER_NUM
    } */
       
   /* rslt = EE_LoadVar(SYSCONF_BMU0_CELLNUM);
    if(rslt)
    {
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_READ);
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_EXT, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_EXT, EE_HDERR_READ);
    }
    SYSCONF_DEAL_PARA_FROM_EE(gwd_para_Bmu0CellNum , MAX_CELL_NUM); */
    
  /*  
    rslt = EE_LoadVar(SYSCONF_BMU0_START_CELLNO);
    if(rslt)
    {
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_READ);
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_EXT, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_EXT, EE_HDERR_READ);
    }
    SYSCONF_DEAL_PARA_FROM_EE(gwd_para_Bmu0StartCellNO,MAX_CELL_NUM);
  */  
    
  /*  rslt = EE_LoadVar(SYSCONF_BMU0_TEMPERNUM);
    if(rslt)
    {
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_READ);
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_EXT, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_EXT, EE_HDERR_READ);
    }
    SYSCONF_DEAL_PARA_FROM_EE(gwd_para_Bmu0TemperNum,MAX_TEMPER_NUM);
  */
    
  /*
    rslt = EE_LoadVar(SYSCONF_BMU0_START_TEMPERNO);
    if(rslt)
    {
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_READ);
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_EXT, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_EXT, EE_HDERR_READ);
    }
    SYSCONF_DEAL_PARA_FROM_EE(gwd_para_Bmu0StartTemperNO,MAX_TEMPER_NUM);
  */

  /*  rslt = EE_LoadVar(SYSCONF_BMU_CELLNUM_G1);
    //rslt = 1; //test
    if(rslt)
    {
        gwd_para_BmuCellNumG1[0]= 12;
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_READ);
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_EXT, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_EXT, EE_HDERR_READ);
    }
    for(i = 0; i < 15; i++)
    {
      SYSCONF_DEAL_PARA_FROM_EE(gwd_para_BmuCellNumG1[i],MAX_PACK_CELL_NUM);
    }
  */  
 
  /*
    rslt = EE_LoadVar(SYSCONF_BMU_CELLNUM_G2);
    if(rslt)
    {
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_READ);
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_EXT, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_EXT, EE_HDERR_READ);
    }
    for(i = 0; i < 15; i++)
    {
      SYSCONF_DEAL_PARA_FROM_EE(gwd_para_BmuCellNumG2[i],MAX_PACK_CELL_NUM);
    }
  */
  /*  
    rslt = EE_LoadVar(SYSCONF_BMU_START_CELLNO_G1);
    if(rslt)
    {
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_READ);
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_EXT, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_EXT, EE_HDERR_READ);
    }
    for(i = 0; i < 15; i++)
    {
      SYSCONF_DEAL_PARA_FROM_EE(gwd_para_BmuStartCellNO_G1[i],MAX_CELL_NUM);
    }
   */ 
   /* rslt = EE_LoadVar(SYSCONF_BMU_START_CELLNO_G2);
    if(rslt)
    {
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_READ);
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_EXT, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_EXT, EE_HDERR_READ);
    }
    for(i = 0; i < 15; i++)
    {
      SYSCONF_DEAL_PARA_FROM_EE(gwd_para_BmuStartCellNO_G2[i],MAX_CELL_NUM);
    }
   */ 
  /*  rslt = EE_LoadVar(SYSCONF_BMU_TEMPERNUM_G1);
    //rslt = 1; //test
    if(rslt)
    {
        gwd_para_BmuTemperNumG1[0] = 8;
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_READ);
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_EXT, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_EXT, EE_HDERR_READ);
    }    
    for(i = 0; i < 15; i++)
    {                                   
      SYSCONF_DEAL_PARA_FROM_EE(gwd_para_BmuTemperNumG1[i],MAX_TEMPER_NUM);
    }
  */  
  /*  rslt = EE_LoadVar(SYSCONF_BMU_TEMPERNUM_G2);
    if(rslt)
    {
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_READ);
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_EXT, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_EXT, EE_HDERR_READ);
    }
    for(i = 0; i < 15; i++)
    {
      SYSCONF_DEAL_PARA_FROM_EE(gwd_para_BmuTemperNumG2[i],MAX_TEMPER_NUM);
    }
    
    rslt = EE_LoadVar(SYSCONF_BMU_START_TEMPERNO_G1);
    if(rslt)
    {
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_READ);
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_EXT, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_EXT, EE_HDERR_READ);
    }
    for(i = 0; i < 15; i++)
    {
      SYSCONF_DEAL_PARA_FROM_EE(gwd_para_BmuStartTemperNO_G1[i],MAX_TEMPER_NUM);
    }
    
    rslt = EE_LoadVar(SYSCONF_BMU_START_TEMPERNO_G2);
    
    if(rslt)
    {
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_READ);
        rslt = Err_UpdateHwErrLevel(ERR_HW_EE_EXT, ERR_LEVEL_TWO);
        rslt = Err_UpdateHwErrDetail(ERR_HW_EE_EXT, EE_HDERR_READ);
    }
    for(i = 0; i < 15; i++)
    {
      SYSCONF_DEAL_PARA_FROM_EE(gwd_para_BmuStartTemperNO_G2[i],MAX_TEMPER_NUM);
    }
 */   
    //rslt = EE_LoadVar(SYSCONF_RSV_FOR_12_20);
    //

    // prevent C5917: Removed dead assignment 
    (void)rslt;


#ifdef SYSCONF_ENABLE_WRITE_CFG_TO_FALSH
   Prj_WrDefaultCfg();
   Prj_WrDefaultErrorThrhd();
#endif


}

GeneralReturnType SysConf_InitParaTable(eSysConf_ParameterNOType ParaNO, uLWord ParaAddrInRam, uByte ParameterLength)
{
    // check Parameter Table
    if(ParameterIndexTable[ParaNO].ParameterNO != ParaNO)
    {
        return SYSCONF_ERR_PARA_NUM_ERR;
    }
    if (ParameterLength > SYS_MAX_PARA_LENGTH)
    {
        return  SYSCONF_ERR_PARA_OVER_MAX_LENGTH;
    }

    if( (ParaAddrInRam < 0xFFFF) || (ParaAddrInRam > 0xFFFFFF) )  // not banked ram
    {
        return SYSCONF_ERR_PARA_ADDR_ERR;
    } 

    ParameterIndexTable[ParaNO].ParameterValuePtr = (void * RAMPTR)ParaAddrInRam;
    ParameterIndexTable[ParaNO].ParameterLength = ParameterLength;
    
    return ERR_OK;
}

GeneralReturnType SysConf_GetParaHandle(eSysConf_ParameterNOType ParaNO, uLWord * ParaAddrInRam, uByte * length)
{
    uWord datestore_record1_StrartNO;
    uWord datestore_record1_EndNO;
    uWord temp_ParaNO;
    
    temp_ParaNO = ParaNO;
 
    // check Parameter Table
    if((NULL == ParaAddrInRam) || (NULL == length))
    {
      return SYSCONF_ERR_INPUT_POINTER_NULL;
    }
    if(ParameterIndexTable[temp_ParaNO].ParameterNO != temp_ParaNO)
    {
        return SYSCONF_ERR_PARA_NUM_ERR;
    }
    * ParaAddrInRam = (uLWord)(ParameterIndexTable[temp_ParaNO].ParameterValuePtr);
    * length = ParameterIndexTable[temp_ParaNO].ParameterLength;
    if((0 == *ParaAddrInRam) || (0 == *length))
    {
       return SYSCONF_ERR_PARA_RAM_ADDR_ERR;
    }
    return  ERR_OK;
}

GeneralReturnType SysConf_GetParaAddrInEE(eSysConf_ParameterNOType ParaNO,  sSysConf_ParameterAddrInRamType * ParameterAddrInRamPtr)
{
    if (ParameterAddrInRamPtr == NULL)
    {
       return SYSCONF_ERR_INPUT_POINTER_NULL;
    }
    
    // check Parameter Address Table
    if(ParameterAddrTable[ParaNO].ParameterNO != ParaNO)
           
    {
        return SYSCONF_ERR_PARA_NUM_ERR;
    }    

    ParameterAddrInRamPtr->MaxParameterLength = ParameterAddrTable[ParaNO].MaxParameterLength;
    ParameterAddrInRamPtr->CpuEEParaBakupNum =  ParameterAddrTable[ParaNO].CpuEEParaBakupNum;
    ParameterAddrInRamPtr->ExtEEParaBakupNum =  ParameterAddrTable[ParaNO].ExtEEParaBakupNum;
    ParameterAddrInRamPtr->TargetRelativeAddr.CpuEeAddr = ParameterAddrTable[ParaNO].TargetRelativeAddr.CpuEeAddr;
    ParameterAddrInRamPtr->TargetRelativeAddr.ExtEeAddr = ParameterAddrTable[ParaNO].TargetRelativeAddr.ExtEeAddr;

    return ERR_OK;
}

GeneralReturnType SysConf_UpdataParaToRam(eSysConf_ParameterNOType ParaNo, uByte * ValuePtr)
{
    uLWord ParaAddrInRam;
    uByte length,i;

    if(!SysConf_GetParaHandle(ParaNo,&ParaAddrInRam,&length))
    {
        for(i = 0; i < length; i++)
        {
            *((uByte * RAMPTR)ParaAddrInRam) = *(ValuePtr + i);
            ParaAddrInRam++;
        }

        return ERR_OK;
    }

    return SYSCONF_ERR_GET_HANDLE_FAILED;
}

GeneralReturnType SysConf_GetParaValueFromRam(eSysConf_ParameterNOType ParaNo, uByte * ValuePtr, uByte * LengthPtr)
{
    uLWord ParaAddrInRam;
    uByte i;

    if(!SysConf_GetParaHandle(ParaNo,&ParaAddrInRam,LengthPtr))
    {
        for(i = 0; i < (* LengthPtr); i++)
        {
            *(ValuePtr + i) = *((uByte *RAMPTR)ParaAddrInRam);
            ParaAddrInRam++;
        }
        return ERR_OK;
    }

    return SYSCONF_ERR_GET_HANDLE_FAILED;
}
#pragma DATA_SEG DEFAULT 


