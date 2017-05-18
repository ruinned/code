// Can_Lcfg.c
#include "Can_Lcfg.h"
#include "CAN4.h"
#include "CPUCAN_INT_EN.h"
#include "Ecu.h"
#ifdef CAR_CAN_EN
#include "CPU_CAR_CAN_STB.h"
#include "CPUCAN_CAR_EN.h"
#include "CAN0.h"
#ifdef CHG_CAN_EN
#include "CAN1.h"
#include "CHG_CAN_POW.h" 
#endif
#endif

static void CAN0_SetBaudrate(uByte Baudrate)
{
    /* CAN0CTL1: CANE=1,CLKSRC=0,LOOPB=0,LISTEN=0,BORM=0,WUPM=0,SLPAK=0,INITAK=0 */
    CAN0CTL1 = 0x80U;                    /* Set the control register */
    /* CAN0CTL0: RXFRM=0,RXACT=0,CSWAI=0,SYNCH=0,TIME=0,WUPE=0,SLPRQ=0,INITRQ=1 */
    CAN0CTL0 = 0x01U;

    switch(Baudrate)
    {
        case 0: //0 is 250k
        {
             /* CAN0BTR0: SJW1=0,SJW0=0,BRP5=0,BRP4=0,BRP3=1,BRP2=0,BRP1=1,BRP0=1 */
    	      CAN0BTR0 = 0x0BU;                    /* Set the device timing register */
             /* CAN0BTR1: SAMP=0,TSEG22=0,TSEG21=1,TSEG20=0,TSEG13=1,TSEG12=0,TSEG11=1,TSEG10=1 */
            CAN0BTR1 = 0x2BU;                    /* Set the device timing register */
            break;
        }
        case 1: //1 is 500k
        {
            /* CAN0BTR0: SJW1=0,SJW0=0,BRP5=0,BRP4=0,BRP3=0,BRP2=1,BRP1=0,BRP0=1 */
    	      CAN0BTR0 = 0x05U;                    /* Set the device timing register */
            /* CAN0BTR1: SAMP=0,TSEG22=0,TSEG21=1,TSEG20=0,TSEG13=1,TSEG12=0,TSEG11=1,TSEG10=1 */
            CAN0BTR1 = 0x2BU;                    /* Set the device timing register */
            break;
        }
        default : //default is 500k
        {
           /* CAN0BTR0: SJW1=0,SJW0=0,BRP5=0,BRP4=0,BRP3=0,BRP2=1,BRP1=0,BRP0=1 */
    	     CAN0BTR0 = 0x05U;                    /* Set the device timing register */
           /* CAN0BTR1: SAMP=0,TSEG22=0,TSEG21=1,TSEG20=0,TSEG13=1,TSEG12=0,TSEG11=1,TSEG10=1 */
           CAN0BTR1 = 0x2BU;                    /* Set the device timing register */
           break;
        } 
    }
    CAN0CTL1_CLKSRC = 0x01U;             /* Select the clock source from bus clock */
    CAN0CTL0_INITRQ = 0x01U;             /* Disable device */
    while(CAN0CTL1_INITAK == 0U) {}      /* Wait for device initialization acknowledge */
}

static void CAN1_SetBaudrate(uByte Baudrate)
{
    /* CAN1CTL1: CANE=1,CLKSRC=0,LOOPB=0,LISTEN=0,BORM=0,WUPM=0,SLPAK=0,INITAK=0 */
    CAN1CTL1 = 0x80U;                    /* Set the control register */
    /* CAN1CTL0: RXFRM=0,RXACT=0,CSWAI=0,SYNCH=0,TIME=0,WUPE=0,SLPRQ=0,INITRQ=1 */
    CAN1CTL0 = 0x01U;                    /* Set the control register */

    switch(Baudrate)
    {
        case 0: //0 is 250k
        {
             /* CAN1BTR0: SJW1=0,SJW0=0,BRP5=0,BRP4=0,BRP3=1,BRP2=0,BRP1=1,BRP0=1 */
             CAN1BTR0 = 0x0BU;                    /* Set the device timing register */
             /* CAN1BTR1: SAMP=0,TSEG22=0,TSEG21=1,TSEG20=0,TSEG13=1,TSEG12=0,TSEG11=1,TSEG10=1 */
             CAN1BTR1 = 0x2BU;                    /* Set the device timing register */
             break;
        }
        case 1:   //1 is 500k
        {
             /* CAN1BTR0: SJW1=0,SJW0=0,BRP5=0,BRP4=0,BRP3=0,BRP2=1,BRP1=0,BRP0=1 */
             CAN1BTR0 = 0x05U;                    /* Set the device timing register */
             /* CAN1BTR1: SAMP=0,TSEG22=0,TSEG21=1,TSEG20=0,TSEG13=1,TSEG12=0,TSEG11=1,TSEG10=1 */
             CAN1BTR1 = 0x2BU;
             break;
        }
        default : //default is 500k
        {
             /* CAN1BTR0: SJW1=0,SJW0=0,BRP5=0,BRP4=0,BRP3=0,BRP2=1,BRP1=0,BRP0=1 */
             CAN1BTR0 = 0x05U;                    /* Set the device timing register */
             /* CAN1BTR1: SAMP=0,TSEG22=0,TSEG21=1,TSEG20=0,TSEG13=1,TSEG12=0,TSEG11=1,TSEG10=1 */
             CAN1BTR1 = 0x2BU;
             break;
        } 
    }
    CAN1CTL1_CLKSRC = 0x01U;             /* Select the clock source from bus clock */
    CAN1CTL0_INITRQ = 0x01U;             /* Disable device */
    while(CAN1CTL1_INITAK == 0U) {}      /* Wait for device initialization acknowledge */ 
}

