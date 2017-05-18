#ifndef ATB_H
#define ATB_H

/* define for unlock application */
#define ATB_FLAG_UNLOCK_SUCCESSED 1
#define ATB_FLAG_UNLOCK_FAILED 0

/* define for modify baudrate */
#define ATB_BAUDRATE_500K   0
#define ATB_BAUDRATE_250K   1

/*
 * use this function need define PLACEMENT "BTLDR_ROM" in file *.prm, like this:
 *
  ROM_BOOT = READ_ONLY  0xDC00 TO 0xDCFF;  
  BOOT_INTERFACE_ROM     INTO    ROM_BOOT;
 */
#define ATB_QUICK_BOOT 

extern unsigned char ATB_GetAppBaudrate(void);
extern unsigned char ATB_UnlockApp(void);
 
#ifdef ATB_QUICK_BOOT  
#pragma CODE_SEG BOOT_INTERFACE_ROM
extern void ATB_GoIntoBoot(void); 
#pragma CODE_SEG DEFAULT
#endif


#endif /* ATB_H */