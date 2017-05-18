// SysConf.h
#ifndef SYSCONF_H
#define SYSCONF_H

#include "GeneralTypeDef.h"
#include "ECU.h"


typedef enum
{
    SYSCONF_CPU_EE,  // store to ee in cpu
    SYSCONF_EXT_EE,   // store to ee in extern chip, such as 24c64
    SYSCONF_BOTH_EE  // store to ee both cpu and extern ee
} eSysConf_StoreTargetType;

typedef enum
{
    SYSCONF_BAKUP_ZERO,
    SYSCONF_BAKUP_ONE = 1,
    SYSCONF_BAKUP_TWO,
    SYSCONF_BAKUP_THREE
} eSysConf_BakupNumType;

typedef struct
{
    uWord CpuEeAddr;
    uWord ExtEeAddr;
} sSysConf_TargetAddressType;


typedef enum
{
    /* Project Parameter Index */

    /* 0 */ SYSCONF_PRODUCTNO,    /* product NO */
    /* 1 */ SYSCONF_CFG_VER,      /* config version,*/
    /* 2 */ SYSCONF_CONFIG_ST,    /* config status, */
    /* 3 */ SYSCONF_CONFIG_UPDATA_INFO, /* Config update information */
    /* 4 */ SYSCONF_PROGRAM_UPDATE_INFO, /* Program update information */
    /* 5 */ SYSCONF_RSV_FOR_0_4, /* Reserve for para0 to para4 */

    /* 6 */ SYSCONF_BMU_NUMBER,  /* bmu number */
    /* 7 */ SYSCONF_CHIP_NUMBER, //SYSCONF_RSVBAK_0,                     //SYSCONF_BMU0_CELLNUM, /* BCU_C(Bmu0) cellnum */
    /* 8 */ SYSCONF_RSVBAK_1,                     //SYSCONF_BMU0_START_CELLNO,            /* BCU_C(Bmu0) Start Cell NO*/
    /* 9 */ SYSCONF_RSVBAK_2,          //SYSCONF_BMU0_TEMPERNUM,               /* Temperature Number of BCU_C(Bmu0)*/
    /* 10 */ SYSCONF_RSVBAK_3,        //SYSCONF_BMU0_START_TEMPERNO, /* BCU_C(Bmu0) Start Temperature NO.*/
    /* 11 */ SYSCONF_RSVBAK_4,        //SYSCONF_BMU0_FUSE_POS, /* Reserve for para6 to para10 */

    /* 12 */ SYSCONF_RSVBAK_5,   //SYSCONF_BMU_CELLNUM_G1, /* Bmu cell number, from bmu1 to bmu15*/
    /* 13 */ SYSCONF_RSVBAK_6,  //SYSCONF_BMU_CELLNUM_G2, /* Bmu cell number, from bmu16 to bmu30*/
    /* 14 */ SYSCONF_RSVBAK_7,  //SYSCONF_BMU_START_CELLNO_G1,/*  Bmu start cell NO., from bmu1 to bmu15*/
    /* 15 */ SYSCONF_RSVBAK_8, //SYSCONF_BMU_START_CELLNO_G2,/*  Bmu start cell NO., from bmu16 to bmu30*/
    /* 16 */ SYSCONF_RSVBAK_9, //SYSCONF_BMU_TEMPERNUM_G1, /* Temperature Number of Bmu, from from bmu1 to bmu15 */
    /* 17 */ SYSCONF_RSVBAK_10,  //SYSCONF_BMU_TEMPERNUM_G2, /* Temperature Number of Bmu, from from bmu16 to bmu30 */
    
    
    /* 18 */ SYSCONF_CHIP_FUSE_POS,//SYSCONF_BMU_START_TEMPERNO_G1, /*  Bmu start Temperature NO., from bmu1 to bmu15*/
    /* 19 */ SYSCONF_RSVBAK_11,//SYSCONF_TEMPERNUM_PER_CHIP,//SYSCONF_BMU_START_TEMPERNO_G2,/*  Bmu start Temperature NO., from bmu16 to bmu30*/
    /* 20 */ SYSCONF_CELLNUM_PER_CHIP,/* Cell number per Chip in BCU_C(Bmu0) */
    /* 21 */ SYSCONF_RSV_FOR_12_20, /* Reserve for para6 to para19*/

    /* 22 */ERRORALARM_THRHD_LVV1,    /* Error thrhd level1*/
    /* 23 */ERRORALARM_RSV_FOR_21_21, /* Reserve for para21 to para21*/
    /* 24 */ERRORALARM_THRHD_LVV2,    /* Error thrhd level2*/
    /* 25 */ERRORALARM_RSV_FOR_23_23, /* Reserve for para23 to para23*/
    /* 26 */ERRORALARM_THRHD_LVV3,    /* Error thrhd level3*/
    /* 27 */ERRORALARM_RSV_FOR_22_26, /* Reserve for para22 to para26*/

    /* 28 */THM_HEAT_MANAGE_PARA,      /* Heat manage parameter*/
    /* 29 */THM_RSV_FOR_28_28,         /* Reserve for para28 to para28*/

    /* 30 */SYSTEMST_CHG_CTRL_PARA,    /* Charger control parameter*/
    /* 31 */SYSTEMST_RSV_FOR_30_30,    /* Reserve for para30 to para30*/

    /* 32 */CURDET_CURRENT_CALIB_PARA,    /* Current calibration parameter*/
    /* 33 */SUMVDET_SUMVIN_CALIB_PARA,    /* Sumv_in calibration parameter*/
    /* 34 */SUMVDET_SUMVOUT_CALIB_PARA, /* Sumv_in backup calibration parameter*/
    /* 35 */SUMVDET_SUMVINBAK_CALIB_PARA,   /* Sumv_out calibration parameter*/
    /* 36 */SUMVDET_SUMVOUTBAK_CALIB_PARA,        /* Insulation calibration parameter*/
    /* 37 */ISODET_ISO_CALIB_PARA,                /* Reserve for para32 to para36*/


    /* 38 */SYSTEMST_BAT_PACK_INFO_G1,     /* Battery pack information1 */
    /* 39 */SYSTEMST_BAT_PACK_INFO_G2,     /* Battery pack information2 */
    /* 40 */SYSTEMST_RSV_FOR_38_39,        /* Reserve for para38to para39*/

    /* 41 */SYSTEMST_BAT_PACK_INFO_G3,     /* Battery pack information3 */
    /* 42 */SYSTEMST_BAT_PACK_INFO_G4,     /* Battery pack information4 */
    /* 43 */SYSTEMST_RSV_FOR_41_42,        /* Reserve for para41 to para42*/

    /* 44 */SYSTEMST_HSTY_CHG_DCH_INFO,        /* History charge and discharge information*/
    /* 45 */SYSTEMST_HSTY_INRES_INFO,          /* History Internal resistance information*/
    /* 46 */SYSTEMST_HSTY_CELLV_INFO,          /* History voltage information*/
    /* 47 */SYSTEMST_HSTY_TEMPER_INFO,         /* History temper information*/
    /* 48 */SYSTEMST_HSTY_POWER_INFO,          /* History power information*/
    /* 49 */SYSTEMST_RSV_FOR_44_48,        /* Reserve for para44 to para48*/

    /* 50 */SYSTEMST_HSTY_TEMPER_AREA_INFO_G1, /* History temperature area information1*/
    /* 51 */SYSTEMST_HSTY_TEMPER_AREA_INFO_G2, /* History temperature area information2*/
    /* 52 */SYSTEMST_HSTY_TEMPER_AREA_INFO_G3, /* History temperature area information3*/
    /* 53 */SYSTEMST_HSTY_TEMPER_AREA_INFO_G4, /* History temperature area information4*/
    /* 54 */SYSTEMST_HSTY_TEMPER_AREA_INFO_G5, /* History temperature area information5*/
    /* 55 */SYSTEMST_HSTY_TEMPER_AREA_INFO_G6, /* History temperature area information6*/
    /* 56 */SYSTEMST_HSTY_TEMPER_AREA_INFO_G7, /* History temperature area information7*/
    /* 57 */SYSTEMST_HSTY_TEMPER_AREA_INFO_G8, /* History temperature area information8*/
    /* 58 */SYSTEMST_HSTY_CURRENT_AREA_INFO_G1,   /* History current information1*/
    /* 59 */SYSTEMST_HSTY_CURRENT_AREA_INFO_G2,   /* History current information2*/
    /* 60 */SYSTEMST_HSTY_SOC_AREA_INFO_G1,       /* History soc area information1*/
    /* 61 */SYSTEMST_HSTY_SOC_AREA_INFO_G2,       /* History soc area information2*/
    /* 62 */SYSTEMST_RSV_FOR_50_61,            /* Reserve for para50 to para61*/

    /* 63 */EE_RECOGNIZE,
    /* 64 */SYSTEMST_REMAIN_CAPACITY_BAK_G1,
    /* 65 */SYSTEMST_REMAIN_CAPACITY_BAK_G2,
    /* 66 */SYSTEMST_REMAIN_CAPACITY_BAK_G3,
    /* 67 */SYSTEMST_REMAIN_CAPACITY_BAK_G4,
    /* 68 */SYSTEMST_REMAIN_CAPACITY_BAK_G5,
    /* 69 */SYSTEMST_REMAIN_CAPACITY_BAK_G6,
    /* 70 */SYSTEMST_REMAIN_AH_CNT_BAK_G1,
    /* 71 */SYSTEMST_REMAIN_AH_CNT_BAK_G2,
    /* 72 */SYSTEMST_REMAIN_AH_CNT_BAK_G3,
    /* 73 */SYSTEMST_REMAIN_AH_CNT_BAK_G4,
    /* 74 */SYSTEMST_REMAIN_AH_CNT_BAK_G5,
    /* 75 */SYSTEMST_REMAIN_AH_CNT_BAK_G6,
    /* 76 */SYSTEMST_RATED_ACTUAL_CAPACITY,
    /* 77 */SYSTEMST_SOH,
    /* 78 */SYSTEMST_TOTAL_CHG_DCH_AH,       /* Total charge AH,Total discharge AH*/
    /* 79 */SYSTEMST_SOH_CTRL_PARA,          /* Total control para*/
    /* 80 */SYSTEMST_SUM_INRES,               /* Total Internal resistance */
    /* 81 */SYSTEMST_POWEROFF_RESET_INFO,
    /* 82 */SYSTEMST_BMS_TOTAL_RUNTIME,
    /* 83 */SYSTEMST_CONTACTOR_LIFE,
    /* 84 */SYSTEMST_REMAIN_MILEAGE,
    /* 85 */SYSTEMST_DATA_RECORD_PAGE,    /* History record page,error record page */
    /* 86 */BALM_CALC_BAL_MODE,       /* calc bal mode*/
    /* 87 */SYSTEMST_RSV_FOR_87,       /* Reserve for para87*/
    /* 88 */SYSTEMST_RSV_FOR_88,       /* Reserve for para88*/
    
    /* 88 */SYSTEMST_VEHICLE_VIN,
    /* 89 */SYSTEMST_VEHICLE_PLATE,
    /* 90 */SYSTEMST_RSV_FOR_88_89,       /* Reserve for para88 to para89 */

    /* 91 */SYSTEMST_BAL_TIME_1_15,      /* Cellv remaining  balancetime 1-15*/
    /* 92 */SYSTEMST_BAL_TIME_16_30,
    /* 93 */SYSTEMST_BAL_TIME_31_45,
    /* 94 */SYSTEMST_BAL_TIME_46_60,
    /* 95 */SYSTEMST_BAL_TIME_61_75,
    /* 96 */SYSTEMST_BAL_TIME_76_90,
    /* 97 */SYSTEMST_BAL_TIME_91_105,
    /* 98 */SYSTEMST_BAL_TIME_106_120,
    /* 99 */SYSTEMST_BAL_TIME_121_135,
    /* 100 */SYSTEMST_BAL_TIME_136_150,
    /* 101*/SYSTEMST_BAL_TIME_151_165,
    /* 102 */SYSTEMST_BAL_TIME_166_180,
    /* 103 */SYSTEMST_BAL_TIME_181_195,
    /* 104 */SYSTEMST_BAL_TIME_196_210,
    /* 105 */SYSTEMST_BAL_TIME_211_225,
    /* 106 */SYSTEMST_BAL_TIME_226_240,
    /* 107 */SYSTEMST_BAL_TIME_241_255,
    /* 108 */SYSTEMST_BAL_TIME_256_270,
    /* 109 */SYSTEMST_BAL_TIME_271_285,
    /* 110 */SYSTEMST_BAL_TIME_286_300,

    /* 111 */SYSTEMST_TOTAL_CHG_BAL_TIME_1_20, /* Cellv total charge balancetime 1-15*/
    /* 112 */SYSTEMST_TOTAL_CHG_BAL_TIME_21_40,
    /* 113 */SYSTEMST_TOTAL_CHG_BAL_TIME_41_60,
    /* 114 */SYSTEMST_TOTAL_CHG_BAL_TIME_61_80,
    /* 115 */SYSTEMST_TOTAL_CHG_BAL_TIME_81_100,
    /* 116 */SYSTEMST_TOTAL_CHG_BAL_TIME_101_120,
    /* 117 */SYSTEMST_TOTAL_CHG_BAL_TIME_121_140,
    /* 118 */SYSTEMST_TOTAL_CHG_BAL_TIME_141_160,
    /* 119 */SYSTEMST_TOTAL_CHG_BAL_TIME_161_180,
    /* 120 */SYSTEMST_TOTAL_CHG_BAL_TIME_181_200,
    /* 121 */SYSTEMST_TOTAL_CHG_BAL_TIME_201_220,
    /* 122 */SYSTEMST_TOTAL_CHG_BAL_TIME_221_240,
    /* 123 */SYSTEMST_TOTAL_CHG_BAL_TIME_241_260,
    /* 124 */SYSTEMST_TOTAL_CHG_BAL_TIME_261_280,
    /* 125 */SYSTEMST_TOTAL_CHG_BAL_TIME_281_300,

    /* 126 */SYSTEMST_TOTAL_DCH_BAL_TIME_1_20, /* Cellv total discharge balancetime 1-15*/
    /* 127 */SYSTEMST_TOTAL_DCH_BAL_TIME_21_40,
    /* 128 */SYSTEMST_TOTAL_DCH_BAL_TIME_41_60,
    /* 129 */SYSTEMST_TOTAL_DCH_BAL_TIME_61_80,
    /* 130 */SYSTEMST_TOTAL_DCH_BAL_TIME_81_100,
    /* 131 */SYSTEMST_TOTAL_DCH_BAL_TIME_101_120,
    /* 132 */SYSTEMST_TOTAL_DCH_BAL_TIME_121_140,
    /* 133 */SYSTEMST_TOTAL_DCH_BAL_TIME_141_160,
    /* 134 */SYSTEMST_TOTAL_DCH_BAL_TIME_161_180,
    /* 135 */SYSTEMST_TOTAL_DCH_BAL_TIME_181_200,
    /* 136 */SYSTEMST_TOTAL_DCH_BAL_TIME_201_220,
    /* 137 */SYSTEMST_TOTAL_DCH_BAL_TIME_221_240,
    /* 138 */SYSTEMST_TOTAL_DCH_BAL_TIME_241_260,
    /* 139 */SYSTEMST_TOTAL_DCH_BAL_TIME_261_280,
    /* 140 */SYSTEMST_TOTAL_DCH_BAL_TIME_281_300,

    /* 141 */TEMPERDET_BMU0_18B20ROM_G1,
    /* 142 */TEMPERDET_BMU0_18B20ROM_G2,
    /* 143 */TEMPERDET_BMU0_18B20ROM_G3,
    /* 144 */TEMPERDET_BMU0_18B20ROM_G4,
    /* 145 */QR_CODE_INFOR,   //SYSTEMST_RSV_FOR1,
    /* 146 */SUMVDET_SUMVINBAK1_CALIB_PARA,
    /* 147 */SUMVDET_SUMVOUTBAK1_CALIB_PARA,
    /* 148 */FL_BOOT_MODE,

} eSysConf_ParameterNOType;