static void CAN4_SetBaudrate(uByte Baudrate)
{
    /* CAN4CTL1: CANE=1,CLKSRC=0,LOOPB=0,LISTEN=0,BORM=0,WUPM=0,SLPAK=0,INITAK=0 */
    CAN4CTL1 = 0x80U;                    /* Set the control register */
    /* CAN4CTL0: RXFRM=0,RXACT=0,CSWAI=0,SYNCH=0,TIME=0,WUPE=0,SLPRQ=0,INITRQ=1 */
    CAN4CTL0 = 0x01U;

    switch(Baudrate)
    {
        case 0: // 0 is 250k
        {
             /* CAN4BTR0: SJW1=0,SJW0=1,BRP5=0,BRP4=0,BRP3=1,BRP2=0,BRP1=1,BRP0=1 */
             CAN4BTR0 = 0x4BU;                    /* Set the device timing register */
             /* CAN4BTR1: SAMP=0,TSEG22=0,TSEG21=1,TSEG20=0,TSEG13=1,TSEG12=0,TSEG11=1,TSEG10=1 */
             CAN4BTR1 = 0x2BU;                    /* Set the device timing register */
             break;
        }
        case 1:  // 1 is 500k
        {
             /* CAN4BTR0: SJW1=0,SJW0=1,BRP5=0,BRP4=0,BRP3=0,BRP2=1,BRP1=0,BRP0=1 */
             CAN4BTR0 = 0x45U;                    /* Set the device timing register */
             /* CAN4BTR1: SAMP=0,TSEG22=0,TSEG21=1,TSEG20=0,TSEG13=1,TSEG12=0,TSEG11=1,TSEG10=1 */
             CAN4BTR1 = 0x2BU;                    /* Set the device timing register */
             break;
        }
        default :  // default is 500k
        {
             /* CAN4BTR0: SJW1=0,SJW0=1,BRP5=0,BRP4=0,BRP3=0,BRP2=1,BRP1=0,BRP0=1 */
             CAN4BTR0 = 0x45U;                    /* Set the device timing register */
             /* CAN4BTR1: SAMP=0,TSEG22=0,TSEG21=1,TSEG20=0,TSEG13=1,TSEG12=0,TSEG11=1,TSEG10=1 */
             CAN4BTR1 = 0x2BU;
             break;
        } 
    }
    CAN4CTL1_CLKSRC = 0x01U;             /* Select the clock source from bus clock */
    CAN4CTL0_INITRQ = 0x01U;             /* Disable device */
    while(CAN4CTL1_INITAK == 0U) {}      /* Wait for device initialization acknowledge */ 
}



void CAN4_EnableEvent_Func(void)
{
    CAN4_EnableEvent();
}

void CAN4_DisableEvent_Func(void)
{
    CAN4_DisableEvent();
}
#ifdef CAR_CAN_EN

void CAN0_EnableEvent_Func(void)
{
   CAN0_EnableEvent();
}

void CAN0_DisableEvent_Func(void)
{
   CAN0_DisableEvent();
}


#ifdef CHG_CAN_EN
 void CAN1_EnableEvent_Func(void)
{
   CAN1_EnableEvent();
}

void CAN1_DisableEvent_Func(void)
{
   CAN1_DisableEvent();
}


#endif
#endif

eCan_HWBufStatusType CAN4_GetRxStatus(void)
{
    // call pe code
    if(CAN4_GetStateRX())
    {
        return CAN_HWBUF_HAD_DATE;
    }
    else
    {
        return CAN_HWBUF_EMPTY;
    }
}

eCan_HWBufStatusType CAN4_GetTxStatus(void)
{
    return (eCan_HWBufStatusType)0;
}

static const sCan_RxTxHandleType Can_RxTxHandle[] =
{
    {
        CAN4_ReadFrame,
        CAN4_SendFrame,
    },
#ifdef CAR_CAN_EN
    {
        CAN0_ReadFrame,
        CAN0_SendFrame,
    },
#ifdef CHG_CAN_EN
    {
        CAN1_ReadFrame,
        CAN1_SendFrame,
    },
#endif
#endif

} ;

