// ErrorAlarm.h
#ifndef ERRORALARM_H
#define ERRORALARM_H


#include "GeneralTypeDef.h"
#include "Ecu.h"


// ErrorCode define
typedef enum
{
    ERR_CODE_INDEX_OVER = ERR_DATA_ERRORALARM_BASE,//17184
    ERR_CODE_LEVEL_OVER,                           //17185
    ERR_CODE_PAGE_OVER,                            //17186
    ERR_CODE_BMU_NO_OVER,                          //17187
    ERR_CODE_BMU_NO_ZERO,                          //17188
    ERR_CODE_TYPE_OVER,                            //17189
    ERR_CODE_THRHD_INVALID                         //17190
    //must less than 32
} eErr_CodeType;

typedef enum
{
    ERR_BAT,
    ERR_BATSYS,
    ERR_HW,
    ERR_ALL
} eErr_TypeType;

typedef enum
{
    ERR_LEVEL_NORMAL,
    ERR_LEVEL_ONE,
    ERR_LEVEL_TWO,
    ERR_LEVEL_THREE,
    ERR_LEVEL_INVALD = 0xFFU
} eErr_LevelType;

typedef struct
{
    uByte By[8];
} sErr_ByteNType;

typedef uWord Err_HwErrDetailType;

#define ERR_HW_DETAIL_INVALID 0xFFFFU




typedef enum
{
    ERR_BAT_SUMV_HIGH,
    ERR_BAT_SUMV_LOW,
    ERR_BAT_CELLV_HIGH,
    ERR_BAT_CELLV_LOW,
    ERR_BAT_DELTV_SINGLPACK,
    ERR_BAT_DELTV_ALLPACK,
    ERR_BAT_DELTT_SINGLPACK,
    ERR_BAT_DELTT_ALLPACK,
    ERR_BAT_T_HIGH,
    ERR_BAT_T_LOW,
    ERR_BAT_CUR_HIGH_CHG,
    ERR_BAT_CUR_HIGH_DCH,
    ERR_BAT_CUR_HIGH_FDBCK,
    ERR_BAT_RSV_FOR_3_2,
    ERR_BAT_SOC_HIGH,
    ERR_BAT_SOC_LOW,
    ERR_BAT_SOC_HIGH_CELL,
    ERR_BAT_SOC_LOW_CELL,
    ERR_BAT_SOC_DIFF_CELL,
    ERR_BAT_AVRGV_DIFF_ALLPACK,
    ERR_BAT_GROUP_NO_DIFF,
    ERR_BAT_INTRES_HIGH_CELL,
    ERR_BAT_INTRES_DIFF_CELL,
    ERR_BAT_INTRES_HIGH,
    ERR_BAT_SOC_DIFF_ALLPACK,
    ERR_BAT_MANU_DIFF,
    ERR_BAT_TYPE_DIFF,
    ERR_BAT_SHORT_CIRCUIT,
    ERR_BAT_SOH_LOW,
    ERR_BAT_SOH_LOW_CELL,
    ERR_BAT_SOH_DIFF_CELL,
    ERR_BAT_POLE_T_HIGH,
    ERR_BAT_RSV_FOR_7_6
} eErr_BatErrIndexType;

