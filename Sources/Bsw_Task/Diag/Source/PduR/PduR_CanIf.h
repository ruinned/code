/*============================================================================*/
/*  Copyright (C) 2009-2013, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <PduR_CanIf.h>
 *  @brief      <PduR>
 *  
 *  <Compiler: ALL MCU:ALL>
 *  
 *  @author     <xuehua.chen>
 *  @date       <2013-10-16>
 */
/*============================================================================*/

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0       20131016  xuehua.chen Initial version
 * 
 */
/*============================================================================*/

/*=======[I N C L U D E S]====================================================*/

#ifndef  PDUR_CANIF_H			 
#define  PDUR_CANIF_H

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define PDUR_CANIF_H_VENDOR_ID          60U
#define PDUR_CANIF_H_AR_MAJOR_VERSION   2U
#define PDUR_CANIF_H_AR_MINOR_VERSION   3U
#define PDUR_CANIF_H_AR_PATCH_VERSION   0U
#define PDUR_CANIF_H_SW_MAJOR_VERSION   1U
#define PDUR_CANIF_H_SW_MINOR_VERSION   0U
#define PDUR_CANIF_H_SW_PATCH_VERSION   0U

/*=======[I N C L U D E S]====================================================*/
#include "Com_Cbk.h"


/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[F U N C T I O N   I M P L E M E N T A T V O N S]====================*/

/* zero cast config , include by CanIf config file */
//#define PduR_CanIfRxIndication Com_RxIndication
		
//#define PduR_CanIfTxConfirmation Com_TxConfirmation

//#define PduR_CanIfRxIndication

//#define PduR_CanIfTxConfirmation

#endif /* #ifndef PDUR_CANIF_H */

/*=======[E N D   O F   F I L E]==============================================*/
