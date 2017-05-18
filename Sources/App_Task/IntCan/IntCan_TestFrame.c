#include "IntCan.h"
#include "BatterySt.h"
#include "CarCan.h"
#include "CellvIf.h"

#define FLAG_ENABLE 1
#define FLAG_DISABLE 0

void IntCan_AddTestData(void)
{
    uByte msg_data1[8],msg1_send_en;
    uByte msg_data2[8],msg2_send_en;
    uLWord lwtmp;
    
    msg1_send_en = FLAG_ENABLE;
    msg2_send_en = FLAG_DISABLE;
    
    lwtmp = CellvIf_GetCellvCrcErrCnt();
    msg_data1[0] = (uByte)((lwtmp & 0xff000000) >> 24);
    msg_data1[1] = (uByte)((lwtmp & 0x00ff0000) >> 16);
    msg_data1[2] = (uByte)((lwtmp & 0x0000ff00) >> 8);
    msg_data1[3] = (uByte)(lwtmp & 0x000000ff);
    lwtmp = CellvIf_GetTemperCrcErrCnt();
    msg_data1[4] = (uByte)((lwtmp & 0xff000000) >> 24);
    msg_data1[5] = (uByte)((lwtmp & 0x00ff0000) >> 16);
    msg_data1[6] = (uByte)((lwtmp & 0x0000ff00) >> 8);
    msg_data1[7] = (uByte)(lwtmp & 0x000000ff);
    
    msg_data2[0] = 9;
    msg_data2[1] = 10;
    msg_data2[2] = 11;
    msg_data2[3] = 12;
    msg_data2[4] = 13;
    msg_data2[5] = 14;
    msg_data2[6] = 15;
    msg_data2[7] = 16; 
       
    IntCan_WrTestFrameToBuff(1, &msg_data1[0], msg1_send_en);
    IntCan_WrTestFrameToBuff(2, &msg_data2[0], msg2_send_en);
}