/* redefine Enable and Disable */
uByte CAN4_EnableMSCANandHw(void)
{
   uByte rslt;
   rslt = CAN4_Enable();
   
   /* enable TJA1051 */
   CPUCAN_INT_EN_PutVal(FALSE);
   
   return rslt;
}

uByte CAN4_DisableMSCANandHw(void)
{
   uByte rslt;
   rslt = CAN4_Disable();
   
   /* enable TJA1051 */
   CPUCAN_INT_EN_PutVal(TRUE);
   return rslt;
}

#ifdef CAR_CAN_EN
uByte CAN0_EnableMSCANandHw(void)
{
   uByte rslt;
   rslt = CAN0_Enable();
   
   /* enable TJA1051 */
   CPUCAN_CAR_EN_PutVal(TRUE);
   return rslt;
}

uByte CAN0_DisableMSCANandHw(void)
{
   uByte rslt;
   
   rslt = CAN0_Disable();
   
   /* enable TJA1051 */
   CPUCAN_CAR_EN_PutVal(FALSE);
   
   return  rslt;
}

#ifdef CHG_CAN_EN
uByte CAN1_EnableMSCANandHw(void)
{

   uByte rslt;
   rslt = CAN1_Enable();
   
   CHG_CAN_POW_PutVal(FALSE);
   
   return rslt;
   
   /* enable TJA1051 */
   // default enable by hardware
}

uByte CAN1_DisableMSCANandHw(void)
{
   uByte rslt;
   rslt = CAN1_Disable();
   
   CHG_CAN_POW_PutVal(TRUE);
   
   return rslt;
   /* enable TJA1051 */
   // default enable by hardware
}
#endif
#endif


static const sCan_CtrlHandleType Can_CtrlHandle[] =
{
    {
        CAN4_EnableMSCANandHw,
        CAN4_DisableMSCANandHw,
        CAN4_EnableEvent_Func,
        CAN4_DisableEvent_Func,
        CAN4_SetAcceptanceCode,
        CAN4_SetAcceptanceMask,
        CAN4_SetBaudrate,
    } ,
#ifdef CAR_CAN_EN
    {
        CAN0_EnableMSCANandHw,
        CAN0_DisableMSCANandHw,
        CAN0_EnableEvent_Func,
        CAN0_DisableEvent_Func,
        CAN0_SetAcceptanceCode,
        CAN0_SetAcceptanceMask,
        CAN0_SetBaudrate,
    } ,
#ifdef CHG_CAN_EN
    {
        CAN1_EnableMSCANandHw,
        CAN1_DisableMSCANandHw,
        CAN1_EnableEvent_Func,
        CAN1_DisableEvent_Func,
        CAN1_SetAcceptanceCode,
        CAN1_SetAcceptanceMask,
        CAN1_SetBaudrate,
    } ,
#endif // CHG_CAN_EN     
#endif // CAR_CAN_EN
};


void Can4_WakeAndSleepCtrl(uByte mode)
{

}

#ifdef CAR_CAN_EN
void Can0_WakeAndSleepCtrl(uByte mode)
{
    CPU_CAR_CAN_STB_PutVal(mode);
}

#ifdef CHG_CAN_EN
void Can1_WakeAndSleepCtrl(uByte modevoid)
{
}


#endif

#endif



sCan_NmCtrlType  Can_NmCtrl[] =
{
   {
     Can4_WakeAndSleepCtrl 
   },
#ifdef CAR_CAN_EN
   {
    Can0_WakeAndSleepCtrl  
   },

#ifdef CHG_CAN_EN
{
   Can1_WakeAndSleepCtrl
   
},

#endif

#endif

};


const sCan_HandleType Can_Handle[] =
{
    {
        &(Can_RxTxHandle[0]),
        &(Can_CtrlHandle[0]),
        &(Can_NmCtrl[0])
    },
#ifdef CAR_CAN_EN
    {
        &(Can_RxTxHandle[1]),
        &(Can_CtrlHandle[1]),
         &(Can_NmCtrl[1])
    },
#ifdef CHG_CAN_EN
    {
        &(Can_RxTxHandle[2]),
        &(Can_CtrlHandle[2]),
         &(Can_NmCtrl[2])
    },
#endif // CHG_CAN_EN     
#endif // CAR_CAN_EN
};


sCan_RxTxStatusType Can_RxTxStatus[] =
{
    {
        CAN_HWBUF_EMPTY,
        CAN_HWBUF_EMPTY
    },
#ifdef CAR_CAN_EN
    {
        CAN_HWBUF_EMPTY,
        CAN_HWBUF_EMPTY
    },
#ifdef CHG_CAN_EN
    {
        CAN_HWBUF_EMPTY,
        CAN_HWBUF_EMPTY
    },
#endif // CHG_CAN_EN     
#endif // CAR_CAN_EN

};