// SumvIf.c
#include "Sumv_Lcfg.h"
#include "SumvIf.h"
#include "Ecu.h"

uWord  gwd_Sumv_RefVolt_AD ;
uWord  gwd_Sumv_PowerVolt;

#define SUMVIF_POWERVOLT_GAIN  61440

static void SumvIf_InitIO(eSumv_IdNoType Sumv_IdNo);
static void SumvIf_IICStart(eSumv_IdNoType Sumv_IdNo);
static void SumvIf_IICStop(eSumv_IdNoType Sumv_IdNo);
static GeneralReturnType SumvIf_IICAck(eSumv_IdNoType Sumv_IdNo);
static void SumvIf_IICAckCpu(eSumv_IdNoType Sumv_IdNo);
static void SumvIf_WrByte(eSumv_IdNoType Sumv_IdNo, uByte data);
static uByte SumvIf_RdByte(eSumv_IdNoType Sumv_IdNo);
static GeneralReturnType SumvIf_WrConfig(eSumv_IdNoType Sumv_IdNo, uWord config);
static GeneralReturnType SumvIf_RdRegister(eSumv_IdNoType Sumv_IdNo, uByte reg_addr, uByte* p_data);
static void SumvIf_PowerOn(eSumv_IdNoType Sumv_IdNo);
static void SumvIf_PowerOff(eSumv_IdNoType Sumv_IdNo);


/* initial cpu IO */
static void SumvIf_InitIO(eSumv_IdNoType Sumv_IdNo)
{
    const sSumv_OperationType * pDevice;
    
    pDevice = GET_SUMV_HANDLE(Sumv_IdNo);
    //assert(pDevice != NULL);
    
    // power on device
    (*pDevice->Sumv_Pow.ClrVal)();
    
    
    (*pDevice->Cpu_Sumv_Scl.SetVal)();
    (*pDevice->Cpu_Sumv_Sda.SetVal)();
    (*pDevice->Sumv_Pow.SetVal)();

    (*pDevice->Cpu_Sumv_Sda.SetIoDirOut)();

}

/* generate the start signal of IIC */
static void SumvIf_IICStart(eSumv_IdNoType Sumv_IdNo)
{
    const sSumv_OperationType * pDevice;
    
    pDevice = GET_SUMV_HANDLE(Sumv_IdNo);
    //assert(pDevice != NULL);
    
    (*pDevice->Cpu_Sumv_Sda.SetVal)();
    (*pDevice->Cpu_Sumv_Sda.SetIoDirOut)();
    (*pDevice->Cpu_Sumv_Scl.SetVal)();

    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;

    (*pDevice->Cpu_Sumv_Sda.ClrVal)();

    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;

    (*pDevice->Cpu_Sumv_Scl.ClrVal)();
}

/* generate the stop signal of IIC */
static void SumvIf_IICStop(eSumv_IdNoType Sumv_IdNo)
{
    const sSumv_OperationType * pDevice;
    
    pDevice = GET_SUMV_HANDLE(Sumv_IdNo);
    //assert(pDevice != NULL);
    
    (*pDevice->Cpu_Sumv_Sda.ClrVal)();
    (*pDevice->Cpu_Sumv_Sda.SetIoDirOut)();
    (*pDevice->Cpu_Sumv_Scl.SetVal)();

    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;

    (*pDevice->Cpu_Sumv_Sda.SetVal)();
}

/* generate the ack signal of IIC */
static GeneralReturnType SumvIf_IICAck(eSumv_IdNoType Sumv_IdNo)
{    
    const sSumv_OperationType * pDevice;
    
    pDevice = GET_SUMV_HANDLE(Sumv_IdNo);
    //assert(pDevice != NULL);
    
    (*pDevice->Cpu_Sumv_Sda.SetIoDirIn)();
    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;

    (*pDevice->Cpu_Sumv_Scl.ClrVal)();
    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;

    if((*pDevice->Cpu_Sumv_Sda.GetVal)())
    {
        /* no ack presents */
        SumvIf_IICStop(Sumv_IdNo);
        return (GeneralReturnType)SUMVIF_ERR_IICACK;
    }

    (*pDevice->Cpu_Sumv_Scl.SetVal)();
    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;

    (*pDevice->Cpu_Sumv_Scl.ClrVal)();
    
    return (GeneralReturnType)SUMVIF_ERR_OK;
}

