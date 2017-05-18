// Xgate_StartIf.c  
// Description: Define Start xgate from CPU12X   


#include "Xgate_Vector.h"
#include "IO_Map.h"
#include "Xgate_Config.h"
#include "AC_CP_IOC.h"
#include "Timer_1ms.h"


#pragma push

/* Two stacks in XGATE core3 */ 
#pragma DATA_SEG XGATE_STK_L
word XGATE_STACK_L[1]; 
#pragma DATA_SEG XGATE_STK_H
word XGATE_STACK_H[1];

#pragma pop

#define ROUTE_INTERRUPT(vec_adr, cfdata)                \
  INT_CFADDR= (vec_adr) & 0xF0;                         \
  INT_CFDATA_ARR[((vec_adr) & 0x0F) >> 1]= (cfdata)

/* define for start interrupt in XGATE 
   vector address= 2 * channel id in XGATE
*/
#define VEC_ADDR_XSRAM20K_ACCESS_VIOLATON 0x60  // Channel 30 - XSRAM20K Access Violation
#define VEC_ADDR_XGATE_SOFTWARE_ERROR     0x62  // Channel 31 - XGATE Software Error Interrupt     
#define VEC_ADDR_XGATE_SOFTWARE_TRIGGER7  0x64  // Channel 32 - XGATE Software Trigger 7           
#define VEC_ADDR_XGATE_SOFTWARE_TRIGGER6  0x66  // Channel 33 - XGATE Software Trigger 6           
#define VEC_ADDR_XGATE_SOFTWARE_TRIGGER5  0x68  // Channel 34 - XGATE Software Trigger 5           
#define VEC_ADDR_XGATE_SOFTWARE_TRIGGER4  0x6A  // Channel 35 - XGATE Software Trigger 4           
#define VEC_ADDR_XGATE_SOFTWARE_TRIGGER3  0x6C  // Channel 36 - XGATE Software Trigger 3           
#define VEC_ADDR_XGATE_SOFTWARE_TRIGGER2  0x6E  // Channel 37 - XGATE Software Trigger 2           
#define VEC_ADDR_XGATE_SOFTWARE_TRIGGER1  0x70  // Channel 38 - XGATE Software Trigger 1           
#define VEC_ADDR_XGATE_SOFTWARE_TRIGGER0  0x72  // Channel 39 - XGATE Software Trigger 0       
#define VEC_ADDR_PIT3 0x74  // Channel 3A - Periodic Interrupt Timer           
#define VEC_ADDR_PIT2 0x76  // Channel 3B - Periodic Interrupt Timer           
#define VEC_ADDR_PIT1 0x78  // Channel 3C - Periodic Interrupt Timer           
#define VEC_ADDR_PIT0 0x7A  // Channel 3D - Periodic Interrupt Timer           
#define VEC_ADDR_RSV_3E  0x7C // Channel 3E - Reserved                           
#define VEC_ADDR_API  0x7E  // Channel 3F - Autonomous Periodical interrupt API
#define VEC_ADDR_LVI  0x80  // Channel 40 - Low Voltage interrupt LVI
#define VEC_ADDR_IIC1  0x82  // Channel 41 - IIC1 Bus                 
#define VEC_ADDR_SCI5  0x84  // Channel 42 - SCI5                     
#define VEC_ADDR_SCI4  0x86 // Channel 43 - SCI4                     
#define VEC_ADDR_SCI3  0x88 // Channel 44 - SCI3                     
#define VEC_ADDR_SCI2  0x8A // Channel 45 - SCI2                     
#define VEC_ADDR_PWM_EMERGENCY_SHUTDOWN  0x8C  // Channel 46 - PWM Emergency Shutdown   
#define VEC_ADDR_PORT_P 0x8E  // Channel 47 - Port P Interrupt         
#define VEC_ADDR_CAN4_TX    0x90  // Channel 48 - CAN4 transmit            
#define VEC_ADDR_CAN4_RX    0x92  // Channel 49 - CAN4 receive             
#define VEC_ADDR_CAN4_ERR   0x94  // Channel 4A - CAN4 errors              
#define VEC_ADDR_CAN4_WKUP  0x96  // Channel 4B - CAN4 wake-up             
#define VEC_ADDR_CAN3_TX    0x98  // Channel 4C - CAN3 transmit            
#define VEC_ADDR_CAN3_RX    0x9A  // Channel 4D - CAN3 receive             
#define VEC_ADDR_CAN3_ERR   0x9C  // Channel 4E - CAN3 errors              
#define VEC_ADDR_CAN3_WKUP  0x9E  // Channel 4F - CAN3 wake-up             
#define VEC_ADDR_CAN2_TX    0xA0  // Channel 50 - CAN2 transmit
#define VEC_ADDR_CAN2_RX    0xA2 // Channel 51 - CAN2 receive 
#define VEC_ADDR_CAN2_ERR   0xA4 // Channel 52 - CAN2 errors  
#define VEC_ADDR_CAN2_WKUP  0xA6  // Channel 53 - CAN2 wake-up 
#define VEC_ADDR_CAN1_TX    0xA8 // Channel 54 - CAN1 transmit
#define VEC_ADDR_CAN1_RX    0xAA // Channel 55 - CAN1 receive 
#define VEC_ADDR_CAN1_ERR   0xAC  // Channel 56 - CAN1 errors  
#define VEC_ADDR_CAN1_WKUP  0xAE   // Channel 57 - CAN1 wake-up 
#define VEC_ADDR_CAN0_TX    0xB0 // Channel 58 - CAN0 transmit
#define VEC_ADDR_CAN0_RX    0xB2 // Channel 59 - CAN0 receive 
#define VEC_ADDR_CAN0_ERR   0xB4  // Channel 5A - CAN0 errors  
#define VEC_ADDR_CAN0_WKUP  0xB6   // Channel 5B - CAN0 wake-up 
#define VEC_ADDR_FLASH   0xB8  // Channel 5C - FLASH 
#define VEC_ADDR_EEPROM   0xBA // Channel 5D - EEPROM
#define VEC_ADDR_SPI2     0xBC  // Channel 5E - SPI2  
#define VEC_ADDR_SPI1     0xBE // Channel 5F - SPI1
#define VEC_ADDR_IIC0     0xC0 // Channel 60 - IIC0 Bus                         
#define VEC_ADDR_RSV_61  0xC2 // Channel 61 - Reserved                         
#define VEC_ADDR_CRG_SCM  0xC4 // Channel 62 - CRG Self Clock Mode              
#define VEC_ADDR_CRG_PLL_LOCK  0xC6 // Channel 63 - CRG PLL lock                     
#define VEC_ADDR_PAB_OVER  0xC8  // Channel 64 - Pulse Accumulator B Overflow     
#define VEC_ADDR_MDCU 0xCA // Channel 65 - Modulus Down Counter underflow   
#define VEC_ADDR_PORT_H  0xCC  // Channel 66 - Port H                           
#define VEC_ADDR_PORT_J  0xCE  // Channel 67 - Port J                           
#define VEC_ADDR_ATD1  0xD0 // Channel 68 - ATD1                             
#define VEC_ADDR_ATD0  0xD2  // Channel 69 - ATD0                             
#define VEC_ADDR_SCI1  0xD4  // Channel 6A - SCI1                             
#define VEC_ADDR_SCI0  0xD6  // Channel 6B - SCI0                             
#define VEC_ADDR_SPI0  0xD8 // Channel 6C - SPI0                             
#define VEC_ADDR_PAIE  0xDA  // Channel 6D - Pulse accumulator input edge     
#define VEC_ADDR_PAA_OVER 0xDC // Channel 6E - Pulse accumulator A overflow     
#define VEC_ADDR_ECT_OVER 0xDE // Channel 6F - Enhanced Capture Timer overflow  
#define VEC_ADDR_ECT_CH7  0xE0 // Channel 70 - Enhanced Capture Timer channel 7                                 
#define VEC_ADDR_ECT_CH6  0xE2  // Channel 71 - Enhanced Capture Timer channel 6 
#define VEC_ADDR_ECT_CH5  0xE4 // Channel 72 - Enhanced Capture Timer channel 5 
#define VEC_ADDR_ECT_CH4  0xE6 // Channel 73 - Enhanced Capture Timer channel 4 
#define VEC_ADDR_ECT_CH3  0xE8 // Channel 74 - Enhanced Capture Timer channel 3 
#define VEC_ADDR_ECT_CH2  0xEA  // Channel 75 - Enhanced Capture Timer channel 2 
#define VEC_ADDR_ECT_CH1  0xEC  // Channel 76 - Enhanced Capture Timer channel 1 
#define VEC_ADDR_ECT_CH0  0xEE // Channel 77 - Enhanced Capture Timer channel 0 
#define VEC_ADDR_RTI   0xF0 // Channel 78 - Real Time Interrupt 
#define VEC_ADDR_IRQ   0xF2 // Channel 79 - IRQ

