
#include "TemperIf_Type.h"
#include "TemperIf.h"
#if ((ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R) || (ECU_CATEGORY == M_BCU_C))

/*********************************
       18B20 command  
**********************************/
/*Skip ROM*/
#define SKIPROM	    0xCC 
/*Read ROM*/
#define RDROM	    0x33
/*Match ROM*/
#define MATCHROM 	0x55
/*Search ROM*/
#define SRCHROM 	0xF0	
/*Copy Scratchpad to RAM*/
#define COPYSCR     0x48
/*Read Scratchpad*/
#define RDSCRTCH	0xBE
/*Write Scratchpad*/
#define WRSCRTCH    0x4E	
 /*Recall Memory*/
#define RECALLEE    0xB8
 /*Convert Temper*/
#define CNVRTT 	    0x44	
 /*Convert voltage, reserved for DS2438 */
#define CNVRTV      0xb4

/*********************************
       18B20 error code  
**********************************/
#define B20HWERR     0x0001
#define B20SENSORERR 0x0002

#define FLAG_OK   0
#define FLAG_FAIL 1

uByte gby_rom_num[MAX_B20_NUM][9];  //rom number
uByte gby_rom_num_bak[MAX_B20_NUM][9]; //rom number
//uByte temper_AD_H;	      
//uByte temper_AD_L;	      


   /***** Delay Modules***************/
                         
/***************************************************************/
//delay2us
/***************************************************************/
static void delay02(void)
{
    asm NOP;
}

/***************************************************************/
//delay 5us
/***************************************************************/
static void delay05(void)                
{
    asm NOP;
    asm NOP;
    asm NOP;
    asm NOP;
    asm NOP;
    asm NOP;
    asm NOP;
    asm NOP;
}

/***************************************************************/
//delay90us

/***************************************************************/
static void delay90(void)
{
    uByte i;

    for (i = 63; i > 0; i--)	  
    {
        asm NOP;
    }
}

/***************************************************************/
//delay 600us
/***************************************************************/
static void delay600(void)
{                                
    uWord i;

    for (i = 430; i > 0; i--)
    {
        asm NOP;
        asm NOP;
        asm NOP;
    }
}

void int_enable(void) 
{
   asm nop;
}

void int_disable(void) 
{
   asm nop;
}

static void Init_Sp(eTemperIf_IdNumType TemperIf_Id) 
{
    const sTemperIf_OperationType *TemperIf_OperaPtr;

    TemperIf_OperaPtr = GET_TEMPEROPERATION(TemperIf_Id);
    
    if(TemperIf_OperaPtr->TemperIf_18B20PutValPtr != NULL) 
    {   
        (*(TemperIf_OperaPtr->TemperIf_18B20PutValPtr))(1);
        delay05();
        delay05();
        (*(TemperIf_OperaPtr->TemperIf_18B20PutValPtr))(0);
        delay600();
        (*(TemperIf_OperaPtr->TemperIf_18B20PutValPtr))(1);
        delay600(); 
    }
}

static void Write_Bit(eTemperIf_IdNumType TemperIf_Id,uByte bit) 
{
    const sTemperIf_OperationType *TemperIf_OperaPtr;

    TemperIf_OperaPtr = GET_TEMPEROPERATION(TemperIf_Id);
   
    if(TemperIf_OperaPtr->TemperIf_18B20PutValPtr != NULL) 
    {  
        (*(TemperIf_OperaPtr->TemperIf_18B20PutValPtr))(0);
        delay05();
        delay02();
        (*(TemperIf_OperaPtr->TemperIf_18B20PutValPtr))(bit);
        delay90();
        (*(TemperIf_OperaPtr->TemperIf_18B20PutValPtr))(1);
        delay05();
        delay05();
    }
}

