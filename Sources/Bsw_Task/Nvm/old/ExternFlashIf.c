#include "ExternFlashIf.h"
//#include "Nvm_Lcfg.h"
//#include "ECU.h"

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

void (*ExternFlashIf_WriteCmd)(uByte cmd, uWord page, uWord baddr);

//************************************************************/
// Function: write a byte to 45db
// Trans para: void
//************************************************************/
void ExternFlashIf_WriteByte(uByte data)
{
    uByte i;
    uByte btmp;

    for(i = 0; i < 8; i++)
    {
        btmp = data << i;

        if((btmp & 0x80) == 0x80)
        {
            //(*ExternFlash_Operation.ExternFlash_Si.SetVal)();
            ExternFlash_Si_SetVal();
            
        }
        else
        {
            //(*ExternFlash_Operation.ExternFlash_Si.ClrVal)();
            ExternFlash_Si_ClrVal();
        }

        //(*ExternFlash_Operation.ExternFlash_Sck.SetVal)();
        ExternFlash_Sck_SetVal();
        //(*ExternFlash_Operation.ExternFlash_Sck.ClrVal)();
        ExternFlash_Sck_ClrVal();
    }
}

//************************************************************/
// Function: read a byte from 45db
// Trans para: void
//************************************************************/
uByte ExternFlashIf_ReadByte(void)
{
    uByte i;
    uByte btmp;

    btmp = 0;

    for(i = 8; i > 0; i--)
    {
        //(*ExternFlash_Operation.ExternFlash_Sck.SetVal)();
        ExternFlash_Sck_SetVal();

        btmp <<= 1;
        //if((*ExternFlash_Operation.ExternFlash_So.GetVal)())
        if(ExternFlash_So_GetVal())
        {
            btmp |= 0x01;
        }

        //(*ExternFlash_Operation.ExternFlash_Sck.ClrVal)();
        ExternFlash_Sck_ClrVal();
    }
    return btmp;
}

//************************************************************/
// Function: write command to 45db041
// Trans para: commad, page, address
//************************************************************/
void ExternFlashIf_WriteCmdSmallCap(uByte cmd, uWord page, uWord baddr)
{
    uLWord lwtmp;
    uByte btmp;

    lwtmp = cmd;
    lwtmp = lwtmp << 15;
    lwtmp = lwtmp + page;
    lwtmp = lwtmp << 9;
    lwtmp = lwtmp + baddr;

    btmp = (uByte)(lwtmp >> 24);
    ExternFlashIf_WriteByte(btmp);

    btmp = (uByte)(lwtmp >> 16);
    ExternFlashIf_WriteByte(btmp);

    btmp = (uByte)(lwtmp >> 8);
    ExternFlashIf_WriteByte(btmp);

    btmp = (uByte)lwtmp;
    ExternFlashIf_WriteByte(btmp);
}

//************************************************************/
// Function: write command to 45db161
// Trans para: commad, page, address
//************************************************************/
void ExternFlashIf_WriteCmdLargeCap(uByte cmd, uWord page, uWord baddr)
{
    uLWord lwtmp;
    uByte btmp;

    lwtmp = cmd;
    lwtmp = lwtmp << 14;
    lwtmp = lwtmp + page;
    lwtmp = lwtmp << 10;
    lwtmp = lwtmp + baddr;

    btmp = (uByte)(lwtmp >> 24);
    ExternFlashIf_WriteByte(btmp);

    btmp = (uByte)(lwtmp >> 16);
    ExternFlashIf_WriteByte(btmp);

    btmp = (uByte)(lwtmp >> 8);
    ExternFlashIf_WriteByte(btmp);

    btmp = (uByte)lwtmp;
    ExternFlashIf_WriteByte(btmp);
}

//************************************************************/
// Function: chip erase
// Trans para: delay time
//************************************************************/
void ExternFlashIf_ChipErase(void)
{
    //(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
    ExternFlash_Cs_ClrVal();

    ExternFlashIf_WriteByte(CHIP_ERASE_HH);
    ExternFlashIf_WriteByte(CHIP_ERASE_HL);
    ExternFlashIf_WriteByte(CHIP_ERASE_LH);
    ExternFlashIf_WriteByte(CHIP_ERASE_LL);

    //(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
    ExternFlash_Cs_SetVal();
}

//************************************************************/
// Function: page erase
// Trans para: page, delay time
//************************************************************/
void ExternFlashIf_PageErase(uWord page)
{
    //(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
    ExternFlash_Cs_ClrVal();
    (*ExternFlashIf_WriteCmd)(PAGE_ERASE, page, 0);
    //(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
    ExternFlash_Cs_SetVal();
}

//************************************************************/
// Function: block erase
// Trans para: block, delay time
//************************************************************/
void ExternFlashIf_BlockErase(uWord block)
{
    block <<= 3;

    //(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
    ExternFlash_Cs_ClrVal();
    (*ExternFlashIf_WriteCmd)(BLOCK_ERASE, block, 0);
    //(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
    ExternFlash_Cs_SetVal();
}

