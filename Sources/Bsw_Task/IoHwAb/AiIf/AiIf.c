/*****************************************************************************/                                                                                                                                  
/* FILE NAME : AiIf.c                                                        */                                             
/*---------------------------------------------------------------------------*/                                                        
/* AUTHOR : PHQ                                                              */                                               
/*---------------------------------------------------------------------------*/                                              
/* TIME : 15/10                                                              */                                                                    
/*---------------------------------------------------------------------------*/                                             
/* DESCRIPTION : 衔接应用层与底层模块，在应用层中所有对Ai端口的操作只通过调用*/                                                      
/*               AiIf中间层中的接口函数即可                                  */                                                        
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* HISTORY :                                                                 */
/*                                                                           */
/*    Author    |    Time    |    Version    |    description                */
/* -------------|------------|---------------|------------------------------ */
/*    PHQ       |    15/10   |    1.0        |    build the module           */
/*****************************************************************************/
#include "AiIf.h"



GeneralReturnType AiIf_MeasureChannel(uByte AiIf_ChannelId,uWord *AiIf_AdValuePtr)
{
   GeneralReturnType rslt;
   uByte Ai_rslt;
  // const sAiIf_PropertyType *AiIf_PropPtr;
   //const sAiIf_OperationType *AiIf_OperaPtr;
   
   rslt = (GeneralReturnType)ERR_OK;
   
   if(AiIf_ChannelId >= AIIF_MAX_CHANNEL_NUM)  // the channel number is out of the range
   {
       rslt = (GeneralReturnType)ERR_IO_RANGE;
       return rslt;
   }
   if(NULL == AiIf_AdValuePtr) 
   {
       rslt = (GeneralReturnType)ERR_IO_VALUE_PTR_NULL;
       return rslt;
   }
   
   if(AiIf_ChannelId < 16)  //channel0~channel14---AI1
   {
       Ai_rslt = AI1_Enable(); //enable ad measure
    	 if(Ai_rslt != ERR_OK) 
    	 {
    	     return (GeneralReturnType)Ai_rslt;
    	 }
    	 Ai_rslt = AI1_MeasureChanWait(AiIf_ChannelId);  //start ad and measure
    	 if(Ai_rslt != ERR_OK) 
    	 {
    	     return (GeneralReturnType)Ai_rslt;
    	 }
    	 Ai_rslt = AI1_GetChanValue(AiIf_ChannelId,AiIf_AdValuePtr); //read ad value
    	 if(Ai_rslt != ERR_OK) 
    	 {
    	     return (GeneralReturnType)Ai_rslt;
    	 }
    	 Ai_rslt = AI1_Disable(); //disbale ad measure
       if(Ai_rslt != ERR_OK) 
    	 {
    	     return (GeneralReturnType)Ai_rslt;
    	 }

   }
   else if((AiIf_ChannelId > 15) && (AiIf_ChannelId <= 21))   //channel15~channel16 AI2
   {
       Ai_rslt = AI2_Enable(); //enable ad measure
    	 if(Ai_rslt != ERR_OK) 
    	 {
    	     return (GeneralReturnType)Ai_rslt;
    	 }
    	 Ai_rslt = AI2_MeasureChanWait(AiIf_ChannelId - 16);  //start ad and measure
    	 if(Ai_rslt != ERR_OK) 
    	 {
    	     return (GeneralReturnType)Ai_rslt;
    	 }
    	 Ai_rslt = AI2_GetChanValue((AiIf_ChannelId - 16),AiIf_AdValuePtr); //read ad value
    	 if(Ai_rslt != ERR_OK) 
    	 {
    	     return (GeneralReturnType)Ai_rslt;
    	 }
    	 Ai_rslt = AI2_Disable(); //disbale ad measure
       if(Ai_rslt != ERR_OK) 
    	 {
    	     return (GeneralReturnType)Ai_rslt;
    	 }   
   }

   return rslt;
   
}




/*
GeneralReturnType AiIf_MeasureChannel(uByte AiIf_ChannelId,uWord *AiIf_AdValuePtr)
{
   GeneralReturnType rslt;
   uByte Ai_rslt;
   const sAiIf_PropertyType *AiIf_PropPtr;
   const sAiIf_OperationType *AiIf_OperaPtr;
   
   rslt = (GeneralReturnType)ERR_OK;
   
   if(AiIf_ChannelId >= AIIF_MAX_CHANNEL_NUM)  // the channel number is out of the range
   {
       rslt = (GeneralReturnType)ERR_IO_RANGE;
       return rslt;
   }
   if(NULL == AiIf_AdValuePtr) 
   {
       rslt = (GeneralReturnType)ERR_IO_VALUE_PTR_NULL;
       return rslt;
   }
   AiIf_PropPtr = GET_AIPROPERTY(AiIf_ChannelId);  //get the channel property pointer
   AiIf_OperaPtr = GET_AIOPERATION((uByte)(AiIf_PropPtr->AiIf_AiIdNum));  //get the channel operation pointer

   if((uByte)(AiIf_PropPtr->AiIf_ChannelIdNum) == AiIf_ChannelId) //match the AI number and the channel number
   { 
       if((AiIf_OperaPtr->AiIf_EnablePtr != NULL) 
       && (AiIf_OperaPtr->AiIf_MeasureChanWaitPtr != NULL)
       && (AiIf_OperaPtr->AiIf_GetChanValuePtr != NULL)
       && (AiIf_OperaPtr->AiIf_DisablePtr != NULL)) 
       {  
        	 Ai_rslt = (*AiIf_OperaPtr->AiIf_EnablePtr)(); //enable ad measure
        	 if(Ai_rslt != ERR_OK) 
        	 {
        	     return (GeneralReturnType)Ai_rslt;
        	 }
        	 Ai_rslt = (*AiIf_OperaPtr->AiIf_MeasureChanWaitPtr)((uByte)(AiIf_PropPtr->AiIf_AdcChannelIdNum));  //start ad and measure
        	 if(Ai_rslt != ERR_OK) 
        	 {
        	     return (GeneralReturnType)Ai_rslt;
        	 }
        	 Ai_rslt = (*AiIf_OperaPtr->AiIf_GetChanValuePtr)((uByte)(AiIf_PropPtr->AiIf_AdcChannelIdNum),AiIf_AdValuePtr); //read ad value
        	 if(Ai_rslt != ERR_OK) 
        	 {
        	     return (GeneralReturnType)Ai_rslt;
        	 }
        	 Ai_rslt = (*AiIf_OperaPtr->AiIf_DisablePtr)(); //disbale ad measure
           if(Ai_rslt != ERR_OK) 
        	 {
        	     return (GeneralReturnType)Ai_rslt;
        	 }
       } 
       else 
       {
           rslt = (GeneralReturnType)ERR_IO_FUNCTION_PTR_NULL;  //the pointer of channel function is null 
       }
   } 
   else 
   {
       rslt = (GeneralReturnType)ERR_IO_ID_NUM_NOTMATCH;   //the pointer of the channel id is not match
   }

   return rslt;
   
}

*/