// For SysConf 0x4600~0x461F
#define  SYSCONF_ERR_PARA_NUM_ERR        0x4600
#define  SYSCONF_ERR_PARA_RAM_ADDR_ERR   0x4601
#define  SYSCONF_ERR_INPUT_POINTER_NULL  0x4602
#define  SYSCONF_ERR_GET_HANDLE_FAILED   0x4603
#define  SYSCONF_ERR_PARA_OVER_MAX_LENGTH   0x4604
#define  SYSCONF_ERR_PARA_ADDR_ERR       0x4605


typedef struct
{
    uWord User; /* User of Monitor */
    uByte Date[6]; /* Date of operation, Year-Month-Day-Hour-Minute-Second */
} sSysConf_MonitorOperationInfo;

typedef struct
{
    uWord para_BmuBatNum; /* BmuXX bat num */
    uWord Para_BmuStartCellNO; /* Bmuxx Start Cell NO.*/
} sSysConf_BmuVoltCfgInfo;

typedef struct
{
    uWord para_BmuTemperNum; /* BmuXX temper num */
    uWord Para_BmuStartTemperNO; /* Bmuxx Start temper NO.*/
} sSysConf_BmuTemperCfgInfo;


typedef struct
{
    eSysConf_ParameterNOType ParameterNO;
    void * RAMPTR ParameterValuePtr;
    uByte ParameterLength;
} sSysConf_ParameterType;