#define ERR_MASK_BAT_SUMV_HIGH          0x03U
#define ERR_MASK_BAT_SUMV_LOW           0x0CU
#define ERR_MASK_BAT_CELLV_HIGH         0x30U
#define ERR_MASK_BAT_CELLV_LOW          0xC0U
#define ERR_MASK_BAT_DELTV_SINGLPACK    0x03U
#define ERR_MASK_BAT_DELTV_ALLPACK      0x0CU
#define ERR_MASK_BAT_DELTT_SINGLPACK    0x30U
#define ERR_MASK_BAT_DELTT_ALLPACK      0xC0U
#define ERR_MASK_BAT_T_HIGH             0x03U
#define ERR_MASK_BAT_T_LOW              0x0CU
#define ERR_MASK_BAT_CUR_HIGH_CHG       0x30U
#define ERR_MASK_BAT_CUR_HIGH_DCH       0xC0U
#define ERR_MASK_BAT_CUR_HIGH_FDBCK     0x03U
#define ERR_MASK_BAT_RSV_FOR_3_2		    0x0CU
#define ERR_MASK_BAT_SOC_HIGH			      0x30U
#define ERR_MASK_BAT_SOC_LOW			      0x30U
#define ERR_MASK_BAT_SOC_HIGH_CELL		  0x03U
#define ERR_MASK_BAT_SOC_LOW_CELL		    0x0CU
#define ERR_MASK_BAT_SOC_DIFF_CELL	  	0x30U
#define ERR_MASK_BAT_AVRGV_DIFF_ALLPACK 0xC0U
#define ERR_MASK_BAT_GROUP_NO_DIFF		  0x03U
#define ERR_MASK_BAT_INTRES_HIGH_CELL   0x0CU
#define ERR_MASK_BAT_INTRES_DIFF_CELL   0x30U
#define ERR_MASK_BAT_INTRES_HIGH		    0xC0U
#define ERR_MASK_BAT_SOC_DIFF_ALLPACK   0x03U
#define ERR_MASK_BAT_MANU_DIFF			    0x0CU
#define ERR_MASK_BAT_TYPE_DIFF			    0x30U
#define ERR_MASK_BAT_SHORT_CIRCUIT	  	0xC0U
#define ERR_MASK_BAT_SOH_LOW			      0x03U
#define ERR_MASK_BAT_SOH_LOW_CELL	    	0x0CU
#define ERR_MASK_BAT_SOH_DIFF_CELL		  0x30U
#define ERR_MASK_BAT_RSV_FOR_7_6		    0xC0U


typedef enum
{
    // page 0
    ERR_BATSYS_POS_ADJOIN,
    ERR_BATSYS_POS_NOT_CLOSED,
    ERR_BATSYS_POS_OPEN_CUR,
    ERR_BATSYS_NEG_OPEN_CUR,
    ERR_BATSYS_NEG_ADJOIN,
    ERR_BATSYS_NEG_NOT_CLOSED,
    ERR_BATSYS_PRE_ADJOIN,
    ERR_BATSYS_PRE_NOT_CLOSED,
    ERR_BATSYS_DCDC_ADJOIN,
    ERR_BATSYS_DCDC_NOT_CLOSED,
    ERR_BATSYS_HVIL_FAIL,
    ERR_BATSYS_HVIL_ABNRML,
    ERR_BATSYS_CHG_ADJOIN,
    ERR_BATSYS_CHG_NOT_CLOSED,
    ERR_BATSYS_CHG_OPEN_CUR,
    ERR_BATSYS_RSV_FOR_3_6,
    ERR_BATSYS_PRE_CHG,
    ERR_BATSYS_RSV_FOR_4_2,
    ERR_BATSYS_RSV_FOR_4_4,
    ERR_BATSYS_RSV_FOR_4_6,
    ERR_BATSYS_FAN_OFF,
    ERR_BATSYS_FAN_ON,
    ERR_BATSYS_HEATER_NOT_ON,
    ERR_BATSYS_HEATER_NOT_OFF,
    ERR_BATSYS_T_HIGH_HEATER,
    ERR_BATSYS_T_HIGH_INLET,
    ERR_BATSYS_T_LOW_INLET,
    ERR_BATSYS_DELTT_INOUTLET,
    ERR_BATSYS_T_HIGH_OUTLET,
    ERR_BATSYS_T_LOW_OUTLET,
    ERR_BATSYS_T_HIGH_POLE,
    ERR_BATSYS_RSV_FOR_7_6,
    // page 1
    ERR_BATSYS_SHUT_DOWN,
    ERR_BATSYS_SUPPLY_V_HIGH,
    ERR_BATSYS_SUPPLY_V_LOW,
    ERR_BATSYS_RSV_FOR_8_6,
    ERR_BATSYS_FAN_ADJOIN,
    ERR_BATSYS_FAN_NOT_CLOSED,
    ERR_BATSYS_RSV_FOR_9_4,
    ERR_BATSYS_LIQUD_SENSOR_ALRM,
    ERR_BATSYS_PRESS_SENSOR_ALRM,
    ERR_BATSYS_CLLSN_SENSOR_ALRM,
    ERR_BATSYS_HUMDT_SENSOR_ALRM,
    ERR_BATSYS_SMOKE_SENSOR_ALRM,
    ERR_BATSYS_T_HIGH_FUSE,
    ERR_BATSYS_FUSE_BROKEN,
    ERR_BATSYS_RSV_FOR_11_4,
    ERR_BATSYS_RSV_FOR_11_6,
    ERR_BATSYS_ISO_POS,
    ERR_BATSYS_ISO_NEG,
    ERR_BATSYS_ISO_MID,
    ERR_BATSYS_ISO_ERR,
    ERR_BATSYS_HEATER_PASE,
    ERR_BATSYS_RSV_FOR_13_2,
    ERR_BATSYS_RSV_FOR_13_4,
    ERR_BATSYS_RSV_FOR_13_6,
    ERR_BATSYS_RSV_FOR_14_0,
    ERR_BATSYS_RSV_FOR_14_2,
    ERR_BATSYS_RSV_FOR_14_4,
    ERR_BATSYS_RSV_FOR_14_6,
    ERR_BATSYS_RSV_FOR_15_0,
    ERR_BATSYS_RSV_FOR_15_2,
    ERR_BATSYS_RSV_FOR_15_4,
    ERR_BATSYS_RSV_FOR_15_6,
} eErr_BatSysErrIndexType;

