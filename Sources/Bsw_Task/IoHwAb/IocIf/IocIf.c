//#include "IocIf_Type.h"
#include "IocIf.h"
//#include "AC_CP_IOC.h"


#define  LOW_TO_HIGH_LEVEL  0   //rising trigger
#define  HIGH_TO_LOW_LEVEL  1   //falling trigger
#define  TIMECNTFULLVALUE  0xffff  //the timer count is overflow value
#define  TIMEMS_PER_CNT    375     //the time value of per 1 count
#define  TIMEMS_TO_US      1000
#define  IOC_ONEHUNDRED_PERCENT  1000


#define FLAG_FAIL  0
#define FLAG_OK    1

uWord gby_IocTriggerType = LOW_TO_HIGH_LEVEL;  //the trigger type,the first trigger is rissing 
uWord gwd_IocLowToHighValue = 0;    //the rising trigger value
uWord gwd_IocHighToLowValue = 0;    //the falling trigger value
uLWord glwd_IocLowLevelCnt = 0;    //the ioc high level period
uLWord glwd_IocHighLevelCnt = 0;   //the ioc low level period

uByte gby_readcnt = 0;
uWord gwd_value[4] = {0,0,0,0};
uByte gby_iocflag = 0;
uByte gby_ioctimecnt = 0;

uByte gby_ioc_max_err = 0;
uByte gby_ioc_err = 0;
#define  CP_PERIOD_UP_LIMIT   1200
#define  CP_PERIOD_DOWN_LIMIT   800
#define IOC_CURRENT_DUTY  95
#define MAX_DET_HIGHLEVEL_CNT  20
uLWord glwd_highcnt = 0;
 

GeneralReturnType IocIf_Enable(uByte IocIf_Id)  
{
    GeneralReturnType rslt;
    uByte Ioc_rslt;

    rslt = (GeneralReturnType)ERR_OK;
    if(IocIf_Id >= IOCIF_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;
    }
    
    Ioc_rslt = AC_CP_IOC_Enable(); 
    if(Ioc_rslt != ERR_OK) 
    {
        return (GeneralReturnType)Ioc_rslt;
    }
    return rslt;
    
    
    
  /*  
    GeneralReturnType rslt;
    uByte Ioc_rslt;
    const sIocIf_PropertyType *IocIf_PropPtr;
    const sIocIf_OperationType *IocIf_OperaPtr;

    rslt = (GeneralReturnType)ERR_OK;
    if(IocIf_Id >= IOCIF_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;
    }
    
    IocIf_PropPtr = GET_IOCPROPERTY(IocIf_Id);   //point to property
    IocIf_OperaPtr = GET_IOCOPERATION(IocIf_Id); //point to operation

    if((uByte)(IocIf_PropPtr->IocIf_IdNum) == IocIf_Id) 
    {
        if(IocIf_OperaPtr->IocIf_EnablePtr != NULL) 
        {
            Ioc_rslt = (*IocIf_OperaPtr->IocIf_EnablePtr)(); //the function output the value
            if(Ioc_rslt != ERR_OK) 
            {
                return (GeneralReturnType)Ioc_rslt;
            }
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
    
  */
    
}

GeneralReturnType IocIf_Disable(uByte IocIf_Id)  
{
    GeneralReturnType rslt;
    uByte Ioc_rslt;

    rslt = (GeneralReturnType)ERR_OK;
    if(IocIf_Id >= IOCIF_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;
    }
    
    Ioc_rslt = AC_CP_IOC_Disable(); //the function output the value
    if(Ioc_rslt != ERR_OK) 
    {
        return (GeneralReturnType)Ioc_rslt;
    } 

    return rslt;

  /*  
    GeneralReturnType rslt;
    uByte Ioc_rslt;
    const sIocIf_PropertyType *IocIf_PropPtr;
    const sIocIf_OperationType *IocIf_OperaPtr;

    rslt = (GeneralReturnType)ERR_OK;
    if(IocIf_Id >= IOCIF_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;
    }
    
    IocIf_PropPtr = GET_IOCPROPERTY(IocIf_Id);   //point to property
    IocIf_OperaPtr = GET_IOCOPERATION(IocIf_Id); //point to operation

    if((uByte)(IocIf_PropPtr->IocIf_IdNum) == IocIf_Id) 
    {
        if(IocIf_OperaPtr->IocIf_DisablePtr != NULL) 
        {
            Ioc_rslt = (*IocIf_OperaPtr->IocIf_DisablePtr)(); //the function output the value
            if(Ioc_rslt != ERR_OK) 
            {
                return (GeneralReturnType)Ioc_rslt;
            }
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
  */  
  
}

