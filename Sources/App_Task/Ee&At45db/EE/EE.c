#include "EE.h"
#include "NvmIf.h"
#include "SchIf.h"
#include "EE_CRC16.h"
#include "ErrorAlarm.h"
#include "BmsSysSt.h"
#include "LvM.h"
#include "WdgIf.h"


/* define max data length and buffer length */
#define EE_MAX_DATA_LEN       32
#define MAX_ADDR_BUF_NUM      32

/* define count for power down */
#define POWER_DOWN_CNT_500MS  100
#define POWER_DOWN_CNT_1000MS 200

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

uWord gwd_RecognizeData;
uLWord glwd_CntSaveAh;
uByte gby_ReadData[EE_MAX_DATA_LEN];


#ifdef EE_DEBUG
uByte gby_TestData[EE_MAX_DATA_LEN];
#endif

#pragma DATA_SEG DEFAULT


typedef struct
{
    uByte cnt;
} sEE_TmCntType;

sEE_TmCntType EE_TmCnt;


#define BAKUP_NULL 0x00
#define BAKUP_A   0x00
#define BAKUP_B   0x01
#define BAKUP_C   0x02

/* define relative address for use */

/* define CPU EE address */
#define EE_COPY1_ADDR    0xc00
#define EE_COPY2_ADDR    0xd55
#define EE_COPY3_ADDR    0xeaa
#define EE_JMP_ADDR      0x155

/* define EE max used address */
#define CPUEE_MAX_USE_ADDR     0xd54
#define EXTERNEE_MAX_USE_ADDR  0x2000

#define EE_COPY_END      CPUEE_MAX_USE_ADDR

/* define EE recognize data address */
#define CPUEE_REC_ADDR     0xc00
#define EXTERNEE_REC_ADDR  0xc02


/* define busy flag for extern EE */
#define EXTERNEE_BUSY 1
#define EXTERNEE_NO_BUSY   0

/* recognize data for EE */
#define REC_DATA           0x5AA5

/* good crc value for EE */		 
#define GOOD_CRC           0xF0B8    

/* define read and write mask */
#define RD_SUC_EE          0x40      /* read success */
#define RD_FAIL_EE         0x20      /* read fail */
#define WR_SUC_EE          0x60      /* write success */
#define WR_FAIL_EE         0x80      /* write fail */

#define RD_SUC_ABC         0x40			 /* read success, data block of a, b, c are ok */
#define RD_SUC_AB          0x41			 /* read success, data block of a, b are ok */
#define RD_SUC_AC          0x42			 /* read success, data block of a, c are ok */
#define RD_SUC_BC          0x43			 /* read success, data block of b, c are ok */
#define RD_SUC_A           0x44      /* read success, data block of a is ok */
#define RD_SUC_B           0x45      /* read success, data block of b is ok */
#define RD_SUC_C           0x46      /* read success, data block of c is ok */

#define WR_SUC_ABC         0x60			 /* write success, data block of a, b, c are ok */
#define WR_SUC_AB          0x61			 /* write success, data block of a, b are ok */
#define WR_SUC_AC          0x62			 /* write success, data block of a, c are ok */
#define WR_SUC_BC          0x63			 /* write success, data block of b, c are ok*/
#define WR_SUC_A           0x64			 /* write success, data block of a is ok */
#define WR_SUC_B           0x65			 /* write success, data block of b is ok */

#define RD_FAIL_ABC        0x20			 /* read fail, data block of a, b, c are fail*/
#define RD_AB_CHK_A_FAIL   0x21      /* read fail, check a, b ok, but get block of a fail */
#define RD_A_CHK_B_CHK_FAIL_A_FAIL    0x22   /* read fail, check a ok, b fail, but get block of a fail */
#define RD_A_CHK_FAIL_B_CHK_B_FAIL    0x23   /* read fail, check a fail, b ok, but get block of b fail */
#define RD_C_CHK_AB_CHK_FAIL_C_FAIL   0x24   /* read fail, check a, b fail, c ok, but get block of c fail */
#define RD_FAIL_LEN        0x25      /* read fail, data length error */
#define RD_FAIL_ADDR       0x26      /* read fail, data addr error */

#define WR_FAIL_AB_CHK_FAIL  0x80    /* write fail, check block a, b fail */
#define WR_FAIL_AB_RD_FAIL   0x81    /* write fail, read block a, b fail after write */
#define WR_FAIL_LEN          0x82    /* write fail, data length error */
#define WR_FAIL_ADDR         0x83    /* write fail, data addr error */


const uWord gwd_MaxDataAddr[2] = 
{
  CPUEE_MAX_USE_ADDR,
  EXTERNEE_MAX_USE_ADDR
};
uWord gwd_AddrBuf[MAX_ADDR_BUF_NUM];
uWord gwd_AddrBufCnt;
uByte gby_BlockSaveAh;
uByte gby_ExternEE_Status;
uByte gby_EeErrFlag;

/* inner function */
static GeneralReturnType EE_CheckArray(eEE_DeviceType EE_Device, uByte * p_data, uWord addr, uByte dlen, uWord * crc_ptr);
static GeneralReturnType EE_CompareArray(eEE_DeviceType EE_Device, uByte * p_data, uWord addr, uByte dlen, uWord crc);
static GeneralReturnType EE_ReadBlocks3(eEE_DeviceType EE_Device, eSysConf_ParameterNOType ParameterNo,uByte * RAMPTR p_data, uWord addr, uByte dlen);
static GeneralReturnType EE_ReadBlocks1(eEE_DeviceType EE_Device, uByte * RAMPTR p_data, uWord addr, uByte dlen);
static GeneralReturnType EE_WriteBlocks3(eEE_DeviceType EE_Device, uByte * RAMPTR p_data, uWord addr, uByte dlen, uWord crc);
static GeneralReturnType EE_WriteBlocks1(eEE_DeviceType EE_Device, uByte * RAMPTR p_data, uWord addr, uByte dlen, uWord crc);
static GeneralReturnType EE_SaveVar3(eEE_DeviceType EE_Device, uByte * RAMPTR p_data, uWord addr, uByte dlen);
static GeneralReturnType EE_SaveVar1(eEE_DeviceType EE_Device, uByte * RAMPTR p_data, uWord addr, uByte dlen);
static GeneralReturnType EE_LoadVar3(eEE_DeviceType EE_Device,eSysConf_ParameterNOType ParameterNo, uByte * RAMPTR p_data, uWord addr, uByte dlen);
static GeneralReturnType EE_LoadVar1(eEE_DeviceType EE_Device, uByte * RAMPTR p_data, uWord addr, uByte dlen);
static GeneralReturnType EE_InitDevice(eEE_DeviceType EE_Device);
static GeneralReturnType EE_SaveBufToExternEE(void);
static void EE_SaveAddrNoToBuf(eSysConf_ParameterNOType ParameterNo, uWord OffSet);

//******************************************************************//
// Function: check array data
// Trans Para: uWord addr, uWord dlen, uWord *crc
//******************************************************************//
static GeneralReturnType EE_CheckArray(eEE_DeviceType EE_Device, uByte* p_data, uWord addr, uByte dlen, uWord * crc_ptr)
{
    GeneralReturnType rslt;
    uByte btmp;
    uByte i;
    uWord wtmp;
    uWord lwd_crc;
    uWord addr_bak;

    addr_bak = addr;
    rslt = (uWord)EE_ERR_OK;
    lwd_crc = 0xffff;

    if(addr < EE_COPY_END)
    {    
        if(addr >= ((EE_JMP_ADDR << 1) + EE_COPY1_ADDR))
        {
            addr_bak -= EE_JMP_ADDR << 1;
        }
        else if(addr  >= EE_JMP_ADDR + EE_COPY1_ADDR)
        {
            addr_bak -= EE_JMP_ADDR;
        }
        else
        {
            //addr_bak -= 0;
        }
    }

    
    rslt = (*NvmIf_Operation[EE_Device].ReadArray)(p_data, addr, dlen + 2);
    wtmp = *(p_data + dlen);
    wtmp = (wtmp << 8) + (*(p_data + dlen + 1)); 
    
    for (i = 0; i < dlen; i++)
    {
        /* calc crc by data */
        lwd_crc = EE_CalcCRC16SingleByte(p_data++, sizeof(*p_data), lwd_crc);
    }

    /* calc crc by addr */
    lwd_crc = EE_CalcCRC16SingleByte((uByte *)(&addr_bak), sizeof(addr_bak), lwd_crc);

    /* calc crc by dlen */
    lwd_crc = EE_CalcCRC16SingleByte((uByte *)(&dlen), sizeof(dlen), lwd_crc);

    /* check crc */    
    btmp = (uByte)(wtmp & 0xff);
    lwd_crc = EE_CalcCRC16SingleByte(&btmp, sizeof(btmp), lwd_crc);
    btmp = (uByte)(wtmp >> 8);
    lwd_crc = EE_CalcCRC16SingleByte(&btmp, sizeof(btmp), lwd_crc);

    if(lwd_crc != GOOD_CRC)
    {
        rslt = (uWord)EE_ERR_CRC;
    }
    else
    {
        (*crc_ptr) = wtmp;
    }

    return rslt;
}

//******************************************************************//
// Function: Compare array data
// Trans Para: uByte* p_data, uWord addr, uWord dlen, uWord crc
//******************************************************************//
static GeneralReturnType EE_CompareArray(eEE_DeviceType EE_Device, uByte * p_data, uWord addr, uByte dlen, uWord crc)
{
    GeneralReturnType rslt;
    uByte lby_data;
    uByte btmp;
    uByte i;
    uWord wtmp;
    uWord lwd_crc;
    uWord addr_bak;

    addr_bak = addr;
    rslt = (uWord)EE_ERR_OK;
    lwd_crc = 0xffff;

    if(addr < EE_COPY_END)
    {
      if(addr >= ((EE_JMP_ADDR << 1) + EE_COPY1_ADDR))
        {
            addr_bak -= EE_JMP_ADDR << 1;
        }
        else if(addr  >= EE_JMP_ADDR + EE_COPY1_ADDR)
        {
            addr_bak -= EE_JMP_ADDR;
        }
        else
        {
            //addr_bak -= 0;
        }	
    }
    for (i = 0; i < dlen; i++)
    {
        rslt |= (*NvmIf_Operation[EE_Device].GetByte)(addr++, &lby_data);

        if(*(p_data++) != lby_data)
        {
            rslt = (uWord)EE_ERR_COMPARE_ARRAY;
            break;
        }

        /* calc crc by data */
        lwd_crc = EE_CalcCRC16SingleByte(&lby_data, sizeof(lby_data), lwd_crc);
    }

    if((uWord)EE_ERR_OK == rslt)
    {
        /* calc crc by addr */
        lwd_crc = EE_CalcCRC16SingleByte((uByte *)(&addr_bak), sizeof(addr_bak), lwd_crc);
        /* calc crc by dlen */
        lwd_crc = EE_CalcCRC16SingleByte((uByte *)(&dlen), sizeof(dlen), lwd_crc);

        /* read crc */
        rslt |= (*NvmIf_Operation[EE_Device].GetByte)(addr++, &btmp);
        wtmp = btmp;
        wtmp <<= 8;
        rslt |= (*NvmIf_Operation[EE_Device].GetByte)(addr++, &btmp);
        wtmp += btmp;

        btmp = (uByte)(wtmp & 0xff);
        lwd_crc = EE_CalcCRC16SingleByte(&btmp, sizeof(btmp), lwd_crc);
        btmp = (uByte)(wtmp >> 8);
        lwd_crc = EE_CalcCRC16SingleByte(&btmp, sizeof(btmp), lwd_crc);

        if((crc != wtmp) || (lwd_crc != GOOD_CRC))
        {
            rslt = (uWord)EE_ERR_CRC;
        }
    }

    return rslt;
}

