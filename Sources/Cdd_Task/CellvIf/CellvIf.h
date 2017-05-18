#ifndef __CELLVIF_H__
#define __CELLVIF_H__

#include "GeneralTypeDef.h"

#define AD_CNVT_MODE_NORMAL 0
#define AD_CNVT_MODE_OPEN_CIRCUIT_PU   1
#define AD_CNVT_MODE_OPEN_CIRCUIT_PD   2
#define AD_CNVT_MODE_BAL_CIRCUIT       3


#define DCH_BALANCE_ON  0x11
#define DCH_BALANCE_OFF  0

#define ERR_WR_CFG      1
#define ERR_PEC_CHK     2


#define  ERR_CRC_CELLV   ERR_CDD_CELLVIF_BASE               //CRC error 
#define  ERR_WR_CELLV    (ERR_CDD_CELLVIF_BASE + 1)         //write error
#define  ERR_BUSY_CELLV  (ERR_CDD_CELLVIF_BASE + 2)         //busy
#define  ERR_CFG_CELLV   (ERR_CDD_CELLVIF_BASE + 3)         //configuration register error
#define  ERR_DCC_LTC     (ERR_CDD_CELLVIF_BASE + 4)         //discharge cell register error
#define  ERR_MCI_LTC     (ERR_CDD_CELLVIF_BASE + 5)         //mask cell interrupt error
#define  ERR_VUV_LTC     (ERR_CDD_CELLVIF_BASE + 6)         //undervoltage comparison voltage error
#define  ERR_VOV_LTC     (ERR_CDD_CELLVIF_BASE + 7)         //overvoltage comparison voltage error
#define  ERR_TYPE_BLC    (ERR_CDD_CELLVIF_BASE + 8)         //type of balance is error
#define  ERR_NOT_INIT    (ERR_CDD_CELLVIF_BASE + 9)  
#define  ERR_UNEXPECT     (ERR_CDD_CELLVIF_BASE + 10)       //unexpected error

#define  ERR_CELLV_INIT  (ERR_CDD_CELLVIF_BASE + 11)
#define  ERR_CELLV_START (ERR_CDD_CELLVIF_BASE + 12)
#define  ERR_CELLV_RDCV  (ERR_CDD_CELLVIF_BASE + 13)
#define  ERR_CELLV_CFG   (ERR_CDD_CELLVIF_BASE + 14)
#define  ERR_CELLV_NOT_MEASURE (ERR_CDD_CELLVIF_BASE + 15)
#define  ERR_CELLV_SLEFTEST    (ERR_CDD_CELLVIF_BASE + 16)
#define  ERR_CELLV_MUXFAIL     (ERR_CDD_CELLVIF_BASE + 17)
#define  ERR_CELLV_VREF_FAIL   (ERR_CDD_CELLVIF_BASE + 18)
#define  ERR_CELLV_GPIO        (ERR_CDD_CELLVIF_BASE + 19)
#define  ERR_BALANCE_DCH       (ERR_CDD_CELLVIF_BASE + 20)
#define  ERR_BALANCE_DIAG      (ERR_CDD_CELLVIF_BASE + 21)
#define  ERR_CELLV_MUX         (ERR_CDD_CELLVIF_BASE + 22)
#define  ERR_CELLV_VREF        (ERR_CDD_CELLVIF_BASE + 23)
#define  ERR_CELLV_DIAG        (ERR_CDD_CELLVIF_BASE + 24)
#define  ERR_CELLV_NULL_POINTER  (ERR_CDD_CELLVIF_BASE + 25)

#define  ERR_CELLV_IDOUTRANG   ERR_UNEXPECT//(ERR_CDD_CELLVIF_BASE + 25)
#define  ERR_CELLV_ROWOUTRANG   (ERR_CDD_CELLVIF_BASE + 26)
#define  ERR_CELLV_BALANCE_CMD   (ERR_CDD_CELLVIF_BASE + 27)
#define  MAX_CELLV_NUM_PER_CHIP  12

#if (ERR_CELLV_GPIO > ERR_CDD_CELLVIF_MAX)
#error "Error code over "ERR_CDD_CELLVIF_MAX" !"
#endif


