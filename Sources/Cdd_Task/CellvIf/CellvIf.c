#include "CellvIf.h"
//#include "Cellv_Cfg.h"
#include "EcuConf.h"
#include "Ecu.h"
#include "WdgIf.h"
#include "LTC_CS.h"
#include "LTC_SPI.h"


uLWord glwd_Cnt,glwd_Cnt1,glwd_Cnt3;

//uByte config1[MAX_CELLV_IC_MODULE_NUM][8];
uByte Config[MAX_CELLV_IC_MODULE_NUM][6];
uLWord glwd_Cnt2;

uLWord CellvIf_GetCellvCrcErrCnt(void)
{
    return glwd_Cnt;
}

uLWord CellvIf_GetTemperCrcErrCnt(void)
{
    return glwd_Cnt3;
}

void CellvIf_CsOutPutHigh(void)
{
    LTC_CS_PutVal(1);
    ECU_DELAY100US(15);
}
void CellvIf_CsOutPutLow(void)
{
    LTC_CS_PutVal(0);
    ECU_DELAY100US(7);
}   

uWord CellvIf_Init(uByte device_num)
{
   uByte i; 
   uWord rslt;                                 
   
   if(device_num > MAX_CELLV_IC_MODULE_NUM)
   {
       return ERR_CELLV_IDOUTRANG; 
   }
   
   for(i = 0; i < device_num; i++) 
   {
      Config[i][0] = 0xFE;
      Config[i][1] = 0x00;
      Config[i][2] = 0x00;
      Config[i][3] = 0x00;
      Config[i][4] = 0x00;
      Config[i][5] = 0x00;
   }

   for(i = 0; i < 3; i++) 
   {
      rslt = CellvIf_WrAndRdCfg(device_num, Config);//, config1); 
      if(ERR_OK == rslt)
      {
         break;
      }
      else
      {
         glwd_Cnt2++;
      }
   }

   if(i >= 3) 
   {
     return ERR_WR_CFG;
   }
   return ERR_OK;
}

uWord CellvIf_StartCnvt(uByte mode)
{
  uByte cmd[4];
  uWord cmd_pec;
        

  if(AD_CNVT_MODE_NORMAL == mode) 
  {  cmd[0] = 0x03;
     cmd[1] = 0x60;//0xE8;//0x68;
  } 
  else if(AD_CNVT_MODE_OPEN_CIRCUIT_PD == mode)
  {
     cmd[0] = 0x03;
     cmd[1] = 0x28;//0xA8;//0x28;
  }
  else if(AD_CNVT_MODE_OPEN_CIRCUIT_PU == mode)
  {
     cmd[0] = 0x03;
     cmd[1] = 0x68;//0xE8;//0x68;
  }
  else if(AD_CNVT_MODE_BAL_CIRCUIT == mode)
  {
     cmd[0] = 0x03;
     cmd[1] = 0x70;
  }
  else
  {
     cmd[0] = 0x03;
     cmd[1] = 0x60;
  }
 
  cmd_pec = CellvIf_Pec15Calc(2, cmd);
  cmd[2] = (uByte)(cmd_pec >> 8);
  cmd[3] = (uByte)(cmd_pec);
   
  CellvIf_CsOutPutLow();
  CellvIf_SpiWrArray(4, cmd);
  CellvIf_CsOutPutHigh();
  
  return ERR_OK;

}