//************************************************************/
// Function: read data block from EE, 3 copy
// Trans para: point of data, address, leng.
//************************************************************/
static GeneralReturnType EE_ReadBlocks3(eEE_DeviceType EE_Device, eSysConf_ParameterNOType ParameterNo, uByte * RAMPTR p_data, uWord addr, uByte dlen)
{
    GeneralReturnType rslt;
    GeneralReturnType rslt_rd;
    GeneralReturnType rslt_a;
    GeneralReturnType rslt_b;
    GeneralReturnType rslt_c;

    uWord addr_a;
    uWord addr_b;
    uWord addr_c;

    uWord lwd_crc_a;
    uWord lwd_crc_b;
    uWord lwd_crc_c;

    uByte i;
    uByte rd_data[EE_MAX_DATA_LEN];
    uByte rd_data_bak[EE_MAX_DATA_LEN];

    rslt_rd = (uWord)EE_ERR_OK;

    addr_a = addr;
    addr_b = (addr + EE_JMP_ADDR);
    addr_c = (addr + 2 * EE_JMP_ADDR);

    /* begin */
    rslt_a = EE_CheckArray(EE_Device, rd_data, addr_a, dlen, &lwd_crc_a);
    rslt_b = EE_CheckArray(EE_Device, rd_data_bak, addr_b, dlen, &lwd_crc_b);

    if((uWord)EE_ERR_OK == rslt_a)
    {
        if((uWord)EE_ERR_OK == rslt_b)
        {
            /* get data from block a */
            rslt_rd = RD_SUC_AB;

            for(i = 0; i < dlen; i++)
            {
                *(p_data ++) = rd_data[i];
            }

            rslt_c = EE_CheckArray(EE_Device, rd_data_bak, addr_c, dlen, &lwd_crc_c);
            
            if((uWord)EE_ERR_OK != rslt_c)
            {
                /* block a,b is OK, rewrite block c */
                (*NvmIf_Operation[EE_Device].WriteEnable)();
                if(CPUEE == EE_Device)
                {
                   rslt = (*NvmIf_Operation[EE_Device].WriteArray)(rd_data, addr_c, (dlen + 2));
                   WdgIf_Trig();
                }
                else
                {
                 
                    EE_SaveAddrNoToBuf(ParameterNo,BAKUP_C);
                }
                (*NvmIf_Operation[EE_Device].WriteDisable)();
            }
            else
            {
                /* block a,b,c is OK */
                rslt_rd = RD_SUC_ABC;
            }
        }
        else
        {
            /* get data from block a */
            for(i = 0; i < dlen; i++)
            {
                *(p_data ++) = rd_data[i];
            }
            rslt_rd = RD_SUC_A;
          
            rslt_c = EE_CheckArray(EE_Device, rd_data_bak, addr_c, dlen, &lwd_crc_c);
            
            (*NvmIf_Operation[EE_Device].WriteEnable)();
            if((uWord)EE_ERR_OK == rslt_c)
            {
                /* block a,c is OK, rewrite block b */
               if(CPUEE == EE_Device)
               {
                    rslt = (*NvmIf_Operation[EE_Device].WriteArray)(rd_data, addr_b, (dlen + 2));
                    WdgIf_Trig(); 
               }
               else
               {

                  EE_SaveAddrNoToBuf(ParameterNo, BAKUP_B);
               }              
               rslt_rd = RD_SUC_AC;
            }
            else
            {
                /* block a is OK, rewrite block b, c */
               
               if(CPUEE == EE_Device)
               {
                  rslt = (*NvmIf_Operation[EE_Device].WriteArray)(rd_data, addr_b, (dlen + 2));
                  WdgIf_Trig();
                  rslt = (*NvmIf_Operation[EE_Device].WriteArray)(rd_data, addr_c, (dlen + 2)); 
                  WdgIf_Trig();
               }
               else
               {
                  EE_SaveAddrNoToBuf(ParameterNo, BAKUP_B);
                  EE_SaveAddrNoToBuf(ParameterNo, BAKUP_C);
               }
            }
            (*NvmIf_Operation[EE_Device].WriteDisable)();
        }
    }
    else
    {
        if((uWord)EE_ERR_OK == rslt_b)
        {
            /* get data from block b */
            for(i = 0; i < dlen; i++)
            {
                *(p_data ++) = rd_data_bak[i];
            }
            rslt_rd = RD_SUC_B;

            rslt_c = EE_CheckArray(EE_Device, rd_data, addr_c, dlen, &lwd_crc_c);
            
            (*NvmIf_Operation[EE_Device].WriteEnable)();
            if((uWord)EE_ERR_OK == rslt_c)
            {
                /* block b,c is OK, rewrite block a  */
                
               if(CPUEE == EE_Device)
               {
                   rslt = (*NvmIf_Operation[EE_Device].WriteArray)(rd_data_bak, addr_a, (dlen + 2));
                   WdgIf_Trig();
               }
               else
               {
                  EE_SaveAddrNoToBuf(ParameterNo, BAKUP_A);
               }                              
               rslt_rd = RD_SUC_BC;
            }
            else
            {
                /* block b is OK, rewrite block a, c */
               
               if(CPUEE == EE_Device)
               {
                   rslt = (*NvmIf_Operation[EE_Device].WriteArray)(rd_data_bak, addr_a, (dlen + 2));
                   WdgIf_Trig();
                   rslt = (*NvmIf_Operation[EE_Device].WriteArray)(rd_data_bak, addr_c, (dlen + 2));
                   WdgIf_Trig();
               }
               else
               {
                  EE_SaveAddrNoToBuf(ParameterNo, BAKUP_A);
                  EE_SaveAddrNoToBuf(ParameterNo, BAKUP_C);
               }
            }
            (*NvmIf_Operation[EE_Device].WriteDisable)();
        }
        else
        {
            rslt_c = EE_CheckArray(EE_Device, rd_data, addr_c, dlen, &lwd_crc_c);
            
            if((uWord)EE_ERR_OK == rslt_c)
            {
                /* block c is OK, get data from block c */
                for(i = 0; i < dlen; i++)
                {
                    *(p_data ++) = rd_data[i];
                }
                rslt_rd = RD_SUC_C;

                (*NvmIf_Operation[EE_Device].WriteEnable)();
              
                
               if(CPUEE == EE_Device)
               {
                    rslt = (*NvmIf_Operation[EE_Device].WriteArray)(rd_data, addr_a, (dlen + 2));
                    WdgIf_Trig();
                    rslt = (*NvmIf_Operation[EE_Device].WriteArray)(rd_data, addr_b, (dlen + 2));
                    WdgIf_Trig();
               }
               else
               {

                  EE_SaveAddrNoToBuf(ParameterNo, BAKUP_A);
                  EE_SaveAddrNoToBuf(ParameterNo, BAKUP_B);
               }
                
               (*NvmIf_Operation[EE_Device].WriteDisable)();
            }
            else
            {
                rslt_rd = RD_FAIL_ABC;
            }
        }
    }
    
    if( ((uWord)EE_ERR_OK == rslt_rd) || (RD_SUC_EE == (rslt_rd & RD_SUC_EE)) )
    {
        rslt_rd = (uWord)EE_ERR_OK; 
    }
    else
    {
        rslt_rd = (uWord)EE_ERR_READBLOCK3;
    }
    return rslt_rd;
}

//************************************************************/
// Function: read data block from EE, 1 copy
// Trans para: point of data, address, leng.
//************************************************************/
static GeneralReturnType EE_ReadBlocks1(eEE_DeviceType EE_Device, uByte * RAMPTR p_data, uWord addr, uByte dlen)
{
    uByte i;
    uByte rd_data[EE_MAX_DATA_LEN];
    GeneralReturnType rslt_rd;
    uWord rslt_tmp;
    uWord addr_a;
    uWord lwd_crc_a;

    rslt_rd = (uWord)EE_ERR_OK;
    addr_a = addr;

    /* begin */
    rslt_rd = EE_CheckArray(EE_Device, rd_data, addr_a, dlen, &lwd_crc_a);

    if((uWord)EE_ERR_OK == rslt_rd)
    {
        rslt_tmp = RD_SUC_A;
        rslt_rd = (uWord)EE_ERR_OK;

        for(i = 0; i < dlen; i++)
        {
            *(p_data ++) = rd_data[i];
        }
    }
    
    if((uWord)EE_ERR_OK == rslt_rd)
    {
        rslt_rd = (uWord)EE_ERR_OK; 
    }
    else
    {
        rslt_rd = (uWord)EE_ERR_READBLOCK1;
    }

    return rslt_rd;
}

