#ifndef IOIF_TYPE_H
#define IOIF_TYPE_H

#define  ERR_IO_BASE  0x150

typedef enum 
{
   //ERR_DO_OK,
   ERR_IO_RANGE = ERR_IO_BASE,                /* 0x150 for the DO number out of range */
   ERR_IO_VALUE_PTR_NULL,     /* 0x151 for the para pointer is null */
   ERR_IO_PROPERTY_PTR_NULL,  /* 0x152 for the pointer of property is null */
   ERR_IO_OPERATION_PTR_NULL, /* 0x153 for the pointer of operation is null */
   ERR_IO_ID_NUM_NOTMATCH,    /* 0x154 for the id number is not match */
   ERR_IO_FUNCTION_PTR_NULL,  /* 0x155 for the pointer of function is null */ 
   ERR_IO_CP_NOT_UPDATE,
}eIoIf_ReturnType;

#endif