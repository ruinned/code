#ifndef EXTERNFLASHIF_H
#define EXTERNFLASHIF_H

#include "GeneralTypeDef.h"
#include "ECU.h"

#include "ExternFlash_Sck.h"
#include "ExternFlash_Si.h"
#include "ExternFlash_Cs.h"
#include "ExternFlash_So.h"
#include "ExternFlash_Rst.h"

/* define read commands, SCK mode is inactive clock polarity low */
#define CONRD   0x68      // continuous array read
#define MMPRD   0x52	    // main memory read
#define BUF1RD  0x54      // buffer 1 read
#define BUF2RD  0x56      // buffer 2 read
#define STREGRD  0x57     // status register read

/* define program and erase commands, SCK mode is any */
#define BUF1WR  0x84	    // buffer 1 write
#define BUF2WR  0x87	    // buffer 2 write
#define BUF1MP  0x83      // buffer 1 to main memory page program with built-in erase
#define BUF2MP  0x86      // buffer 2 to main memory page program with built-in erase

#define PAGE_ERASE   0x81	// page erase
#define BLOCK_ERASE  0x50 // block erase

#define MPTRB1  0x82	    // main memory program through buffer 1
#define MPTRB2  0x85	    // main memory program through buffer 2

/* define additional commands */
#define MTOB1T  0x53	    // main memory to buffer 1 transfer	
#define MTOB2T  0x55	    // main memory to buffer 2 transfer
#define MB1CMP  0x60	    // memory buffer 1 compare
#define MB2CMP  0x61	    // memory buffer 2 compare
#define RWM_BUF1  0x58	  // auto memory page rewrite through buffer 1
#define RWM_BUF2  0x59	  // auto memory page rewrite through buffer 2

#define DONTC   0x00      // start address of page
#define BUF1    1         // buffer no
#define BUF2    2         // buffer no

#define FLASH_SMALLCAP	 0x18
#define FLASH_LAGRGECAP	 0x28

#define CHIP_ERASE_HH    0xc7
#define CHIP_ERASE_HL    0x94
#define CHIP_ERASE_LH    0x80
#define CHIP_ERASE_LL    0x9a

// errorcode define
typedef enum
{
    EXTERNFLASHIF_ERR_OK,
    EXTERNFLASHIF_ERR_CHECK_FAIL = ERR_BSW_NVM_BASE + 20U,
}eExternFlashIf_ErrCodeType;

extern uByte ExternFlashIf_Init(void);
extern uByte ExternFlashIf_ReadStatusReg(void);
extern void ExternFlashIf_WriteBuf(uByte buf_no, uWord buf_addr, uByte * RAMPTR p_data, uWord dlen);
extern void ExternFlashIf_ReadBuf(uWord buf_no, uByte * RAMPTR p_data, uWord dlen);
extern void ExternFlashIf_WritePage(uWord page, uByte * RAMPTR p_data, uWord dlen);
extern void ExternFlashIf_ReadPage(uWord page, uByte * RAMPTR p_data, uWord dlen);
extern uWord ExternFlashIf_CheckPage(uWord page, uByte * RAMPTR p_data, uWord dlen);
extern void ExternFlashIf_ChipErase(void);
extern void ExternFlashIf_PageErase(uWord page);
extern void ExternFlashIf_BlockErase(uWord block);

#endif /* _EXTERNFLASHIF_H*/