typedef struct
{
    const eSysConf_ParameterNOType ParameterNO;
    const uByte MaxParameterLength;
    //const eSysConf_StoreTargetType StoreTarget;   // Cpu EE or External EE (Such as 24C64)
    const eSysConf_BakupNumType CpuEEParaBakupNum;
    const eSysConf_BakupNumType ExtEEParaBakupNum;
    const sSysConf_TargetAddressType TargetRelativeAddr;  // Address in target (Cpu EE or External EE)
} sSysConf_ParameterAddressType;

typedef struct
{
    uByte MaxParameterLength;
    eSysConf_BakupNumType CpuEEParaBakupNum;
    eSysConf_BakupNumType ExtEEParaBakupNum;
    sSysConf_TargetAddressType TargetRelativeAddr;
} sSysConf_ParameterAddrInRamType;

/* void SysConf_Init(void);
 *
 * Initial Internal Parameter in sysconf module
 */
void SysConf_Init(void);

/* GeneralReturnType SysConf_InitParaTable(eSysConf_ParameterNOType PareNO, void * ParameterValuePtr, uByte ParemeterLength);
 *
 * Initial Extenal Parameter pointer and length, which be placed other module, such as SOC, SOH, History Infromation
 * Parameter placed in SycConf will initial at define, which cannot be initial by this function
 * Input: PareNO --- ParameterNO, deine in eSysConf_ParameterNOType
 *        PareAddrInRam --- The ram address that point to the external parameter, Use in this function such as
          Parameter.ParameterValuePtr = (void *)PareAddrInRam;
 *        ParemeterLength --- length of external parameter
 */