uByte CellvIf_RdValue(uByte device_num, uWord cell_codes[][12])
{  
    uByte cell_data[80];
    uByte pec_error = 0;
    uWord parsed_cell;
    uWord received_pec;
    uWord data_pec;
    uByte data_counter=0; 
    uByte current_ic;
    uByte current_cell;
    uByte cell_reg;
    
    pec_error = ERR_OK; 
    CellvIf_WakeIdle();
    
    for(cell_reg = 0; cell_reg < 4; cell_reg++)         			 			
    {
        data_counter = 0;
        CellvIf_RdCvReg(cell_reg, device_num, cell_data);							
  	    
        for (current_ic = 0 ; current_ic < device_num; current_ic++) 		
        {																 	  		
  	     	 for(current_cell = 0; current_cell < 3; current_cell++)
  	     	 {
  	     	     parsed_cell = cell_data[data_counter] + (cell_data[data_counter + 1] << 8);
  	     	     if(((current_cell  + (cell_reg * 3))==0)&&(parsed_cell!=0))																					 														        
               cell_codes[current_ic][current_cell  + (cell_reg * 3)] = parsed_cell+50;
  	     	     
  	     	     else if(((current_cell  + (cell_reg * 3))==11)&&(parsed_cell!=0))	
  	     	     cell_codes[current_ic][current_cell  + (cell_reg * 3)] = parsed_cell+30;
  	     	     
  	     	     else
  	     	     cell_codes[current_ic][current_cell  + (cell_reg * 3)] = parsed_cell;
               
               data_counter += 2;	
  	     	 }
           
           received_pec = (cell_data[data_counter] << 8) + cell_data[data_counter + 1]; 
  																				  
           data_pec = CellvIf_Pec15Calc(6, &cell_data[current_ic * 8]);
           if(received_pec != data_pec)
           {
              pec_error |= (1 << current_ic);//ERR_PEC_CHK;
              if(glwd_Cnt < 6666666)
              {  
                 glwd_Cnt++;
              }
           } 
           else 
           {
              //pec_error = ERR_OK;
              glwd_Cnt1++;
           }
           data_counter += 2;																												
        } 
    }
    return (pec_error);
}



void CellvIf_StartAdAxCvnt(void)
{
  uByte cmd[4];
  uWord cmd_pec;
 
  cmd[0] = 0x05;
  cmd[1] = 0x60;
  cmd_pec = CellvIf_Pec15Calc(2, cmd);
   
  cmd[2] = (uByte)(cmd_pec >> 8);
  cmd[3] = (uByte)(cmd_pec);
 
  CellvIf_WakeIdle(); 
    
  CellvIf_CsOutPutLow();
  CellvIf_SpiWrArray(4,cmd);
  CellvIf_CsOutPutHigh();

}



uByte CellvIf_RdAuxValue(uByte total_ic, uWord aux_codes[][6])
{
    
  const uByte NUM_RX_BYT = 8;
  const uByte BYT_IN_REG = 6;
  const uByte GPIO_IN_REG = 3;
  
  uByte data[80];
  uByte data_counter = 0; 
  uByte pec_error = 0;
  uWord parsed_aux;
  uWord received_pec;
  uWord data_pec;
  uByte gpio_reg;
  uByte current_gpio;
  uByte current_ic;

  CellvIf_WakeIdle();

  for( gpio_reg = 0; gpio_reg < 2; gpio_reg++)//two reg
  {
      data_counter = 0;
      CellvIf_RdAuxReg(gpio_reg, total_ic,data);
      								  
      for (current_ic = 0 ; current_ic < total_ic; current_ic++)
      {
          																 	  		
      	for( current_gpio = 0; current_gpio < GPIO_IN_REG; current_gpio++)
      	{
    	    parsed_aux = data[data_counter] + (data[data_counter + 1] << 8);              																
          aux_codes[current_ic][current_gpio +(gpio_reg * GPIO_IN_REG)] = parsed_aux;
          data_counter = data_counter + 2;
      	}
     
        received_pec = (data[data_counter] << 8) + data[data_counter + 1]; 																				    
        data_pec = CellvIf_Pec15Calc(BYT_IN_REG, &data[current_ic * NUM_RX_BYT]);
        if(received_pec != data_pec)
        {
          pec_error |= (0x01 << current_ic);
          if(glwd_Cnt3 < 66666666)
          {  
              glwd_Cnt3++;
          }
        }
     
        data_counter = data_counter + 2;																												
     
      }
      
  }
  
  return (pec_error);
}


