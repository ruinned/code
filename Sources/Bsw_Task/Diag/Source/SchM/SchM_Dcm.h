/************************************************************************
*				        ESEC UESTC
* 	 Copyright (C) 2005-2011 ESEC UESTC. All Rights Reserved.
***********************************************************************/

/**
 * Log:  
 * Revision 1.0 2011-6-22ÏÂÎç02:15:18 chenms
 * description: create
 *
 */

/**
 * @file 	SchM.h 
 * @brief
 *	<li>Function£º </li>
 *  <li>Design Points£º</li>
 *  <p>
 *  
 *  </p>
 * @author 	chenms
 * @date 	2011-6-22
 * 
 */


/******************************* definitions ***********************************/
#define SchM_Enter_Dcm(exclusiveArea)  SchM_Enter_Dcm_##exclusiveArea
#define SchM_Exit_Dcm(exclusiveArea)   SchM_Exit_Dcm_##exclusiveArea

#define SchM_Enter_Dcm_Dcm_ChannelCtrl
#define SchM_Exit_Dcm_Dcm_ChannelCtrl

#define SchM_Enter_Dcm_Dcm_MsgCtrl
#define SchM_Exit_Dcm_Dcm_MsgCtrl

#define SchM_Enter_Dcm_Dcm_ProtocolCtrl
#define SchM_Exit_Dcm_Dcm_ProtocolCtrl

#define SchM_Enter_Dcm_Dcm_ProtocolPreemptCtrl
#define SchM_Exit_Dcm_Dcm_ProtocolPreemptCtrl

#define SchM_Enter_Dcm_Dcm_MkCtrl
#define SchM_Exit_Dcm_Dcm_MkCtrl

#define SchM_Enter_Dcm_Dcm_SesCtrl
#define SchM_Exit_Dcm_Dcm_SesCtrl

#define SchM_Enter_Dcm_Dcm_SecCtrl
#define SchM_Exit_Dcm_Dcm_SecCtrl

#define SchM_Enter_Dcm_Dcm_Channel
#define SchM_Exit_Dcm_Dcm_Channel

#define SchM_Enter_Dcm_Dcm_CommCtrl
#define SchM_Exit_Dcm_Dcm_CommCtrl

#define SchM_Enter_Dcm_Dcm_LinkControlCtrl
#define SchM_Exit_Dcm_Dcm_LinkControlCtrl

/****************************** implementations ********************************/
/****@req DCM-APIR-001[DCM333] ****/
typedef uint8  SchM_ReturnType;
#define  SCHM_E_NOFUNC   ((uint8)0x05)
#define  SCHM_E_STATE    ((uint8)0x07)
#define  SCHM_E_OK       ((uint8)0x00)
#define  SCHM_E_LIMIT    ((uint8)0x04)