GeneralReturnType SysConf_InitParaTable(eSysConf_ParameterNOType ParaNO, uLWord ParaAddrInRam, uByte ParameterLength);

/* GeneralReturnType SysConf_GetParaHandle(eSysConf_ParameterNOType PareNO, uWord * PareAddrInRam, uByte * length);
 *
 * Get Parameter handle for read paramete from RAM or write value to parameter in RAM
 * Input: PareNO --- ParameterNO, deine in eSysConf_ParameterNOType
 * Output: PareAddrInRam --- parameter address in RAM, Use such as
                value = *((uByte *)PareAddrInRam);
 * length --- parameter length
 * Return: Interface Run Result
 */
GeneralReturnType SysConf_GetParaHandle(eSysConf_ParameterNOType ParaNO, uLWord * ParaAddrInRam, uByte * length);

/* GeneralReturnType Get_PareAddr(eSysConf_ParameterNOType PareNO, sSysConf_ParameterAddressType * ParameterAddrPtr);
 *
 * Get Parameter Address in EE (Include CPU and extern EE) for read paramete from EE or write value to parameter in EE
 * Input: PareNO --- ParameterNO, deine in eSysConf_ParameterNOType
 * Output: ParameterAddrInRamPtr --- Parameter Address include CPUEE and ExtEE, define in sSysConf_ParameterAddressType
 * Return: Interface Run Result
 */
