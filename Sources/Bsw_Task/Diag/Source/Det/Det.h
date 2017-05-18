/*============================================================================*/
/** Copyright (C) 2009-2012, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  
 *  @file       Det.h
 *  @brief      <Briefly describe file(one line)>
 *  
 *  <Detailed description of the file(multi-line)>
 *  
 *  @author     stanley
 *  @date       2012-9-11
 */
/*============================================================================*/


#ifndef DET_H 
#define DET_H

/*=======[I N C L U D E S]====================================================*/
#include "Std_Types.h"
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/*=======[M A C R O S]========================================================*/
#define DET_MODULE_ID           15
#define DET_H_AR_MAJOR_VERSION	2
#define DET_H_AR_MINOR_VERSION	2

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
extern void Det_ReportError(
            uint16 ModuleId,
            uint8 InstanceId,
            uint8 ApiId,
            uint8 ErrorId);

/*=======[I N T E R N A L   D A T A]==========================================*/
extern uint16 Det_ModuleId;
extern uint8 Det_InstanceId;
extern uint8 Det_ApiId;
extern uint8 Det_ErrorId;
/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* DET_H */

/*=======[R E V I S I O N   H I S T O R Y]====================================*/

/*=======[E N D   O F   F I L E]==============================================*/