/* generate the ack signal of IIC */
static void SumvIf_IICAckCpu(eSumv_IdNoType Sumv_IdNo)
{
    const sSumv_OperationType * pDevice;
    
    pDevice = GET_SUMV_HANDLE(Sumv_IdNo);
    //assert(pDevice != NULL);
    
    (*pDevice->Cpu_Sumv_Sda.ClrVal)();
    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;

    (*pDevice->Cpu_Sumv_Sda.SetIoDirOut)();
    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;

    (*pDevice->Cpu_Sumv_Scl.SetVal)();
    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;

    (*pDevice->Cpu_Sumv_Scl.ClrVal)();
    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;

    (*pDevice->Cpu_Sumv_Sda.SetIoDirIn)();
}

/* write one uByte to IIC bus */
static void SumvIf_WrByte(eSumv_IdNoType Sumv_IdNo, uByte data)
{
    uByte i;
    uByte bit;
    const sSumv_OperationType * pDevice;
    
    pDevice = GET_SUMV_HANDLE(Sumv_IdNo);
    //assert(pDevice != NULL);
    
    (*pDevice->Cpu_Sumv_Sda.SetVal)();
    (*pDevice->Cpu_Sumv_Sda.SetIoDirOut)();
    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;

    for (i = 0; i < 8; i++)
    {
        bit = data & 0x80;
        data <<= 1;

        if (bit == 0x80)
        {
            (*pDevice->Cpu_Sumv_Sda.SetVal)();
        }
        else
        {
            (*pDevice->Cpu_Sumv_Sda.ClrVal)();
        }

        asm nop;
        asm nop;
        asm nop;
        asm nop;
        asm nop;
        asm nop;

        (*pDevice->Cpu_Sumv_Scl.SetVal)();
        asm nop;
        asm nop;
        asm nop;
        asm nop;
        asm nop;
        asm nop;

        (*pDevice->Cpu_Sumv_Scl.ClrVal)();
    }

    (*pDevice->Cpu_Sumv_Sda.ClrVal)();
}

/* read one uByte from IIC bus */
static uByte SumvIf_RdByte(eSumv_IdNoType Sumv_IdNo)
{
    uByte i;
    uByte data;
    const sSumv_OperationType * pDevice;
    
    pDevice = GET_SUMV_HANDLE(Sumv_IdNo);
    //assert(pDevice != NULL);
    
    data = 0;
    (*pDevice->Cpu_Sumv_Sda.SetIoDirIn)();
    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;
    asm nop;

    for (i = 0; i < 8; i++)
    {
        data <<= 1;

        (*pDevice->Cpu_Sumv_Scl.SetVal)();
        
        if ((*pDevice->Cpu_Sumv_Sda.GetVal)())
        {
            data |= 0x01;
        }

        asm nop;
        asm nop;
        asm nop;
        asm nop;
        asm nop;
        asm nop;

        (*pDevice->Cpu_Sumv_Scl.ClrVal)();
        asm nop;
        asm nop;
        asm nop;
        asm nop;
        asm nop;
        asm nop;
    }

    return data;
}

static GeneralReturnType SumvIf_WrConfig(eSumv_IdNoType Sumv_IdNo, uWord config)
{
    uByte i;

    Sumv_IdNo %= 2;
    
    for (i = 0; i < 3; i++)
    {
        SumvIf_IICStop(Sumv_IdNo);
        SumvIf_IICStart(Sumv_IdNo);

        /*select sda1115,write mode*/
        SumvIf_WrByte(Sumv_IdNo, SUMVIF_WRADDR);

        if ((GeneralReturnType)SUMVIF_ERR_OK != SumvIf_IICAck(Sumv_IdNo))
        {
            continue;
        }

        /*select sda1115 config register */
        SumvIf_WrByte(Sumv_IdNo, SUMVIF_CONGFIG_ADDR);

        if ((GeneralReturnType)SUMVIF_ERR_OK != SumvIf_IICAck(Sumv_IdNo))
        {
            continue;
        }

        /*select sda1115 config data high byte */
        SumvIf_WrByte(Sumv_IdNo, (uByte)(config >> 8));

        if ((GeneralReturnType)SUMVIF_ERR_OK != SumvIf_IICAck(Sumv_IdNo))
        {
            continue;
        }

        /*select sda1115 config data lower byte */
        SumvIf_WrByte((Sumv_IdNo), (uByte)config);

        if ((GeneralReturnType)SUMVIF_ERR_OK != SumvIf_IICAck(Sumv_IdNo))
        {
            continue;
        }

        SumvIf_IICStop(Sumv_IdNo);

        return (GeneralReturnType)SUMVIF_ERR_OK;
    }

    return (GeneralReturnType)SUMVIF_ERR_WRCONFIG;
}