GeneralReturnType IocIf_EnableEvent(uByte IocIf_Id) 
{
    GeneralReturnType rslt;
    uByte Ioc_rslt;

    rslt = (GeneralReturnType)ERR_OK;
    if(IocIf_Id >= IOCIF_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;
    }
    Ioc_rslt = AC_CP_IOC_EnableEvent(); //the function output the value
    if(Ioc_rslt != ERR_OK) 
    {
        return (GeneralReturnType)Ioc_rslt;
    }
    return rslt;
    
    
   /* 
    GeneralReturnType rslt;
    uByte Ioc_rslt;
    const sIocIf_PropertyType *IocIf_PropPtr;
    const sIocIf_OperationType *IocIf_OperaPtr;  

    rslt = (GeneralReturnType)ERR_OK;
    if(IocIf_Id >= IOCIF_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;
    }
    
    IocIf_PropPtr = GET_IOCPROPERTY(IocIf_Id);   //point to property
    IocIf_OperaPtr = GET_IOCOPERATION(IocIf_Id); //point to operation

    if((uByte)(IocIf_PropPtr->IocIf_IdNum) == IocIf_Id) 
    {
        if(IocIf_OperaPtr->IocIf_EnableEventPtr != NULL) 
        {
            Ioc_rslt = (*IocIf_OperaPtr->IocIf_EnableEventPtr)(); //the function output the value
            if(Ioc_rslt != ERR_OK) 
            {
                return (GeneralReturnType)Ioc_rslt;
            }
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
  */
}

GeneralReturnType IocIf_DisableEvent(uByte IocIf_Id) 
{
    GeneralReturnType rslt;
    uByte Ioc_rslt;

    rslt = (GeneralReturnType)ERR_OK;
    if(IocIf_Id >= IOCIF_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;
    }
    Ioc_rslt = AC_CP_IOC_DisableEvent(); //the function output the value
    if(Ioc_rslt != ERR_OK) 
    {
        return (GeneralReturnType)Ioc_rslt;
    }
    return rslt;
    
    
   /* 
    GeneralReturnType rslt;
    uByte Ioc_rslt;
    const sIocIf_PropertyType *IocIf_PropPtr;
    const sIocIf_OperationType *IocIf_OperaPtr;

    rslt = (GeneralReturnType)ERR_OK;
    if(IocIf_Id >= IOCIF_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;
    }
    
    IocIf_PropPtr = GET_IOCPROPERTY(IocIf_Id);   //point to property
    IocIf_OperaPtr = GET_IOCOPERATION(IocIf_Id); //point to operation
    
    if((uByte)(IocIf_PropPtr->IocIf_IdNum) == IocIf_Id) 
    {
        if(IocIf_OperaPtr->IocIf_DisableEventPtr != NULL) 
        {
            Ioc_rslt = (*IocIf_OperaPtr->IocIf_DisableEventPtr)(); //the function output the value
            if(Ioc_rslt != ERR_OK) 
            {
                return (GeneralReturnType)Ioc_rslt;
            }
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
  */
}


GeneralReturnType IocIf_Init(void)
{
   GeneralReturnType rslt;
   
   rslt = IocIf_EnableEvent((uByte)IOCIF_AC_CP_NO);
   rslt = IocIf_Enable((uByte)IOCIF_AC_CP_NO);
   ECT_TCTL3 &= 0xF3;
   ECT_TCTL3 |= 0x04;
   return rslt;
}

void IocIf_Task(uByte IocIf_Id) 
{
    gwd_value[gby_readcnt] = 0;

    if(LOW_TO_HIGH_LEVEL == gby_IocTriggerType)   //the rising trigger if comming
    {
        
        ECT_TCTL3 &= 0xF3;
        ECT_TCTL3 |= 0x08; //change to falling trigger
        gby_IocTriggerType = HIGH_TO_LOW_LEVEL;
    } 
    else  //the falling trigger if comming
    {
        ECT_TCTL3 &= 0xF3;
        ECT_TCTL3 |= 0x04;//change to rising trigger
        gby_IocTriggerType = LOW_TO_HIGH_LEVEL; 
    }
    gwd_value[gby_readcnt] = ECT_TC5;
    gby_readcnt++;
    if(gby_readcnt >= 4)
    {
       gby_readcnt = 0;
       gby_iocflag = 1;
       ECT_TCTL3 &= 0xf3; //stop trigger 
    } 
}

