// ClockIf.c
#include "ClockIf.h"
//#include "Clock_Lcfg.h"

uByte gby_RtcTime[6];

static void ClockIf_IICStart(void);
static void ClockIf_IICStop(void);
static GeneralReturnType ClockIf_IICAckDevice(void);
static void ClockIf_IICAckCpu(void);
static void ClockIf_IICNoAckCpu(void);
static void ClockIf_WriteByte(uByte by);
static uByte ClockIf_ReadByte(void);
static void ClockIf_Wait(uByte delay);

//************************************************************/
// Function: initial of Clock
// Trans para: void
//************************************************************/
void ClockIf_Init(void)
{
    //(*Clock_Operation.Clock_Scl.SetOutput)();
    
    //(*Clock_Operation.Clock_Sda.SetOutput)();
    ExternTimer_Sda_SetOutput();
}

//************************************************************/
// Function: initialize serial port, prepare to send data
// Trans para: void
//************************************************************/
static void ClockIf_IICStart(void)
{
    //(*Clock_Operation.Clock_Sda.SetVal)();
    ExternTimer_Sda_SetVal(); 
    //(*Clock_Operation.Clock_Sda.SetOutput)();
    ExternTimer_Sda_SetOutput();
    ClockIf_Wait(3);
    //(*Clock_Operation.Clock_Scl.SetVal)();
    ExternTimer_Scl_SetVal();
    ClockIf_Wait(3);
    //(*Clock_Operation.Clock_Sda.ClrVal)();
    ExternTimer_Sda_ClrVal();
    ClockIf_Wait(3);
    //(*Clock_Operation.Clock_Scl.ClrVal)();
    ExternTimer_Scl_ClrVal();
}

//************************************************************/
// Function: stop seiral port
// Trans para: void
//************************************************************/
static void ClockIf_IICStop(void)
{
    //(*Clock_Operation.Clock_Sda.ClrVal)();
    ExternTimer_Sda_ClrVal();
    //(*Clock_Operation.Clock_Sda.SetOutput)();
    ExternTimer_Sda_SetOutput();
    ClockIf_Wait(3);
    //(*Clock_Operation.Clock_Scl.SetVal)();
    ExternTimer_Scl_SetVal();
    ClockIf_Wait(3);
    //(*Clock_Operation.Clock_Sda.SetVal)();
    ExternTimer_Sda_SetVal();
    ClockIf_Wait(3);
}

//************************************************************/
// Function:
// Trans para: void
//************************************************************/
static GeneralReturnType ClockIf_IICAckDevice(void)
{
    //(*Clock_Operation.Clock_Sda.SetVal)();
    ExternTimer_Sda_SetVal();
    ClockIf_Wait(3);
    //(*Clock_Operation.Clock_Sda.SetInput)();
    ExternTimer_Sda_SetInput();
    //(*Clock_Operation.Clock_Scl.SetVal)();
    ExternTimer_Scl_SetVal();
    ClockIf_Wait(3);
    
    //if((*Clock_Operation.Clock_Sda.GetVal)())
    if(ExternTimer_Sda_GetVal())
    {
        /* no ack presents */
        ClockIf_IICStop();
        return (GeneralReturnType)CLOCKIF_ERR_IICACK;
    }
    
    //(*Clock_Operation.Clock_Scl.ClrVal)();
    ExternTimer_Scl_ClrVal();
    (GeneralReturnType)CLOCKIF_ERR_OK;
}

//************************************************************/
// Function:
// Trans para: void
//************************************************************/
static void ClockIf_IICAckCpu(void)
{
    //(*Clock_Operation.Clock_Sda.ClrVal)();
    ExternTimer_Sda_ClrVal();
    //(*Clock_Operation.Clock_Sda.SetOutput)();
    ExternTimer_Sda_SetOutput();
    ClockIf_Wait(3);
    //(*Clock_Operation.Clock_Scl.SetVal)();
    ExternTimer_Scl_SetVal();
    ClockIf_Wait(3);
    //(*Clock_Operation.Clock_Scl.ClrVal)();
    ExternTimer_Scl_ClrVal();
}

//************************************************************/
// Function:
// Trans para: void
//************************************************************/
static void ClockIf_IICNoAckCpu(void)
{
    //(*Clock_Operation.Clock_Sda.SetVal)();
    ExternTimer_Sda_SetVal();
    //(*Clock_Operation.Clock_Sda.SetOutput)();
    ExternTimer_Sda_SetOutput();
    ClockIf_Wait(3);
    //(*Clock_Operation.Clock_Scl.SetVal)();
    ExternTimer_Scl_SetVal();
    ClockIf_Wait(3);
    //(*Clock_Operation.Clock_Scl.ClrVal)();
    ExternTimer_Scl_ClrVal();
}

