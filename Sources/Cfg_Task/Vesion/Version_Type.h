// Version_Type.h
#ifndef VERSION_TYPE_H
#define VERSION_TYPE_H

#include "GeneralTypeDef.h"
 

#define VERSON_MAX_NAME_CHARACTOR_NUM  14 
#define VERSON_MAX_STATUS_CHARACTOR_NUM  7                                             
typedef struct 
{
	char Name[VERSON_MAX_NAME_CHARACTOR_NUM];
	unsigned char SW_major_version;
	unsigned int SW_minor_version;
	unsigned int SW_revision_version; 
	unsigned char rsv1;
	unsigned char rsv2;
  char Status[VERSON_MAX_STATUS_CHARACTOR_NUM];
} sVersion_CodeType; 

typedef uWord ModuleID_Type;
#define NULL_MODULE_ID     0   // 0 -- NOT define 
                                              

#endif