static GeneralReturnType SumvIf_RdRegister(eSumv_IdNoType Sumv_IdNo, uByte reg_addr, uByte* p_data)
{
    Sumv_IdNo %= 2;    
    SumvIf_IICStop(Sumv_IdNo);
    SumvIf_IICStart(Sumv_IdNo);

    /* select sda1115, write mode */
    SumvIf_WrByte(Sumv_IdNo, SUMVIF_WRADDR);

    if ((GeneralReturnType)SUMVIF_ERR_OK != SumvIf_IICAck(Sumv_IdNo))
    {
        return (GeneralReturnType)SUMVIF_ERR_RDREGISTER;
    }

    /* select sda1115 register addr to be read */
    SumvIf_WrByte(Sumv_IdNo, reg_addr);

    if ((GeneralReturnType)SUMVIF_ERR_OK != SumvIf_IICAck(Sumv_IdNo))
    {
        return (GeneralReturnType)SUMVIF_ERR_RDREGISTER;
    }

    /* restart IIC bus */
    SumvIf_IICStart(Sumv_IdNo); 

    /*select sda1115,read mode*/
    SumvIf_WrByte(Sumv_IdNo, SUMVIF_RDADDR);

    if ((GeneralReturnType)SUMVIF_ERR_OK != SumvIf_IICAck(Sumv_IdNo))
    {
        return (GeneralReturnType)SUMVIF_ERR_RDREGISTER ;
    }

    *(p_data++) = SumvIf_RdByte(Sumv_IdNo);

    SumvIf_IICAckCpu(Sumv_IdNo);

    *p_data = SumvIf_RdByte(Sumv_IdNo);

    SumvIf_IICAckCpu(Sumv_IdNo);

    /* stop IIC bus */
    SumvIf_IICStop(Sumv_IdNo);

    return (GeneralReturnType)SUMVIF_ERR_OK;
}

/* ================================================== */

/* sumv module power on */
static void SumvIf_PowerOn(eSumv_IdNoType Sumv_IdNo)
{
    const sSumv_OperationType * pDevice;
    
    pDevice = GET_SUMV_HANDLE(Sumv_IdNo);
    //assert(pDevice != NULL);
    
    (*pDevice->Sumv_Pow.ClrVal)();
}

/* sumv module power off */
static void SumvIf_PowerOff(eSumv_IdNoType Sumv_IdNo)
{
    const sSumv_OperationType * pDevice;
    
    pDevice = GET_SUMV_HANDLE(Sumv_IdNo);
    //assert(pDevice != NULL);
    
    (*pDevice->Sumv_Pow.SetVal)();
}

/* initial sumv module */
GeneralReturnType SumvIf_Init(eSumv_IdNoType Sumv_IdNo)
{  
    Sumv_IdNo %= 2;  
    if((Sumv_IdNo >= 0) && (Sumv_IdNo < 2)) 
    {
        /* configure pin for ads1115 */
        SumvIf_InitIO(Sumv_IdNo);  
        
        /* power off to power on */
       // SumvIf_PowerOff(Sumv_IdNo);
        SumvIf_PowerOn(Sumv_IdNo);

        return (GeneralReturnType)SUMVIF_ERR_OK;
    } 
    else 
    {
        return (GeneralReturnType) SUMVIF_ERROR_IDNO;
    }
}