void CellvIf_RdAuxReg(uByte reg, uByte total_ic, uByte *data)
{
  const uByte REG_LEN = 8; 
  uByte cmd[4];
  uWord cmd_pec;
  
  if (reg == 0)		
  {
    cmd[0] = 0x00;    
    cmd[1] = 0x0C;
  }
  else if(reg == 1)	
  {
    cmd[0] = 0x00;
    cmd[1] = 0x0e;
  } 
  else					
  {
     cmd[0] = 0x00;
     cmd[1] = 0x0C;		  
  }
 
  cmd_pec = CellvIf_Pec15Calc(2, cmd);
  cmd[2] = (uint8_t)(cmd_pec >> 8);
  cmd[3] = (uint8_t)(cmd_pec);
  
  
  //CellvIf_WakeIdle(); 

  CellvIf_CsOutPutLow();
  CellvIf_SpiWrAndRd(cmd, 4, data,(REG_LEN*total_ic));
  CellvIf_CsOutPutHigh();

}

void CellvIf_RdCvReg(uByte reg,  uByte total_ic, uByte *data)
{ 
  uByte cmd[4];
  uWord cmd_pec;
  
  if(reg == 0)     
  {
    cmd[0] = 0x00;
    cmd[1] = 0x04;   
  }
  else if(reg == 1) 
  {
    cmd[0] = 0x00;
    cmd[1] = 0x06;    
  } 
  else if(reg == 2) 
  {
    cmd[0] = 0x00;
    cmd[1] = 0x08;
  } 
  else if(reg == 3) 
  {
    cmd[0] = 0x00;
    cmd[1] = 0x0A;
  } 
  else 
  {
    cmd[0] = 0x00;
    cmd[1] = 0x04;   
  }


 // CellvIf_WakeIdle();
 
  cmd_pec = CellvIf_Pec15Calc(2, cmd);
  cmd[2] = (uByte)(cmd_pec >> 8);
  cmd[3] = (uByte)(cmd_pec); 

  CellvIf_CsOutPutLow();
  CellvIf_SpiWrAndRd(cmd, 4, data,(8 * total_ic));
  CellvIf_CsOutPutHigh();

}


uWord CellvIf_CtrlBalanceSw(uByte DeviceId, uByte Row, uByte Cmd)
{
    uByte lby_num;
    
    if(Row < 8)                             
    {                                       
        lby_num = Row;
        if (DCH_BALANCE_ON == Cmd)
        {
            Config[DeviceId][4] |= (uByte)(0x01U << lby_num);
        }
        else
        {
            Config[DeviceId][4] &=  (~(0x01U << lby_num));
        }
    }
    else if (Row < 12)
    {
        lby_num = Row - 8;

        if (DCH_BALANCE_ON == Cmd)
        {
            Config[DeviceId][5] |= (uByte)(0x01U << lby_num);
        }
        else
        {
            Config[DeviceId][5] &=  (uByte)(~(0x01U << lby_num));
        }
    }
    else
    {
        return ERR_BALANCE_DCH;
    } 
    return ERR_OK;
}

uWord CellvIf_ActiveBalSw(void)
{
    uByte i;
    uWord rslt =  ERR_OK;
    
    for(i = 0; i < 3; i++)
    {
        rslt = CellvIf_WrAndRdCfg(Gby_Ic_Num,  Config);//,  config1);
        if(ERR_OK == rslt)
        {
          break;
        }
    }
    
    if (i >= 3)
    {
        return ERR_BALANCE_DCH;           
    }  
    
    
    return rslt;
}

