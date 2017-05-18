#include "RelayIf_Type.h"
#include "RelayIf.h"

#define  PWM_HIHG_LEVEL  1000
#define  PWM_LOW_LEVEL  30

GeneralReturnType RelayIf_IcChangeMode(uByte RelayIf_Id,uByte Mode) 
{
   GeneralReturnType rslt;
   GeneralReturnType Ic_st;
   sRelayIf_PropertyType *RelayIf_PropPtr;
   const sRelayIf_OperationType *RelayIf_OperaPtr;
   sRelayIf_PropertyType *RelayIf_PropPtr1;
   const sRelayIf_OperationType *RelayIf_OperaPtr1;
   
    rslt = (GeneralReturnType)ERR_OK;
    Ic_st = (GeneralReturnType)ERR_OK;
    if(RelayIf_Id > TLE8102_NO)  //out of the range
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;  
    }
    RelayIf_PropPtr = GET_RELAYPROPERTY(0); //point to property
    RelayIf_OperaPtr = GET_RELAYOPERATION(0);  //point to operation
    
    RelayIf_PropPtr1 = GET_RELAYPROPERTY(1); //point to property
    RelayIf_OperaPtr1 = GET_RELAYOPERATION(1);  //point to operation
   
    if(RelayIf_OperaPtr->RelayIf_IcInitPtr != NULL)
    { 
        if(!Mode)
        {
            Ic_st = (*RelayIf_OperaPtr->RelayIf_IcInitPtr)(RELAYIF_IC_SPI_MODE); //init the IC with the IO mode
            if(Ic_st != ERR_OK)
            {
               return Ic_st;
            }
            if(RelayIf_PropPtr != NULL)
            {
                RelayIf_PropPtr->RelayIf_Mode = IC_SPI_MODE;
            }
            if(RelayIf_PropPtr1 != NULL)
            {
                RelayIf_PropPtr1->RelayIf_Mode = IC_SPI_MODE;
            }  
        }
        else
        {
            Ic_st = (*RelayIf_OperaPtr->RelayIf_IcInitPtr)(RELAYIF_IC_IO_MODE); //init the IC with the IO mode
            if(Ic_st != ERR_OK)
            {
               return Ic_st;
            }
            if(RelayIf_PropPtr != NULL)
            {
                RelayIf_PropPtr->RelayIf_Mode = IC_IO_MODE;
            }
            if(RelayIf_PropPtr1 != NULL)
            {
                RelayIf_PropPtr1->RelayIf_Mode = IC_IO_MODE;
            }
        }
    }
    else if(RelayIf_OperaPtr1->RelayIf_IcInitPtr != NULL)
    { 
        if(!Mode)
        {
            Ic_st = (*RelayIf_OperaPtr1->RelayIf_IcInitPtr)(RELAYIF_IC_SPI_MODE); //init the IC with the IO mode
            if(Ic_st != ERR_OK)
            {
               return Ic_st;
            }
            if(RelayIf_PropPtr != NULL)
            {
                RelayIf_PropPtr->RelayIf_Mode = IC_SPI_MODE;
            }
            if(RelayIf_PropPtr1 != NULL)
            {
                RelayIf_PropPtr1->RelayIf_Mode = IC_SPI_MODE;
            }
        }
        else
        {
            Ic_st = (*RelayIf_OperaPtr1->RelayIf_IcInitPtr)(RELAYIF_IC_IO_MODE); //init the IC with the IO mode
            if(Ic_st != ERR_OK)
            {
               return Ic_st;
            }
            if(RelayIf_PropPtr != NULL)
            {
                RelayIf_PropPtr->RelayIf_Mode = IC_IO_MODE;
            }
            if(RelayIf_PropPtr1 != NULL)
            {
                RelayIf_PropPtr1->RelayIf_Mode = IC_IO_MODE;
            }
        }
    }
    else 
    {
        rslt = (GeneralReturnType)ERR_IO_FUNCTION_PTR_NULL;  //the function pointer is null
    }

    return rslt;
}

