#ifndef EXTERNFLASH_H
#define EXTERNFLASH_H

#include "GeneralTypeDef.h"
#include "ECU.h"

#define EXTERNFLASH_CMD_WRITE			1
#define EXTERNFLASH_CMD_WRITE_SECTOR	2
#define EXTERNFLASH_CMD_ERASE			3
#define EXTERNFLASH_CMD_ERASE_SECTOR	4
#define EXTERNFLASH_CMD_ERASE_CHIP		5

#define EXTERNFLASH_MAIN_LEN	   20

 typedef enum
 {
	EXTERNFLASH_WAIT_OK,		
	EXTERNFLASH_READ_SECTOR, 
	EXTERNFLASH_ERASE,				
	EXTERNFLASH_WRITE,				
	EXTERNFLASH_WAIT_COMP,		
	EXTERNFLASH_WRITE_ERASE, 
	EXTERNFLASH_ERASE_CHIP,	
	EXTERNFLASH_IDLE,				
 }eExternFlash_step;
 
 extern void ExternFlash_MainCbk(void);
 extern void ExternFlash_FillQueue(uLWord addr, uByte * RAMPTR p_data, uWord dlen, uByte type);
extern void ExternFlash_Init(void);
extern uByte ExternFlash_QEmpty(void);
#endif /* _EXTERNFLASH_H*/