#define PAGE0 0U
#define PAGE1 1U

#define ERR_MASK_BATSYS_POS_ADJOIN        0x03U
#define ERR_MASK_BATSYS_POS_NOT_CLOSED	  0x0CU
#define ERR_MASK_BATSYS_POS_OPEN_CUR	  0x30U
#define ERR_MASK_BATSYS_NEG_OPEN_CUR	  0xC0U
#define ERR_MASK_BATSYS_NEG_ADJOIN		  0x03U
#define ERR_MASK_BATSYS_NEG_NOT_CLOSED	  0x0CU
#define ERR_MASK_BATSYS_PRE_ADJOIN		  0x30U
#define ERR_MASK_BATSYS_PRE_NOT_CLOSED	  0xC0U
#define ERR_MASK_BATSYS_DCDC_ADJOIN 	  0x03U
#define ERR_MASK_BATSYS_DCDC_NOT_CLOSED	  0x0CU
#define ERR_MASK_BATSYS_HVIL_FAIL		  0x30U
#define ERR_MASK_BATSYS_HVIL_ABNRML		  0xC0U
#define ERR_MASK_BATSYS_CHG_ADJOIN		  0x03U
#define ERR_MASK_BATSYS_CHG_NOT_CLOSED	  0x0CU
#define ERR_MASK_BATSYS_CHG_OPEN_CUR	  0x30U
#define ERR_MASK_BATSYS_RSV_FOR_3_6		  0xC0U
#define ERR_MASK_BATSYS_PRE_CHG 		  0x03U
#define ERR_MASK_BATSYS_RSV_FOR_4_2		  0x0CU
#define ERR_MASK_BATSYS_RSV_FOR_4_4		  0x30U
#define ERR_MASK_BATSYS_RSV_FOR_4_6		  0xC0U
#define ERR_MASK_BATSYS_FAN_OFF 		  0x03U
#define ERR_MASK_BATSYS_FAN_ON			  0x0CU
#define ERR_MASK_BATSYS_HEATER_NOT_ON	  0x30U
#define ERR_MASK_BATSYS_HEATER_NOT_OFF	  0xC0U
#define ERR_MASK_BATSYS_T_HIGH_HEATER	  0x03U
#define ERR_MASK_BATSYS_T_HIGH_INLET	  0x0CU
#define ERR_MASK_BATSYS_T_LOW_INLET		  0x30U
#define ERR_MASK_BATSYS_DELTT_INOUTLET	  0xC0U
#define ERR_MASK_BATSYS_T_HIGH_OUTLET 	  0x03U
#define ERR_MASK_BATSYS_T_LOW_OUTLET	  0x0CU
#define ERR_MASK_BATSYS_T_HIGH_POLE		  0x30U
#define ERR_MASK_BATSYS_RSV_FOR_7_6		  0xC0U