GeneralReturnType RelayIf_Ctrl(uByte RelayIf_Id,uByte Relay_cmd)
{
    GeneralReturnType rslt;
    GeneralReturnType Spi_st;
    GeneralReturnType Ic_st;
    uWord Err_Code;
    const sRelayIf_PropertyType *RelayIf_PropPtr;
    const sRelayIf_OperationType *RelayIf_OperaPtr;
    
    rslt = (GeneralReturnType)ERR_OK;
    if(RelayIf_Id >= RELAYIF_MAX_NUM)  //out of the range
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;  
    }
    RelayIf_PropPtr = GET_RELAYPROPERTY(RelayIf_Id); //point to property
    RelayIf_OperaPtr = GET_RELAYOPERATION(RelayIf_Id);  //point to operation
    
    if((uByte)(RelayIf_PropPtr->RelayIf_IdNum) == RelayIf_Id)   //match the spi number
    {
        if(IO_MODE == RelayIf_PropPtr->RelayIf_Mode)   //use the do ctrl
        {
          #ifndef ECU_M_IOHWAB_PWMIF
            if(RelayIf_OperaPtr->RelayIf_DoCtrlPtr != NULL) 
            {  
                if(RelayIf_OperaPtr->RelayIf_OeCtrlPtr != NULL)
                {
                  
                    (*RelayIf_OperaPtr->RelayIf_OeCtrlPtr)((bool)0);
                    (*RelayIf_OperaPtr->RelayIf_DoCtrlPtr)((bool)Relay_cmd); 
                    if(RelayIf_OperaPtr->RelayIf_ClkDoCtrlPtr != NULL)  //ctrl the latch
                    {
                        (*RelayIf_OperaPtr->RelayIf_ClkDoCtrlPtr)((bool)FALSE);
                        asm nop;
                        asm nop;
                        asm nop;
                        (*RelayIf_OperaPtr->RelayIf_ClkDoCtrlPtr)((bool)TRUE);
                    }
                }
            } 
          #else
            if((RelayIf_OperaPtr->RelayIf_PwmCtrlPtr != NULL) && (RelayIf_OperaPtr->RelayIf_PwmSetRatioPtr != NULL))
            {
               if(OPEN_RELAY == Relay_cmd)
               {
                   rslt = (*RelayIf_OperaPtr->RelayIf_PwmSetRatioPtr)((uWord)PWM_HIHG_LEVEL);
                   if(rslt != ERR_OK)
                   {
                       return rslt;
                   } 
               }
               else
               {
                   rslt = (*RelayIf_OperaPtr->RelayIf_PwmSetRatioPtr)((uWord)PWM_LOW_LEVEL);
                   if(rslt != ERR_OK)
                   {
                       return rslt;
                   }
               }
               rslt = (*RelayIf_OperaPtr->RelayIf_PwmCtrlPtr)();
               if(rslt != ERR_OK)
               {
                   return rslt;
               }
            }
          #endif
            else 
            {
                rslt = (GeneralReturnType)ERR_IO_FUNCTION_PTR_NULL;  //the function pointer is null
            }
        } 
        else if(IC_IO_MODE == RelayIf_PropPtr->RelayIf_Mode)  //use the io mode of the IC
        {
            if(RelayIf_OperaPtr->RelayIf_IcDoCtrlPtr != NULL) 
            {  
                Ic_st = (*RelayIf_OperaPtr->RelayIf_IcDoCtrlPtr)((bool)Relay_cmd);
                if(Ic_st != ERR_OK)
                {
                   return Ic_st;
                }
            } 
            else 
            {
                rslt = (GeneralReturnType)ERR_IO_FUNCTION_PTR_NULL;  //the function pointer is null
            }
        }
        else      //SPI MODE
        {
            if(RelayIf_OperaPtr->RelayIf_IcSpiCtrlPtr != NULL) 
            {
                Spi_st = (*RelayIf_OperaPtr->RelayIf_IcSpiCtrlPtr)(Relay_cmd,&Err_Code);
                if(Spi_st != ERR_OK)
                {
                    return Spi_st;
                }
            } 
            else 
            {
                rslt = (GeneralReturnType)ERR_IO_FUNCTION_PTR_NULL;  //the function pointer is null
            }
        }
    } 
    else 
    {
        rslt = (GeneralReturnType)ERR_IO_ID_NUM_NOTMATCH;   //the id number is not match
    }

    return rslt;             
}

#ifndef ECU_M_IOHWAB_PWMIF
GeneralReturnType RelayIf_FeedBack(uByte RelayIf_Id,uByte *Relay_ValuePtr)
{
    GeneralReturnType rslt;
    const sRelayIf_PropertyType *RelayIf_PropPtr;
    const sRelayIf_OperationType *RelayIf_OperaPtr;
    
    rslt = (GeneralReturnType)ERR_OK;
    if(RelayIf_Id >= RELAYIF_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;  
    }
    if(NULL == Relay_ValuePtr) 
    {
        rslt = (GeneralReturnType)ERR_IO_VALUE_PTR_NULL;
        return rslt;
    }
    RelayIf_PropPtr = GET_RELAYPROPERTY(RelayIf_Id); //point to property
    RelayIf_OperaPtr = GET_RELAYOPERATION(RelayIf_Id);  //point to operation
    
    if((uByte)(RelayIf_PropPtr->RelayIf_IdNum) == RelayIf_Id)   //match the spi number
    {
        if(RelayIf_OperaPtr->RelayIf_FeedBackPtr != NULL)
        {
            (*RelayIf_OperaPtr->RelayIf_FeedBackPtr)(Relay_ValuePtr); 
        }
        else 
        {
            rslt = (GeneralReturnType)ERR_IO_FUNCTION_PTR_NULL;  //the function pointer is null
        } 
    } 
    else 
    {
        rslt = (GeneralReturnType)ERR_IO_ID_NUM_NOTMATCH;   //the id number is not match
    }

    return rslt; 
}
#endif