//************************************************************/
// Function: read buf
// Trans para: buffer no., data array, data num
//************************************************************/
void ExternFlashIf_ReadBuf(uWord buf_no, uByte * RAMPTR p_data, uWord dlen)
{
    uWord i;

    //(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
    ExternFlash_Cs_ClrVal();

    if(BUF1 == buf_no)
    {
        (*ExternFlashIf_WriteCmd)(BUF1RD, DONTC, 0);
        ExternFlashIf_WriteByte(DONTC);

        //(*ExternFlash_Operation.ExternFlash_Sck.SetVal)();
        ExternFlash_Sck_SetVal();
        //(*ExternFlash_Operation.ExternFlash_Sck.ClrVal)();
        ExternFlash_Sck_ClrVal();

        for(i = 0; i < dlen; i++)
        {
            *p_data++ = ExternFlashIf_ReadByte();
        }

        //(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
        ExternFlash_Cs_SetVal();
    }
    else if(BUF2 == buf_no)
    {
        (*ExternFlashIf_WriteCmd)(BUF2RD, DONTC, 0);
        ExternFlashIf_WriteByte(DONTC);

        //(*ExternFlash_Operation.ExternFlash_Sck.SetVal)();
        ExternFlash_Sck_SetVal();
        //(*ExternFlash_Operation.ExternFlash_Sck.ClrVal)();
        ExternFlash_Sck_ClrVal();

        for(i = 0; i < dlen; i++)
        {
            *p_data++ = ExternFlashIf_ReadByte();
        }

        //(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
        ExternFlash_Cs_SetVal();
    }
}

//************************************************************/
// Function: write buffer
// Trans para: buffer no., buffer address, data array, data num
//************************************************************/
void ExternFlashIf_WriteBuf(uByte buf_no, uWord buf_addr, uByte * RAMPTR p_data, uWord dlen)
{
    uWord i;

    if(BUF1 == buf_no)
    {
        /* write page and address */
        //(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
        ExternFlash_Cs_ClrVal();
        (*ExternFlashIf_WriteCmd)(BUF1WR, DONTC, buf_addr);

        for(i = 0; i < dlen; i++)
        {
            ExternFlashIf_WriteByte(*p_data++);
        }
        
        //(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
        ExternFlash_Cs_SetVal();
    }
    else if(BUF2 == buf_no)
    {
        /* write page and address */
        //(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
        ExternFlash_Cs_ClrVal();
        (*ExternFlashIf_WriteCmd)(BUF2WR, DONTC, buf_addr);

        for(i = 0; i < dlen; i++)
        {
            ExternFlashIf_WriteByte(*p_data++);
        }

        //(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
        ExternFlash_Cs_SetVal();
    }
}

//************************************************************/
// Function: read memory page
// Trans para: page, data array, data num
//************************************************************/
void ExternFlashIf_ReadPage(uWord page, uByte * RAMPTR p_data, uWord dlen)
{
    uWord i;

    //(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
    ExternFlash_Cs_ClrVal();
    (*ExternFlashIf_WriteCmd)(MMPRD, page, 0);

    //32 NC bit
    ExternFlashIf_WriteByte(DONTC);
    ExternFlashIf_WriteByte(DONTC);
    ExternFlashIf_WriteByte(DONTC);
    ExternFlashIf_WriteByte(DONTC);

    //(*ExternFlash_Operation.ExternFlash_Sck.SetVal)();
    ExternFlash_Sck_SetVal();
    //(*ExternFlash_Operation.ExternFlash_Sck.ClrVal)();
    ExternFlash_Sck_ClrVal();

    for(i = 0; i < dlen; i++)
    {
        *p_data++ = ExternFlashIf_ReadByte();
    }

    //(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
    ExternFlash_Cs_SetVal();
}

//************************************************************/
// Function: write main memory page through buffer
// Trans para: buffer no., buffer address, data array, data num
//************************************************************/
void ExternFlashIf_WritePage(uWord page, uByte * RAMPTR p_data, uWord dlen)
{
    uWord i;
    uByte buf_no;
    uWord buf_addr;

    buf_no = BUF1;
    buf_addr = 0;
    
    if(BUF1 == buf_no)
    {
        /* write page and start address */
        //(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
        ExternFlash_Cs_ClrVal();
        (*ExternFlashIf_WriteCmd)(MPTRB1, page, buf_addr);

        for(i = 0; i < dlen; i++)
        {
            ExternFlashIf_WriteByte(*p_data++);
        }

        //(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
        ExternFlash_Cs_SetVal();
    }
    else if(BUF2 == buf_no)
    {
        /* write page and start address */
        //(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
        ExternFlash_Cs_ClrVal();

        (*ExternFlashIf_WriteCmd)(MPTRB2, page, buf_addr);

        for(i = 0; i < dlen; i++)
        {
            ExternFlashIf_WriteByte(*p_data++);
        }

        //(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
        ExternFlash_Cs_SetVal();
    }
}