//************************************************************/
// Function:write data block to EE, 3 copy
// Trans para: point of data, address, leng.
//************************************************************/
static GeneralReturnType EE_WriteBlocks3(eEE_DeviceType EE_Device, uByte * RAMPTR p_data, uWord addr, uByte dlen, uWord crc)
{
    GeneralReturnType rslt_a;
    GeneralReturnType rslt_b;
    GeneralReturnType rslt_c;
    GeneralReturnType rslt_wr_a;
    GeneralReturnType rslt_wr_b;
    GeneralReturnType rslt_wr_c;
    GeneralReturnType rslt_wr;
    uByte i;
    uByte wr_data[EE_MAX_DATA_LEN];
    uWord addr_a;
    uWord addr_b;
    uWord addr_c;
    uWord lwd_crc;

    addr_a = addr;
    addr_b = (addr + EE_JMP_ADDR);
    addr_c = (addr + 2 * EE_JMP_ADDR);

    //prepare data and crc value to save
    for(i = 0; i < dlen; i++)
    {
        wr_data[i] = *(p_data + i);
    }
    wr_data[i++] = (uByte)((crc & 0xff00) >> 8);
    wr_data[i++] = (uByte)(crc & 0x00ff);

    // check data transmission is ok
    lwd_crc = EE_CalcCRC16AllByte(wr_data, addr, dlen);
    if(crc != lwd_crc)
    {
        return (uWord)EE_ERR_CRC;
    }

    // begin to write
    rslt_wr_a = (*NvmIf_Operation[EE_Device].WriteArray)(wr_data, addr_a, (dlen + 2));      //write block a

    if((uWord)EE_ERR_OK == rslt_wr_a)
    {
        rslt_a = EE_CompareArray(EE_Device, wr_data, addr_a, dlen, crc);
    }
    else
    {
        rslt_a = (uWord)EE_ERR_WRITE_ARRAY;
    }

    rslt_wr_b = (*NvmIf_Operation[EE_Device].WriteArray)(wr_data, addr_b, (dlen + 2));      //write block b

    if((uWord)EE_ERR_OK == rslt_wr_b)
    {
        rslt_b = EE_CompareArray(EE_Device, wr_data, addr_b, dlen, crc);
    }
    else
    {
        rslt_b = (uWord)EE_ERR_WRITE_ARRAY;
    }

    rslt_wr = (uWord)EE_ERR_OK;
    if( ((uWord)EE_ERR_OK == rslt_a) || ((uWord)EE_ERR_OK == rslt_b) )
    {
        rslt_wr_c = (*NvmIf_Operation[EE_Device].WriteArray)(wr_data, addr_c, (dlen + 2));   //write block c

        if((uWord)EE_ERR_OK == rslt_wr_c)
        {
            rslt_c = EE_CompareArray(EE_Device, wr_data, addr_c, dlen, crc);
        }
        else
        {
            rslt_c = (uWord)EE_ERR_WRITE_ARRAY;
        }

        if(((uWord)EE_ERR_OK == rslt_a) && ((uWord)EE_ERR_OK == rslt_b) && ((uWord)EE_ERR_OK == rslt_c))
        {
            rslt_wr = WR_SUC_ABC;
        }
        else if(((uWord)EE_ERR_OK == rslt_b) && ((uWord)EE_ERR_OK == rslt_c))
        {
            rslt_wr = WR_SUC_BC;
        }
        else if(((uWord)EE_ERR_OK == rslt_a) && ((uWord)EE_ERR_OK == rslt_c))
        {
            rslt_wr = WR_SUC_AC;
        }
        else  if(((uWord)EE_ERR_OK == rslt_a) && ((uWord)EE_ERR_OK == rslt_b))
        {
            rslt_wr = WR_SUC_AB;
        }
        else if((uWord)EE_ERR_OK == rslt_a)
        {
            rslt_wr = WR_SUC_A;
        }
        else
        {
            rslt_wr = WR_SUC_B;
        }
    }
    else  if(((uWord)EE_ERR_OK != rslt_wr_a) && ((uWord)EE_ERR_OK != rslt_wr_b))
    {
        rslt_wr = WR_FAIL_AB_CHK_FAIL;
    }
    else if(((uWord)EE_ERR_OK == rslt_wr_a) && ((uWord)EE_ERR_OK == rslt_wr_b)
            && ((uWord)EE_ERR_OK != rslt_a) && ((uWord)EE_ERR_OK != rslt_b))
    {
        rslt_wr = WR_FAIL_AB_RD_FAIL;
    }
    else
    {
        return (uWord)EE_ERR_WRITEBLOCK3;
    }

    if( ((uWord)EE_ERR_OK == rslt_wr) || (WR_SUC_EE == (rslt_wr & WR_SUC_EE)) )
    {
        return (uWord)EE_ERR_OK;
    }
    else
    {
        return (uWord)EE_ERR_WRITEBLOCK3;
    }
}

//************************************************************/
// Function:write data block to EE, 1 cpoy
// Trans para: point of data, address, leng.
//************************************************************/
static GeneralReturnType EE_WriteBlocks1(eEE_DeviceType EE_Device, uByte * RAMPTR p_data, uWord addr, uByte dlen, uWord crc)
{
    GeneralReturnType rslt_wr_a;
    GeneralReturnType rslt_wr;
    uWord rslt_tmp;
    uByte i;
    uByte wr_data[EE_MAX_DATA_LEN];

    uWord addr_a;
    uWord lwd_crc;
    
    addr_a = addr;
    rslt_wr = (uWord)EE_ERR_OK;

    //prepare data and crc value to save
    for(i = 0; i < dlen; i++)
    {
        wr_data[i] = *(p_data + i);
    }
    wr_data[i++] = (uByte)((crc & 0xff00) >> 8);
    wr_data[i++] = (uByte)(crc & 0x00ff);

    // check data transmission is ok
    lwd_crc = EE_CalcCRC16AllByte(wr_data, addr, dlen);
    if(crc != lwd_crc)
    {
        return (uWord)EE_ERR_CRC;
    }

    // begin to write
    rslt_wr_a = (*NvmIf_Operation[EE_Device].WriteArray)(wr_data, addr_a, (dlen + 2));      //write block a

    if((uWord)EE_ERR_OK == rslt_wr_a)
    {
        if(CPUEE == EE_Device)    
        {
            rslt_tmp = EE_CompareArray(EE_Device, wr_data, addr_a, dlen, crc);
            rslt_wr = rslt_tmp;
        }
        else
        {
            rslt_tmp = WR_SUC_A;
            rslt_wr = (uWord)EE_ERR_OK;
        }
    }
    else
    {
        rslt_wr = (uWord)EE_ERR_WRITE_ARRAY;
    }
    
    if((uWord)EE_ERR_OK == rslt_wr)
    {
        return (uWord)EE_ERR_OK;
    }
    else
    {
        return (uWord)EE_ERR_WRITEBLOCK1;
    }
}

//************************************************************/
// Function:Save var to EE, 3 copy
// Trans para: point of data ,address, leng.
//************************************************************/
GeneralReturnType EE_SaveVar3(eEE_DeviceType EE_Device, uByte * RAMPTR p_data, uWord addr, uByte dlen)
{
    GeneralReturnType rslt_wr;
    uByte i;
    uByte wr_data[EE_MAX_DATA_LEN];
    uWord crc;

    /* clear rslt */
    rslt_wr = (uWord)EE_ERR_OK;
   
    // check address and data length is ok
    if((addr + dlen + 1) > gwd_MaxDataAddr[EE_Device])
    {
        return (uWord)EE_ERR_DATA_ADDR;
    }

    if((dlen + 2) > EE_MAX_DATA_LEN)
    {
        return (uWord)EE_ERR_DATA_LENGTH;
    }
    
    (*NvmIf_Operation[EE_Device].WriteEnable)();
    
    //prepare data for calc crc
    for(i = 0; i < dlen; i++)
    {
        wr_data[i] = *(p_data + i);
    }

    // prepare crc value for save
    crc = EE_CalcCRC16AllByte(wr_data, addr, dlen);

    for(i = 0; i < 3; i++)
    {
        rslt_wr = EE_WriteBlocks3(EE_Device, p_data, addr, dlen, crc);

        if((uWord)EE_ERR_OK == rslt_wr)
        {
            break;
        }
    }

    (*NvmIf_Operation[EE_Device].WriteDisable)();

    return rslt_wr;
}

//************************************************************/
// Function:Save var to EE, 1 copy
// Trans para: point of data ,address, leng.
//************************************************************/
GeneralReturnType EE_SaveVar1(eEE_DeviceType EE_Device, uByte * RAMPTR p_data, uWord addr, uByte dlen)
{
    GeneralReturnType rslt_wr;
    uByte i;
    uByte wr_data[EE_MAX_DATA_LEN];
    uWord crc;

    /* clear rslt */
    rslt_wr = (uWord)EE_ERR_OK;

    // check address and data length is ok
    if((addr + dlen + 1) > gwd_MaxDataAddr[EE_Device]) ////
    {
        return (uWord)EE_ERR_DATA_ADDR;
    }

    if((dlen + 2) > EE_MAX_DATA_LEN)
    {
        return (uWord)EE_ERR_DATA_LENGTH;
    }

    (*NvmIf_Operation[EE_Device].WriteEnable)();

    //prepare data for calc crc
    for(i = 0; i < dlen; i++)
    {
        wr_data[i] = *(p_data + i);
    }
    
    // prepare crc value for save
    crc = EE_CalcCRC16AllByte(wr_data, addr, dlen);
          
    // begin to save
    for(i = 0; i < 3; i++)
    {
        rslt_wr = EE_WriteBlocks1(EE_Device, p_data, addr, dlen, crc);

        if((uWord)EE_ERR_OK == rslt_wr)
        {
            break;
        }
    }

    (*NvmIf_Operation[EE_Device].WriteDisable)();

    return rslt_wr;
}

//************************************************************/
// Function:Load var from EE, 3 copy
// Trans para: point of data ,address, leng.
//************************************************************/
GeneralReturnType EE_LoadVar3(eEE_DeviceType EE_Device, eSysConf_ParameterNOType ParameterNo, uByte * RAMPTR p_data, uWord addr, uByte dlen)
{
    GeneralReturnType rslt;
    uByte i;

    /* clear rlst */
    rslt = (uWord)EE_ERR_OK;

    if((addr + dlen + 1) > gwd_MaxDataAddr[EE_Device]) ////
    {
        return (uWord)EE_ERR_DATA_ADDR;
    }

    if((dlen + 2) > EE_MAX_DATA_LEN)
    {
        return (uWord)EE_ERR_DATA_LENGTH;
    }

    for(i = 0; i < 3; i++)
    {
        rslt = EE_ReadBlocks3(EE_Device, ParameterNo,p_data, addr, dlen);

        if((uWord)EE_ERR_OK == rslt)
        {
            break;
        }
    }

    return rslt;
}

