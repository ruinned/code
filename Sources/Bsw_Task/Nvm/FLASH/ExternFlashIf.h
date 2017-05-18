#if 1

#ifndef EXTERNFLASHIF_H
#define EXTERNFLASHIF_H

#include "GeneralTypeDef.h"
#include "ECU.h"

#define AT45DB_SMALL_PAGE		//011D~041D	page=264bytes	
//#define AT45DB_MIDDLE_PAGE		//161D,321D	page=528bytes
//#define AT45DB_LAGRGE_PAGE		//642D	page=1056bytes

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

#define FLASH_MEMORY_LAGRGEPAGE_SIZE		1056U
#define FLASH_MEMORY_MIDDLEPAGE_SIZE		528U
#define FLASH_MEMORY_SMALLPAGE_SIZE		264U
#define FLASH_MEMORY_PAGE_NUMB			4096U
#define FLASH_MEMORY_MIDDLEPAGE_NUMB		4096U
#define FLASH_MEMORY_SMALLPAGE_NUMB		2048U
#define FLASH_MEMORY_LAGRGEMAX_ADDR		0x420000L 	//(FLASH_MEMORY_PAGE_NUMB*FLASH_MEMORY_LAGRGEPAGE_SIZE)
#define FLASH_MEMORY_MIDDLEMAX_ADDR		0x210000L 	//(FLASH_MEMORY_MIDDLEPAGE_NUMB*FLASH_MEMORY_MIDDLEPAGE_SIZE)
#define FLASH_MEMORY_SMALLMAX_ADDR		0x84000L 	//(FLASH_MEMORY_SMALLPAGE_NUMB*FLASH_MEMORY_SMALLPAGE_SIZE)

// errorcode define
typedef enum
{
    EXTERNFLASHIF_ERR_OK,
    EXTERNFLASHIF_ERR_CHECK_FAIL = ERR_BSW_NVM_BASE + 20U,
    EXTERNFLASHIF_ERR_MEMORY_FAIL,
    EXTERNFLASHIF_ERR_DATA_EMPTY,
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
extern void ExternFlashIf_ReadPageAddr(uWord page, uWord buff_addr, uByte * RAMPTR p_data, uWord dlen);
extern uWord ExternFlashIf_ReadData(uLWord data_addr, uByte * RAMPTR p_data, uWord dlen);
extern void ExternFlashIf_WritePageAddr(uWord page, uWord buff_addr, uByte * RAMPTR p_data, uWord dlen);
extern uWord ExternFlashIf_WriteData(uLWord data_addr, uByte * RAMPTR p_data, uWord dlen);
extern uByte ExternFlashIf_InIdle(void);
#endif /* _EXTERNFLASHIF_H*/


#else

#ifndef EXTERNFLASHIF_H
#define EXTERNFLASHIF_H

#include "GeneralTypeDef.h"
#include "ECU.h"

// errorcode define
typedef enum
{
    EXTERNFLASHIF_ERR_OK,
    EXTERNFLASHIF_ERR_CHECK_FAIL = ERR_BSW_NVM_BASE + 20U,
}eExternFlashIf_ErrCodeType;

#define FLASH_IS25LP_TYPE			0x9D
#define FLASH_SIZE_128M		0x6018
#define FLASH_SIZE_64M		0x6017
#define FLASH_SIZE_32M		0x6016
#define FLASH_SECTOR_SIZE		4096U
#define FLASH_PAGE_SIZE		256U

#define FLASH_WIP_STATUS		0X01
#define FLASH_WEL_STATUS		0X02

#define FLASH_CMD_NORMAL_READ		0x03
#define FLASH_CMD_PAGE_PROGRAM	0x02
#define FLASH_CMD_SECTOR_ERASE		0xD7
#define FLASH_CMD_BLOCK_ERASE		0x52
#define FLASH_CMD_CHIP_ERASE		0xC7
#define FLASH_CMD_WRITE_ENABLE		0x06
#define FLASH_CMD_WRITE_DISABLE		0x04
#define FLASH_CMD_READ_STATUS		0x05
#define FLASH_CMD_WRITE_STATUS		0x01
#define FLASH_CMD_READ_FUNCTION	0x48
#define FLASH_CMD_WRITE_FUNCTION	0x02
#define FLASH_CMD_READ_PRODUCT_ID_JEDEC	0x9F
#define FLASH_CMD_READ_PRODUCT_ID	0xAB

extern uByte ExternFlashIf_Init(void);
extern void ExternFlashIf_ChipErase(void);
extern void ExternFlashIf_BlockErase(uWord block);
extern void ExternFlashIf_SectorErase(uWord Sector);
extern uByte ExternFlashIf_ReadStatus(void);
extern uByte ExternFlashIf_InIdle(void);
extern void ExternFlashIf_ReadPage(uWord Page, uByte * RAMPTR p_data, uWord dlen);
extern void ExternFlashIf_WritePage(uWord Page, uByte * RAMPTR p_data, uWord dlen);
extern uWord ExternFlashIf_CheckPage(uWord Page, uByte * RAMPTR p_data, uWord dlen);
extern uByte ExternFlashIf_InIdle(void);
extern void ExternFlashIf_NormalRead(uLWord addr, uByte * RAMPTR p_data, uWord dlen);

#endif /* _EXTERNFLASHIF_H*/

#endif