//************************************************************/
// Function: rewrite page
// Trans para: page, buffer no.
//************************************************************/
void ExternFlashIf_ReWritePage(uWord page, uByte buf_no)
{
    uByte cmd;

    switch(buf_no)
    {
    case BUF1:
    {
        cmd = RWM_BUF1;
        break;
    }

    case BUF2:
    {
        cmd = RWM_BUF2;
        break;
    }
    default:
    {
        cmd = RWM_BUF1;
        break;
    }
    }

    //(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
    ExternFlash_Cs_ClrVal();
    (*ExternFlashIf_WriteCmd)(cmd, page, 0);
    //(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
    ExternFlash_Cs_SetVal();
}

//************************************************************/
// Function: check memory page
// Trans para: page, data array, data num
//************************************************************/
uWord ExternFlashIf_CheckPage(uWord page, uByte * RAMPTR p_data, uWord dlen)
{
    uWord i;
    uByte data;
    uWord err_code;

    err_code = (uWord)EXTERNFLASHIF_ERR_OK;

    //(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
    ExternFlash_Cs_ClrVal();

    (*ExternFlashIf_WriteCmd)(MMPRD, page, 0);

    //32 NC bit
    ExternFlashIf_WriteByte(DONTC);
    ExternFlashIf_WriteByte(DONTC);
    ExternFlashIf_WriteByte(DONTC);
    ExternFlashIf_WriteByte(DONTC);

    //(*ExternFlash_Operation.ExternFlash_Sck.SetVal)();
    ExternFlash_Sck_SetVal();
    //(*ExternFlash_Operation.ExternFlash_Sck.ClrVal)();
    ExternFlash_Sck_ClrVal();

    for(i = 0; i < dlen; i++)
    {
        data = ExternFlashIf_ReadByte();
        if(data != (*p_data++))
        {
            err_code = (uWord)EXTERNFLASHIF_ERR_CHECK_FAIL;
            break;
        }
    }

    //(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
    ExternFlash_Cs_SetVal();

    return err_code;
}

//************************************************************/
// Function: buffer to main memory page
// Trans para: buffer no., main memory page
//************************************************************/
void ExternFlashIf_BufToMainMemory(uByte buf_no, uWord page)
{
    if(BUF1 == buf_no)
    {
        //(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
        ExternFlash_Cs_ClrVal();
        (*ExternFlashIf_WriteCmd)(BUF1MP, page, DONTC);
        //(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
        ExternFlash_Cs_SetVal();

    }
    else if(BUF2 == buf_no)
    {
        //(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
        ExternFlash_Cs_ClrVal();
        (*ExternFlashIf_WriteCmd)(BUF2MP, page, DONTC);
       // (*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
       ExternFlash_Cs_SetVal();
    }
}

//************************************************************/
// Function: read status register
// Trans para: void
//************************************************************/
uByte ExternFlashIf_ReadStatusReg(void)
{
    uByte btmp;

    //(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
    ExternFlash_Cs_ClrVal();
    ExternFlashIf_WriteByte(STREGRD);

    //(*ExternFlash_Operation.ExternFlash_Sck.SetVal)();
    ExternFlash_Sck_SetVal();
    //(*ExternFlash_Operation.ExternFlash_Sck.ClrVal)();
    ExternFlash_Sck_ClrVal();

    btmp = ExternFlashIf_ReadByte();

    //(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
    ExternFlash_Cs_SetVal();

    return btmp;
}

//************************************************************/
// Function: initial extern flash hardware
// Trans para: void
//************************************************************/
uByte ExternFlashIf_Init(void)
{
    uByte status_reg;

    //(*ExternFlash_Operation.ExternFlash_Cs.SetOutput)();
    ExternFlash_Cs_SetOutput();
    //(*ExternFlash_Operation.ExternFlash_Sck.SetOutput)();
    ExternFlash_Sck_SetOutput();
    //(*ExternFlash_Operation.ExternFlash_Si.SetOutput)();
    ExternFlash_Si_SetOutput();
    //(*ExternFlash_Operation.ExternFlash_Rst.SetOutput)();
    ExternFlash_Rst_SetOutput();
    //(*ExternFlash_Operation.ExternFlash_So.SetInput)();
    ExternFlash_So_SetInput();
    //(*ExternFlash_Operation.ExternFlash_Rst.SetVal)();
    ExternFlash_Rst_SetVal();

    /* use spi mode0 */
    //(*ExternFlash_Operation.ExternFlash_Sck.ClrVal)();
    ExternFlash_Sck_ClrVal();
    //(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
    ExternFlash_Cs_SetVal();

    status_reg = ExternFlashIf_ReadStatusReg();

    if(FLASH_LAGRGECAP == (status_reg & FLASH_LAGRGECAP))
    {
        ExternFlashIf_WriteCmd = ExternFlashIf_WriteCmdLargeCap;
    }
    else
    {
        ExternFlashIf_WriteCmd = ExternFlashIf_WriteCmdSmallCap;
    }

    return status_reg;
}

#pragma DATA_SEG DEFAULT
