#include "PwmIf_Type.h"
#include "PwmIf.h"

#ifdef ECU_M_IOHWAB_PWMIF
GeneralReturnType PwmIf_Start(uByte PwmIf_Id) 
{
    GeneralReturnType rslt;
    uByte Pwm_rslt;
    sPwmIf_PropertyType *PwmIf_PropPtr;
    const sPwmIf_OperationType *PwmIf_OperaPtr;
    
    rslt = (GeneralReturnType)ERR_OK;
    if(PwmIf_Id >= PWMIF_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;
    }
    PwmIf_PropPtr = GET_PWMPROPERTY(PwmIf_Id);  //point to property
    PwmIf_OperaPtr = GET_PWMOPERATION(PwmIf_Id);  //point to operation
    
    /*if(PwmIf_PropPtr != NULL) 
    {
        if(PwmIf_OperaPtr != NULL) 
        {  */ 
            if((uByte)(PwmIf_PropPtr->PwmIf_IdNum) == PwmIf_Id) //match the pwm number
            {
                if(PwmIf_OperaPtr->PwmIf_EnablePtr != NULL) 
                {
                    Pwm_rslt = (*PwmIf_OperaPtr->PwmIf_EnablePtr)();   //enable the pwm ,start the pwm function
                    if(Pwm_rslt != ERR_OK) 
                    {
                        return  (GeneralReturnType)Pwm_rslt;
                    }
                } 
                else 
                {
                    rslt = (GeneralReturnType)ERR_IO_FUNCTION_PTR_NULL; //the function pointer is null
                }
            } 
            else 
            {
                rslt = (GeneralReturnType)ERR_IO_ID_NUM_NOTMATCH;  //the id number is not match
            }   
       /* } 
        else 
        {
            rslt = (GeneralReturnType)ERR_IO_OPERATION_PTR_NULL; //the operation pointer in null
        }
    } 
    else
    {
        rslt = (GeneralReturnType)ERR_IO_PROPERTY_PTR_NULL;  //the property pointer in null
    } */
    return rslt; 
}

GeneralReturnType PwmIf_Stop(uByte PwmIf_Id) 
{
    GeneralReturnType rslt;
    uByte Pwm_rslt;
    sPwmIf_PropertyType *PwmIf_PropPtr;
    const sPwmIf_OperationType *PwmIf_OperaPtr;
    
    rslt = (GeneralReturnType)ERR_OK;
    if(PwmIf_Id >= PWMIF_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;
    }
    PwmIf_PropPtr = GET_PWMPROPERTY(PwmIf_Id);  //point to property
    PwmIf_OperaPtr = GET_PWMOPERATION(PwmIf_Id);  //point to operation
    
   /* if(PwmIf_PropPtr != NULL) 
    {
        if(PwmIf_OperaPtr != NULL) 
        { */  
            if((uByte)(PwmIf_PropPtr->PwmIf_IdNum) == PwmIf_Id) //match the pwm number
            {
                if(PwmIf_OperaPtr->PwmIf_DisablePtr != NULL) 
                {
                    Pwm_rslt = (*PwmIf_OperaPtr->PwmIf_DisablePtr)();   //enable the pwm ,start the pwm function
                    if(Pwm_rslt != ERR_OK) 
                    {
                        return (GeneralReturnType)Pwm_rslt;
                    }
                } 
                else 
                {
                    rslt = (GeneralReturnType)ERR_IO_FUNCTION_PTR_NULL; //the function pointer is null
                }
            } 
            else 
            {
                rslt = (GeneralReturnType)ERR_IO_ID_NUM_NOTMATCH;  //the id number is not match
            }   
       /* } 
        else 
        {
            rslt = (GeneralReturnType)ERR_IO_OPERATION_PTR_NULL; //the operation pointer in null
        }
    } 
    else
    {
        rslt = (GeneralReturnType)ERR_IO_PROPERTY_PTR_NULL;  //the property pointer in null
    } */
    return rslt; 
} 