//************************************************************/
// Function:Load var from EE, 1 copy
// Trans para: point of data ,address, leng.
//************************************************************/
GeneralReturnType EE_LoadVar1(eEE_DeviceType EE_Device, uByte * RAMPTR p_data, uWord addr, uByte dlen)
{
    GeneralReturnType rslt;
    uByte i;

    /* clear rslt */
    rslt = (uWord)EE_ERR_OK;

    if((addr + dlen + 1) > gwd_MaxDataAddr[EE_Device])
    {
        return (uWord)EE_ERR_DATA_ADDR;
    }

    if((dlen + 2) > EE_MAX_DATA_LEN)
    {
        return (uWord)EE_ERR_DATA_LENGTH;
    }

    for(i = 0; i < 3; i++)
    {
        rslt = EE_ReadBlocks1(EE_Device, p_data, addr, dlen);

        if((uWord)EE_ERR_OK == rslt)
        {
            break;
        }
    }

    return rslt;
}

//************************************************************/
// Function: initialization of ee
// Trans para: void
//************************************************************/
GeneralReturnType EE_InitDevice(eEE_DeviceType EE_Device)
{
    GeneralReturnType rslt[2];
    uByte dlen;
    uByte * RAMPTR p_data;
    uLWord data_ptr;
    sSysConf_ParameterAddrInRamType ParameterAddr;

    if(SysConf_GetParaHandle(EE_RECOGNIZE, &data_ptr, &dlen))
    {
        return (uWord)EE_ERR_GET_POINTER;
    }
    
    if(SysConf_GetParaAddrInEE(EE_RECOGNIZE, &ParameterAddr))
    {
        return (uWord)EE_ERR_GET_ADDR;
    }
    
    p_data = (uByte * RAMPTR)data_ptr;

    (*NvmIf_Operation[EE_Device].Init)();

    if(CPUEE == EE_Device) 
    {
        rslt[0] = EE_LoadVar3(EE_Device, EE_RECOGNIZE,p_data, ParameterAddr.TargetRelativeAddr.CpuEeAddr, dlen);
    }
    else                     
    {
        rslt[0] = EE_LoadVar1(EE_Device, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);
    }

    if( ((uWord)EE_ERR_OK == rslt[0]) && (REC_DATA == gwd_RecognizeData) )
    {
        rslt[1] = (uWord)EE_ERR_OK;
    }
    else
    {
        gwd_RecognizeData = REC_DATA;

        if(CPUEE == EE_Device) 
        {
            if((uWord)EE_ERR_OK == EE_SaveVar3(EE_Device, p_data, ParameterAddr.TargetRelativeAddr.CpuEeAddr, dlen))
            {
                rslt[0] = EE_LoadVar3(EE_Device, EE_RECOGNIZE, p_data, ParameterAddr.TargetRelativeAddr.CpuEeAddr, dlen);

                if( ((uWord)EE_ERR_OK == rslt[0]) && (REC_DATA == gwd_RecognizeData) )
                {
                    rslt[1] = (uWord)EE_ERR_OK;
                }
                else
                {
                    rslt[1] = (uWord)EE_ERR_INITEE;
                }
            }
            else
            {
                rslt[1] = (uWord)EE_ERR_INITEE;
            }
        }
        else
        {
        
           rslt[1] = (uWord)EE_ERR_INITEE;
           
           EE_SaveAddrNoToBuf(EE_RECOGNIZE,BAKUP_NULL);

        }
    }

    return rslt[1];
}


GeneralReturnType EE_SaveVar(eEE_DeviceType EE_Device, eSysConf_ParameterNOType ParameterNo)
{
    GeneralReturnType rslt;
    uByte dlen;
    uByte * RAMPTR p_data;
    uLWord data_ptr;
    eSysConf_ParameterNOType ah_addr_no;
    eSysConf_ParameterNOType ah_cnt_addr_no;
    sSysConf_ParameterAddrInRamType ParameterAddr;

    /*if(gby_PowerDownCnt > POWER_DOWN_CNT_500MS)
    {
        // forbid save when power off last for 500ms
        return (uWord)EE_ERR_SAVEVAR;
    }*/
    
    if(SysConf_GetParaHandle(ParameterNo, &data_ptr, &dlen))
    {
        return (uWord)EE_ERR_GET_POINTER;
    }
    
    if(SysConf_GetParaAddrInEE(ParameterNo, &ParameterAddr))
    {
        return (uWord)EE_ERR_GET_ADDR;
    }
    
    p_data = (uByte * RAMPTR)data_ptr;

    if(dlen > ParameterAddr.MaxParameterLength)
    {
        return (uWord)EE_ERR_DATA_LENGTH;
    }

    if(SYSTEMST_REMAIN_CAPACITY_BAK_G1 != ParameterNo)
    {
        // save normal parameter
        switch(EE_Device)
        {
        case CPUEE:
        {
            if(SYSCONF_BAKUP_ONE == ParameterAddr.CpuEEParaBakupNum)
            {
                rslt = EE_SaveVar1(CPUEE, p_data, ParameterAddr.TargetRelativeAddr.CpuEeAddr, dlen);
            }
            else
            {
                rslt = EE_SaveVar3(CPUEE, p_data, ParameterAddr.TargetRelativeAddr.CpuEeAddr, dlen);
            }
            break;
        }
        case EXTERNEE:
        {
            if(SYSCONF_BAKUP_ONE == ParameterAddr.ExtEEParaBakupNum)
            {
                if(EXTERNEE_NO_BUSY == gby_ExternEE_Status)
                {
                    rslt = EE_SaveVar1(EXTERNEE, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);

                    gby_ExternEE_Status = EXTERNEE_BUSY;
                    EE_TmCnt.cnt = 0;
                }
                else
                {
                 
                    EE_SaveAddrNoToBuf(ParameterNo, BAKUP_NULL);
                    rslt = (uWord)EE_ERR_OK;
                }
            }
            else
            {
               
                if(EXTERNEE_NO_BUSY == gby_ExternEE_Status)
                {
                     rslt = EE_SaveVar1(EXTERNEE, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);
                        
                     gby_ExternEE_Status = EXTERNEE_BUSY;
                     EE_TmCnt.cnt = 0;
                   
                     EE_SaveAddrNoToBuf(ParameterNo, BAKUP_B);
                     EE_SaveAddrNoToBuf(ParameterNo, BAKUP_C);
                }
                else
                {
                     EE_SaveAddrNoToBuf(ParameterNo, BAKUP_A);
                     EE_SaveAddrNoToBuf(ParameterNo, BAKUP_B);
                     EE_SaveAddrNoToBuf(ParameterNo, BAKUP_C);
                     rslt = (uWord)EE_ERR_OK;
                
                }
                 
            }
            break;
        }
        case BOTHEE:
        {
            if(SYSCONF_BAKUP_ONE == ParameterAddr.CpuEEParaBakupNum)
            {
                rslt = EE_SaveVar1(CPUEE, p_data, ParameterAddr.TargetRelativeAddr.CpuEeAddr, dlen);
            }
            else
            {
                if(SYSCONF_BAKUP_THREE == ParameterAddr.CpuEEParaBakupNum)
                {
                   rslt = EE_SaveVar3(CPUEE, p_data, ParameterAddr.TargetRelativeAddr.CpuEeAddr, dlen);
                }
                else
                {
                   rslt = (uWord)EE_ERR_OK;
                }
            }

            if((uWord)EE_ERR_OK == rslt)
            {
                if(EXTERNEE_NO_BUSY == gby_ExternEE_Status)
                {
                    if(SYSCONF_BAKUP_ONE == ParameterAddr.ExtEEParaBakupNum)
                    {
                       rslt = EE_SaveVar1(EXTERNEE, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);
                    
                       gby_ExternEE_Status = EXTERNEE_BUSY;
                       EE_TmCnt.cnt = 0;
                    }
                    else
                    {
                      if(SYSCONF_BAKUP_THREE == ParameterAddr.ExtEEParaBakupNum)
                      {
                        rslt = EE_SaveVar1(EXTERNEE, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);
                    
                       gby_ExternEE_Status = EXTERNEE_BUSY;
                       EE_TmCnt.cnt = 0;
                     
                       EE_SaveAddrNoToBuf(ParameterNo, BAKUP_B);
                       EE_SaveAddrNoToBuf(ParameterNo, BAKUP_C);
                      }
                      else
                      {
                         rslt = (uWord)EE_ERR_OK;
                      }
                    }
                }
                else
                {
                    if(SYSCONF_BAKUP_ONE == ParameterAddr.ExtEEParaBakupNum)
                    {
                        EE_SaveAddrNoToBuf(ParameterNo, BAKUP_NULL);
                        rslt = (uWord)EE_ERR_OK;
                    }
                    else
                    {
                        EE_SaveAddrNoToBuf(ParameterNo, BAKUP_A);
                        EE_SaveAddrNoToBuf(ParameterNo, BAKUP_B);
                        EE_SaveAddrNoToBuf(ParameterNo, BAKUP_C);
                        rslt = (uWord)EE_ERR_OK;
                    }
                       
                }
            } 
         
        
            break;
        }
        default:
        {
            rslt = (uWord)EE_ERR_SAVEVAR;
            break;
        }
        }
    }
    else
    {
        // save ah
        glwd_CntSaveAh++;
        gby_BlockSaveAh = glwd_CntSaveAh % 6;

        ah_addr_no = (eSysConf_ParameterNOType)((uWord)SYSTEMST_REMAIN_CAPACITY_BAK_G1 + gby_BlockSaveAh);
        ah_cnt_addr_no = (eSysConf_ParameterNOType)((uWord)SYSTEMST_REMAIN_AH_CNT_BAK_G1 + gby_BlockSaveAh);
        
        if(SysConf_GetParaHandle(ah_addr_no, &data_ptr, &dlen))
        {
            return (uWord)EE_ERR_GET_POINTER;
        }
    
        if(SysConf_GetParaAddrInEE(ah_addr_no, &ParameterAddr))
        {
            return (uWord)EE_ERR_GET_ADDR;
        }

        p_data = (uByte * RAMPTR)data_ptr;
        
        if(SYSCONF_BAKUP_THREE == ParameterAddr.CpuEEParaBakupNum)
        {
            rslt = EE_SaveVar3(CPUEE, p_data, ParameterAddr.TargetRelativeAddr.CpuEeAddr, dlen);
        }
        else
        {
            rslt = (uWord)EE_ERR_OK;
        }
        if((uWord)EE_ERR_OK == rslt)
        {
            if(EXTERNEE_NO_BUSY == gby_ExternEE_Status)
            {
                
                if(SYSCONF_BAKUP_ONE == ParameterAddr.ExtEEParaBakupNum)
                {
                    rslt = EE_SaveVar1(EXTERNEE, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);    
                    gby_ExternEE_Status = EXTERNEE_BUSY;
                    EE_TmCnt.cnt = 0;
                }
                else
                {
                     rslt = EE_SaveVar1(EXTERNEE, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);
                     gby_ExternEE_Status = EXTERNEE_BUSY;
                     EE_TmCnt.cnt = 0;
                     
                     EE_SaveAddrNoToBuf(ah_addr_no, BAKUP_B);
                     EE_SaveAddrNoToBuf(ah_addr_no, BAKUP_C);
                     
                }
            }
            else
            {
                 if(SYSCONF_BAKUP_ONE == ParameterAddr.ExtEEParaBakupNum)
                 {
                      EE_SaveAddrNoToBuf(ah_addr_no, BAKUP_NULL); 
                      rslt = (uWord)EE_ERR_OK;
                 }
                 else
                 {
                      EE_SaveAddrNoToBuf(ah_addr_no, BAKUP_A);
                      EE_SaveAddrNoToBuf(ah_addr_no, BAKUP_B);
                      EE_SaveAddrNoToBuf(ah_addr_no, BAKUP_C);
                      rslt = (uWord)EE_ERR_OK;
                 }
                
            }

            if(SysConf_GetParaHandle(ah_cnt_addr_no, &data_ptr, &dlen))
            {
                return (uWord)EE_ERR_GET_POINTER;
            }
            
            if(SysConf_GetParaAddrInEE(ah_cnt_addr_no, &ParameterAddr))
            {
                return (uWord)EE_ERR_GET_ADDR;
            }
            
            p_data = (uByte * RAMPTR)data_ptr;

            if(SYSCONF_BAKUP_THREE == ParameterAddr.CpuEEParaBakupNum)
            {
                rslt = EE_SaveVar3(CPUEE, p_data, ParameterAddr.TargetRelativeAddr.CpuEeAddr, dlen);
            }
            else
            {
                rslt = (uWord)EE_ERR_OK;
            }
            if((uWord)EE_ERR_OK == rslt)
            {
               if(SYSCONF_BAKUP_ONE == ParameterAddr.ExtEEParaBakupNum)
               {
                    EE_SaveAddrNoToBuf(ah_cnt_addr_no, BAKUP_NULL);
                    rslt = (uWord)EE_ERR_OK;
               }
               else
               {      
                    EE_SaveAddrNoToBuf(ah_cnt_addr_no, BAKUP_A);
                    EE_SaveAddrNoToBuf(ah_cnt_addr_no, BAKUP_B);
                    EE_SaveAddrNoToBuf(ah_cnt_addr_no, BAKUP_C);
                    rslt = (uWord)EE_ERR_OK;
               }
               
            }
        }
    }

    return rslt;
}