static uByte Read_Bit(eTemperIf_IdNumType TemperIf_Id) 
{
    uByte i;
    const sTemperIf_OperationType *TemperIf_OperaPtr;

    TemperIf_OperaPtr = GET_TEMPEROPERATION(TemperIf_Id);
    
    if((TemperIf_OperaPtr->TemperIf_18B20PutValPtr != NULL) && (TemperIf_OperaPtr->TemperIf_18B20GetValPtr != NULL))
    {  
        (*(TemperIf_OperaPtr->TemperIf_18B20PutValPtr))(FALSE);
        delay05();
        
        (*(TemperIf_OperaPtr->TemperIf_18B20PutValPtr))(TRUE);
        delay05();
        delay05();
        delay02();
        
        if((*(TemperIf_OperaPtr->TemperIf_18B20GetValPtr))() != 0) 
        {
            i = 1;
        } 
        else 
        {
            i = 0;
        }
        delay90();
        delay05();
        
        (*(TemperIf_OperaPtr->TemperIf_18B20PutValPtr))(TRUE);
        delay05();
    }
    return i;
}

static void Write_Byte(eTemperIf_IdNumType TemperIf_Id,uByte wr1820) 
{
    uByte i;
    int_disable();
    for(i = 0;i < 8; i++) 
    {
        Write_Bit(TemperIf_Id,(wr1820 & 0x01));
        wr1820 = wr1820 >> 1;
    }
    int_enable();
}

static void Read_Scratch(eTemperIf_IdNumType TemperIf_Id,uByte* point_scrdata) 
{
    uByte i, j;
    uByte x;
    
    const sTemperIf_OperationType *TemperIf_OperaPtr;
    TemperIf_OperaPtr = GET_TEMPEROPERATION(TemperIf_Id);
    if((TemperIf_OperaPtr->TemperIf_18B20PutValPtr != NULL) && (TemperIf_OperaPtr->TemperIf_18B20GetValPtr != NULL)) 
    {  
        for (i = 0; i < 9; i++)
        {
            *point_scrdata = 0;

            for (j = 0; j < 8; j++)
            {
                int_disable();

                *point_scrdata>>=1;

                (*(TemperIf_OperaPtr->TemperIf_18B20PutValPtr))(FALSE);   //pull down 
                delay05();

                (*(TemperIf_OperaPtr->TemperIf_18B20PutValPtr))(TRUE);      

                delay05();
                delay05();				
                delay02();
                
                if((*(TemperIf_OperaPtr->TemperIf_18B20GetValPtr))() == 0) 
                {
                    x = 0;
                } 
                else 
                {
                    x = 1;
                }

                if (1 == x)
                {
                    *point_scrdata |= 0x80;
                }
                else
                {
                    *point_scrdata &= 0x7F;
                } 


                int_enable();

                delay90();				//delay 90us
            }

            point_scrdata++;
        }
    }
}

static uByte Check_Crc(uByte* point_scr,uByte type)
{
    uByte i, j, byta;
    uByte crc = 0;

    for (i = 0; i < type; i++)
    {
        byta = *point_scr;	      //receive data

        for (j = 0; j < 8; j++)
        {
            if (((byta ^ crc) & 0x01) == 0x01)   
            {
                crc = crc ^ 0x18;
                crc = crc >> 1;
                crc = crc | 0x80;
            }
            else
            {
                crc = crc >> 1;
            }

            byta = byta >> 1;
        }

        point_scr++;
    }

    return	crc;
}

 
void TemperIf_Init(void) 
{
    const sTemperIf_PropertyType *TemperIf_PropPtr;
    const sTemperIf_OperationType *TemperIf_OperaPtr;
    
    TemperIf_PropPtr = GET_TEMPERPROPERTY(TEMPERIF_18B20_NO);  //point to property
    TemperIf_OperaPtr = GET_TEMPEROPERATION(TEMPERIF_18B20_NO); //point to operation
    
    if(TemperIf_PropPtr->TemperIf_SensorType == T18B20) 
    {
        (*(TemperIf_OperaPtr->TemperIf_18B20PutValPtr))(FALSE);
        Init_Sp(TEMPERIF_18B20_NO);
        Write_Byte(TEMPERIF_18B20_NO,SKIPROM);
        Write_Byte(TEMPERIF_18B20_NO,RECALLEE);
    } 
    else 
    {
        //for NTC 
    } 
}

