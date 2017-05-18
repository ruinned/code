
#ifndef EE_CRC16_H
#define EE_CRC16_H

#include "GeneralTypeDef.h"

extern uWord EE_CalcCRC16AllByte(uByte * p_data, uWord addr, uByte dlen);
extern uWord EE_CalcCRC16SingleByte(uByte * p_data, uByte dlen, uWord crc_orin);

#endif /* EE_CRC16_H */