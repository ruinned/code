#ifndef AICFG_H
#define AICFG_H

#include "AiIf_Type.h"

extern const sAiIf_HandleType AiIf_Handle;

#define GET_AIPROPERTY(AiIf_ChannelId) \
 (AiIf_Handle.AiIf_PropertyPtr + AiIf_ChannelId)  //get the property pointer

#define GET_AIOPERATION(AiIf_AdcId) \
 (AiIf_Handle.AiIf_OperationPtr + AiIf_AdcId)  //get the operation pointer

#endif /* AICFG_H */