GeneralReturnType TemperIf_StartCnvt(eTemperIf_IdNumType TemperIf_Id) 
{
    GeneralReturnType st;
    const sTemperIf_PropertyType *TemperIf_PropPtr;
    const sTemperIf_OperationType *TemperIf_OperaPtr;
    
   /* if(TemperIf_Id >= TEMPER_MAX_NUM) 
    {
        st = (GeneralReturnType)ERR_TEMPER_RANGE;
        return st;
    } */
    st = (GeneralReturnType)ERR_OK;
    
    TemperIf_PropPtr = GET_TEMPERPROPERTY(TemperIf_Id);  //point to property
    TemperIf_OperaPtr = GET_TEMPEROPERATION(TemperIf_Id); //point to operation
    
    if(TemperIf_PropPtr->TemperIf_SensorType == T18B20) 
    {
        Init_Sp(TemperIf_Id);
        Write_Byte(TemperIf_Id,SKIPROM);
        Write_Byte(TemperIf_Id,CNVRTT);
        (*(TemperIf_OperaPtr->TemperIf_18B20PutValPtr))(TRUE);
    } 
    else 
    {
        //for NTC 
    } 
    return st;
}

void TemperIf_Srch18B20Rom(uByte *T18b20RomPtr,uByte *T18b20RomNumPtr) 
{      
    uByte bit0, bit1;
    uByte i=0, j=0, k=0;
    uByte errflag=0, counter=0, counterbak=0;
    uByte temp[10];
    uByte lby_scrdata[8];
    uByte lby_scr[9],icnt,jcnt;
    
    const sTemperIf_PropertyType *TemperIf_PropPtr;
    //const sTemperIf_OperationType *TemperIf_OperaPtr;

    TemperIf_PropPtr = GET_TEMPERPROPERTY(TEMPERIF_18B20_NO);  //point to property
    //TemperIf_OperaPtr = GET_TEMPEROPERATION(TEMPERIF_18B20_NO); //point to operation
    
    if(T18B20 == TemperIf_PropPtr->TemperIf_SensorType) 
    {
        for (i = 0; i < 10; i++)
        {
            temp[i] = 0;
        }
        while ((j < MAX_B20_NUM) && (errflag == 0))
        {
            Init_Sp(TEMPERIF_18B20_NO);
            Write_Byte(TEMPERIF_18B20_NO,SRCHROM);

            for (i = 0; i < 64; i++)
            {
                while ((i <= counterbak) && (j > 0))
                {
                    bit0 = Read_Bit(TEMPERIF_18B20_NO);
                    bit1 = Read_Bit(TEMPERIF_18B20_NO);
                    if (i < counterbak)
                    {
                        if ((bit0 == 0) && (bit1 == 0))
                        {
                            if ((temp[i / 8] & (0x01 << i % 8)) != (0x01 << i % 8))
                            {
                                counter = i;
                            }
                        }

                        Write_Bit(TEMPERIF_18B20_NO,(uByte)(temp[i / 8] & (0x01 << (i % 8))) >> (i % 8));
                    }
                    else
                    {
                        Write_Bit(TEMPERIF_18B20_NO,1);
                        temp[i / 8] = temp[i / 8] | (1 << (i % 8)); 
                    }

                    i++;
                }

                bit0 = Read_Bit(TEMPERIF_18B20_NO);
                bit1 = Read_Bit(TEMPERIF_18B20_NO);

                if ((bit0 == 0) && (bit1 == 0))
                {
                    counter = i;
                    temp[i / 8] = temp[i / 8] & (0xFF - (1 << (i % 8)));	

                    Write_Bit(TEMPERIF_18B20_NO,0);
                }
                else if ((bit0 == 0) && (bit1 == 1))
                {
                    temp[i / 8] = temp[i / 8] & (0xFF - (1 << (i % 8)));	

                    Write_Bit(TEMPERIF_18B20_NO,0);
                }
                else if ((bit0 == 1) && (bit1 == 0))
                {
                    Write_Bit(TEMPERIF_18B20_NO,1);
                    temp[i / 8] = temp[i / 8] | (1 << (i % 8)); 
                }
                else
                {
                    errflag = 1;
                    i = 100;
                } 
            }

            for (k = 0; k < 8; k++)
            {
                lby_scrdata[k] = temp[k];
            }

            if ((lby_scrdata[7] == Check_Crc(lby_scrdata, 7))
                    && (temp[0] == 0x28))
            {
                for (k = 0; k < 8; k++)
                {
                    gby_rom_num[j][k] = temp[k];
                }

                j++;
            }
            else
            {
                errflag = 1;
                i = 100;
            }
            counterbak = counter;
            counter = 0;
        }
        
        *T18b20RomNumPtr = j;
        /* store RomNo by User NO */
      	for(icnt = 0; icnt < MAX_B20_NUM; icnt++)
      	{
      	      Init_Sp(TEMPERIF_18B20_NO);  
              Write_Byte(TEMPERIF_18B20_NO,MATCHROM);      //match ROM	  
              
              for (jcnt = 0; jcnt < 8; jcnt++)
              {
                  Write_Byte(TEMPERIF_18B20_NO,gby_rom_num[icnt][jcnt]);       
              }
              Write_Byte(TEMPERIF_18B20_NO,RDSCRTCH);            //read scratchpad command

              Read_Scratch(TEMPERIF_18B20_NO,lby_scr);
              
              
      	    if((lby_scr[2] > 0)&& (lby_scr[2] <= MAX_B20_NUM)
      	        && (lby_scr[2] == lby_scr[3]))	    
      	    {
                  for (jcnt = 0; jcnt < 8; jcnt++)
                  {
                      gby_rom_num_bak[lby_scr[2] - 1][jcnt] = gby_rom_num[icnt][jcnt];
                  }
      	    }
      	}
    	
      	/* updata gby_rom_num[MAX_B20_NUM][9] from gby_rom_numbak */
      	for(icnt = 0; icnt < MAX_B20_NUM; icnt++)
      	{ 			
      		for(jcnt = 0; jcnt < 9; jcnt++)
      		{
      			*(T18b20RomPtr++) = gby_rom_num_bak[icnt][jcnt];
      		}
      	} 
    }  
}

