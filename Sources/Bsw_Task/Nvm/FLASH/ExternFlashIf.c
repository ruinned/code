#if 1

#include "ExternFlashIf.h"
#include "Nvm_Lcfg.h"
#include "ECU.h"

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
static void ExternFlash_Delay(void)
{
    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop; 
}


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
            (*ExternFlash_Operation.ExternFlash_Si.SetVal)();
        }
        else
        {
            (*ExternFlash_Operation.ExternFlash_Si.ClrVal)();
        }

        (*ExternFlash_Operation.ExternFlash_Sck.SetVal)();
        (*ExternFlash_Operation.ExternFlash_Sck.ClrVal)();
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
        (*ExternFlash_Operation.ExternFlash_Sck.SetVal)();
        ExternFlash_Delay();
        btmp <<= 1;
        if((*ExternFlash_Operation.ExternFlash_So.GetVal)())
        {
            btmp |= 0x01;
        }

        (*ExternFlash_Operation.ExternFlash_Sck.ClrVal)();
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
    lwtmp = lwtmp << 9;		/* buffer addr Bit0~Bit8 */
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
    lwtmp = lwtmp << 10;		/* buffer addr Bit0~Bit9 */
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
    (*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();

    ExternFlashIf_WriteByte(CHIP_ERASE_HH);
    ExternFlashIf_WriteByte(CHIP_ERASE_HL);
    ExternFlashIf_WriteByte(CHIP_ERASE_LH);
    ExternFlashIf_WriteByte(CHIP_ERASE_LL);

    (*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
}

//************************************************************/
// Function: page erase
// Trans para: page, delay time
//************************************************************/
void ExternFlashIf_PageErase(uWord page)
{
    (*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
    (*ExternFlashIf_WriteCmd)(PAGE_ERASE, page, 0);
    (*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
}

//************************************************************/
// Function: block erase
// Trans para: block, delay time
//************************************************************/
void ExternFlashIf_BlockErase(uWord block)
{
    block <<= 3;

    (*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
    (*ExternFlashIf_WriteCmd)(BLOCK_ERASE, block, 0);
    (*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
}

//************************************************************/
// Function: read buf
// Trans para: buffer no., data array, data num
//************************************************************/
void ExternFlashIf_ReadBuf(uWord buf_no, uByte * RAMPTR p_data, uWord dlen)
{
    uWord i;

    (*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();

    if(BUF1 == buf_no)
    {
        (*ExternFlashIf_WriteCmd)(BUF1RD, DONTC, 0);
        ExternFlashIf_WriteByte(DONTC);

        (*ExternFlash_Operation.ExternFlash_Sck.SetVal)();
        (*ExternFlash_Operation.ExternFlash_Sck.ClrVal)();

        for(i = 0; i < dlen; i++)
        {
            *p_data++ = ExternFlashIf_ReadByte();
        }

        (*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
    }
    else if(BUF2 == buf_no)
    {
        (*ExternFlashIf_WriteCmd)(BUF2RD, DONTC, 0);
        ExternFlashIf_WriteByte(DONTC);

        (*ExternFlash_Operation.ExternFlash_Sck.SetVal)();
        (*ExternFlash_Operation.ExternFlash_Sck.ClrVal)();

        for(i = 0; i < dlen; i++)
        {
            *p_data++ = ExternFlashIf_ReadByte();
        }

        (*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
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
        (*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
        (*ExternFlashIf_WriteCmd)(BUF1WR, DONTC, buf_addr);

        for(i = 0; i < dlen; i++)
        {
            ExternFlashIf_WriteByte(*p_data++);
        }

        (*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
    }
    else if(BUF2 == buf_no)
    {
        /* write page and address */
        (*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
        (*ExternFlashIf_WriteCmd)(BUF2WR, DONTC, buf_addr);

        for(i = 0; i < dlen; i++)
        {
            ExternFlashIf_WriteByte(*p_data++);
        }

        (*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
    }
}

//************************************************************/
// Function: read memory page
// Trans para: page, data array, data num
//************************************************************/
void ExternFlashIf_ReadPage(uWord page, uByte * RAMPTR p_data, uWord dlen)
{
    uWord i;

    (*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
    (*ExternFlashIf_WriteCmd)(MMPRD, page, 0);

    //32 NC bit
    ExternFlashIf_WriteByte(DONTC);
    ExternFlashIf_WriteByte(DONTC);
    ExternFlashIf_WriteByte(DONTC);
    ExternFlashIf_WriteByte(DONTC);

    (*ExternFlash_Operation.ExternFlash_Sck.SetVal)();
    (*ExternFlash_Operation.ExternFlash_Sck.ClrVal)();

    for(i = 0; i < dlen; i++)
    {
        *p_data++ = ExternFlashIf_ReadByte();
    }

    (*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
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
        (*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
        (*ExternFlashIf_WriteCmd)(MPTRB1, page, buf_addr);

        for(i = 0; i < dlen; i++)
        {
            ExternFlashIf_WriteByte(*p_data++);
        }

        (*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
    }
    else if(BUF2 == buf_no)
    {
        /* write page and start address */
        (*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();

        (*ExternFlashIf_WriteCmd)(MPTRB2, page, buf_addr);

        for(i = 0; i < dlen; i++)
        {
            ExternFlashIf_WriteByte(*p_data++);
        }

        (*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
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

    (*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
    (*ExternFlashIf_WriteCmd)(cmd, page, 0);
    (*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
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

    (*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();

    (*ExternFlashIf_WriteCmd)(MMPRD, page, 0);

    //32 NC bit
    ExternFlashIf_WriteByte(DONTC);
    ExternFlashIf_WriteByte(DONTC);
    ExternFlashIf_WriteByte(DONTC);
    ExternFlashIf_WriteByte(DONTC);

    (*ExternFlash_Operation.ExternFlash_Sck.SetVal)();
    (*ExternFlash_Operation.ExternFlash_Sck.ClrVal)();

    for(i = 0; i < dlen; i++)
    {
        data = ExternFlashIf_ReadByte();
        if(data != (*p_data++))
        {
            err_code = (uWord)EXTERNFLASHIF_ERR_CHECK_FAIL;
            break;
        }
    }

    (*ExternFlash_Operation.ExternFlash_Cs.SetVal)();

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
        (*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
        (*ExternFlashIf_WriteCmd)(BUF1MP, page, DONTC);
        (*ExternFlash_Operation.ExternFlash_Cs.SetVal)();

    }
    else if(BUF2 == buf_no)
    {
        (*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
        (*ExternFlashIf_WriteCmd)(BUF2MP, page, DONTC);
        (*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
    }
}

//************************************************************/
// Function: read status register
// Trans para: void
//************************************************************/
uByte ExternFlashIf_ReadStatusReg(void)
{
    uByte btmp;

    (*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
    ExternFlashIf_WriteByte(STREGRD);

    (*ExternFlash_Operation.ExternFlash_Sck.SetVal)();
    (*ExternFlash_Operation.ExternFlash_Sck.ClrVal)();

    btmp = ExternFlashIf_ReadByte();

    (*ExternFlash_Operation.ExternFlash_Cs.SetVal)();

    return btmp;
}
//************************************************************/
// Function: read memory page+buffer addr
// Trans para: page, addr, data array, data num
//************************************************************/
void ExternFlashIf_ReadPageAddr(uWord page, uWord buff_addr, uByte * RAMPTR p_data, uWord dlen)
{
    uWord i;

    (*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
    (*ExternFlashIf_WriteCmd)(MMPRD, page, buff_addr);

    /* 32 NC bit	*/
    ExternFlashIf_WriteByte(DONTC);
    ExternFlashIf_WriteByte(DONTC);
    ExternFlashIf_WriteByte(DONTC);
    ExternFlashIf_WriteByte(DONTC);

    (*ExternFlash_Operation.ExternFlash_Sck.SetVal)();
    (*ExternFlash_Operation.ExternFlash_Sck.ClrVal)();

    for(i = 0; i < dlen; i++)
    {
        *p_data++ = ExternFlashIf_ReadByte();
    }

    (*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
}

//************************************************************/
// Function: read memory data from data_addr to data_addr+dlen
// Trans para: data_addr, data array, data num
//************************************************************/
uWord ExternFlashIf_ReadData(uLWord data_addr, uByte * RAMPTR p_data, uWord dlen)
{
	uWord i;
	uWord err_code;
	uWord page_addr;
	uWord buf_addr;
	uWord page_size;
	uWord last_page_len;
	uLWord max_addr;

	err_code = (uWord)EXTERNFLASHIF_ERR_OK;

#if defined(AT45DB_LAGRGE_PAGE)
	page_size = FLASH_MEMORY_LAGRGEPAGE_SIZE;
	max_addr = FLASH_MEMORY_LAGRGEMAX_ADDR;
#elif defined(AT45DB_MIDDLE_PAGE)
	page_size = FLASH_MEMORY_MIDDLEPAGE_SIZE;		
	max_addr = FLASH_MEMORY_MIDDLEMAX_ADDR;
#elif defined(AT45DB_SMALL_PAGE)
	page_size = FLASH_MEMORY_SMALLPAGE_SIZE;		
	max_addr = FLASH_MEMORY_SMALLMAX_ADDR;
#endif

	/* check the addr	*/
	if((data_addr >= max_addr)||(data_addr+dlen >= max_addr))
	{
		return (uWord)EXTERNFLASHIF_ERR_MEMORY_FAIL;
	}
	if((p_data == NULL)||(dlen == 0))
	{
		return (uWord)EXTERNFLASHIF_ERR_DATA_EMPTY;
	}

	page_addr = data_addr/page_size;
	buf_addr = data_addr%page_size;
	last_page_len = (dlen - (page_size - buf_addr))%page_size;

	if(buf_addr + dlen > page_size)
	{
		/* read 1th  page data */
		ExternFlashIf_ReadPageAddr(page_addr, buf_addr, p_data, page_size - buf_addr);

		/* read 2th~N-1 page data */
		for(i = 0; i < (dlen - (page_size - buf_addr) - last_page_len)/page_size; i++)
		{
			ExternFlashIf_ReadPage(page_addr + i + 1, &p_data[page_size - buf_addr +i*page_size], page_size);
		}

		/* read the N(last) page data	*/
		if(last_page_len)
		{
			ExternFlashIf_ReadPage(page_addr + i + 1, &p_data[page_size - buf_addr +i*page_size], last_page_len);
		}
	}
	else
	{
		/* only one page */
		ExternFlashIf_ReadPageAddr(page_addr, buf_addr, p_data, dlen);
	}

	return err_code;

}

//************************************************************/
// Function: write data to memory from data_addr to data_addr+dlen
// Trans para: data_addr, data array, data num
//************************************************************/
void ExternFlashIf_WritePageAddr(uWord page, uWord buff_addr, uByte * RAMPTR p_data, uWord dlen)
{
	uByte status;
	uByte buf_no;
	
	buf_no = BUF1;

	/* read the page data from main memory to the buf */
	if(BUF1 == buf_no)
	{
		(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
		(*ExternFlashIf_WriteCmd)(MTOB1T, page, DONTC);
		(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
	}
	else if(BUF2 == buf_no)
	{
		(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
		(*ExternFlashIf_WriteCmd)(MTOB2T, page, DONTC);
		(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
	}
	
	/* wait the status ready */
	status = ExternFlashIf_ReadStatusReg();	
	while((status&0x80) == 0)
	{
		status = ExternFlashIf_ReadStatusReg();	
	}

	/* write the data to buf */
	ExternFlashIf_WriteBuf(buf_no, buff_addr, p_data, dlen);

	/* buf data write to the main memory */
	ExternFlashIf_BufToMainMemory(buf_no, page);

	/* wait the status ready */
	status = ExternFlashIf_ReadStatusReg();	
	while((status&0x80) == 0)
	{
		status = ExternFlashIf_ReadStatusReg();	
	}

}

//************************************************************/
// Function: write memory page+buffer addr
// Trans para: page, addr, data array, data num
//************************************************************/
uWord ExternFlashIf_WriteData(uLWord data_addr, uByte * RAMPTR p_data, uWord dlen)
{ 
	uWord i;
	uWord err_code;
	uWord page_addr;
	uWord buf_addr;
	uWord page_size;
	uWord last_page_len;
	uLWord max_addr;

	err_code = (uWord)EXTERNFLASHIF_ERR_OK;

#if defined(AT45DB_LAGRGE_PAGE)
	page_size = FLASH_MEMORY_LAGRGEPAGE_SIZE;
	max_addr = FLASH_MEMORY_LAGRGEMAX_ADDR;
#elif defined(AT45DB_MIDDLE_PAGE)
	page_size = FLASH_MEMORY_MIDDLEPAGE_SIZE;		
	max_addr = FLASH_MEMORY_MIDDLEMAX_ADDR;
#elif defined(AT45DB_SMALL_PAGE)
	page_size = FLASH_MEMORY_SMALLPAGE_SIZE;		
	max_addr = FLASH_MEMORY_SMALLMAX_ADDR;
#endif

	/* check the addr */
	if((data_addr >= max_addr)||(data_addr+dlen >= max_addr))
	{
		return (uWord)EXTERNFLASHIF_ERR_MEMORY_FAIL;
	}
	if((p_data == NULL)||(dlen == 0))
	{
		return (uWord)EXTERNFLASHIF_ERR_DATA_EMPTY;
	}

	page_addr = data_addr/page_size;
	buf_addr = data_addr%page_size;
	last_page_len = (dlen - (page_size - buf_addr))%page_size;

	if(buf_addr + dlen > page_size)
	{
		/* write 1th  page data */
		ExternFlashIf_WritePageAddr(page_addr, buf_addr, p_data, page_size - buf_addr);

		/* write 2th~N-1 page data */
		for(i = 0; i < (dlen - (page_size - buf_addr) - last_page_len)/page_size; i++)
		{
			ExternFlashIf_WritePageAddr(page_addr + i + 1, 0, &p_data[page_size - buf_addr +i*page_size], page_size);
		}

		/* write the N(last) page data */
		if(last_page_len)
		{
			ExternFlashIf_WritePageAddr(page_addr + i + 1, 0, &p_data[page_size - buf_addr +i*page_size], last_page_len);
		}
	}
	else
	{
		/* only one page */
		ExternFlashIf_WritePageAddr(page_addr, buf_addr, p_data, dlen);
	}

	return err_code;

}

//************************************************************/
// Function: initial extern flash hardware
// Trans para: void
//************************************************************/
uByte ExternFlashIf_Init(void)
{
    uByte status_reg;

    (*ExternFlash_Operation.ExternFlash_Cs.SetOutput)();
    (*ExternFlash_Operation.ExternFlash_Sck.SetOutput)();
    (*ExternFlash_Operation.ExternFlash_Si.SetOutput)();
    (*ExternFlash_Operation.ExternFlash_Rst.SetOutput)();
    (*ExternFlash_Operation.ExternFlash_So.SetInput)();
    (*ExternFlash_Operation.ExternFlash_Rst.SetVal)();

    /* use spi mode0 */
    (*ExternFlash_Operation.ExternFlash_Sck.ClrVal)();
    (*ExternFlash_Operation.ExternFlash_Cs.SetVal)();

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

uByte ExternFlashIf_InIdle(void)
{
	uByte status = 0;
	uByte ret = 1;
	
	status = ExternFlashIf_ReadStatusReg();	
	if((status&0x80) == 0)
	{
		ret = 0;	
	}
	
	
	return ret;
}

#pragma DATA_SEG DEFAULT


#else
#include "ExternFlashIf.h"
#include "Nvm_Lcfg.h"
#include "Ecu.h"
#include "ExternFlash.h"

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

uLWord ExternFlashIf_Memory_Size;

void ExternFlashIf_WriteByte(uByte data)
{
    uByte i;
    uByte btmp;

    for(i = 0; i < 8; i++)
    {
        btmp = data << i;

        if((btmp & 0x80) == 0x80)
        {
            (*ExternFlash_Operation.ExternFlash_Si.SetVal)();
        }
        else
        {
            (*ExternFlash_Operation.ExternFlash_Si.ClrVal)();
        }

        (*ExternFlash_Operation.ExternFlash_Sck.SetVal)();
        (*ExternFlash_Operation.ExternFlash_Sck.ClrVal)();
    }
}

static void ExternFlash_Delay(void)
{
    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop; 
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
        (*ExternFlash_Operation.ExternFlash_Sck.SetVal)();
        ExternFlash_Delay();
        btmp <<= 1;
        if((*ExternFlash_Operation.ExternFlash_So.GetVal)())
        {
            btmp |= 0x01;
        }

        (*ExternFlash_Operation.ExternFlash_Sck.ClrVal)();
    }
    return btmp;
}

//************************************************************/
// Function: read the status from flash
// Trans para: void
//************************************************************/
uByte ExternFlashIf_ReadStatus(void)
{
	uByte status;
	
	(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
	
	ExternFlashIf_WriteByte(FLASH_CMD_READ_STATUS);
	status = ExternFlashIf_ReadByte();
	
	(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
	return status;
}

//************************************************************/
// Function: write some cycle signal to flash
// Trans para: len
//************************************************************/
void ExternFlashIf_Dummy(uWord len)
{
	uWord i;
	
	for(i = 0; i < len; i++)
	{
		(*ExternFlash_Operation.ExternFlash_Sck.SetVal)();
		(*ExternFlash_Operation.ExternFlash_Sck.ClrVal)();
	}
}

//************************************************************/
// Function: write enable write flag to flash
// Trans para: void
//************************************************************/
void ExternFlashIf_WriteEnable(void)
{
	(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
	
	ExternFlashIf_WriteByte(FLASH_CMD_WRITE_ENABLE);
	
	(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
}

//************************************************************/
// Function: wait the flash WIP already
// Trans para: void
//************************************************************/
void ExternFlashIf_WaitWipComplete(void)
{
	uByte status;
	
	status = ExternFlashIf_ReadStatus();
	while((status&FLASH_WIP_STATUS) == FLASH_WIP_STATUS)
	{
		status = ExternFlashIf_ReadStatus();
	}
}

uByte ExternFlashIf_InIdle(void)
{
	uByte status = 0;
	uByte ret = 0;
	
	status = ExternFlashIf_ReadStatus();
	if(!(status&(FLASH_WIP_STATUS|FLASH_WEL_STATUS)))
	{
		ret = 1;
	}
	
	return ret;
}

//************************************************************/
// Function: wait the flash WEL already
// Trans para: void
//************************************************************/
void ExternFlashIf_WaitWelComplete(void)
{
	uByte status;
	
	status = ExternFlashIf_ReadStatus();
	while((status&FLASH_WEL_STATUS) == FLASH_WEL_STATUS)
	{
		status = ExternFlashIf_ReadStatus();
	}
}

uByte ExternFlashIf_IsWelComplete(void)
{
	uByte status;
	uByte ret = 0;
	
	status = ExternFlashIf_ReadStatus();
	if((status&FLASH_WEL_STATUS) != FLASH_WEL_STATUS)
	{
		ret = 1;
	}
	
	return ret;
}

//************************************************************/
// Function: read the flash product JEDEC ID
// Trans para: void
//************************************************************/
uLWord ExternFlashIf_ReadProductJedecID(void)
{
	uLWord ret = 0;
	
	(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
	
	ExternFlashIf_WriteByte(FLASH_CMD_READ_PRODUCT_ID_JEDEC);
	ret = ((uLWord)ExternFlashIf_ReadByte()<<16);
	ret = ret + ((uLWord)ExternFlashIf_ReadByte()<<8);
	ret = ret + ExternFlashIf_ReadByte();
	
	(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
	
	return ret;
}

//************************************************************/
// Function: erase the flash sector size
// Trans para: addr
//************************************************************/
void ExternFlashIf_SectorErase(uWord Sector)
{
  uLWord addr;
  
  addr = Sector * 4096;
	ExternFlashIf_WriteEnable();
	
	(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
	
	ExternFlashIf_WriteByte(FLASH_CMD_SECTOR_ERASE);
	ExternFlashIf_WriteByte((uByte)((addr&0xff0000)>>16));
	ExternFlashIf_WriteByte((uByte)((addr&0xff00)>>8));
	ExternFlashIf_WriteByte((uByte)(addr&0xff));
	
	(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();

}

//************************************************************/
// Function: erase the flash block size
// Trans para: addr
//************************************************************/
void ExternFlashIf_BlockErase(uLWord addr)
{
	ExternFlashIf_WriteEnable();
	
	(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
	
	ExternFlashIf_WriteByte(FLASH_CMD_SECTOR_ERASE);
	ExternFlashIf_WriteByte((uByte)((addr&0xff0000)>>16));
	ExternFlashIf_WriteByte((uByte)((addr&0xff00)>>8));
	ExternFlashIf_WriteByte((uByte)(addr&0xff));
	
	(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();
	
}

//************************************************************/
// Function: erase the flash chip
// Trans para: void
//************************************************************/
void ExternFlashIf_ChipErase(void)
{
	ExternFlashIf_WriteEnable();
	
	(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
	
	ExternFlashIf_WriteByte(FLASH_CMD_CHIP_ERASE);
	
	(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();

	//ExternFlashIf_WaitWelComplete();
}


void ExternFlashIf_ReadPage(uWord Page, uByte * RAMPTR p_data, uWord dlen)
{
	uWord i;
	uLWord addr;
	
	addr = Page * FLASH_PAGE_SIZE;	
//	ExternFlashIf_WaitWipComplete();

	(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
	
	ExternFlashIf_WriteByte(FLASH_CMD_NORMAL_READ);
	ExternFlashIf_WriteByte((uByte)((addr&0xff0000)>>16));
	ExternFlashIf_WriteByte((uByte)((addr&0xff00)>>8));
	ExternFlashIf_WriteByte((uByte)(addr&0xff));

	for(i = 0; i < dlen; i++)
	{
		*p_data++ = ExternFlashIf_ReadByte();
	}
	
	(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();

}


void ExternFlashIf_WritePage(uWord Page, uByte * RAMPTR p_data, uWord dlen)
{
	uWord i;
	uLWord addr;
	
	addr = Page * FLASH_PAGE_SIZE;
	ExternFlashIf_WriteEnable();
  
	(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
	
	ExternFlashIf_WriteByte(FLASH_CMD_PAGE_PROGRAM);
	ExternFlashIf_WriteByte((uByte)((addr&0xff0000)>>16));
	ExternFlashIf_WriteByte((uByte)((addr&0xff00)>>8));
	ExternFlashIf_WriteByte((uByte)(addr&0xff));

	for(i = 0; i < dlen; i++)
	{
		ExternFlashIf_WriteByte(*p_data++); 
	}
	
	(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();

}

//************************************************************/
// Function: check memory page
// Trans para: page, data array, data num
//************************************************************/
uWord ExternFlashIf_CheckPage(uWord Page, uByte * RAMPTR p_data, uWord dlen)
{
  uWord i;
	uLWord addr;
	uByte data;
  uWord err_code;

  err_code = (uWord)EXTERNFLASHIF_ERR_OK;
	addr = Page * FLASH_PAGE_SIZE;	
//	ExternFlashIf_WaitWipComplete();

	(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
	
	ExternFlashIf_WriteByte(FLASH_CMD_NORMAL_READ);
	ExternFlashIf_WriteByte((uByte)((addr&0xff0000)>>16));
	ExternFlashIf_WriteByte((uByte)((addr&0xff00)>>8));
	ExternFlashIf_WriteByte((uByte)(addr&0xff));

  for(i = 0; i < dlen; i++)
  {
      data = ExternFlashIf_ReadByte();
      if(data != (*p_data++))
      {
          err_code = (uWord)EXTERNFLASHIF_ERR_CHECK_FAIL;
          break;
      }
  }

  (*ExternFlash_Operation.ExternFlash_Cs.SetVal)();

  return err_code;
}

void ExternFlashIf_NormalRead(uLWord addr, uByte * RAMPTR p_data, uWord dlen)
{
	uWord i;
	//ExternFlashIf_WaitWipComplete();

	(*ExternFlash_Operation.ExternFlash_Cs.ClrVal)();
	
	ExternFlashIf_WriteByte(FLASH_CMD_NORMAL_READ);
	ExternFlashIf_WriteByte((uByte)((addr&0xff0000)>>16));
	ExternFlashIf_WriteByte((uByte)((addr&0xff00)>>8));
	ExternFlashIf_WriteByte((uByte)(addr&0xff));

	for(i = 0; i < dlen; i++)
	{
		*p_data++ = ExternFlashIf_ReadByte();
	}
	
	(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();

}

//************************************************************/
// Function: initial extern flash hardware
// Trans para: void
//************************************************************/
uByte ExternFlashIf_Init(void)
{
	uByte status_reg;
	uLWord Product_ID;

	(*ExternFlash_Operation.ExternFlash_Cs.SetOutput)();
	(*ExternFlash_Operation.ExternFlash_Sck.SetOutput)();
	(*ExternFlash_Operation.ExternFlash_Si.SetOutput)();
	(*ExternFlash_Operation.ExternFlash_Rst.SetOutput)();
	(*ExternFlash_Operation.ExternFlash_So.SetInput)();
	(*ExternFlash_Operation.ExternFlash_Rst.SetVal)();

	/* use spi mode0 */
	(*ExternFlash_Operation.ExternFlash_Sck.ClrVal)();
	(*ExternFlash_Operation.ExternFlash_Cs.SetVal)();

	status_reg = ExternFlashIf_ReadStatus();
	Product_ID = ExternFlashIf_ReadProductJedecID();

	if(FLASH_IS25LP_TYPE == (Product_ID >>16))
	{
		if(FLASH_SIZE_128M == (Product_ID&0xFFFF))
		{
			ExternFlashIf_Memory_Size = 0x1000000;
		}
		else if(FLASH_SIZE_64M == (Product_ID&0xFFFF))
		{
			ExternFlashIf_Memory_Size = 0x800000;
		}
		else
		{
			ExternFlashIf_Memory_Size = 0x400000;
		}
	}
	
	return status_reg;
}


#pragma DATA_SEG DEFAULT
#endif