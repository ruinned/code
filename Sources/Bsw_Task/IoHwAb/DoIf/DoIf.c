/*****************************************************************************/                                                                                                                                  
/* FILE NAME : DoIf.c                                                        */                                             
/*---------------------------------------------------------------------------*/                                                        
/* AUTHOR : PHQ                                                              */                                               
/*---------------------------------------------------------------------------*/                                              
/* TIME : 15/10                                                              */                                                                    
/*---------------------------------------------------------------------------*/                                             
/* DESCRIPTION : 衔接应用层与底层模块，在应用层中所有对Do端口的操作只通过调用*/                                                      
/*               DoIf中间层中的接口函数即可                                  */                                                        
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* HISTORY :                                                                 */
/*                                                                           */
/*    Author    |    Time    |    Version    |    description                */
/* -------------|------------|---------------|------------------------------ */
/*    PHQ       |    15/10   |    1.0        |    build the module           */
/*****************************************************************************/

#include "DoIf.h"


GeneralReturnType DoIf_Ctrl(uByte DoIf_Id,bool DoIf_Cmd) 
{
    GeneralReturnType rslt;

    rslt = (GeneralReturnType)ERR_OK;
    if(DoIf_Id >= DOIF_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;
    }
     
    switch(DoIf_Id)
    {
        case DOIF_CPUDO1_NO:
        {
            CPUDO1_PutVal(DoIf_Cmd);
            break;
        }
        case DOIF_CON_NO:
        {
            CON1_PutVal(DoIf_Cmd);
            break;
        }
        case DOIF_CPU_CHG_S_NO:
        {
            CPU_CHG_S_PutVal(DoIf_Cmd);
            break;
        }
        case DOIF_CPU_CHG_P_NO:
        {
            CPU_CHG_P_PutVal(DoIf_Cmd);
            break;
        }
        case DOIF_SLAVER_POW_CTRL_NO:
        {
            Slaver_Pow_Ctrl_PutVal(DoIf_Cmd);
            break;
        }
        case DOIF_CPUDO2_NO:
        {
            CPUDO2_PutVal(DoIf_Cmd);
            break;    
        }
        case DOIF_ISO_CTRL_POS_NO:
        {
            ISO_CTRL_POS_PutVal(DoIf_Cmd); 
            break;   
        }
        case DOIF_ISO_CTRL_NEG_NO:
        {
            ISO_CTRL_NEG_PutVal(DoIf_Cmd);
            break;
        }
        case DOIF_LOCK_LOCK_NO:
        {
            Lock_Lock_PutVal(DoIf_Cmd); 
            break;   
        }
        case DOIF_LOCK_UNLOCK_NO:
        {
            Lock_UnLock_PutVal(DoIf_Cmd);
            break;
        }
        default:break;
    }
    
    return rslt;

}


/*
GeneralReturnType DoIf_Init(uByte DoIf_Id) 
{
    GeneralReturnType rslt;
    sDoIf_PropertyType *DoIf_PropPtr;
    const sDoIf_OperationType *DoIf_OperaPtr;
    
    rslt = (GeneralReturnType)ERR_OK;
    if(DoIf_Id >= DOIF_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;
    }
    DoIf_PropPtr = GET_DOPROPERTY(DoIf_Id);  //point to property
    DoIf_OperaPtr = GET_DOOPERATION(DoIf_Id); //point to operation

    if((uByte)(DoIf_PropPtr->DoIf_IdNum) == DoIf_Id) 
    {
        if(DoIf_OperaPtr->DoIf_PutValPtr != NULL) 
        {
            (*DoIf_OperaPtr->DoIf_PutValPtr)((bool)DoIf_PropPtr->DoIf_Cmd); //the function output the value
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


GeneralReturnType DoIf_Ctrl(uByte DoIf_Id,bool DoIf_Cmd) 
{
    GeneralReturnType rslt;
    sDoIf_PropertyType *DoIf_PropPtr;
    const sDoIf_OperationType *DoIf_OperaPtr;
    
    rslt = (GeneralReturnType)ERR_OK;
    if(DoIf_Id >= DOIF_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;
    }
    DoIf_PropPtr = GET_DOPROPERTY(DoIf_Id);  //point to property
    DoIf_OperaPtr = GET_DOOPERATION(DoIf_Id); //point to operation
     
    if((uByte)(DoIf_PropPtr->DoIf_IdNum) == DoIf_Id) 
    { 
        if(DoIf_OperaPtr->DoIf_PutValPtr != NULL) 
        {
            if(DoIf_Cmd) 
            {
                DoIf_PropPtr->DoIf_Cmd = DOIF_CMD_HIGH;
                if(DOIF_NORMAL_LOGIC_TYPE == DoIf_PropPtr->DoIf_LogicType) 
                {  
                    (*DoIf_OperaPtr->DoIf_PutValPtr)(DOIF_CMD_HIGH);   //the function output the value
                } 
                else 
                {
                    (*DoIf_OperaPtr->DoIf_PutValPtr)(DOIF_CMD_LOW);   //the function output the value
                }
            } 
            else 
            {
                DoIf_PropPtr->DoIf_Cmd = DOIF_CMD_LOW;
                if(DOIF_NORMAL_LOGIC_TYPE == DoIf_PropPtr->DoIf_LogicType) 
                {  
                    (*DoIf_OperaPtr->DoIf_PutValPtr)(DOIF_CMD_LOW);   //the function output the value
                } 
                else 
                {
                    (*DoIf_OperaPtr->DoIf_PutValPtr)(DOIF_CMD_HIGH);   //the function output the value
                }
            }
        }
        else 
        {
            rslt = (GeneralReturnType)ERR_IO_FUNCTION_PTR_NULL;   //the function pointer is null
        }
    }
    else 
    {
        rslt = (GeneralReturnType)ERR_IO_ID_NUM_NOTMATCH;     //the id number is not match
    }

    return rslt;
}

*/
 


