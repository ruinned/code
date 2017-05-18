#ifndef DICFG_H
#define DICFG_H

#include "DiIf_Type.h"

extern const sDiIf_HandleType DiIf_Handle;

#define GET_DIPROPERTY(DiIf_Id) \
 (DiIf_Handle.DiIf_PropertyPtr + DiIf_Id)   //get the pointer of property

#define GET_DIOPERATION(DiIf_Id) \
(DiIf_Handle.DiIf_OperationPtr + DiIf_Id)   //get the pointer of operation


#endif /* DICFG_H */