//************************
//发送数据
//************************
static void ClockIf_WriteByte(uByte by)
{
    uByte i;
    uByte data;
    
    //(*Clock_Operation.Clock_Sda.SetVal)();
    ExternTimer_Sda_SetVal();
    ClockIf_Wait(3);
    //(*Clock_Operation.Clock_Sda.SetOutput)();
    ExternTimer_Sda_SetOutput();
    
    for(i = 0; i < 8; i++)
    {
        //(*Clock_Operation.Clock_Scl.ClrVal)();
        ExternTimer_Scl_ClrVal();

        ClockIf_Wait(3);

        data = by & 0x80;
        by <<= 1;

        if (data == 0x80)
        {
            //(*Clock_Operation.Clock_Sda.SetVal)();
            ExternTimer_Sda_SetVal();
        }
        else
        {
            //(*Clock_Operation.Clock_Sda.ClrVal)();
            ExternTimer_Sda_ClrVal();
        }

        ClockIf_Wait(3);

        //(*Clock_Operation.Clock_Scl.SetVal)();
        ExternTimer_Scl_SetVal();

        ClockIf_Wait(3);
    }

    //(*Clock_Operation.Clock_Scl.ClrVal)();
    ExternTimer_Scl_ClrVal();
    ClockIf_Wait(3);

    // device ack
    (void)ClockIf_IICAckDevice();
}

//************************
//读取数据
//************************
static uByte ClockIf_ReadByte(void)
{
    uByte i;
    uByte data;

    data = 0;

    //(*Clock_Operation.Clock_Sda.SetVal)();
    ExternTimer_Sda_SetVal();
    ClockIf_Wait(3);
    //(*Clock_Operation.Clock_Sda.SetInput)();
    ExternTimer_Sda_SetInput();
    for (i = 0; i < 8; i++)
    {
        data <<= 1;

        //(*Clock_Operation.Clock_Scl.ClrVal)();
        ExternTimer_Scl_ClrVal();
        ClockIf_Wait(3);

        //(*Clock_Operation.Clock_Scl.SetVal)();
         ExternTimer_Scl_SetVal();
        ClockIf_Wait(3);

        //if((*Clock_Operation.Clock_Sda.GetVal)())
        if(ExternTimer_Sda_GetVal())
        {
            data |= 0x01;
        }
    }

    //(*Clock_Operation.Clock_Scl.ClrVal)();
    ExternTimer_Scl_ClrVal();

    return data;
}

//************************
// short delay
//************************
static void ClockIf_Wait(uByte delay)
{
    uByte i;
    for(i = delay; i > 0; i--)
    {
        asm NOP;
        asm NOP;
    }
}

