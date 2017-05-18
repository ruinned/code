#include "CpuEEIf.h"
#include "Nvm_Lcfg.h"

#ifdef  CPUEE_EEE_START_INT
#define CPUEEIF_ADDR_BASE  CPUEE_EEE_START_INT
#else 
#ifdef  CPUEE_AREA_START_INT
#define CPUEEIF_ADDR_BASE  CPUEE_AREA_START_INT
#endif 
#endif

#define CALC_ABSOLUTE_ADDR(relative_addr) ((relative_addr) + CPUEEIF_ADDR_BASE)

volatile uByte CpuEEIf_CCRreg;
#define CPUEEIF_SAVESTATUSREG() \
/*lint -save  -e950 Disable MISRA rule (1.1) checking. */\
    { __asm(pshc); __asm(sei); __asm(movb 1,SP+,CpuEEIf_CCRreg); } /* This macro is just used by CPUEEIF. It saves CCR register and disable global interrupts. */ \
/*lint -restore Enable MISRA rule (1.1) checking. */
#define CPUEEIF_RESTORESTATUSREG() \
/*lint -save  -e950 Disable MISRA rule (1.1) checking. */\
    { __asm(movb CpuEEIf_CCRreg, 1,-SP); __asm(pulc); } /* This macro is just used by CPUEEIF. It restores CCR register saved in SaveStatusReg(). */ \
/*lint -restore Enable MISRA rule (1.1) checking. */

//************************************************************/
// Function: initial CpuEE
// Trans para: void
//************************************************************/
void CpuEEIf_Init(void)
{
    //(*CpuEE_Operation.Init)();    
}

//************************************************************/
// Function: enable write of CpuEE
// Trans para: void
//************************************************************/
void CpuEEIf_WriteEnable(void)
{
    
}

//************************************************************/
// Function: disable write of CpuEE
// Trans para: void
//************************************************************/
void CpuEEIf_WriteDisable(void)
{
    
}    

//************************************************************/
// Function: get byte of CpuEE
// Trans para: point of data, address
//************************************************************/
uWord CpuEEIf_GetByte(uWord relative_addr, uByte * p_data)
{
    uWord rslt;
    CPUEEIF_SAVESTATUSREG();
    rslt = (*CpuEE_Operation.GetByte)((far uWord * far)(CALC_ABSOLUTE_ADDR(relative_addr)), p_data);    
    CPUEEIF_RESTORESTATUSREG();
    return rslt;    
}

//************************************************************/
// Function: set byte of CpuEE
// Trans para: point of data, address
//************************************************************/
uWord CpuEEIf_SetByte(uWord relative_addr, uByte data)
{
    uWord rslt;
    CPUEEIF_SAVESTATUSREG();
    rslt = (*CpuEE_Operation.SetByte)((far uWord * far)(CALC_ABSOLUTE_ADDR(relative_addr)), data);    
    CPUEEIF_RESTORESTATUSREG();
    return rslt;
}   

uWord CpuEEIf_SetWord(uWord relative_addr, uWord data)
{
    uWord rslt;
    CPUEEIF_SAVESTATUSREG();
    rslt = (*CpuEE_Operation.SetWord)((far uWord * far)(CALC_ABSOLUTE_ADDR(relative_addr)), data);    
    CPUEEIF_RESTORESTATUSREG();
    return rslt;
}


uWord CpuEEIf_SetLong(uWord relative_addr, uLWord data)
{
    uWord rslt;
    CPUEEIF_SAVESTATUSREG();
    rslt = (*CpuEE_Operation.SetLong)((far uWord * far)(CALC_ABSOLUTE_ADDR(relative_addr)), data);    
    CPUEEIF_RESTORESTATUSREG();
    return rslt;
}  
//************************************************************/
// Function: disable write of CpuEE
// Trans para: void
//************************************************************/
uWord CpuEEIf_ReadArray(uByte* p_data, uWord relative_addr, uByte dlen)
{
    uWord rslt;
    uByte i;
    
    rslt = (uWord)CPUEEIF_ERR_OK;
    for (i = 0; i < dlen; i++)
    {
        rslt |= CpuEEIf_GetByte(relative_addr++, p_data++);
    }
    
    if(!rslt)
    {
        return (uWord)CPUEEIF_ERR_OK;    
    }
    else
    {
        return (uWord)CPUEEIF_ERR_RDARRAY;  
    }
}

//************************************************************/
// Function: write array of CpuEE
// Trans para: void
//************************************************************/
uWord CpuEEIf_WriteArray(uByte* p_data, uWord relative_addr, uByte dlen)
{
    uWord rslt;
    uByte i;
    
    rslt = (uWord)CPUEEIF_ERR_OK;

    while(dlen != 0)
    {        
        if(relative_addr & 0x03) 
        {                      
              if (relative_addr & 0x01) 
              {           
                  dlen -= 1;
                  rslt |= CpuEEIf_SetByte(relative_addr++, *p_data++);
              }
              else
              {
                   if(dlen >= 2)
                   {
                       dlen -= 2;
                       rslt |= CpuEEIf_SetWord(relative_addr, *(uWord*)p_data); 
                       p_data += 2;
                       relative_addr += 2;
                   }
                   else
                   {
                       dlen -= 1;
                       rslt |= CpuEEIf_SetByte(relative_addr++, *p_data++); 
                   }
              }                  
        }
        else 
        {
            if(dlen >= 4)
            {
                dlen -= 4;
                rslt |= CpuEEIf_SetLong(relative_addr, *(uLWord*)p_data); 
                p_data += 4;
                relative_addr += 4;
            }
            else
            {             
                  if(dlen >= 2)
                  {
                       dlen -= 2;
                       rslt |= CpuEEIf_SetWord(relative_addr, *(uWord*)p_data); 
                       p_data += 2;
                       relative_addr += 2;
                   }
                   else
                   {
                       dlen -= 1;
                       rslt |= CpuEEIf_SetByte(relative_addr++, *p_data++); 
                   } 
            }
        }
  
    }
    
    if(!rslt)
    {
        return (uWord)CPUEEIF_ERR_OK;    
    }
    else
    {
        return (uWord)CPUEEIF_ERR_WRARRAY;  
    }
}