uByte CellvIf_WrAndRdCfg(uByte total_ic, uByte config[][6])//, uByte r_config[][8])
{
  uByte data[84],cmd[4];
  uWord cfg_pec;
  uByte cmd_index; 
  uByte current_ic;
  uByte current_byte;
  uByte pec_error = ERR_OK;
  uWord received_pec;
  uWord data_pec; 
  uByte j,r_config[MAX_CELLV_IC_MODULE_NUM][6];
  
  data[0] = 0x00;
  data[1] = 0x01;
  data[2] = 0x3d;
  data[3] = 0x6e;
  
  cmd_index = 4;
  for (current_ic = total_ic; current_ic > 0; current_ic--) 			
  {																																				
    for (current_byte = 0; current_byte < 6; current_byte++) 
    {																		
      data[cmd_index] = config[current_ic - 1][current_byte]; 						
      cmd_index++;                
    }

    cfg_pec = (uWord)CellvIf_Pec15Calc(6, &config[current_ic - 1][0]);		
    data[cmd_index] = (uByte)(cfg_pec >> 8);
    data[cmd_index + 1] = (uByte)cfg_pec;
    cmd_index += 2;
  }
  
 
  CellvIf_WakeIdle(); 
  
   		
  CellvIf_CsOutPutLow();
  CellvIf_SpiWrArray((4 + 8 * total_ic), data);
  CellvIf_CsOutPutHigh();
  
  /*cmd[0] = 0x00;
  cmd[1] = 0x02;
  cmd[2] = 0x2b;
  cmd[3] = 0x0A;
 
 
  CellvIf_CsOutPutLow();
  CellvIf_SpiWrAndRd(cmd, 4, data, (8 * total_ic));        
  CellvIf_CsOutPutHigh();
  														
  pec_error = 0;
  for (current_ic = 0; current_ic < total_ic; current_ic++) 		
  { 																																																									
    for (current_byte = 0; current_byte < 8; current_byte++)					
    {
      r_config[current_ic][current_byte] = data[current_byte + (current_ic * 8)];
    }

    received_pec = (r_config[current_ic][6] << 8) + r_config[current_ic][7];
    data_pec = CellvIf_Pec15Calc(6, &r_config[current_ic][0]);
    if(received_pec != data_pec)
    {
      pec_error = ERR_PEC_CHK;
    } 
    else 
    {
      pec_error = ERR_OK;
    }
  }

  for (current_ic = 0; current_ic < total_ic; current_ic++) 
  {
      for(j = 0; j < 6; j++)
      {
         if(Config[current_ic][j] != r_config[current_ic][j])
         {
          pec_error = ERR_WR_CFG;
         break;
         }
      }
  } */

  return(pec_error);
   
}

uWord CellvIf_Pec15Calc(uByte len, 	uByte *data)
{
	 uWord remainder,addr;
 	 uByte i;
	
	 remainder = 16;
	 for(i = 0; i < len;i++) 
	 {
	   addr = ((remainder >> 7) ^ data[i]) & 0xff;
		 remainder = (remainder << 8) ^ crc15Table[addr];
 	 }
	 return(remainder * 2);
}


void CellvIf_SpiWrArray(uByte len, uByte data[]) 
{
    uByte i;
    for( i = 0; i < len; i++) 
    {
       (void)CellvIf_SpiData(data[i]);
    }
}


void CellvIf_SpiWrAndRd(uByte tx_Data[], uByte tx_len, uByte *rx_data, uByte rx_len)
{
    uByte i;
    for( i = 0; i < tx_len; i++) 
    {
       (void)CellvIf_SpiData(tx_Data[i]);
    }
    
    for( i = 0; i < rx_len; i++) 
    {
       *(rx_data++) = CellvIf_SpiData(0xFFU);
    }
}

uByte CellvIf_SpiData(uByte data)
{
   uWord i;
   uByte rslt_data;
   
   for(i = 0; i < 65000; i++) 
   {
      if(ERR_OK == LTC_SPI_SendChar(data)) 
      {
         break;
      }
   }
   
   for(i = 0; i < 65000; i++)
   {
      if(ERR_OK == LTC_SPI_RecvChar(&rslt_data))
      {
         return rslt_data;
      }   
   }
   return 0xFF; 
}

void CellvIf_WakeIdle(void) 
{  
  uByte cmd[2];
  
  cmd[0] = 0;
  cmd[1] = 0;
  CellvIf_CsOutPutLow(); 
  CellvIf_SpiWrArray(2, cmd);
  CellvIf_CsOutPutHigh();
  //ECU_DELAY100US(5);
  //ECU_DELAY100US(2);
  
  CellvIf_CsOutPutLow(); 
  CellvIf_SpiWrArray(1, cmd);
  CellvIf_CsOutPutHigh();
 // ECU_DELAY100US(2);

}