static const uWord crc15Table[256] = 
{
    0x0,    0xc599, 0xceab, 0xb32,  0xd8cf, 0x1d56, 0x1664, 0xd3fd, 0xf407, 0x319e, 0x3aac,  
    0xff35, 0x2cc8, 0xe951, 0xe263, 0x27fa, 0xad97, 0x680e, 0x633c, 0xa6a5, 0x7558, 0xb0c1, 
    0xbbf3, 0x7e6a, 0x5990, 0x9c09, 0x973b, 0x52a2, 0x815f, 0x44c6, 0x4ff4, 0x8a6d, 0x5b2e,
    0x9eb7, 0x9585, 0x501c, 0x83e1, 0x4678, 0x4d4a, 0x88d3, 0xaf29, 0x6ab0, 0x6182, 0xa41b,
    0x77e6, 0xb27f, 0xb94d, 0x7cd4, 0xf6b9, 0x3320, 0x3812, 0xfd8b, 0x2e76, 0xebef, 0xe0dd, 
    0x2544, 0x2be,  0xc727, 0xcc15, 0x98c,  0xda71, 0x1fe8, 0x14da, 0xd143, 0xf3c5, 0x365c, 
    0x3d6e, 0xf8f7, 0x2b0a, 0xee93, 0xe5a1, 0x2038, 0x7c2,  0xc25b, 0xc969, 0xcf0,  0xdf0d, 
    0x1a94, 0x11a6, 0xd43f, 0x5e52, 0x9bcb, 0x90f9, 0x5560, 0x869d, 0x4304, 0x4836, 0x8daf,
    0xaa55, 0x6fcc, 0x64fe, 0xa167, 0x729a, 0xb703, 0xbc31, 0x79a8, 0xa8eb, 0x6d72, 0x6640,
    0xa3d9, 0x7024, 0xb5bd, 0xbe8f, 0x7b16, 0x5cec, 0x9975, 0x9247, 0x57de, 0x8423, 0x41ba,
    0x4a88, 0x8f11, 0x57c,  0xc0e5, 0xcbd7, 0xe4e,  0xddb3, 0x182a, 0x1318, 0xd681, 0xf17b, 
    0x34e2, 0x3fd0, 0xfa49, 0x29b4, 0xec2d, 0xe71f, 0x2286, 0xa213, 0x678a, 0x6cb8, 0xa921, 
    0x7adc, 0xbf45, 0xb477, 0x71ee, 0x5614, 0x938d, 0x98bf, 0x5d26, 0x8edb, 0x4b42, 0x4070, 
    0x85e9, 0xf84,  0xca1d, 0xc12f, 0x4b6, 0xd74b,  0x12d2, 0x19e0, 0xdc79, 0xfb83, 0x3e1a, 0x3528, 
    0xf0b1, 0x234c, 0xe6d5, 0xede7, 0x287e, 0xf93d, 0x3ca4, 0x3796, 0xf20f, 0x21f2, 0xe46b, 0xef59, 
    0x2ac0, 0xd3a,  0xc8a3, 0xc391, 0x608,  0xd5f5, 0x106c, 0x1b5e, 0xdec7, 0x54aa, 0x9133, 0x9a01, 
    0x5f98, 0x8c65, 0x49fc, 0x42ce, 0x8757, 0xa0ad, 0x6534, 0x6e06, 0xab9f, 0x7862, 0xbdfb, 0xb6c9, 
    0x7350, 0x51d6, 0x944f, 0x9f7d, 0x5ae4, 0x8919, 0x4c80, 0x47b2, 0x822b, 0xa5d1, 0x6048, 0x6b7a, 
    0xaee3, 0x7d1e, 0xb887, 0xb3b5, 0x762c, 0xfc41, 0x39d8, 0x32ea, 0xf773, 0x248e, 0xe117, 0xea25, 
    0x2fbc, 0x846,  0xcddf, 0xc6ed, 0x374,  0xd089, 0x1510, 0x1e22, 0xdbbb, 0xaf8,  0xcf61, 0xc453, 
    0x1ca,  0xd237, 0x17ae, 0x1c9c, 0xd905, 0xfeff, 0x3b66, 0x3054, 0xf5cd, 0x2630, 0xe3a9, 0xe89b, 
    0x2d02, 0xa76f, 0x62f6, 0x69c4, 0xac5d, 0x7fa0, 0xba39, 0xb10b, 0x7492, 0x5368, 0x96f1, 0x9dc3, 
    0x585a, 0x8ba7, 0x4e3e, 0x450c, 0x8095
}; 

uWord CellvIf_Init(uByte device_num);

uWord CellvIf_StartCnvt(uByte mode);

uByte CellvIf_RdValue(uByte device_num, uWord cell_codes[][12]);

void CellvIf_RdCvReg(uByte reg, uByte total_ic, uByte *data);


uByte CellvIf_RdAuxValue(uByte total_ic, uWord aux_codes[][6]);

void CellvIf_RdAuxReg(uByte reg, uByte total_ic, uByte *data);

void CellvIf_StartAdAxCvnt(void);

uWord CellvIf_CtrlBalanceSw(uByte DeviceId, uByte row, uByte Cmd);

uWord CellvIf_ActiveBalSw(void);

uByte CellvIf_WrAndRdCfg(uByte total_ic, uByte config[][6]);//, uByte r_config[][8]);


void CellvIf_WakeIdle(void);

uByte CellvIf_SpiData(uByte data);

uWord CellvIf_Pec15Calc(uByte len, uByte *data);

void CellvIf_SpiWrArray( uByte length, uByte *data);

void CellvIf_SpiWrAndRd(uByte tx_Data[], uByte tx_len, uByte *rx_data, uByte rx_len);

extern uLWord CellvIf_GetCellvCrcErrCnt(void);

extern uLWord CellvIf_GetTemperCrcErrCnt(void);

#endif /* __CELLVIF_H__ */