GeneralReturnType EE_SaveVarForCalib(eEE_DeviceType EE_Device, eSysConf_ParameterNOType ParameterNo)
{
    GeneralReturnType rslt[3];
    uByte i;
    uByte dlen;
    uByte * RAMPTR p_data;
    uLWord data_ptr;
    eSysConf_ParameterNOType ah_addr_no;
    eSysConf_ParameterNOType ah_cnt_addr_no;
    sSysConf_ParameterAddrInRamType ParameterAddr;

    if(SysConf_GetParaHandle(ParameterNo, &data_ptr, &dlen))
    {
        return (uWord)EE_ERR_GET_POINTER;
    }
    
    if(SysConf_GetParaAddrInEE(ParameterNo, &ParameterAddr))
    {
        return (uWord)EE_ERR_GET_ADDR;
    }
    
    p_data = (uByte * RAMPTR)data_ptr;

    if(dlen > ParameterAddr.MaxParameterLength)
    {
        return (uWord)EE_ERR_DATA_LENGTH;
    }

    if(EXTERNEE_BUSY == gby_ExternEE_Status)
    {
         ECU_DELAYMS(5);
         gby_ExternEE_Status = EXTERNEE_NO_BUSY;
    }
     
    if(SYSTEMST_REMAIN_CAPACITY_BAK_G1 != ParameterNo)
    {
        // save normal parameter
        switch(EE_Device)
        {
            case CPUEE:
            {
                if(SYSCONF_BAKUP_ONE == ParameterAddr.CpuEEParaBakupNum)
                {
                    rslt[0] = EE_SaveVar1(CPUEE, p_data, ParameterAddr.TargetRelativeAddr.CpuEeAddr, dlen);
                }
                else
                {
                    rslt[0] = EE_SaveVar3(CPUEE, p_data, ParameterAddr.TargetRelativeAddr.CpuEeAddr, dlen);
                }
                break;
            }
            case EXTERNEE:
            {
                if(SYSCONF_BAKUP_ONE == ParameterAddr.ExtEEParaBakupNum)
                {
                    rslt[0] = EE_SaveVar1(EXTERNEE, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);

                    ECU_DELAYMS(5);
                    
                    rslt[0] |= EE_LoadVar1(EXTERNEE, gby_ReadData, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);
                    
                    if((uWord)EE_ERR_OK == rslt[0])
                    {
                        for(i = 0; i < dlen; i++)
                        {
                            if(gby_ReadData[i] != (*(p_data + i)))
                            {
                                rslt[0] = (uWord)EE_ERR_SAVEVAR;
                            }
                        }
                    }      
                }
                else
                {
                    rslt[0] = EE_SaveVar1(EXTERNEE, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);
                    ECU_DELAYMS(5);
                    
                    rslt[0] |= EE_LoadVar1(EXTERNEE, gby_ReadData, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);
                    
                    if((uWord)EE_ERR_OK == rslt[0])
                    {
                        for(i = 0; i < dlen; i++)
                        {
                            if(gby_ReadData[i] != (*(p_data + i)))
                            {
                                rslt[0] = (uWord)EE_ERR_SAVEVAR;
                            }
                        }
                    } 
                   
                  
                    rslt[1] = EE_SaveVar1(EXTERNEE, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr + EE_JMP_ADDR, dlen);
                    ECU_DELAYMS(5);
                     
                    rslt[1] |= EE_LoadVar1(EXTERNEE, gby_ReadData, ParameterAddr.TargetRelativeAddr.ExtEeAddr + EE_JMP_ADDR, dlen);
                    
                    if((uWord)EE_ERR_OK == rslt[1])
                    {
                        for(i = 0; i < dlen; i++)
                        {
                            if(gby_ReadData[i] != (*(p_data + i)))
                            {
                                rslt[1] = (uWord)EE_ERR_SAVEVAR;
                            }
                        }
                    } 
                    
                    if(((uWord)EE_ERR_OK == rslt[1]) || ((uWord)EE_ERR_OK == rslt[0]))
                    {
                      rslt[2] = EE_SaveVar1(EXTERNEE, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr + 2 * EE_JMP_ADDR, dlen);
                      ECU_DELAYMS(5);
                      
                       rslt[2] |= EE_LoadVar1(EXTERNEE, gby_ReadData, ParameterAddr.TargetRelativeAddr.ExtEeAddr + 2 * EE_JMP_ADDR, dlen);
                    
                      if((uWord)EE_ERR_OK == rslt[2])
                      {
                          for(i = 0; i < dlen; i++)
                          {
                              if(gby_ReadData[i] != (*(p_data + i)))
                              {
                                  rslt[2] = (uWord)EE_ERR_SAVEVAR;
                              }
                          }
                      } 
                    }
                    if(((uWord)EE_ERR_OK == rslt[0]) || ((uWord)EE_ERR_OK == rslt[1]) || ((uWord)EE_ERR_OK == rslt[2]) )
                    {
                        rslt[0] = (uWord)EE_ERR_OK;
                    }
                    else
                    {
                        rslt[0] = (uWord)EE_ERR_SAVEVAR;
                    }
                  
                }
                break;
            }
            case BOTHEE:
            {
                if(SYSCONF_BAKUP_ONE == ParameterAddr.CpuEEParaBakupNum)
                {
                    rslt[0] = EE_SaveVar1(CPUEE, p_data, ParameterAddr.TargetRelativeAddr.CpuEeAddr, dlen);
                }
                else
                {
                
                    if(SYSCONF_BAKUP_THREE == ParameterAddr.CpuEEParaBakupNum)
                    {
                      rslt[0] = EE_SaveVar3(CPUEE, p_data, ParameterAddr.TargetRelativeAddr.CpuEeAddr, dlen);
                    }
                    else
                    {
                      rslt[0] = (uWord)EE_ERR_OK;
                    }
                }

                if((uWord)EE_ERR_OK == rslt[0])
                {
                    if(SYSCONF_BAKUP_ONE == ParameterAddr.ExtEEParaBakupNum)
                    {  
                        rslt[0] = EE_SaveVar1(EXTERNEE, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);
                        
                        ECU_DELAYMS(5);
                        
                        rslt[0] |= EE_LoadVar1(EXTERNEE, gby_ReadData, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);
                        
                        if((uWord)EE_ERR_OK == rslt[0])
                        {
                            for(i = 0; i < dlen; i++)
                            {
                                if(gby_ReadData[i] != (*(p_data + i)))
                                {
                                    rslt[0] = (uWord)EE_ERR_SAVEVAR;
                                }
                            }
                        }
                    }
                    else if(SYSCONF_BAKUP_THREE == ParameterAddr.ExtEEParaBakupNum)
                    {     
                        rslt[0] = EE_SaveVar1(EXTERNEE, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);
                        ECU_DELAYMS(5);
                    
                        rslt[0] |= EE_LoadVar1(EXTERNEE, gby_ReadData, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);
                    
                        if((uWord)EE_ERR_OK == rslt[0])
                        {
                            for(i = 0; i < dlen; i++)
                            {
                                if(gby_ReadData[i] != (*(p_data + i)))
                                {
                                    rslt[0] = (uWord)EE_ERR_SAVEVAR;
                                }
                            }
                        } 
                   
                  
                        rslt[1] = EE_SaveVar1(EXTERNEE, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr + EE_JMP_ADDR, dlen);
                        ECU_DELAYMS(5);
                         
                        rslt[1] |= EE_LoadVar1(EXTERNEE, gby_ReadData, ParameterAddr.TargetRelativeAddr.ExtEeAddr + EE_JMP_ADDR, dlen);
                        
                        if((uWord)EE_ERR_OK == rslt[1])
                        {
                            for(i = 0; i < dlen; i++)
                            {
                                if(gby_ReadData[i] != (*(p_data + i)))
                                {
                                    rslt[1] = (uWord)EE_ERR_SAVEVAR;
                                }
                            }
                        } 
                    
                        if(((uWord)EE_ERR_OK == rslt[1]) || ((uWord)EE_ERR_OK == rslt[0]))
                        {
                          rslt[2] = EE_SaveVar1(EXTERNEE, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr + 2 * EE_JMP_ADDR, dlen);
                          ECU_DELAYMS(5);
                          
                           rslt[2] |= EE_LoadVar1(EXTERNEE, gby_ReadData, ParameterAddr.TargetRelativeAddr.ExtEeAddr + 2 * EE_JMP_ADDR, dlen);
                        
                          if((uWord)EE_ERR_OK == rslt[2])
                          {
                              for(i = 0; i < dlen; i++)
                              {
                                  if(gby_ReadData[i] != (*(p_data + i)))
                                  {
                                      rslt[2] = (uWord)EE_ERR_SAVEVAR;
                                  }
                              }
                          } 
                        }
                        if(((uWord)EE_ERR_OK == rslt[0]) || ((uWord)EE_ERR_OK == rslt[1]) || ((uWord)EE_ERR_OK == rslt[2]) )
                        {
                            rslt[0] = (uWord)EE_ERR_OK;
                        }
                        else
                        {
                            rslt[0] = (uWord)EE_ERR_SAVEVAR;
                        }              
                   }                
                }
                break;
            }
            default:
            {
                rslt[0] = (uWord)EE_ERR_SAVEVAR;
                break;
            }
        }
    }
    else
    {
        // save ah
        glwd_CntSaveAh++;
        gby_BlockSaveAh = glwd_CntSaveAh % 6;

        ah_addr_no = (eSysConf_ParameterNOType)((uWord)SYSTEMST_REMAIN_CAPACITY_BAK_G1 + gby_BlockSaveAh);
        ah_cnt_addr_no = (eSysConf_ParameterNOType)((uWord)SYSTEMST_REMAIN_AH_CNT_BAK_G1 + gby_BlockSaveAh);
        
        if(SysConf_GetParaHandle(ah_addr_no, &data_ptr, &dlen))
        {
            return (uWord)EE_ERR_GET_POINTER;
        }
    
        if(SysConf_GetParaAddrInEE(ah_addr_no, &ParameterAddr))
        {
            return (uWord)EE_ERR_GET_ADDR;
        }

        p_data = (uByte * RAMPTR)data_ptr;

        if(SYSCONF_BAKUP_THREE == ParameterAddr.CpuEEParaBakupNum)
        {
           rslt[0] = EE_SaveVar3(CPUEE, p_data, ParameterAddr.TargetRelativeAddr.CpuEeAddr, dlen);
        }
        else
        {
           rslt[0] = (uWord)EE_ERR_OK;
        }

        if((uWord)EE_ERR_OK == rslt[0])
        {
            if(EXTERNEE_BUSY == gby_ExternEE_Status)
            {
                ECU_DELAYMS(5);
                gby_ExternEE_Status = EXTERNEE_NO_BUSY;
            }
            if(SYSCONF_BAKUP_ONE == ParameterAddr.ExtEEParaBakupNum)
            {
                rslt[0] = EE_SaveVar1(EXTERNEE, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);
                    
                ECU_DELAYMS(5);
                    
                rslt[0] |= EE_LoadVar1(EXTERNEE, gby_ReadData, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);
                
                if((uWord)EE_ERR_OK == rslt[0])
                {
                    for(i = 0; i < dlen; i++)
                    {
                        if(gby_ReadData[i] != (*(p_data + i)))
                        {
                            rslt[0] = (uWord)EE_ERR_SAVEVAR;
                        }
                    }
                } 
            }
            else 
            {
                rslt[0] = EE_SaveVar1(EXTERNEE, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);
                ECU_DELAYMS(5);
            
                rslt[0] |= EE_LoadVar1(EXTERNEE, gby_ReadData, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);
            
                if((uWord)EE_ERR_OK == rslt[0])
                {
                    for(i = 0; i < dlen; i++)
                    {
                        if(gby_ReadData[i] != (*(p_data + i)))
                        {
                            rslt[0] = (uWord)EE_ERR_SAVEVAR;
                        }
                    }
                } 
           
                rslt[1] = EE_SaveVar1(EXTERNEE, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr + EE_JMP_ADDR, dlen);
                ECU_DELAYMS(5);
                 
                rslt[1] |= EE_LoadVar1(EXTERNEE, gby_ReadData, ParameterAddr.TargetRelativeAddr.ExtEeAddr + EE_JMP_ADDR, dlen);
                
                if((uWord)EE_ERR_OK == rslt[1])
                {
                    for(i = 0; i < dlen; i++)
                    {
                        if(gby_ReadData[i] != (*(p_data + i)))
                        {
                            rslt[1] = (uWord)EE_ERR_SAVEVAR;
                        }
                    }
                } 
            
                if(((uWord)EE_ERR_OK == rslt[1]) || ((uWord)EE_ERR_OK == rslt[0]))
                {
                    rslt[2] = EE_SaveVar1(EXTERNEE, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr + 2 * EE_JMP_ADDR, dlen);
                    ECU_DELAYMS(5);
                  
                   rslt[2] |= EE_LoadVar1(EXTERNEE, gby_ReadData, ParameterAddr.TargetRelativeAddr.ExtEeAddr + 2 * EE_JMP_ADDR, dlen);
                
                  if((uWord)EE_ERR_OK == rslt[2])
                  {
                      for(i = 0; i < dlen; i++)
                      {
                          if(gby_ReadData[i] != (*(p_data + i)))
                          {
                              rslt[2] = (uWord)EE_ERR_SAVEVAR;
                          }
                      }
                  } 
                }
                if(((uWord)EE_ERR_OK == rslt[0]) || ((uWord)EE_ERR_OK == rslt[1]) || ((uWord)EE_ERR_OK == rslt[2]) )
                {
                    rslt[0] = (uWord)EE_ERR_OK;
                }
                else
                {
                    rslt[0] = (uWord)EE_ERR_SAVEVAR;
                } 
            }

            if(SysConf_GetParaHandle(ah_cnt_addr_no, &data_ptr, &dlen))
            {
                return (uWord)EE_ERR_GET_POINTER;
            }
            
            if(SysConf_GetParaAddrInEE(ah_cnt_addr_no, &ParameterAddr))
            {
                return (uWord)EE_ERR_GET_ADDR;
            }
            
            p_data = (uByte * RAMPTR)data_ptr;

            if(SYSCONF_BAKUP_THREE == ParameterAddr.CpuEEParaBakupNum)
            {
                rslt[1] = EE_SaveVar3(CPUEE, p_data, ParameterAddr.TargetRelativeAddr.CpuEeAddr, dlen);
            }
            else
            {
                rslt[1] = (uWord)EE_ERR_OK;
            }

            if((uWord)EE_ERR_OK == rslt[1])
            {
                if(SYSCONF_BAKUP_ONE == ParameterAddr.ExtEEParaBakupNum)
                {
                    rslt[1] = EE_SaveVar1(EXTERNEE, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);
                        
                    ECU_DELAYMS(5);
                        
                    rslt[1] |= EE_LoadVar1(EXTERNEE, gby_ReadData, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);
                    
                    if((uWord)EE_ERR_OK == rslt[1])
                    {
                        for(i = 0; i < dlen; i++)
                        {
                            if(gby_ReadData[i] != (*(p_data + i)))
                            {
                                rslt[1] = (uWord)EE_ERR_SAVEVAR;
                            }
                        }
                    }
                }
                else
                {
                    rslt[0] = EE_SaveVar1(EXTERNEE, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);
                    ECU_DELAYMS(5);
                
                    rslt[0] |= EE_LoadVar1(EXTERNEE, gby_ReadData, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);
                
                    if((uWord)EE_ERR_OK == rslt[0])
                    {
                        for(i = 0; i < dlen; i++)
                        {
                            if(gby_ReadData[i] != (*(p_data + i)))
                            {
                                rslt[0] = (uWord)EE_ERR_SAVEVAR;
                            }
                        }
                    } 
               
              
                    rslt[1] = EE_SaveVar1(EXTERNEE, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr + EE_JMP_ADDR, dlen);
                    ECU_DELAYMS(5);
                     
                    rslt[1] |= EE_LoadVar1(EXTERNEE, gby_ReadData, ParameterAddr.TargetRelativeAddr.ExtEeAddr + EE_JMP_ADDR, dlen);
                    
                    if((uWord)EE_ERR_OK == rslt[1])
                    {
                        for(i = 0; i < dlen; i++)
                        {
                            if(gby_ReadData[i] != (*(p_data + i)))
                            {
                                rslt[1] = (uWord)EE_ERR_SAVEVAR;
                            }
                        }
                    } 
                
                    if(((uWord)EE_ERR_OK == rslt[1]) || ((uWord)EE_ERR_OK == rslt[0]))
                    {
                      rslt[2] = EE_SaveVar1(EXTERNEE, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr + 2 * EE_JMP_ADDR, dlen);
                      ECU_DELAYMS(5);
                      
                       rslt[2] |= EE_LoadVar1(EXTERNEE, gby_ReadData, ParameterAddr.TargetRelativeAddr.ExtEeAddr + 2 * EE_JMP_ADDR, dlen);
                    
                      if((uWord)EE_ERR_OK == rslt[2])
                      {
                          for(i = 0; i < dlen; i++)
                          {
                              if(gby_ReadData[i] != (*(p_data + i)))
                              {
                                  rslt[2] = (uWord)EE_ERR_SAVEVAR;
                              }
                          }
                      } 
                    }
                    if(((uWord)EE_ERR_OK == rslt[0]) || ((uWord)EE_ERR_OK == rslt[1]) || ((uWord)EE_ERR_OK == rslt[2]) )
                    {
                        rslt[1] = (uWord)EE_ERR_OK;
                    }
                    else
                    {
                        rslt[1] = (uWord)EE_ERR_SAVEVAR;
                    } 
                    
                    
                }
                
                //glwd_CntSaveAh++;
            }
            
            rslt[0] |= rslt[1];
        }
    }

    return rslt[0];
}