GeneralReturnType SysConf_GetParaAddrInEE(eSysConf_ParameterNOType ParaNO,  sSysConf_ParameterAddrInRamType  * ParameterAddrInRamPtr);

/* GeneralReturnType SysConf_UpdataParaToRam(eSysConf_ParameterNOType ParaNo, uByte * ValuePtr);
 *
 * Updata Parameter to Ram , which value may come from CAN or EE
 * Input: PareNO --- ParameterNO, deine in eSysConf_ParameterNOType
 *        ValuePtr --- Parameter value pointer, for input parameter value
 * Return: Interface Run Result
 */
GeneralReturnType SysConf_UpdataParaToRam(eSysConf_ParameterNOType ParaNo, uByte * ValuePtr);

/* GeneralReturnType SysConf_GetParaValueFromRam(eSysConf_ParameterNOType ParaNo, uByte * ValuePtr, uByte * LengthPtr);
 *
 * Get Parameter value From ram in parameter table
 * Input: PareNO --- ParameterNO, deine in eSysConf_ParameterNOType
 *        ValuePtr --- parameter value pointer, for output parameter value
 *        LengthPtr --- Parameter length pointer, for output parameter length
 * Return: Interface Run Result
 */
GeneralReturnType SysConf_GetParaValueFromRam(eSysConf_ParameterNOType ParaNo, uByte * ValuePtr, uByte * LengthPtr);


/*
 * Define Callback function
 * These functions must define in project files
 */
extern void Prj_WrDefaultCfg(void);
extern void Prj_WrDefaultErrorThrhd(void);
#endif
