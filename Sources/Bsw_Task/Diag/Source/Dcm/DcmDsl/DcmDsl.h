/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file        <DcmDsl.h>
 *  @brief       <>
 *
 *  <Compiler: CodeWarrior    MCU:XXX>
 *
 *  @author     <chen maosen>
 *  @date       <2013-03-20>
 */
/*============================================================================*/

#ifndef DCMDSL_H
#define DCMDSL_H

/****************************** references *********************************/
#include "DcmDsl_CommManage.h"
#include "DcmDsl_MsgManage.h"
#include "DcmDsl_PendingManage.h"
#include "DcmDsl_ProtocolManage.h"

#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
#include "DcmDsl_SecurityManage.h"
#endif
#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
#include "DcmDsl_SessionManage.h"
#endif


#endif /* DCMDSL_H_ */