GeneralReturnType EE_LoadVar(eSysConf_ParameterNOType ParameterNo)
{
    GeneralReturnType rslt[4];
    uByte dlen;
    uByte * RAMPTR p_data;
    uLWord data_ptr;
    eSysConf_ParameterNOType ah_addr_no;
    sSysConf_ParameterAddrInRamType ParameterAddr;

    if(SysConf_GetParaHandle(ParameterNo, &data_ptr, &dlen))
    {
        return (uWord)EE_ERR_GET_POINTER;
    }
    
    if(SysConf_GetParaAddrInEE(ParameterNo, &ParameterAddr))
    {
        return (uWord)EE_ERR_GET_ADDR;
    }
    
    p_data = (uByte * RAMPTR)data_ptr;

    rslt[0] = (uWord)EE_ERR_OK;

    if(SYSTEMST_REMAIN_CAPACITY_BAK_G1 != ParameterNo)
    {
        // load normal parameter
        if(SYSCONF_BAKUP_ONE == ParameterAddr.CpuEEParaBakupNum)
        {
            if(EXTERNEE_NO_BUSY == gby_ExternEE_Status)
            {
                rslt[1] = EE_LoadVar1(EXTERNEE, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);
            }
            else
            {
                rslt[1] = (uWord)EE_ERR_LOADVAR;
            }

            rslt[2] = EE_LoadVar1(CPUEE, p_data, ParameterAddr.TargetRelativeAddr.CpuEeAddr, dlen);

            if(((uWord)EE_ERR_OK == rslt[1]) && ((uWord)EE_ERR_OK != rslt[2]))
            {
                rslt[3] = EE_SaveVar1(CPUEE, p_data, ParameterAddr.TargetRelativeAddr.CpuEeAddr, dlen);
            }
            else if(((uWord)EE_ERR_OK != rslt[1]) && ((uWord)EE_ERR_OK == rslt[2]))
            {
                if(EXTERNEE_NO_BUSY == gby_ExternEE_Status)
                {                   
                    EE_SaveAddrNoToBuf(ParameterNo, BAKUP_NULL);
                }
            }
            else  if(((uWord)EE_ERR_OK == rslt[1]) && ((uWord)EE_ERR_OK == rslt[2]))
            {
                rslt[0] = (uWord)EE_ERR_OK;
            }
            else
            {
                rslt[0] = (uWord)EE_ERR_LOADVAR;
            }
        }
        else
        {
            if(EXTERNEE_NO_BUSY == gby_ExternEE_Status)
            {
                if(SYSCONF_BAKUP_ONE == ParameterAddr.ExtEEParaBakupNum)
                {
                   rslt[1] = EE_LoadVar1(EXTERNEE, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);
                }
                else
                {
                   rslt[1] = EE_LoadVar3(EXTERNEE, ParameterNo,p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);

                }
            }
            else
            {
                rslt[1] = (uWord)EE_ERR_LOADVAR;
            }

            if(SYSCONF_BAKUP_ZERO == ParameterAddr.CpuEEParaBakupNum)
            {
                if((uWord)EE_ERR_OK == rslt[1])
                {
                    rslt[0] = (uWord)EE_ERR_OK;
                }
                else
                {
                     rslt[0] = (uWord)EE_ERR_LOADVAR;
                }
            }
            else
            {
                rslt[2] = EE_LoadVar3(CPUEE, ParameterNo,p_data, ParameterAddr.TargetRelativeAddr.CpuEeAddr, dlen);

                if(((uWord)EE_ERR_OK == rslt[1]) && ((uWord)EE_ERR_OK != rslt[2]))
                {
                    rslt[3] = EE_SaveVar3(CPUEE, p_data, ParameterAddr.TargetRelativeAddr.CpuEeAddr, dlen);
                }
                else if(((uWord)EE_ERR_OK != rslt[1]) && ((uWord)EE_ERR_OK == rslt[2]))
                {
                    if(EXTERNEE_NO_BUSY == gby_ExternEE_Status)
                    {
                        if(SYSCONF_BAKUP_ONE == ParameterAddr.ExtEEParaBakupNum)
                        {
                             EE_SaveAddrNoToBuf(ParameterNo, BAKUP_NULL);
                        }
                        else
                        {
                            
                             EE_SaveAddrNoToBuf(ParameterNo, BAKUP_A);
                             EE_SaveAddrNoToBuf(ParameterNo, BAKUP_B);
                             EE_SaveAddrNoToBuf(ParameterNo, BAKUP_C);
                        }
                    }
                }
                else if(((uWord)EE_ERR_OK == rslt[1]) && ((uWord)EE_ERR_OK == rslt[2]))
                {
                    rslt[0] = (uWord)EE_ERR_OK;
                }
                else
                {
                    rslt[0] = (uWord)EE_ERR_LOADVAR;
                }
            }
        }
    }
    else
    {
        //load ah
        ah_addr_no = (eSysConf_ParameterNOType)((uWord)SYSTEMST_REMAIN_CAPACITY_BAK_G1 + gby_BlockSaveAh);

        if(SysConf_GetParaHandle(ah_addr_no, &data_ptr, &dlen))
        {
            return (uWord)EE_ERR_GET_POINTER;
        }
    
        if(SysConf_GetParaAddrInEE(ah_addr_no, &ParameterAddr))
        {
            return (uWord)EE_ERR_GET_ADDR;
        }
   
        p_data = (uByte * RAMPTR)data_ptr;

        if(EXTERNEE_NO_BUSY == gby_ExternEE_Status)
        {
            if(SYSCONF_BAKUP_ONE == ParameterAddr.ExtEEParaBakupNum)
            {
               rslt[1] = EE_LoadVar1(EXTERNEE, p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);
            }
            else
            {
               rslt[1] = EE_LoadVar3(EXTERNEE, ParameterNo,p_data, ParameterAddr.TargetRelativeAddr.ExtEeAddr, dlen);

            }
        }
        else
        {
            rslt[1] = (uWord)EE_ERR_LOADVAR;
        }
        if(SYSCONF_BAKUP_ZERO != ParameterAddr.CpuEEParaBakupNum)
        {
          rslt[2] = EE_LoadVar3(CPUEE, ParameterNo,p_data, ParameterAddr.TargetRelativeAddr.CpuEeAddr, dlen);

          if(((uWord)EE_ERR_OK == rslt[1]) && ((uWord)EE_ERR_OK != rslt[2]))
          {
              rslt[3] = EE_SaveVar3(CPUEE, p_data, ParameterAddr.TargetRelativeAddr.CpuEeAddr, dlen);
          }
          else if(((uWord)EE_ERR_OK != rslt[1]) && ((uWord)EE_ERR_OK == rslt[2]))
          {
              if(EXTERNEE_NO_BUSY == gby_ExternEE_Status)
              {
                  if(SYSCONF_BAKUP_ONE == ParameterAddr.ExtEEParaBakupNum)
                  {
                      EE_SaveAddrNoToBuf(ParameterNo, BAKUP_NULL);
                  }
                  else
                  {
                      EE_SaveAddrNoToBuf(ParameterNo, BAKUP_A);
                      EE_SaveAddrNoToBuf(ParameterNo, BAKUP_B);
                      EE_SaveAddrNoToBuf(ParameterNo, BAKUP_C);
                  }
              }
              rslt[0] = (uWord)EE_ERR_OK;
          }
          else if(((uWord)EE_ERR_OK == rslt[1]) && ((uWord)EE_ERR_OK == rslt[2]))
          {
              rslt[0] = (uWord)EE_ERR_OK;
          }
          else
          {
              rslt[0] = (uWord)EE_ERR_LOADVAR;
          }
        }
    }

    return rslt[0];
}