#define ERR_MASK_BATSYS_SHUT_DOWN         0x03U
#define ERR_MASK_BATSYS_SUPPLY_V_HIGH	  0x0CU
#define ERR_MASK_BATSYS_SUPPLY_V_LOW	  0x30U
#define ERR_MASK_BATSYS_RSV_FOR_8_6		  0xC0U
#define ERR_MASK_BATSYS_FAN_ADJOIN 		  0x03U
#define ERR_MASK_BATSYS_FAN_NOT_CLOSED	  0x0CU
#define ERR_MASK_BATSYS_RSV_FOR_9_4		  0x30U
#define ERR_MASK_BATSYS_LIQUD_SENSOR_ALRM 0xC0U
#define ERR_MASK_BATSYS_PRESS_SENSOR_ALRM 0x03U
#define ERR_MASK_BATSYS_CLLSN_SENSOR_ALRM 0x0CU
#define ERR_MASK_BATSYS_HUMDT_SENSOR_ALRM 0x30U
#define ERR_MASK_BATSYS_SMOKE_SENSOR_ALRM 0xC0U
#define ERR_MASK_BATSYS_T_HIGH_FUSE 	  0x03U
#define ERR_MASK_BATSYS_FUSE_BROKEN		  0x0CU
#define ERR_MASK_BATSYS_RSV_FOR_11_4	  0x30U
#define ERR_MASK_BATSYS_RSV_FOR_11_6	  0xC0U
#define ERR_MASK_BATSYS_ISO_POS 		  0x03U
#define ERR_MASK_BATSYS_ISO_NEG			  0x0CU
#define ERR_MASK_BATSYS_ISO_MID			  0x30U
#define ERR_MASK_BATSYS_ISO_ERR			  0xC0U


typedef enum
{
    ERR_HW_DETECT_CELLV,
    ERR_HW_DETECT_TEMPER,
    ERR_HW_DETECT_CUR,
    ERR_HW_DETECT_ISO,
    ERR_HW_DETECT_SUMV_IN,
    ERR_HW_DETECT_SUMV_OUT,
    ERR_HW_CLOCK_EXT,
    ERR_HW_EE_CPU,
    ERR_HW_EE_EXT,
    ERR_HW_FLASH_EXT,
    ERR_HW_FLASH_CPU,
    ERR_HW_CPU_GENERAL,
    ERR_HW_CAN4,
    ERR_HW_CAN0,
    ERR_HW_CAN1,
    ERR_HW_CAN2,
    ERR_HW_CAN3,
    ERR_HW_ETHERNET,
    ERR_HW_RS232,
    ERR_HW_RS485,
    ERR_HW_BAL_CHG,
    ERR_HW_BAL_DCH,
    ERR_HW_DETECT_WH,
    ERR_HW_SOFTWARE,
    ERR_HW_ELOCK,
    ERR_HW_RSV_FOR_6_2,
    ERR_HW_AI,
    ERR_HW_AO,
    ERR_HW_DI,
    ERR_HW_DO,
    ERR_HW_PRE_CHG,//only for Err_HwErrDetail
    ERR_HW_RELAY_DRIVER,
} eErr_HwErrIndexType;