/* start convert sumv */
GeneralReturnType SumvIf_StartConvert(eSumv_IdNoType Sumv_IdNo)
{
   uByte i;
   uByte data[2];
   uWord config;
   eSumv_IdNoType Sumv_IdNo1;
   
   Sumv_IdNo1 = Sumv_IdNo;
   Sumv_IdNo %= 2;
   
   if((Sumv_IdNo >= 0) && (Sumv_IdNo < 2)) 
   {
       for (i = 0; i < 3; i++)
       {
           if(SUMV_INNER_ID == Sumv_IdNo)    //INT_SUMV
           {
               if(SUMV_INNER_ID == Sumv_IdNo1)
               {
                  config = 0x5483;//INTSUMV_CONTINUE_CONV_AING1_128SPS;
               }
               else if(SUMV_BACKUP_ID == Sumv_IdNo1)
               {
                  config = 0x6483;//INTSUMV_CONTINUE_CONV_AING2_128SPS;
               }
               else
               {
                  config = 0x7483;//INTSUMV_CONTINUE_CONV_AING3_128SPS;
               }
            
               if ((GeneralReturnType)SUMVIF_ERR_OK != SumvIf_WrConfig(Sumv_IdNo, config))
               {
                   continue;
               }
           }
           else if(SUMV_EXTER_ID == Sumv_IdNo)  //EXT_SUMV
           {
               if(SUMV_EXTER_ID == Sumv_IdNo1)
               {
                  config = 0x5483;//EXTSUMV_CONTINUE_CONV_AING1_128SPS;
               }
               else if(SUMV_EXT_BACKUP_ID == Sumv_IdNo1)
               {
                  config = 0x6483;//EXTSUMV_CONTINUE_CONV_AING2_128SPS;
               }
               else
               {
                  config = 0x7483;//EXTSUMV_CONTINUE_CONV_AING3_128SPS;
               }
            
               if ((GeneralReturnType)SUMVIF_ERR_OK !=SumvIf_WrConfig(Sumv_IdNo, config))
               {
                   continue;
               }  
           }


           if ((GeneralReturnType)SUMVIF_ERR_OK != SumvIf_RdRegister(Sumv_IdNo, SUMVIF_CONGFIG_ADDR, data))
           {
               continue;
           }

           if ( (data[0] == (uByte)(config >> 8)) && (data[1] == (uByte)config) )
           {
               return (GeneralReturnType)SUMVIF_ERR_OK;
           }
           else
           {
               continue;
           }
       }
    
       return  (GeneralReturnType)SUMVIF_ERR_STARTCONVERT;
   } 
   else 
   {
     return (GeneralReturnType) SUMVIF_ERROR_IDNO;
   }
}

GeneralReturnType SumvIf_RdAd(eSumv_IdNoType Sumv_IdNo, uWord* p_ad)
{
    uByte i;
    uByte data[2];
    uWord sumv_ad;
    
    Sumv_IdNo %= 2;
    if((Sumv_IdNo >= 0) && (Sumv_IdNo < 2)) 
    {
        if(p_ad != NULL)
        {
            for (i = 0; i < 3; i++)
            {
                if ((GeneralReturnType)SUMVIF_ERR_OK != SumvIf_RdRegister(Sumv_IdNo, SUMVIF_CONV_RSLT_ADDR, data))
                {
                    continue;
                }

                sumv_ad = data[0];
                sumv_ad <<= 8;
                sumv_ad += data[1];
            
                if(0x8000 == (sumv_ad & 0x8000)) 
                {
                    sumv_ad &= 0x7fff;
                    sumv_ad--;
                    sumv_ad ^= 0xFFFF;
                    sumv_ad &= 0x7fff;
                    if(sumv_ad > SUMV_ZERO_DRIFT_THRED) 
                    {
                        *p_ad = 0;
                        return  SUMVIF_ERR_NEG;
                    }
                }
            
            
          /*  if(0 == (sumv_ad & 0x8000)) 
            {  */
                /* prevent float pin */
                if (sumv_ad <= SUMV_ZERO_DRIFT_THRED)
                {
                    sumv_ad = 0;
                }
                else if (sumv_ad >= SUMV_OUT_RANGE_THRED) // to check sumv sensor wire is open
                {
                    sumv_ad = 0;
                    return (GeneralReturnType)SUMVIF_ERR_OUTRANGE;
                }
                else
                {
                    *p_ad = sumv_ad;
                }
          /*  } 
            else 
            {
                sumv_ad = 0;
            } */

            /* store ad value */
            //*p_ad = sumv_ad;

            /* no error */
                return (GeneralReturnType)SUMVIF_ERR_OK;
            }

            /* set error sumv to 0A */
            *p_ad = 0;
        
            /* convert-error occur */    
            return (GeneralReturnType)SUMVIF_ERR_ADCONVERT;
        } 
        else 
        {
            return  (GeneralReturnType)  SUMVIF_ERROR_PTR_NULL;
        }
    }
    else 
    {
       return (GeneralReturnType) SUMVIF_ERROR_IDNO;
    }
}

