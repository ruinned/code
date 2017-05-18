// ATB.c
#include "ATB.h"

/*
 * define special flash address for bootloader
 */
#define JMP_TO_BOOT_ADDR   0xDC00 // 19 bytes
#define ADDR_APP_BAUDRATE  0xDC60 // 1 byte, 0 --- 500K 1 ---- 250k other --- 500k

/*
 * define application lock address
 */
#define APP_LOCK_ADDR 0xFBF0 
/*
 * Return :
 * 1 --- BAUDRATE_250K
 * 0 --- BAUDRATE_500K
 */ 

const unsigned char gby_app_baudrate @ ADDR_APP_BAUDRATE = ATB_BAUDRATE_500K;
unsigned char ATB_GetAppBaudrate(void)
{
  #define SUPPORT_BAUDRATE_NUM   2  // support 500k and 250k
  if(*((unsigned char *)ADDR_APP_BAUDRATE) < SUPPORT_BAUDRATE_NUM)
  {
     return *((unsigned char *)ADDR_APP_BAUDRATE);
  }
  else
  {
     /* default is  ATB_BAUDRATE_500K if no set*/
     return ATB_BAUDRATE_500K;
  }
}

unsigned char ATB_UnlockApp(void)
{
  unsigned long seed,key,seed_or_key,seed_and_key,llwd_tmp;   
  seed =  *(unsigned long*)(APP_LOCK_ADDR);
  key =   *((unsigned long*)(APP_LOCK_ADDR) + 1);
  seed_or_key = *((unsigned long*)(APP_LOCK_ADDR) + 2);
  seed_and_key = *((unsigned long*)(APP_LOCK_ADDR) + 3);
  
  // Check Seed&Key 
   llwd_tmp = ((seed >> 7) | (seed << 24) );  
   llwd_tmp *= 3;
   llwd_tmp ^= 0xE98D6215;
   llwd_tmp = ((seed << 11) | (seed >> 20));
 
   if((key == llwd_tmp)
      &&((seed | key) == seed_or_key)
      &&((seed & key) == seed_and_key))
   {
     return ATB_FLAG_UNLOCK_SUCCESSED;     
   }
   else
   {
     return ATB_FLAG_UNLOCK_FAILED;
   }  
}

#ifdef ATB_QUICK_BOOT  
#pragma CODE_SEG BOOT_INTERFACE_ROM
/*
 * Define Interface of Access To BootLoader from application 
 *Tthis defination is used in application
 * serial is:
 * 1.set flag in 0x3FFE to 0x55AA
 * 2.check vector in 0xFFFE
 * 3.if vector == 0xFFFF, the vector is error,do nothing
 * 4.if vector != 0xFFFF, go into bootloader
 * NOTICE: For safe,You must set key check when use this code in application
 */
void ATB_GoIntoBoot(void)
{
/*lint -ident($), disable #27 message */
 asm
  {
     /* close quick entry, this maybe result cannot progam forever when application unnormal 
      * 2014-1-17 by liuzq 
      */
     //LDX #$3FFE;
     //LDD #$55AA; 
     //STD 0,X;
     LDD #64256;      
     STD $380; // disable xgate     
     LDX 65534;		
     LDD 65534;         
     IBEQ D,OUT; 
     LDAA #$FF;   
     STAA $121;    
     JMP 0,X;        
     OUT:;            
  }
/*lint +ident($), enable #27 message */  
} 
#pragma CODE_SEG DEFAULT 

#endif /* ATB_QUICK_BOOT */