#define ERR_MASK_HW_DETECT_CELLV    0x03U
#define ERR_MASK_HW_DETECT_TEMPER   0x0CU
#define ERR_MASK_HW_DETECT_CUR		0x30U
#define ERR_MASK_HW_DETECT_ISO		0xC0U
#define ERR_MASK_HW_DETECT_SUMV_IN  0x03U
#define ERR_MASK_HW_DETECT_SUMV_OUT 0x0CU
#define ERR_MASK_HW_CLOCK_EXT		0x30U
#define ERR_MASK_HW_EE_CPU			0xC0U
#define ERR_MASK_HW_EE_EXT			0x03U
#define ERR_MASK_HW_FLASH_EXT		0x0CU
#define ERR_MASK_HW_FLASH_CPU		0x30U
#define ERR_MASK_HW_CPU_GENERAL		0xC0U
#define ERR_MASK_HW_CAN4			0x03U
#define ERR_MASK_HW_CAN0			0x0CU
#define ERR_MASK_HW_CAN1			0x30U
#define ERR_MASK_HW_CAN2			0xC0U
#define ERR_MASK_HW_CAN3			0x03U
#define ERR_MASK_HW_ETHERNET		0x0CU
#define ERR_MASK_HW_RS232			0x30U
#define ERR_MASK_HW_RS485			0xC0U
#define ERR_MASK_HW_BAL_CHG			0x03U
#define ERR_MASK_HW_BAL_DCH			0x0CU
#define ERR_MASK_HW_DETECT_WH		0x30U
#define ERR_MASK_HW_SOFTWARE		0xC0U
#define ERR_MASK_HW_RSV_FOR_6_0		0x03U
#define ERR_MASK_HW_RSV_FOR_6_2		0x0CU
#define ERR_MASK_HW_AI				0x30U
#define ERR_MASK_HW_AO				0xC0U
#define ERR_MASK_HW_DI				0x03U
#define ERR_MASK_HW_DO				0x0CU
#define ERR_MASK_HW_PRE_CHG			  0x30U//only for Err_HwErrDetail
#define ERR_MASK_HW_RELAY_DRIVER	0xC0U




extern GeneralReturnType Err_UpdateBatSysErrLevel(eErr_BatSysErrIndexType BatSysErrIndex, eErr_LevelType ErrLevel);
extern GeneralReturnType Err_UpdateHwErrLevelFromCan(uByte BmuNo, sErr_ByteNType * HwErrByteNPtr);
extern GeneralReturnType Err_UpdateHwErrDetailFromCan(uByte BmuNo, eErr_HwErrIndexType HwErrIndex, Err_HwErrDetailType HwErrDetail);


extern GeneralReturnType Err_UpdateHwErrLevel(eErr_HwErrIndexType HwErrIndex, eErr_LevelType ErrLevel);
extern GeneralReturnType Err_UpdateHwErrDetail(eErr_HwErrIndexType HwErrIndex, Err_HwErrDetailType HwErrDetail);


extern eErr_LevelType Err_GetBatErrLevel(eErr_BatErrIndexType BatErrIndex);
extern sErr_ByteNType Err_GetBatAllErrLevel(void);


extern eErr_LevelType Err_GetBatSysErrLevel(eErr_BatSysErrIndexType BatSysErrIndex);
extern sErr_ByteNType Err_GetBatSysAllErrLevel(uByte Page);


extern eErr_LevelType Err_GetHwErrLevel(eErr_HwErrIndexType HwErrIndex);
extern sErr_ByteNType Err_GetHwAllErrLevel(void);
extern Err_HwErrDetailType Err_GetHwErrDetail(eErr_HwErrIndexType HwErrIndex);

extern eErr_LevelType Err_GetErrLevel(eErr_TypeType ErrType);


extern void Err_Init(void);
extern void Err_Count(void);
extern void Err_Main(void);
extern uWord Err_GetAlarmThrhdVal(uByte Err_type, uByte Err_level);
extern void Err_DealHwErrDetail(void);

extern uWord Err_GetSumVHighThrd1(void);
extern uWord Err_GetSumVLowThrd1(void);

extern uWord Err_GetCellVHighThrd1(void);

extern uWord Err_GetCellVLowThrd1(void);

extern uWord Err_GetDisChgCurHighThrd1(void) ;

extern uWord Err_GetChgCurHighThrd1(void);

extern uWord Err_GetSocHighThrd1(void);

extern uWord Err_GetSocLowThrd1(void);

extern uWord Err_GetTHighThrd1(void);

extern uWord Err_GetTLowThrd1(void);

//
//extern sErr_ThrhdType Err_Thrhd[ERR_LEVEL_NUM];

#ifdef __DEBUG_TEST_ERRORALARM

extern void Err_InitDebug(void);
extern void Err_MainDebug(void);

#endif//#ifdef __DEBUG_TEST_ERRORALARM

#endif /* ERRORALARM_H */