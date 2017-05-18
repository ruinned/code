/*****************************************************************************/                                                                                                                                  
/* FILE NAME : DiIf.c                                                        */                                             
/*---------------------------------------------------------------------------*/                                                        
/* AUTHOR : PHQ                                                              */                                               
/*---------------------------------------------------------------------------*/                                              
/* TIME : 15/10                                                              */                                                                    
/*---------------------------------------------------------------------------*/                                             
/* DESCRIPTION : 衔接应用层与底层模块，在应用层中所有对Di端口的操作只通过调用*/                                                      
/*               DiIf中间层中的接口函数即可                                  */                                                        
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* HISTORY :                                                                 */
/*                                                                           */
/*    Author    |    Time    |    Version    |    description                */
/* -------------|------------|---------------|------------------------------ */
/*    PHQ       |    15/10   |    1.0        |    build the module           */
/*****************************************************************************/

#include "DiIf.h"



GeneralReturnType DiIf_GetValue(uByte DiIf_Id,bool *DiIf_ValuePtr)
{
   GeneralReturnType rslt;
   
   rslt = (GeneralReturnType)ERR_OK;
   
   if(DiIf_Id >= DIIF_MAX_NUM) 
   {
       rslt = (GeneralReturnType)ERR_IO_RANGE;
       return rslt;
   }
   if(NULL == DiIf_ValuePtr) 
   { 
       rslt = (GeneralReturnType)ERR_IO_VALUE_PTR_NULL;
       return rslt;
   } 

   switch(DiIf_Id)
   {
      case DIIF_PORTA0_NO:
      {
          if(PORTA0_GetVal())
          {
              *DiIf_ValuePtr = DIIF_VALUE_HIGH;    
          }
          else
          {
               *DiIf_ValuePtr = DIIF_VALUE_LOW;     
          }
          break;
      }
      case DIIF_PORTA1_NO:
      {
          if(PORTA1_GetVal())
          {
              *DiIf_ValuePtr = DIIF_VALUE_HIGH;    
          }
          else
          {
               *DiIf_ValuePtr = DIIF_VALUE_LOW;     
          }
          break;
      }
      case DIIF_CPUDI1_NO:
      {
          if(CPUDI1_GetVal())
          {
              *DiIf_ValuePtr = DIIF_VALUE_HIGH;    
          }
          else
          {
               *DiIf_ValuePtr = DIIF_VALUE_LOW;     
          }
          break;
      }
      case DIIF_CPUDI2_NO:
      {
          if(CPUDI2_GetVal())
          {
              *DiIf_ValuePtr = DIIF_VALUE_HIGH;    
          }
          else
          {
               *DiIf_ValuePtr = DIIF_VALUE_LOW;     
          }
          break;
      } 

      case DIIF_CPUDI3_NO:
      {
          if(CPUDI3_GetVal())
          {
              *DiIf_ValuePtr = DIIF_VALUE_HIGH;    
          }
          else
          {
               *DiIf_ValuePtr = DIIF_VALUE_LOW;     
          }
          break;
      }
      case DIIF_CPUDI4_NO:
      {
          if(CPUDI4_GetVal())
          {
              *DiIf_ValuePtr = DIIF_VALUE_HIGH;    
          }
          else
          {
               *DiIf_ValuePtr = DIIF_VALUE_LOW;     
          }
          break;
      }
    /*  case DIIF_OV_POWER_NO:
      {
          if(OV_Power_GetVal())
          {
              *DiIf_ValuePtr = DIIF_VALUE_HIGH;    
          }
          else
          {
               *DiIf_ValuePtr = DIIF_VALUE_LOW;     
          }
          break;
      }
      case DIIF_UV_POWER_NO:
      {
          if(UV_Power_GetVal())
          {
              *DiIf_ValuePtr = DIIF_VALUE_HIGH;    
          }
          else
          {
               *DiIf_ValuePtr = DIIF_VALUE_LOW;     
          }
          break;
      } */
      case DIIF_PORTA2_NO:
      {
          if(PORTA2_GetVal())
          {
              *DiIf_ValuePtr = DIIF_VALUE_HIGH;    
          }
          else
          {
               *DiIf_ValuePtr = DIIF_VALUE_LOW;     
          }
          break;
      }
      default:break;
   }
    return rslt;
}



/*
GeneralReturnType DiIf_GetValue(uByte DiIf_Id,bool *DiIf_ValuePtr)
{
   GeneralReturnType rslt;
   sDiIf_PropertyType *DiIf_PropPtr;
   const sDiIf_OperationType *DiIf_OperaPtr;
   
   rslt = (GeneralReturnType)ERR_OK;
   
   if(DiIf_Id >= DIIF_MAX_NUM) 
   {
       rslt = (GeneralReturnType)ERR_IO_RANGE;
       return rslt;
   }
   if(NULL == DiIf_ValuePtr) 
   { 
       rslt = (GeneralReturnType)ERR_IO_VALUE_PTR_NULL;
       return rslt;
   } 
   DiIf_PropPtr = GET_DIPROPERTY(DiIf_Id);  //point to property
   DiIf_OperaPtr = GET_DIOPERATION(DiIf_Id); //point to operation
   
            if((uByte)(DiIf_PropPtr->DiIf_IdNum) == DiIf_Id) 
            { 
                if(DiIf_OperaPtr->DiIf_GetValPtr != NULL) 
                {
                    if(DIIF_NORMAL_LOGIC_TYPE == DiIf_PropPtr->DiIf_LogicType) 
                    {  
                        if((*DiIf_OperaPtr->DiIf_GetValPtr)()) 
                        {
                           *DiIf_ValuePtr = DIIF_VALUE_HIGH;
                           DiIf_PropPtr->DiIf_Value = DIIF_VALUE_HIGH;
                        } 
                        else 
                        {
                           *DiIf_ValuePtr = DIIF_VALUE_LOW;
                           DiIf_PropPtr->DiIf_Value = DIIF_VALUE_LOW;
                        }
                    } 
                    else 
                    {
                        if((*DiIf_OperaPtr->DiIf_GetValPtr)()) 
                        {
                           *DiIf_ValuePtr = DIIF_VALUE_LOW;
                           DiIf_PropPtr->DiIf_Value = DIIF_VALUE_LOW;
                        } 
                        else 
                        {
                           *DiIf_ValuePtr = DIIF_VALUE_HIGH;
                           DiIf_PropPtr->DiIf_Value = DIIF_VALUE_HIGH;
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