GeneralReturnType PwmIf_SetRatio(uByte PwmIf_Id,uWord Ratio) 
{
    GeneralReturnType rslt;
    uByte Pwm_rslt;
    sPwmIf_PropertyType *PwmIf_PropPtr;
    const sPwmIf_OperationType *PwmIf_OperaPtr;
    
    rslt = (GeneralReturnType)ERR_OK;
    if(PwmIf_Id >= PWMIF_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;
    }
    PwmIf_PropPtr = GET_PWMPROPERTY(PwmIf_Id);  //point to property
    PwmIf_OperaPtr = GET_PWMOPERATION(PwmIf_Id);  //point to operation
    
   /* if(PwmIf_PropPtr != NULL) 
    {
        if(PwmIf_OperaPtr != NULL) 
        { */  
            if((uByte)(PwmIf_PropPtr->PwmIf_IdNum) == PwmIf_Id) //match the pwm number
            {
                if((PwmIf_OperaPtr->PwmIf_DisablePtr != NULL)
                && (PwmIf_OperaPtr->PwmIf_SetRatio16Ptr != NULL)) 
                {
                    Pwm_rslt = (*PwmIf_OperaPtr->PwmIf_DisablePtr)();  //disable the pwm
                    if(Pwm_rslt != ERR_OK) 
                    {
                        return (GeneralReturnType)Pwm_rslt;
                    }
                    PwmIf_PropPtr->PwmIf_Ratio = Ratio;   
                    Pwm_rslt = (*PwmIf_OperaPtr->PwmIf_SetRatio16Ptr)(Ratio); //set the pwm duty
                    if(Pwm_rslt != ERR_OK) 
                    {
                        return (GeneralReturnType)Pwm_rslt;
                    }
                } 
                else 
                {
                    rslt = (GeneralReturnType)ERR_IO_FUNCTION_PTR_NULL; //the function pointer is null
                }
            } 
            else 
            {
                rslt = (GeneralReturnType)ERR_IO_ID_NUM_NOTMATCH;  //the id number is not match
            }
      /*  } 
        else 
        {
            rslt = (GeneralReturnType)ERR_IO_OPERATION_PTR_NULL; //the operation pointer in null
        }
    } 
    else
    {
        rslt = (GeneralReturnType)ERR_IO_PROPERTY_PTR_NULL;  //the property pointer in null
    } */
    return rslt;
}

GeneralReturnType PwmIf_SetStartLevel(uByte PwmIf_Id,bool StartLevel) 
{
    GeneralReturnType rslt;
    uByte Pwm_rslt;
    sPwmIf_PropertyType *PwmIf_PropPtr;
    const sPwmIf_OperationType *PwmIf_OperaPtr;
    
    rslt = (GeneralReturnType)ERR_OK;
    if(PwmIf_Id >= PWMIF_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;
    }
    PwmIf_PropPtr = GET_PWMPROPERTY(PwmIf_Id);  //point to property
    PwmIf_OperaPtr = GET_PWMOPERATION(PwmIf_Id);  //point to operation
    
   /* if(PwmIf_PropPtr != NULL) 
    {
        if(PwmIf_OperaPtr != NULL) 
        { */  
            if((uByte)(PwmIf_PropPtr->PwmIf_IdNum) == PwmIf_Id) //match the pwm number
            {
                if((PwmIf_OperaPtr->PwmIf_DisablePtr != NULL)
                && (PwmIf_OperaPtr->PwmIf_ClrValuePtr != NULL)
                && (PwmIf_OperaPtr->PwmIf_SetValuePtr != NULL)) 
                {
                    Pwm_rslt = (*PwmIf_OperaPtr->PwmIf_DisablePtr)();  //disable the pwm
                    if(Pwm_rslt != ERR_OK) 
                    {
                        return (GeneralReturnType)Pwm_rslt;
                    } 
                    if(!StartLevel) 
                    {
                        Pwm_rslt = (*PwmIf_OperaPtr->PwmIf_ClrValuePtr)();
                        if(Pwm_rslt != ERR_OK) 
                        {
                            return (GeneralReturnType)Pwm_rslt;
                        }
                        PwmIf_PropPtr->PwmIf_StartLevel = START_LEVEL_LOW;
                    }
                    else
                    {
                       Pwm_rslt = (*PwmIf_OperaPtr->PwmIf_SetValuePtr)();
                       if(Pwm_rslt != ERR_OK) 
                       {
                           return (GeneralReturnType)Pwm_rslt;
                       }
                       PwmIf_PropPtr->PwmIf_StartLevel = START_LEVEL_HIGH;
                    }
                } 
                else 
                {
                    rslt = (GeneralReturnType)ERR_IO_FUNCTION_PTR_NULL; //the function pointer is null
                }
            } 
            else 
            {
                rslt = (GeneralReturnType)ERR_IO_ID_NUM_NOTMATCH;  //the id number is not match
            }
       /* } 
        else 
        {
            rslt = (GeneralReturnType)ERR_IO_OPERATION_PTR_NULL; //the operation pointer in null
        }
    } 
    else
    {
        rslt = (GeneralReturnType)ERR_IO_PROPERTY_PTR_NULL;  //the property pointer in null
    } */
    return rslt;
}
#endif