
#include "ExternFlash.h"
#include "ExternFlashIf.h"
#include "Nvm_Lcfg.h"
#include "ECU.h"

const uByte ExternFlash_WriteCmd[] = 
{
	EXTERNFLASH_WAIT_OK,
	EXTERNFLASH_READ_SECTOR,
	EXTERNFLASH_ERASE,
	EXTERNFLASH_WAIT_COMP,
	EXTERNFLASH_WRITE,
	EXTERNFLASH_WAIT_COMP,
	EXTERNFLASH_IDLE,
};

const uByte ExternFlash_WriteSectorCmd[] = 
{
	EXTERNFLASH_WAIT_OK,
	EXTERNFLASH_ERASE,
	EXTERNFLASH_WAIT_COMP,
	EXTERNFLASH_WRITE,
	EXTERNFLASH_WAIT_COMP,
	EXTERNFLASH_IDLE,
};

const uByte ExternFlash_EraseCmd[] = 
{
	EXTERNFLASH_WAIT_OK,
	EXTERNFLASH_READ_SECTOR,
	EXTERNFLASH_ERASE,
	EXTERNFLASH_WAIT_COMP,
	EXTERNFLASH_WRITE_ERASE,
	EXTERNFLASH_WAIT_COMP,
	EXTERNFLASH_IDLE,
};

const uByte ExternFlash_EraseSectorCmd[] = 
{
	EXTERNFLASH_WAIT_OK,
	EXTERNFLASH_ERASE,
	EXTERNFLASH_WAIT_COMP,
	EXTERNFLASH_IDLE,
};

const uByte ExternFlash_EraseChipCmd[] = 
{
	EXTERNFLASH_WAIT_OK,
	EXTERNFLASH_ERASE_CHIP,
	EXTERNFLASH_WAIT_COMP,
	EXTERNFLASH_IDLE,
};

typedef struct
{
	uByte *pStep;
	uByte StepIndex;
	uByte * RAMPTR pData;
	uWord dlen;
	uLWord addr;
}sExternFlash_MainType;

sExternFlash_MainType ExternFlash_Main[EXTERNFLASH_MAIN_LEN];
uByte ExternFlash_Qhead;
uByte ExternFlash_Qtail;
#define EXTERNFLASH_Q_IS_EMPTY()	(ExternFlash_Qhead == ExternFlash_Qtail)
#define EXTERNFLASH_Q_IS_FULL()	(ExternFlash_Qtail == (ExternFlash_Qhead+1)%EXTERNFLASH_MAIN_LEN)
#define EXTERNFLASH_Q_ADD()		{ExternFlash_Qhead = (ExternFlash_Qhead+1)%EXTERNFLASH_MAIN_LEN;}
#define EXTERNFLASH_Q_DEL()		{ExternFlash_Qtail = (ExternFlash_Qtail+1)%EXTERNFLASH_MAIN_LEN;}

void ExternFlash_FillQueue(uLWord addr, uByte * RAMPTR p_data, uWord dlen, uByte type)
{
	if(!EXTERNFLASH_Q_IS_FULL())
	{
		ExternFlash_Main[ExternFlash_Qhead].addr = addr;
		ExternFlash_Main[ExternFlash_Qhead].pData = p_data;
		ExternFlash_Main[ExternFlash_Qhead].dlen = dlen;
		ExternFlash_Main[ExternFlash_Qhead].StepIndex = 0;
		switch(type)
		{
			case EXTERNFLASH_CMD_WRITE:
			{
				ExternFlash_Main[ExternFlash_Qhead].pStep = ExternFlash_WriteCmd;
				break;
			}
			
			case EXTERNFLASH_CMD_WRITE_SECTOR:
			{
				ExternFlash_Main[ExternFlash_Qhead].pStep = ExternFlash_WriteSectorCmd;
				break;
			}
			
			case EXTERNFLASH_CMD_ERASE:
			{
				ExternFlash_Main[ExternFlash_Qhead].pStep = ExternFlash_EraseCmd;
				break;
			}

			case EXTERNFLASH_CMD_ERASE_SECTOR:
			{
				ExternFlash_Main[ExternFlash_Qhead].pStep = ExternFlash_EraseSectorCmd;
				break;
			}

			case EXTERNFLASH_CMD_ERASE_CHIP:
			{
				ExternFlash_Main[ExternFlash_Qhead].pStep = ExternFlash_EraseChipCmd;
				break;
			}
			
			default:
			{
				break;
			}
		}
		EXTERNFLASH_Q_ADD();
	}
}

uByte ExternFlash_QEmpty(void)
{
	return EXTERNFLASH_Q_IS_EMPTY();
}

void ExternFlash_MainCbk(void)
{
	uByte step;
	uLWord addr;
	uByte *p_data;
	uWord dlen;
	
	sExternFlash_MainType* pMainType = &ExternFlash_Main[ExternFlash_Qtail] ;
	
	if(!EXTERNFLASH_Q_IS_EMPTY())
	{
		step = *(pMainType->pStep+pMainType->StepIndex);
		switch(step)
		{	
			case EXTERNFLASH_WAIT_OK:
			{
				if(ExternFlashIf_InIdle())
				{
					/*next step*/
					pMainType->StepIndex++;
				}
				break;
			}
			
			case EXTERNFLASH_READ_SECTOR:
			{
				ExternFlashIf_ReadSector(pMainType->addr, pMainType->pData, pMainType->dlen);
				pMainType->StepIndex++;
				break;
			}

			case EXTERNFLASH_ERASE:
			{
				ExternFlashIf_SectorErase(pMainType->addr);
				pMainType->StepIndex++;
				break;
			}

			case EXTERNFLASH_WAIT_COMP:
			{
				if(ExternFlashIf_InIdle())
				{
					/*next step*/
					pMainType->StepIndex++;
				}
				break;
			}

			case EXTERNFLASH_WRITE:
			{
				ExternFlashIf_SectorProgram(pMainType->addr, pMainType->pData, pMainType->dlen);
				pMainType->StepIndex++;
				break;
			}

			case EXTERNFLASH_WRITE_ERASE:
			{
				ExternFlashIf_SectorProgramErase(pMainType->addr, pMainType->dlen);
				pMainType->StepIndex++;
				break;
			}
			
			case EXTERNFLASH_ERASE_CHIP:
			{
				ExternFlashIf_ChipErase();
				pMainType->StepIndex++;
				break;
			}

			case EXTERNFLASH_IDLE:
			{
				EXTERNFLASH_Q_DEL();
				if(!EXTERNFLASH_Q_IS_EMPTY())
				{
					ExternFlash_Main[ExternFlash_Qtail].StepIndex = 0;
				}
				break;
			}
			
			default:
			{
				break;
			}
		}
	}
}

void ExternFlash_Init(void)
{
	uByte i;
	for(i = 0; i < EXTERNFLASH_MAIN_LEN; i++)
	{
		ExternFlash_Main[i].addr = 0;
		ExternFlash_Main[i].dlen = 0;
		ExternFlash_Main[i].pData = NULL;
		ExternFlash_Main[i].pStep = NULL;
		ExternFlash_Main[i].StepIndex = 0;
	}
	ExternFlash_Qhead = 0;
	ExternFlash_Qtail = 0;
}