GeneralReturnType TemperIf_18b20GetTemperAd(eTemperIf_IdNumType TemperIf_Id,uByte *T18B20AdPtr,uWord *Temper_AdValuePtr) 
{
    uByte i,j, cnt;
    uByte scr[9];
    //uByte flag = 0;
    
    GeneralReturnType st;
    const sTemperIf_PropertyType *TemperIf_PropPtr;
    //const sTemperIf_OperationType *TemperIf_OperaPtr;
    
    st = (GeneralReturnType)ERR_OK;
    TemperIf_PropPtr = GET_TEMPERPROPERTY(TemperIf_Id);  //point to property
    //TemperIf_OperaPtr = GET_TEMPEROPERATION(TemperIf_Id); //point to operation
    
    if(TemperIf_PropPtr->TemperIf_IdNum == TemperIf_Id) 
    {
        if(T18B20 == TemperIf_PropPtr->TemperIf_SensorType) 
        {
            for (i = 0; i < 1; i++)
            {
                cnt = 0;

                Init_Sp(TemperIf_Id);

                Write_Byte(TemperIf_Id,MATCHROM);                      //match ROM

                for (j = 0; j < 8; j++)
                {
                   Write_Byte(TemperIf_Id,*(T18B20AdPtr++));       //				 
                }

                Write_Byte(TemperIf_Id,RDSCRTCH);                      //read scratchpad command 

                Read_Scratch(TemperIf_Id,scr);

                
                if (scr[8] == Check_Crc(scr, 8))
                {
                    if ( (scr[2] <= 10) /*&& (scr[2] == (row + 1)) */
                         && (scr[5] == 0xFF) && (scr[7] == 0x10) )
                    {
                      *Temper_AdValuePtr = (scr[1] << 8) + scr[0];  	                      
                    }
                } 
            }
        } 
        else 
        {
            st = (GeneralReturnType)ERR_TEMPER_TYPE_NOTMATCH;
        }
    } 
    else 
    {
        st = (GeneralReturnType)ERR_TEMPER_ID_NUM_NOTMATCH;
    }
    return st;
}

