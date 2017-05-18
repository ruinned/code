#include "RelayIcDriveIf_Type.h"
#include "RelayIcDriveIf.h"
#include "RelayIf.h"

uByte gby_out_ctrl_st;

GeneralReturnType RelayIcDrive_IcInit(uByte RelayIcDrive_Id,eRelayIcDrive_ModeType Relay_IcMode)
{
    uByte tmp;
    GeneralReturnType rslt;
    GeneralReturnType Spi_st;
    const sRelayIcDrive_PropertyType *RelayIcDrive_PropPtr;
    const sRelayIcDrive_OperationType *RelayIcDrive_OperaPtr;
   
    gby_out_ctrl_st = 0; 
    tmp = 0;
    rslt = (GeneralReturnType)ERR_OK;
    if(RelayIcDrive_Id >= RELAYICDRIVE_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;  
    }
    RelayIcDrive_PropPtr = GET_RELAYICDRIVEPROPERTY(RelayIcDrive_Id); //point to property
    RelayIcDrive_OperaPtr = GET_RELAYICDRIVEOPERATION(RelayIcDrive_Id);  //point to operation
    
    if((uByte)(RelayIcDrive_PropPtr->RelayIcDrive_IdNum) == RelayIcDrive_Id)   //match the spi number
    {
        if(RelayIcDrive_OperaPtr->RelayIcDrive_IcSpiCtrlPtr != NULL) 
        {
            // tle8102 output configaration
            tmp = RELAY_IC_OUT_CONFIG;
            tmp |= RELAY_IC_OUT_CONFIG_CH2_CURR_LIM1;
            tmp |= RELAY_IC_OUT_CONFIG_CH1_CURR_LIM1;
            tmp |= RELAY_IC_OUT_CONFIG_O_TEMP_LATCH;
            Spi_st = (*RelayIcDrive_OperaPtr->RelayIcDrive_IcSpiCtrlPtr)(tmp,NULL);
            if(Spi_st != ERR_OK)
            {
               return Spi_st;
            }
            // tle8102 input,output configaration
            tmp = RELAY_IC_IO_CTRL_CONFIG;       //set spi to be default diagnostic 
            
            if(TLE_IC_IO_MODE == Relay_IcMode)
            {
                tmp |= RELAY_IC_CTRL_CONFIG_DIA_OUT_IO;
                tmp |= RELAY_IC_CTRL_CONFIG_BOL_IO;
            }
            else
            {
                tmp |= RELAY_IC_CTRL_CONFIG_DIA_OUT_SPI;
                tmp |= RELAY_IC_CTRL_CONFIG_BOL_SPI;
            }
            
            Spi_st = (*RelayIcDrive_OperaPtr->RelayIcDrive_IcSpiCtrlPtr)(tmp,NULL);
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
    else 
    {
        rslt = (GeneralReturnType)ERR_IO_ID_NUM_NOTMATCH;   //the id number is not match
    }

    return rslt;              
}

GeneralReturnType RelayIcDrive_IcDiagIoMode(uByte RelayIcDrive_Id)
{
    uByte tmp;
    GeneralReturnType rslt;
    GeneralReturnType Spi_st;
    const sRelayIcDrive_PropertyType *RelayIcDrive_PropPtr;
    const sRelayIcDrive_OperationType *RelayIcDrive_OperaPtr;
   
    rslt = (GeneralReturnType)ERR_OK;
    tmp = 0;
    if(RelayIcDrive_Id >= RELAYICDRIVE_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;  
    }
    RelayIcDrive_PropPtr = GET_RELAYICDRIVEPROPERTY(RelayIcDrive_Id); //point to property
    RelayIcDrive_OperaPtr = GET_RELAYICDRIVEOPERATION(RelayIcDrive_Id);  //point to operation
     
    if((uByte)(RelayIcDrive_PropPtr->RelayIcDrive_IdNum) == RelayIcDrive_Id)   //match the spi number
    {
        // tle8102 input,output configaration
        tmp = RELAY_IC_IO_CTRL_CONFIG;
        tmp |= RELAY_IC_CTRL_CONFIG_DIA_OUT_IO;  // to input and output diagnostic mode
        tmp |= RELAY_IC_CTRL_CONFIG_BOL_IO;
        if(RelayIcDrive_OperaPtr->RelayIcDrive_IcSpiCtrlPtr != NULL) 
        {
            Spi_st = (*RelayIcDrive_OperaPtr->RelayIcDrive_IcSpiCtrlPtr)(tmp,NULL);
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
    else 
    {
        rslt = (GeneralReturnType)ERR_IO_ID_NUM_NOTMATCH;   //the id number is not match
    }
    
    return rslt;
}

GeneralReturnType RelayIcDrive_IcDiagSpiMode(uByte RelayIcDrive_Id)
{
    uByte tmp;
    GeneralReturnType rslt;
    GeneralReturnType Spi_st;
    const sRelayIcDrive_PropertyType *RelayIcDrive_PropPtr;
    const sRelayIcDrive_OperationType *RelayIcDrive_OperaPtr;
   
    rslt = (GeneralReturnType)ERR_OK;
    tmp = 0;
    if(RelayIcDrive_Id >= RELAYICDRIVE_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_RANGE;
        return rslt;  
    }
    RelayIcDrive_PropPtr = GET_RELAYICDRIVEPROPERTY(RelayIcDrive_Id); //point to property
    RelayIcDrive_OperaPtr = GET_RELAYICDRIVEOPERATION(RelayIcDrive_Id);  //point to operation
    
    if((uByte)(RelayIcDrive_PropPtr->RelayIcDrive_IdNum) == RelayIcDrive_Id)   //match the spi number
    {
        // tle8102 input,output configaration
        tmp = RELAY_IC_IO_CTRL_CONFIG;
        tmp |= RELAY_IC_CTRL_CONFIG_DIA_OUT_SPI; // to spi diagnostic mode
        tmp |= RELAY_IC_CTRL_CONFIG_BOL_SPI;
        if(RelayIcDrive_OperaPtr->RelayIcDrive_IcSpiCtrlPtr != NULL) 
        {
            Spi_st = (*RelayIcDrive_OperaPtr->RelayIcDrive_IcSpiCtrlPtr)(tmp,NULL);
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
    else 
    {
        rslt = (GeneralReturnType)ERR_IO_ID_NUM_NOTMATCH;   //the id number is not match
    }

    return rslt;
}

GeneralReturnType RelayIcDrive_IcDiag(uByte RelayIcDrive_Id,uWord *Err_CodePtr)
{
    uByte data,rslt_on,rslt_off;
    //uByte pin_st_on = 0, pin_st_off = 0;
    
    //uWord lwd_temp_err_code = 0;
    
    GeneralReturnType rslt;
    GeneralReturnType Spi_st;
    const sRelayIcDrive_PropertyType *RelayIcDrive_PropPtr;
    const sRelayIcDrive_OperationType *RelayIcDrive_OperaPtr;

    // diagnostic the SPI 
    data = RELAY_IC_DIAG;
    *Err_CodePtr = 0;
    rslt = (GeneralReturnType)ERR_OK;
    
    if(RelayIcDrive_Id >= RELAYICDRIVE_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;  
    }
    RelayIcDrive_PropPtr = GET_RELAYICDRIVEPROPERTY(RelayIcDrive_Id); //point to property
    RelayIcDrive_OperaPtr = GET_RELAYICDRIVEOPERATION(RelayIcDrive_Id);  //point to operation
    
    if((uByte)(RelayIcDrive_PropPtr->RelayIcDrive_IdNum) == RelayIcDrive_Id)   //match the spi number
    { 
        if(RelayIcDrive_OperaPtr->RelayIcDrive_IcSpiCtrlPtr != NULL) 
        {
            Spi_st = (*RelayIcDrive_OperaPtr->RelayIcDrive_IcSpiCtrlPtr)(data,&rslt_on);
            if(Spi_st != ERR_OK)
            {
                return Spi_st;
            }
            if (rslt_on == 0xFF) // check default value
            {
                return rslt;
            }
            
            //
            rslt_on  = 0;
            rslt_off = 0;
            //pin_st_on  = 0;
            //pin_st_off = 0;
            
           // diag_tle8102sg_SPI_mode();  // change diagnostic mode to SPI
            RelayIcDrive_IcDiagSpiMode(RelayIcDrive_Id);
            
            RelayIcDrive_IcSpiCtrlOutput(RelayIcDrive_Id,TLE_IC_CHANNEL1,RELAY_IC_CH_ON,NULL);
            //spi_ctrl_output(TLE8102_CH1, TLE8102_CH_ON, &lwd_temp_err_code);
            RelayIcDrive_IcSpiCtrlOutput(RelayIcDrive_Id,TLE_IC_CHANNEL2,RELAY_IC_CH_ON,NULL);
            //spi_ctrl_output(TLE8102_CH2, TLE8102_CH_ON, &lwd_temp_err_code);
            ECU_DELAYMS(500);                                           
            (void)(*RelayIcDrive_OperaPtr->RelayIcDrive_IcSpiCtrlPtr)(data,&rslt_on);
            //rslt_on = wr_rd_data(data);
            
            RelayIcDrive_IcSpiCtrlOutput(RelayIcDrive_Id,TLE_IC_CHANNEL1,RELAY_IC_CH_OFF,NULL);
            //spi_ctrl_output(TLE8102_CH1, TLE8102_CH_OFF, &lwd_temp_err_code);
            RelayIcDrive_IcSpiCtrlOutput(RelayIcDrive_Id,TLE_IC_CHANNEL2,RELAY_IC_CH_OFF,NULL);
            //spi_ctrl_output(TLE8102_CH2, TLE8102_CH_OFF, &lwd_temp_err_code);
            ECU_DELAYMS(500);
            (void)(*RelayIcDrive_OperaPtr->RelayIcDrive_IcSpiCtrlPtr)(data,&rslt_off);
            //rslt_off = wr_rd_data(data);
            
            
            // change diagnostic mode to IO
           /* diag_tle8102sg_IO_mode();   
            spi_ctrl_output(TLE8102_CH1, TLE8102_CH_ON, &lwd_temp_err_code);
            spi_ctrl_output(TLE8102_CH2, TLE8102_CH_ON, &lwd_temp_err_code);
            if (TLE_CO1 == 1)
            {
                pin_st_on |= 0x01;
            }
            if (TLE_CO2 == 1)
            {
                pin_st_on |= 0x02;
            }
            
            spi_ctrl_output(TLE8102_CH1, TLE8102_CH_OFF, &lwd_temp_err_code);
            spi_ctrl_output(TLE8102_CH2, TLE8102_CH_OFF, &lwd_temp_err_code);
            if (TLE_CO1 == 1)
            {
                pin_st_off |= 0x01;
            }
            if (TLE_CO2 == 1)
            {
                pin_st_off |= 0x02;
            }
            
            // change diagnostic mode to SPI
            diag_tle8102sg_SPI_mode();
            */ 
            
            if ( (0x10 == (rslt_on & 0x30)) && (0x00 == (rslt_off & 0x30)))
            {
                *Err_CodePtr |= RELAY_IC_CH2_SHORT_GND_ERR;
            }
            else if ( 0x10 == (rslt_on & 0x30)) 
            {
                *Err_CodePtr |= RELAY_IC_CH2_UNDER_CURR_ERR;
            }
            else if ( 0x10 == (rslt_off & 0x30)) 
            {
                *Err_CodePtr |= RELAY_IC_CH2_OPEN_LOAD_ERR;
            }
            else if (0x20 == (rslt_on & 0x30))  
            {
                if ( 0x02 == (rslt_on & 0x02)) 
                {
                    *Err_CodePtr |= RELAY_IC_CH2_OVER_TEMPER_ERR;
                }
                else
                {
                    *Err_CodePtr |= RELAY_IC_CH2_OVER_LOAD_ERR;
                }
            }
            
            if ( (0x04 == (rslt_on & 0x0C)) && (0x00 == (rslt_off & 0x0C)) )
            {
                *Err_CodePtr |= RELAY_IC_CH1_SHORT_GND_ERR;
            }
            else if (0x04 == (rslt_on & 0x0C)) 
            {
                *Err_CodePtr |= RELAY_IC_CH1_UNDER_CURR_ERR;
            }
            else if (0x04 == (rslt_off & 0x0C)) 
            {
                *Err_CodePtr |= RELAY_IC_CH1_OPEN_LOAD_ERR;
            }
            else if (0x08 == (rslt_on & 0x0C)) 
            {
                if ( 0x02 == (rslt_off & 0x02)) 
                {
                    *Err_CodePtr |= RELAY_IC_CH1_OVER_TEMPER_ERR;
                }
                else
                {
                    *Err_CodePtr |= RELAY_IC_CH1_OVER_LOAD_ERR;
                }
            }
            
            if ( 0x01 == (rslt_on & 0x01)) 
            {
                *Err_CodePtr |= RELAY_IC_OVER_TEMP_ERR;
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
}

GeneralReturnType RelayIcDrive_IcSpiCtrlOutput(uByte RelayIcDrive_Id,eRelayIcDrive_ChannelType channle, uByte cmd, uWord *Err_CodePtr)
{
    uByte tmp = 0;
    GeneralReturnType rslt;
    GeneralReturnType Spi_st;
    const sRelayIcDrive_PropertyType *RelayIcDrive_PropPtr;
    const sRelayIcDrive_OperationType *RelayIcDrive_OperaPtr;
    bool Relay_Status = 0;
    *Err_CodePtr = 0;
    rslt = (GeneralReturnType)ERR_OK;
    if(RelayIcDrive_Id >= RELAYICDRIVE_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;  
    }
    RelayIcDrive_PropPtr = GET_RELAYICDRIVEPROPERTY(RelayIcDrive_Id); //point to property
    RelayIcDrive_OperaPtr = GET_RELAYICDRIVEOPERATION(RelayIcDrive_Id);  //point to operation
    
    if((uByte)(RelayIcDrive_PropPtr->RelayIcDrive_IdNum) == RelayIcDrive_Id)   //match the spi number
    {
        if (TLE_IC_CHANNEL1 == channle)
        {
            if (RELAY_IC_CH_ON == cmd)
            {
                gby_out_ctrl_st |= RELAY_IC_CTRL_CH1_ON;
            }
            else
            {
                gby_out_ctrl_st &= (~RELAY_IC_CTRL_CH1_ON);
            }
        }
        else
        {
            if (RELAY_IC_CH_ON == cmd)
            {
                gby_out_ctrl_st |= RELAY_IC_CTRL_CH2_ON;
            }
            else
            {
                gby_out_ctrl_st &= (~RELAY_IC_CTRL_CH2_ON);
            }
        }
        tmp = RELAY_IC_OUTPUT_CTRL;
        tmp |= gby_out_ctrl_st;
        if(RelayIcDrive_OperaPtr->RelayIcDrive_IcSpiCtrlPtr != NULL) 
        {
            Spi_st = (*RelayIcDrive_OperaPtr->RelayIcDrive_IcSpiCtrlPtr)(tmp,NULL);
            if(Spi_st != ERR_OK)
            {
               return Spi_st;
            }
            if(RelayIcDrive_OperaPtr->RelayIcDrive_IcReadStatusPtr != NULL) 
            {  
                Relay_Status = (*RelayIcDrive_OperaPtr->RelayIcDrive_IcReadStatusPtr)();
                if(Relay_Status != 0)
                {
                   *Err_CodePtr |= RELAY_IC_FAULT;
                }
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
}

GeneralReturnType RelayIcDrive_IcIoCtrlOutput(uByte RelayIcDrive_Id,eRelayIcDrive_ChannelType channle, uByte cmd, uWord *Err_CodePtr)
{
    GeneralReturnType rslt;
    const sRelayIcDrive_PropertyType *RelayIcDrive_PropPtr;
    const sRelayIcDrive_OperationType *RelayIcDrive_OperaPtr;
    
    if(NULL != Err_CodePtr)
    {
        *Err_CodePtr = 0;    
    }
    
    rslt = (GeneralReturnType)ERR_OK;
    if(RelayIcDrive_Id >= RELAYICDRIVE_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;  
    }
    RelayIcDrive_PropPtr = GET_RELAYICDRIVEPROPERTY(RelayIcDrive_Id); //point to property
    RelayIcDrive_OperaPtr = GET_RELAYICDRIVEOPERATION(RelayIcDrive_Id);  //point to operation
    
    if((uByte)(RelayIcDrive_PropPtr->RelayIcDrive_IdNum) == RelayIcDrive_Id)   //match the spi number
    {
        if (TLE_IC_CHANNEL1 == channle)
        {
            if(RelayIcDrive_OperaPtr->RelayIcDrive_IcCh1DoCtrlPtr != NULL) 
            {
                (*RelayIcDrive_OperaPtr->RelayIcDrive_IcCh1DoCtrlPtr)((bool)cmd);
            }
            else
            {
                rslt = (GeneralReturnType)ERR_IO_FUNCTION_PTR_NULL;
            }
        }
        else 
        {
            if(RelayIcDrive_OperaPtr->RelayIcDrive_IcCh2DoCtrlPtr != NULL) 
            {
                (*RelayIcDrive_OperaPtr->RelayIcDrive_IcCh2DoCtrlPtr)((bool)cmd);
            }
            else
            {
                rslt = (GeneralReturnType)ERR_IO_FUNCTION_PTR_NULL;
            }
        } 
    } 
    else 
    {
        rslt = (GeneralReturnType)ERR_IO_ID_NUM_NOTMATCH;   //the id number is not match
    }

    return rslt;
}


GeneralReturnType RelayIcDrive_IcResetRegister(uByte RelayIcDrive_Id)
{
    GeneralReturnType rslt;
    GeneralReturnType Spi_st;
    const sRelayIcDrive_PropertyType *RelayIcDrive_PropPtr;
    const sRelayIcDrive_OperationType *RelayIcDrive_OperaPtr;

    rslt = (GeneralReturnType)ERR_OK;
    if(RelayIcDrive_Id >= RELAYICDRIVE_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;  
    }
    RelayIcDrive_PropPtr = GET_RELAYICDRIVEPROPERTY(RelayIcDrive_Id); //point to property
    RelayIcDrive_OperaPtr = GET_RELAYICDRIVEOPERATION(RelayIcDrive_Id);  //point to operation
    
    if((uByte)(RelayIcDrive_PropPtr->RelayIcDrive_IdNum) == RelayIcDrive_Id)   //match the spi number
    { 
        if(RelayIcDrive_OperaPtr->RelayIcDrive_IcSpiCtrlPtr != NULL) 
        {
            Spi_st = (*RelayIcDrive_OperaPtr->RelayIcDrive_IcSpiCtrlPtr)((uByte)RELAY_IC_RESET_REG,NULL);
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
    else 
    {
        rslt = (GeneralReturnType)ERR_IO_ID_NUM_NOTMATCH;   //the id number is not match
    }
    return rslt;
}

GeneralReturnType RelayIcDrive_IcGotoSleep(uByte RelayIcDrive_Id)
{
    GeneralReturnType rslt;
    GeneralReturnType Spi_st;
    const sRelayIcDrive_PropertyType *RelayIcDrive_PropPtr;
    const sRelayIcDrive_OperationType *RelayIcDrive_OperaPtr;

    rslt = (GeneralReturnType)ERR_OK;
    if(RelayIcDrive_Id >= RELAYICDRIVE_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;  
    }
    RelayIcDrive_PropPtr = GET_RELAYICDRIVEPROPERTY(RelayIcDrive_Id); //point to property
    RelayIcDrive_OperaPtr = GET_RELAYICDRIVEOPERATION(RelayIcDrive_Id);  //point to operation
    
    if((uByte)(RelayIcDrive_PropPtr->RelayIcDrive_IdNum) == RelayIcDrive_Id)   //match the spi number
    { 
        if(RelayIcDrive_OperaPtr->RelayIcDrive_IcSpiCtrlPtr != NULL) 
        {
            Spi_st = (*RelayIcDrive_OperaPtr->RelayIcDrive_IcSpiCtrlPtr)((uByte)RELAY_IC_SLEEP,NULL);
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
    else 
    {
        rslt = (GeneralReturnType)ERR_IO_ID_NUM_NOTMATCH;   //the id number is not match
    }
    return rslt;
}

GeneralReturnType RelayIcDrive_IcWakeUp(uByte RelayIcDrive_Id)
{
    GeneralReturnType rslt;
    GeneralReturnType Spi_st;
    const sRelayIcDrive_PropertyType *RelayIcDrive_PropPtr;
    const sRelayIcDrive_OperationType *RelayIcDrive_OperaPtr;

    rslt = (GeneralReturnType)ERR_OK;
    if(RelayIcDrive_Id >= RELAYICDRIVE_MAX_NUM) 
    {
        rslt = (GeneralReturnType)ERR_IO_RANGE;
        return rslt;  
    }
    RelayIcDrive_PropPtr = GET_RELAYICDRIVEPROPERTY(RelayIcDrive_Id); //point to property
    RelayIcDrive_OperaPtr = GET_RELAYICDRIVEOPERATION(RelayIcDrive_Id);  //point to operation
     
    if((uByte)(RelayIcDrive_PropPtr->RelayIcDrive_IdNum) == RelayIcDrive_Id)   //match the spi number
    { 
        if(RelayIcDrive_OperaPtr->RelayIcDrive_IcSpiCtrlPtr != NULL) 
        {
            Spi_st = (*RelayIcDrive_OperaPtr->RelayIcDrive_IcSpiCtrlPtr)((uByte)RELAY_IC_WAKE_UP,NULL);
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
    else 
    {
        rslt = (GeneralReturnType)ERR_IO_ID_NUM_NOTMATCH;   //the id number is not match
    }

    return rslt;
}