//************************
// write time
//************************
GeneralReturnType ClockIf_WriteTime(sClockIf_TimeType *time)
{
    uByte i = 0;
    GeneralReturnType rslt;
    sClockIf_TimeType time_wr;
    sClockIf_TimeType time_rd;
    
    time_wr.year = ((time->year / 10) << 4) + (time->year % 10);
    time_wr.month = ((time->month / 10) << 4) + (time->month % 10);
    time_wr.day = ((time->day / 10) << 4) + (time->day % 10);
    time_wr.hour = ((time->hour / 10) << 4) + (time->hour % 10);
    time_wr.minute = ((time->minute / 10) << 4) + (time->minute % 10);
    time_wr.second = ((time->second / 10) << 4) + (time->second % 10);

    // write 3 times until write ok
    while(i < 3)
    {
        ClockIf_IICStart();
        ClockIf_WriteByte(SD2405_WR);
        ClockIf_WriteByte(0x10);
        ClockIf_WriteByte(0x80);   // config WRTC1
        ClockIf_IICStop();

        ClockIf_IICStart();
        ClockIf_WriteByte(SD2405_WR);
        ClockIf_WriteByte(0x0f);
        ClockIf_WriteByte(0x84);   // config WRTC2，WRTC3，enable write
        ClockIf_IICStop();

        ClockIf_IICStart();
        ClockIf_WriteByte(SD2405_WR);
        ClockIf_WriteByte(0x06);
        ClockIf_WriteByte(time_wr.year);  		 //year
        ClockIf_IICStop();

        ClockIf_IICStart();
        ClockIf_WriteByte(SD2405_WR);
        ClockIf_WriteByte(0x05);
        ClockIf_WriteByte(time_wr.month);  		 //month
        ClockIf_IICStop();

        ClockIf_IICStart();
        ClockIf_WriteByte(SD2405_WR);
        ClockIf_WriteByte(0x04);
        ClockIf_WriteByte(time_wr.day);  		   //day
        ClockIf_IICStop();

        ClockIf_IICStart();
        ClockIf_WriteByte(SD2405_WR);
        ClockIf_WriteByte(0x02);
        ClockIf_WriteByte(time_wr.hour | 0x80);  //hour
        ClockIf_IICStop();

        ClockIf_IICStart();
        ClockIf_WriteByte(SD2405_WR);
        ClockIf_WriteByte(0x01);
        ClockIf_WriteByte(time_wr.minute);  	 //min
        ClockIf_IICStop();

        ClockIf_IICStart();
        ClockIf_WriteByte(SD2405_WR);
        ClockIf_WriteByte(0x00);
        ClockIf_WriteByte(time_wr.second);  	 //s
        ClockIf_IICStop();

        if(CLOCKIF_ERR_OK == ClockIf_ReadTime(&time_rd))
        {
            if( (time_rd.year == time->year)
             && (time_rd.month == time->month)
             && (time_rd.day == time->day)
             && (time_rd.hour == time->hour)
             && (time_rd.minute == time->minute)
             && (time_rd.second == time->second))
            {
                rslt = (GeneralReturnType)CLOCKIF_ERR_OK;
                break;
            }
            else
            {
                rslt = (GeneralReturnType)CLOCKIF_ERR_WRTIME;
            }
        }
        else
        {
            rslt = (GeneralReturnType)CLOCKIF_ERR_WRTIME;
        }
        
        i++;
    }
    return rslt;
}

//************************
// read time
//************************
GeneralReturnType ClockIf_ReadTime(sClockIf_TimeType *time)	 
{
    uByte i = 0;
    uByte btmp;
    GeneralReturnType rslt;
    
    // read 3 times until read ok
    while(i < 3)
    {
        ClockIf_IICStart();
        ClockIf_WriteByte(SD2405_RD);		      // direct read BCD
        
        gby_RtcTime[5] = ClockIf_ReadByte();	// second
        ClockIf_IICAckCpu();
        
        gby_RtcTime[4] = ClockIf_ReadByte();	// min
        ClockIf_IICAckCpu();
        
        gby_RtcTime[3] = ClockIf_ReadByte() & 0x3f;	// hour
        ClockIf_IICAckCpu();
        
        btmp = ClockIf_ReadByte();	          // week, don't care
        ClockIf_IICAckCpu();
        
        gby_RtcTime[2] = ClockIf_ReadByte();	// day
        ClockIf_IICAckCpu();
        
        gby_RtcTime[1] = ClockIf_ReadByte();	// month
        ClockIf_IICAckCpu();
        
        gby_RtcTime[0] = ClockIf_ReadByte();	// year
        ClockIf_IICNoAckCpu();
        
        ClockIf_IICStop();

        // BCD to DEC
        time->second = (gby_RtcTime[5] >> 4) * 10 + (gby_RtcTime[5] & 0x0f); 
        time->minute = (gby_RtcTime[4] >> 4) * 10 + (gby_RtcTime[4] & 0x0f);
        time->hour = (gby_RtcTime[3] >> 4) * 10 + (gby_RtcTime[3] & 0x0f);
        time->day = (gby_RtcTime[2] >> 4) * 10 + (gby_RtcTime[2] & 0x0f);
        time->month = (gby_RtcTime[1] >> 4) * 10 + (gby_RtcTime[1] & 0x0f);
        time->year = (gby_RtcTime[0] >> 4) * 10 + (gby_RtcTime[0] & 0x0f);

        // check time in normal value
        if( (time->year < 100)
         && (time->month < 13)
         && (time->day < 32)
         && (time->hour < 24)
         && (time->minute < 60)
         && (time->second < 60) )
        {
            rslt = (GeneralReturnType)CLOCKIF_ERR_OK;
            break;
        }
        else
        {
            rslt = (GeneralReturnType)CLOCKIF_ERR_RDTIME;
        }
        
        i++;
    }
    
    return (GeneralReturnType)rslt;
}