GeneralReturnType TemperIf_NtcGetTemperAd(eTemperIf_IdNumType TemperIf_Id,uWord *Temper_AdValuePtr) 
{
    const sTemperIf_PropertyType *TemperIf_PropPtr;
    const sTemperIf_OperationType *TemperIf_OperaPtr;
    
    GeneralReturnType st;

    st = (GeneralReturnType)ERR_OK;
    TemperIf_PropPtr = GET_TEMPERPROPERTY(TemperIf_Id);  //point to property
    TemperIf_OperaPtr = GET_TEMPEROPERATION(TemperIf_Id); //point to operation
    
    if(TemperIf_PropPtr->TemperIf_IdNum == TemperIf_Id) 
    {
       if(TemperIf_OperaPtr->TemperIf_NtcGetAdcValuePtr != NULL) 
       {
           (void)(*TemperIf_OperaPtr->TemperIf_NtcGetAdcValuePtr)(Temper_AdValuePtr);
       } 
       else 
       {
           st = (GeneralReturnType)ERR_TEMPER_FUNCTION_PTR_NULL;
       }
    } 
    else 
    {
        st = (GeneralReturnType)ERR_TEMPER_ID_NUM_NOTMATCH;
    }
    return st;
}

GeneralReturnType TemperIf_HwErrDiag(uWord * HwErrPtr) 
{
    uByte flag = FLAG_FAIL;
    const sTemperIf_OperationType *TemperIf_OperaPtr;

    TemperIf_OperaPtr = GET_TEMPEROPERATION(TEMPERIF_18B20_NO); 
    
    (*(TemperIf_OperaPtr->TemperIf_18B20PutValPtr))(FALSE); 
    delay05();
    
    if (0 == (*(TemperIf_OperaPtr->TemperIf_18B20GetValPtr))())
    {
        flag = FLAG_OK;
    }
    else
    {
        flag = FLAG_FAIL;
    }

    (*(TemperIf_OperaPtr->TemperIf_18B20PutValPtr))(TRUE);
    delay05();

    if(0 == (*(TemperIf_OperaPtr->TemperIf_18B20GetValPtr))()) 
    {
        flag = FLAG_FAIL;
    } 
    else 
    {
        flag = FLAG_OK;
    }

    if (FLAG_OK == flag)
    {
        *HwErrPtr &= ~B20HWERR;
    }
    else
    {
        *HwErrPtr |= B20HWERR;
    }

    return (GeneralReturnType)flag;
}

GeneralReturnType TemperIf_T18b20CheckState(eTemperIf_IdNumType TemperIf_Id,uWord* p_err_code) 
{
  
}

GeneralReturnType TemperIf_T18b20CheckHardware(eTemperIf_IdNumType TemperIf_Id,uWord* p_err_code) 
{
  
}

#endif //#if ((ECU_CATEGORY == M_BMU) || (ECU_CATEGORY == M_BMU_R) || (ECU_CATEGORY == M_BCU_C))

 

                     