static GeneralReturnType SumvIf_CheckState(eSumv_IdNoType Sumv_IdNo)
{
    uByte i;
    uByte data[2];
    
    Sumv_IdNo %= 2;
    /* read register until read successfully */
    for (i = 0; i < 3; i++)
    {
        if ((GeneralReturnType)SUMVIF_ERR_OK != SumvIf_RdRegister(Sumv_IdNo, SUMVIF_CONGFIG_ADDR, data))
        {
            continue;
        }

        if ( (data[0] == (uByte)(CONTINUE_CONV_AIN0_128SPS >> 8))
          && (data[1] == (uByte)(CONTINUE_CONV_AIN0_128SPS & 0xff)) )
        {
            return (GeneralReturnType)SUMVIF_ERR_OK;
        }
    }

    /* convert-error occur */    
    return (GeneralReturnType)SUMVIF_ERR_RDREGISTER;
}

GeneralReturnType SumvIf_CheckPower(eSumv_IdNoType Sumv_IdNo, uWord* p_refv_ad, uWord* p_volt)
{
    uByte i;
    uByte data[2];
    uWord wtmp;
    uLWord ad_value;
    GeneralReturnType rslt;
    
    Sumv_IdNo %= 2;
    if((Sumv_IdNo >= 0) && (Sumv_IdNo <= 2)) 
    {
        if((p_refv_ad != NULL) && (p_volt != NULL)) 
        {
            data[0] = 0;
            data[1] = 0;
            rslt = SumvIf_Init(Sumv_IdNo);

            // start AN3 channle convert
            if((GeneralReturnType)SUMVIF_ERR_OK == rslt)
            {
                for (i = 0; i < 3; i++)
                {
                    if ((GeneralReturnType)SUMVIF_ERR_OK != SumvIf_WrConfig(Sumv_IdNo, CONTINUE_CONV_AIN3_128SPS))
                    {
                        continue;
                    }

                    if ((GeneralReturnType)SUMVIF_ERR_OK != SumvIf_RdRegister(Sumv_IdNo, SUMVIF_CONGFIG_ADDR, data))
                    {
                        continue;
                    }

                    if ( (data[0] == (uByte)(CONTINUE_CONV_AIN3_128SPS>>8))
                      && (data[1] == (uByte)(CONTINUE_CONV_AIN3_128SPS & 0xFF)) )
                    {
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }

                if(i < 3)
                {
                    ECU_DELAYMS(10);

                    for (i = 0; i < 3; i++)
                    {
                        if ((GeneralReturnType)SUMVIF_ERR_OK != SumvIf_RdRegister(Sumv_IdNo, SUMVIF_CONV_RSLT_ADDR, data))
                        {
                            continue;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                
                if(i < 3)
                {
                    ad_value = data[0];
                    ad_value <<= 8;
                    ad_value += data[1];

                    wtmp = (uWord)ad_value;
                    wtmp >>= 1;        // ref = power_volt/2
                    *p_refv_ad = wtmp;

                    ad_value *= SUMVIF_POWERVOLT_GAIN;
                    ad_value >>= 15;

                    if ((ad_value / 10) > 4)
                    {
                        ad_value = (ad_value / 10) + 1;
                    }
                    else
                    {
                        ad_value = ad_value / 10;
                    }

                    *p_volt = (uWord)ad_value;  // power volt
                    
                    rslt = (GeneralReturnType)SUMVIF_ERR_OK;
                }
                else
                {
                    /* convert-error occur */
                    rslt = (GeneralReturnType)SUMVIF_ERR_POWERVOLT;
                }
            }
            
            return rslt;
       } 
       else
       {
            return (GeneralReturnType) SUMVIF_ERROR_PTR_NULL;
       }
  } 
  else 
  {
     return (GeneralReturnType) SUMVIF_ERROR_IDNO;
  }
}