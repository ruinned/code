// PE_cfg.h
#ifndef PE_CFG_H
#define PE_CFG_H

/*     
 * Define switch for pe use call back interface
 * When Use PE independent Compile,need close this switch(No -D__PE_CBK_EN__ in mcal project)
 * When Add Call Back to Pe Event or mcal,need Use #ifdef PE_CBK_EN/#endif
 * Use compiler option  -D__PE_CBK_EN__   in main project for open  callback interface
 * 
 */

#ifdef __PE_CBK_EN__
#define PE_CBK_EN     
#endif                     


#endif /* PE_CFG_H */