void EE_Dealy5msForSaveVar(void)
{
    ECU_DELAYMS(5);  
    gby_ExternEE_Status = EXTERNEE_NO_BUSY;
}

static void EE_SaveAddrNoToBuf(eSysConf_ParameterNOType ParameterNo, uWord OffSet)
{
    if(gwd_AddrBufCnt < MAX_ADDR_BUF_NUM)
    {
        gwd_AddrBuf[gwd_AddrBufCnt++] = (uWord)ParameterNo + (OffSet << 12);
    }
}

static GeneralReturnType EE_SaveBufToExternEE(void)
{
    GeneralReturnType rslt[2];
    uByte i, dlen;
    uByte * RAMPTR p_data;
    uLWord data_ptr;
    eSysConf_ParameterNOType ParameterNo;
    sSysConf_ParameterAddrInRamType ParameterAddr;
    uWord OffSet;

    if(gwd_AddrBufCnt > 0)
    {
        ParameterNo = (eSysConf_ParameterNOType)(gwd_AddrBuf[0] & 0x0FFF);
        OffSet = (gwd_AddrBuf[0] & 0xF000) >> 12;
        
        if(SysConf_GetParaHandle(ParameterNo, &data_ptr, &dlen))
        {
            return (uWord)EE_ERR_GET_POINTER;
        }
    
        if(SysConf_GetParaAddrInEE(ParameterNo, &ParameterAddr))
        {
            return (uWord)EE_ERR_GET_ADDR;
        }
        
        p_data = (uByte * RAMPTR)data_ptr;

        if(SYSCONF_BAKUP_ONE == ParameterAddr.CpuEEParaBakupNum)
        {
            rslt[0] = EE_LoadVar1(CPUEE, gby_ReadData, ParameterAddr.TargetRelativeAddr.CpuEeAddr, dlen);
        }
        else if(SYSCONF_BAKUP_THREE == ParameterAddr.CpuEEParaBakupNum)
        {
            rslt[0] = EE_LoadVar3(CPUEE, ParameterNo,gby_ReadData, ParameterAddr.TargetRelativeAddr.CpuEeAddr, dlen);
        }
        else
        {
            rslt[0] = (uWord)EE_ERR_OK;
        }
        
        if((uWord)EE_ERR_OK == rslt[0])
        {       
            if(SYSCONF_BAKUP_ZERO == ParameterAddr.CpuEEParaBakupNum)
            {
               rslt[1] = EE_SaveVar1(EXTERNEE, p_data, ((OffSet * EE_JMP_ADDR) + ParameterAddr.TargetRelativeAddr.ExtEeAddr), dlen);
            }
            else
            {
               rslt[1] = EE_SaveVar1(EXTERNEE, gby_ReadData, ((OffSet * EE_JMP_ADDR) + ParameterAddr.TargetRelativeAddr.ExtEeAddr), dlen);
            }
            gby_ExternEE_Status = EXTERNEE_BUSY;
           
            if((uWord)EE_ERR_OK == rslt[1]) 
            {
                for(i = 1; i < gwd_AddrBufCnt; i++)
                {
                    gwd_AddrBuf[i - 1] = gwd_AddrBuf[i];
                    gwd_AddrBuf[i] = 0;
                }
                gwd_AddrBufCnt--;
            }
    
        }
        else
        {
            return (uWord)EE_ERR_LOADVAR;
        }
    }
    
    return (uWord)EE_ERR_OK;
}

void EE_UpdateEeErrFlag(unsigned char flag)
{
    gby_EeErrFlag = flag;
}