GeneralReturnType IocIf_GetIocValue(uByte IocIf_Id,uWord *IocIf_PeriodValuePtr,uByte *IocIf_RatioValuePtr) 
{
    GeneralReturnType rslt;
    uByte lby_IocRatioBak;
    uWord lwd_IocPeriodBak;
    uLWord llwd_period[2],llwd_avrperiod,llwd_period_cnt[2],llwd_highcnt[2];
   // const sIocIf_PropertyType *IocIf_PropPtr;
    
    rslt = (GeneralReturnType)ERR_OK;
    llwd_period[0] = 0;
    llwd_period[1] = 0;
    llwd_period_cnt[0] = 0;
    llwd_period_cnt[1] = 0;
    llwd_avrperiod = 0;
    llwd_highcnt[0] = 0;
    llwd_highcnt[1] = 0;
    
    if(IocIf_Id >= IOCIF_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;
    }
    
    if((NULL == IocIf_PeriodValuePtr) || (NULL == IocIf_RatioValuePtr)) 
    {
        rslt = (GeneralReturnType)ERR_IO_VALUE_PTR_NULL;
        return rslt;
    }
    
    if(1 == gby_iocflag)
    {
        //IocIf_PropPtr = GET_IOCPROPERTY(IocIf_Id);   //point to property
        if(IOCIF_AC_CP_NO == IocIf_Id) 
        { 
            if(gwd_value[2] >= gwd_value[0])
            {
                llwd_period[0] = gwd_value[2] - gwd_value[0]; 
            }
            else
            {
                llwd_period[0] = gwd_value[2];
                llwd_period[0] += TIMECNTFULLVALUE;
                llwd_period[0] -= gwd_value[0];
            }
            llwd_period_cnt[0] = llwd_period[0];
            llwd_period[0] *= TIMEMS_TO_US;
            llwd_period[0] /= TIMEMS_PER_CNT;
            if((llwd_period[0] > CP_PERIOD_UP_LIMIT) || (llwd_period[0] < CP_PERIOD_DOWN_LIMIT))
            { 
                gby_iocflag = 0;
                ECT_TCTL3 &= 0xf3;
                ECT_TCTL3 |= 0x04;
                return ERR_IO_CP_NOT_UPDATE;
            }
            if(gwd_value[3] >= gwd_value[1])
            {
                llwd_period[1] = gwd_value[3] - gwd_value[1]; 
            }
            else
            {
                llwd_period[1] = gwd_value[3];
                llwd_period[1] += TIMECNTFULLVALUE;
                llwd_period[1] -= gwd_value[1];
            } 
            llwd_period_cnt[1] = llwd_period[1];
            llwd_period[1] *= TIMEMS_TO_US;
            llwd_period[1] /= TIMEMS_PER_CNT;
            if((llwd_period[1] > CP_PERIOD_UP_LIMIT) || (llwd_period[1] < CP_PERIOD_DOWN_LIMIT))
            { 
                gby_iocflag = 0;
                ECT_TCTL3 &= 0xf3;
                ECT_TCTL3 |= 0x04;
                return ERR_IO_CP_NOT_UPDATE;
            }
            llwd_avrperiod = llwd_period[0] + llwd_period[1];
            llwd_avrperiod /= 2;
            if((llwd_avrperiod > CP_PERIOD_UP_LIMIT) || (llwd_avrperiod < CP_PERIOD_DOWN_LIMIT))
            { 
                gby_iocflag = 0;
                ECT_TCTL3 &= 0xf3;
                ECT_TCTL3 |= 0x04;
                return ERR_IO_CP_NOT_UPDATE;
            }
            
            if(gwd_value[1] >= gwd_value[0])
            {
                llwd_highcnt[0] = gwd_value[1] - gwd_value[0]; 
            }
            else
            {
                llwd_highcnt[0] = gwd_value[1];
                llwd_highcnt[0] += TIMECNTFULLVALUE;
                llwd_highcnt[0] -= gwd_value[0];
            }
            if(gwd_value[3] >= gwd_value[2])
            {
                llwd_highcnt[1] = gwd_value[3] - gwd_value[2]; 
            }
            else
            {
                llwd_highcnt[1] = gwd_value[3];
                llwd_highcnt[1] += TIMECNTFULLVALUE;
                llwd_highcnt[1] -= gwd_value[2];
            } 
            
            if((llwd_highcnt[1] - llwd_highcnt[0]) > MAX_DET_HIGHLEVEL_CNT)
            {
                gby_iocflag = 0;
                ECT_TCTL3 &= 0xf3;
                ECT_TCTL3 |= 0x04;
                return ERR_IO_CP_NOT_UPDATE;
            }

            llwd_period_cnt[0] += llwd_period_cnt[1];
            llwd_period_cnt[0] /= 2;
            
            llwd_highcnt[0] += llwd_highcnt[1];
            llwd_highcnt[0] /= 2;
           // if((uByte)(IocIf_PropPtr->IocIf_Logic) == IOCIF_ABNORMAL_LOGIC_TYPE)
            //{
            llwd_highcnt[0] = llwd_period_cnt[0] - llwd_highcnt[0];
           // }
            //glwd_highcnt = llwd_highcnt[0];
            llwd_highcnt[0] *= IOC_ONEHUNDRED_PERCENT; 
            if(llwd_period_cnt[0] != 0)
            {
                llwd_highcnt[0] /= llwd_period_cnt[0]; 
                if((llwd_highcnt[0] % 10) >= 5)
                {
                   llwd_highcnt[0] /= 10;
                   llwd_highcnt[0]++;
                }
                else
                {
                   llwd_highcnt[0] /= 10;
                }
                
                *IocIf_RatioValuePtr = (uWord)(llwd_highcnt[0]);
                *IocIf_PeriodValuePtr = (uWord)llwd_period[0]; 
            }  
            else
            {
                rslt = (GeneralReturnType)ERR_IO_CP_NOT_UPDATE;
            }
        } 
        else 
        {
            rslt = (GeneralReturnType)ERR_IO_ID_NUM_NOTMATCH;   //the id number is not match
        }
        gby_iocflag = 0;
        ECT_TCTL3 &= 0xf3;
        ECT_TCTL3 |= 0x04;
    }
    else
    {
        rslt = (GeneralReturnType)ERR_IO_CP_NOT_UPDATE;
    } 
    return rslt;
}

 