//#include "Timer_1ms.h"
void Xgate_SetupAndStart(void) 
{
  /* initialize the XGATE vector block and
     set the XGVBR register to its start address */
  XGVBR= (unsigned int)(void*__far)(XGATE_VectorTable - XGATE_VECTOR_OFFSET);
  INT_XGPRIO = 1;
  
  /* switch software trigger 0 interrupt to XGATE */
  ROUTE_INTERRUPT(VEC_ADDR_XGATE_SOFTWARE_TRIGGER0, 0x81); /* RQST=1 and PRIO=1 */

#ifdef XGATE_ENABLE_TIME  
  (void)Timer_1ms_Enable(); // congfig pti0 register and enable interrupt
  /* switch Periodic Interrupt Timer interrupt to XGATE */   
  ROUTE_INTERRUPT(VEC_ADDR_PIT0, 0x81);  /* RQST=1 and PRIO=1 */  
#endif

  /* switch Ac Cp Interrupt to XGATE */ 
#ifdef XGATE_ENABLE_ACCP    
  ROUTE_INTERRUPT(VEC_ADDR_ECT_CH5, 0x87);  /* RQST=1 and PRIO=1 */
#endif

#ifdef XGATE_ENABLE_EXTFLASH 
  ROUTE_INTERRUPT(VEC_ADDR_XGATE_SOFTWARE_TRIGGER2, 0x81); /* RQST=1 and PRIO=1 */
#endif

  /* when changing your derivative to non-core3 one please remove next five lines */
  XGISPSEL= 1;
  XGISP31= (unsigned int)(void*__far)(XGATE_STACK_L + 1);
  XGISPSEL= 2;
  XGISP74= (unsigned int)(void*__far)(XGATE_STACK_H + 1);
  XGISPSEL= 0;

  /* enable XGATE mode and interrupts */
  XGMCTL= 0xFBC1; /* XGE | XGFRZ | XGIE */

  /* force execution of software trigger 0 handler */
//  XGSWT= 0x0101;  // close interrupt "VEC_ADDR_XGATE_SOFTWARE_TRIGGER0"
}