void EE_Init(void)
{
    uByte i;
    GeneralReturnType rslt[2];
    uLWord cnt_saveah[5];

    gby_ExternEE_Status = EXTERNEE_NO_BUSY;
    
    if(0 == gby_EeErrFlag)
    {
        rslt[0] = Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
        rslt[0] = Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_INIT);
    }

    rslt[0] = SysConf_InitParaTable(EE_RECOGNIZE, (uLWord)(&gwd_RecognizeData), sizeof(gwd_RecognizeData));
    rslt[0] = SysConf_InitParaTable(SYSTEMST_REMAIN_AH_CNT_BAK_G1, (uLWord)(&glwd_CntSaveAh), sizeof(glwd_CntSaveAh));
    rslt[0] = SysConf_InitParaTable(SYSTEMST_REMAIN_AH_CNT_BAK_G2, (uLWord)(&glwd_CntSaveAh), sizeof(glwd_CntSaveAh));
    rslt[0] = SysConf_InitParaTable(SYSTEMST_REMAIN_AH_CNT_BAK_G3, (uLWord)(&glwd_CntSaveAh), sizeof(glwd_CntSaveAh));
    rslt[0] = SysConf_InitParaTable(SYSTEMST_REMAIN_AH_CNT_BAK_G4, (uLWord)(&glwd_CntSaveAh), sizeof(glwd_CntSaveAh));
    rslt[0] = SysConf_InitParaTable(SYSTEMST_REMAIN_AH_CNT_BAK_G5, (uLWord)(&glwd_CntSaveAh), sizeof(glwd_CntSaveAh));
    rslt[0] = SysConf_InitParaTable(SYSTEMST_REMAIN_AH_CNT_BAK_G6, (uLWord)(&glwd_CntSaveAh), sizeof(glwd_CntSaveAh));

    rslt[0] = EE_InitDevice(CPUEE);
    
    if(EE_ERR_OK != rslt[0])
    {
        (void)Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
        (void)Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_READ);
    }
    
    rslt[0] = EE_InitDevice(EXTERNEE);  
    if(EE_ERR_OK != rslt[0])
    {
        (void)Err_UpdateHwErrLevel(ERR_HW_EE_EXT, ERR_LEVEL_TWO);
        (void)Err_UpdateHwErrDetail(ERR_HW_EE_EXT, EE_HDERR_READ);
    }         

    EE_TmCnt.cnt = 0;
    glwd_CntSaveAh = 0;

    rslt[1] = EE_LoadVar(SYSTEMST_REMAIN_AH_CNT_BAK_G1);
    cnt_saveah[0] = glwd_CntSaveAh;
    rslt[1] |= EE_LoadVar(SYSTEMST_REMAIN_AH_CNT_BAK_G2);
    cnt_saveah[1] = glwd_CntSaveAh;
    rslt[1] |= EE_LoadVar(SYSTEMST_REMAIN_AH_CNT_BAK_G3);
    cnt_saveah[2] = glwd_CntSaveAh;
    rslt[1] |= EE_LoadVar(SYSTEMST_REMAIN_AH_CNT_BAK_G4);
    cnt_saveah[3] = glwd_CntSaveAh;
    rslt[1] |= EE_LoadVar(SYSTEMST_REMAIN_AH_CNT_BAK_G5);
    cnt_saveah[4] = glwd_CntSaveAh;
    rslt[1] |= EE_LoadVar(SYSTEMST_REMAIN_AH_CNT_BAK_G6);
    
    if(ERR_OK != rslt[1])
    {
        (void)Err_UpdateHwErrLevel(ERR_HW_EE_CPU, ERR_LEVEL_TWO);
        (void)Err_UpdateHwErrDetail(ERR_HW_EE_CPU, EE_HDERR_READ);
        (void)Err_UpdateHwErrLevel(ERR_HW_EE_EXT, ERR_LEVEL_TWO);
        (void)Err_UpdateHwErrDetail(ERR_HW_EE_EXT, EE_HDERR_READ);
    }
    
    for(i = 0; i < 5; i++)
    {
        if(glwd_CntSaveAh < cnt_saveah[i])
        {
            glwd_CntSaveAh = cnt_saveah[i];
        }
    }

    gby_BlockSaveAh = (uByte)(glwd_CntSaveAh % 6);
}

void EE_MainCountCbk(void)
{
    (EE_TmCnt.cnt < 255) ? (EE_TmCnt.cnt++) : (EE_TmCnt.cnt = 255);
}

void EE_MainCbk(void)
{
    GeneralReturnType rslt;
    
    if(EE_TmCnt.cnt >= TM_10MS)
    {
        EE_TmCnt.cnt = 0;
        gby_ExternEE_Status = EXTERNEE_NO_BUSY;
        rslt = EE_SaveBufToExternEE();
    }
}


uByte EE_GetEeStatus(void)
{
   return gby_ExternEE_Status;
}

void EE_UpdateEeStatus(uByte status)
{
    gby_ExternEE_Status = status;
}


#ifdef EE_DEBUG
void EE_Test(void)
{
    uByte i, j;
    uByte testdata_bak[32];
    uByte max_no;
    GeneralReturnType rslt[7];
    sSysConf_ParameterAddrInRamType ParameterAddr;
    
    max_no = 147;

    
    // prepare test data
    for(i = 0; i < 32; i++)
    {
        gby_TestData[i] = i;
        testdata_bak[i] = i;
        
        if(i < 7)
        {
            rslt[i] = 0;
        }
    }
    
    // init EE
    EE_Init();
    
    for(i = 0; i < max_no; i++)
    {
        // create EE table
        SysConf_GetParaAddrInEE(i, &ParameterAddr);
        SysConf_InitParaTable(i, (uLWord)(&gby_TestData), ParameterAddr.MaxParameterLength);
    }
    
    // begin test 'EE_SaveVar(BOTHEE, i)'
    for(i = 0; i < max_no; i++)
    {
        SysConf_GetParaAddrInEE(i, &ParameterAddr);
        
        // begin to save
        rslt[1] = EE_SaveVar(BOTHEE, i);
        EE_Dealy5msForSaveVar();
        rslt[2] = EE_SaveBufToExternEE();
        EE_Dealy5msForSaveVar();
        
        // load var
        rslt[3] = EE_LoadVar(i);
        
        if(0 != gwd_AddrBufCnt)
        {
            // load Extern EE error
            rslt[4] = (uWord)EE_ERR_LOADVAR;
        }
                
        // check read data whether ok
        for(j = 0; j < ParameterAddr.MaxParameterLength; j++)
        {
            if(gby_TestData[j] != testdata_bak[j])
            {
                rslt[5] = (uWord)EE_ERR_SAVEVAR;
            }
        }
        
        rslt[0] = rslt[5] | rslt[4] | rslt[3] | rslt[2] | rslt[1]; 
        if((uWord)EE_ERR_OK != rslt[0])
        {
            while(1);
        }
    }
    
    // begin test 'EE_SaveVar(CPUEE, i)' and  'EE_SaveVar(EXTERNEE, i)'
    for(i = 0; i < max_no; i++)
    {
        SysConf_GetParaAddrInEE(i, &ParameterAddr);
        // begin to save
        rslt[1] = EE_SaveVar(CPUEE, i);
        EE_Dealy5msForSaveVar();
        rslt[1] |= EE_SaveVar(EXTERNEE, i);
        EE_Dealy5msForSaveVar();
        rslt[2] = EE_SaveBufToExternEE();
        EE_Dealy5msForSaveVar();
        rslt[2] |= EE_SaveBufToExternEE();
        EE_Dealy5msForSaveVar();

        // load var
        rslt[3] = EE_LoadVar(i);
        
        if(0 != gwd_AddrBufCnt)
        {
            // load Extern EE error
            rslt[4] = (uWord)EE_ERR_LOADVAR;
        }
                
        // check read data whether ok
        for(j = 0; j < ParameterAddr.MaxParameterLength; j++)
        {
            if(gby_TestData[j] != testdata_bak[j])
            {
                rslt[5] = (uWord)EE_ERR_SAVEVAR;
            }
        }
        
        rslt[0] = rslt[5] | rslt[4] | rslt[3] | rslt[2] | rslt[1]; 
        if((uWord)EE_ERR_OK != rslt[0])
        {
            while(1);
        }
    }
    
    // begin test save many datas at the same time
    for(i = 0; i < 32; i++)
    {
        // begin to save
        rslt[1] |= EE_SaveVar(BOTHEE, i);
    }
    
    while(0 != gwd_AddrBufCnt)
    {
        rslt[2] |= EE_SaveBufToExternEE();
        EE_Dealy5msForSaveVar();
    }
    
    for(i = 32; i < 64; i++)
    {
        // begin to save
        rslt[1] |= EE_SaveVar(BOTHEE, i);
    }
    
    while(0 != gwd_AddrBufCnt)
    {
        rslt[2] |= EE_SaveBufToExternEE();
        EE_Dealy5msForSaveVar();
    }
    
    for(i = 64; i < 96; i++)
    {
        // begin to save
        rslt[1] |= EE_SaveVar(BOTHEE, i);
    }
    
    while(0 != gwd_AddrBufCnt)
    {
        rslt[2] |= EE_SaveBufToExternEE();
        EE_Dealy5msForSaveVar();
    }
    
    for(i = 96; i < 128; i++)
    {
        // begin to save
        rslt[1] |= EE_SaveVar(BOTHEE, i);
    }
    
    while(0 != gwd_AddrBufCnt)
    {
        rslt[2] |= EE_SaveBufToExternEE();
        EE_Dealy5msForSaveVar();
    }
    
    for(i = 128; i < max_no; i++)
    {
        // begin to save
        rslt[1] |= EE_SaveVar(BOTHEE, i);
    }
    
    while(0 != gwd_AddrBufCnt)
    {
        rslt[2] |= EE_SaveBufToExternEE();
        EE_Dealy5msForSaveVar();
    }

    for(i = 0; i < max_no; i++)
    {
        SysConf_GetParaAddrInEE(i, &ParameterAddr);
        // load var
        rslt[3] |= EE_LoadVar(i);
        
        if(0 != gwd_AddrBufCnt)
        {
            // load Extern EE error
            rslt[4] = (uWord)EE_ERR_LOADVAR;
        }
                
        // check read data whether ok
        for(j = 0; j < ParameterAddr.MaxParameterLength; j++)
        {
            if(gby_TestData[j] != testdata_bak[j])
            {
                rslt[5] = (uWord)EE_ERR_SAVEVAR;
            }
        }
        
        rslt[0] = rslt[5] | rslt[4] | rslt[3] | rslt[2] | rslt[1]; 
        if((uWord)EE_ERR_OK != rslt[0])
        {
            while(1);
        }
    }
}
#endif