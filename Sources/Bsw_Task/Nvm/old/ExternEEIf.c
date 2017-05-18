#include "ExternEEIf.h"
//#include "Nvm_Lcfg.h"
#include "ExtEE_Sck.h"
#include "ExtEE_Sda.h"
#include "ExtEE_Wp.h"

static void ExternEEIf_IICStart(void);
static void ExternEEIf_IICStop(void);
static void ExternEEIf_IICAck(void);
static void ExternEEIf_IICNoAck(void);
static void ExternEEIf_WriteByte(uByte data);
static uByte ExternEEIf_ReadByte(void);

/* address for 24c64 */
#define EXTERNEEIF_ADDWR 	0xA0	 /* address for write */
#define EXTERNEEIF_ADDRD 	0xA1	 /* address for read */

#define EXTERNEEIF_ADDR_BASE  0x0000
#define ExternEEIf_CALC_ABSOLUTE_ADDR(relative_addr) ((relative_addr) + EXTERNEEIF_ADDR_BASE)

//************************************************************/
// Function: initial of ExternEE
// Trans para: void
//************************************************************/
void ExternEEIf_Init(void)
{
    //(*ExternEE_Operation.ExternEE_Sck.ClrVal)();
    ExtEE_Sck_ClrVal();
    //(*ExternEE_Operation.ExternEE_Sda.ClrVal)();
    ExtEE_Sda_ClrVal();
    //(*ExternEE_Operation.ExternEE_Wp.SetVal)();
    ExtEE_Wp_SetVal();
    
    //(*ExternEE_Operation.ExternEE_Sda.SetOutput)();
    ExtEE_Sda_SetOutput();
    //(*ExternEE_Operation.ExternEE_Wp.SetOutput)();
    ExtEE_Wp_SetOutput();
}

//************************************************************/
// Function: enable write of ExternEE
// Trans para: void
//************************************************************/
void ExternEEIf_WriteEnable(void)
{
    //(*ExternEE_Operation.ExternEE_Wp.ClrVal)();
    ExtEE_Wp_ClrVal();
}

//************************************************************/
// Function: disable write of ExternEE
// Trans para: void
//************************************************************/
void ExternEEIf_WriteDisable(void)
{
    //(*ExternEE_Operation.ExternEE_Wp.SetVal)();
    ExtEE_Wp_SetVal();
}

//************************************************************/
// Function: initialize serial port, prepare to send data
// Trans para: void
//************************************************************/
static void ExternEEIf_IICStart(void)
{
    //(*ExternEE_Operation.ExternEE_Sck.SetVal)();
    ExtEE_Sck_SetVal();
    //(*ExternEE_Operation.ExternEE_Sda.SetVal)();
    ExtEE_Sda_SetVal();
    
    //(*ExternEE_Operation.ExternEE_Sda.ClrVal)();
    ExtEE_Sda_ClrVal();
    //(*ExternEE_Operation.ExternEE_Sck.ClrVal)();
    ExtEE_Sck_ClrVal();
}

//************************************************************/
// Function: stop seiral port
// Trans para: void
//************************************************************/
static void ExternEEIf_IICStop(void)
{
    //(*ExternEE_Operation.ExternEE_Sda.ClrVal)();
    ExtEE_Sda_ClrVal();
    //(*ExternEE_Operation.ExternEE_Sck.SetVal)();
    ExtEE_Sck_SetVal();
    //(*ExternEE_Operation.ExternEE_Sda.SetVal)();
    ExtEE_Sda_SetVal();
}

//************************************************************/
// Function:
// Trans para: void
//************************************************************/
static void ExternEEIf_IICAck(void)
{
    //(*ExternEE_Operation.ExternEE_Sda.ClrVal)();
    ExtEE_Sda_ClrVal();
    //(*ExternEE_Operation.ExternEE_Sck.SetVal)();
    ExtEE_Sck_SetVal();
    //(*ExternEE_Operation.ExternEE_Sck.ClrVal)();
    ExtEE_Sck_ClrVal();
}

//************************************************************/
// Function:
// Trans para: void
//************************************************************/
static void ExternEEIf_IICNoAck(void)
{
    //(*ExternEE_Operation.ExternEE_Sda.SetVal)();
    ExtEE_Sda_SetVal();
    //(*ExternEE_Operation.ExternEE_Sck.SetVal)();
    ExtEE_Sck_SetVal();
    //(*ExternEE_Operation.ExternEE_Sck.ClrVal)();
    ExtEE_Sck_ClrVal();
}


//************************************************************/
// Function: write byte of ExternEE
// Trans para: void
//************************************************************/
static void ExternEEIf_WriteByte(uByte data)
{
    uByte i;
    uByte btmp;

    for (i = 0; i < 8; i++)
    {
        btmp = data & 0x80;

        if (0x80 == btmp)
        {
            //(*ExternEE_Operation.ExternEE_Sda.SetVal)();
            ExtEE_Sda_SetVal();
        }
        else
        {
            //(*ExternEE_Operation.ExternEE_Sda.ClrVal)();
            ExtEE_Sda_ClrVal();
        }

        //(*ExternEE_Operation.ExternEE_Sck.SetVal)();
        ExtEE_Sck_SetVal();
        //(*ExternEE_Operation.ExternEE_Sck.ClrVal)();
        ExtEE_Sck_ClrVal();
        
        data = data << 1;
    }
}

//************************************************************/
// Function: read byte of ExternEE
// Trans para: void
//************************************************************/
static uByte ExternEEIf_ReadByte(void)
{
    uByte i;
    uByte btmp;

    btmp = 0;

    //(*ExternEE_Operation.ExternEE_Sda.SetInput)();
    ExtEE_Sda_SetInput();

    for (i = 0; i < 8; i++)
    {
        btmp <<= 1;

        //(*ExternEE_Operation.ExternEE_Sck.SetVal)();
        ExtEE_Sck_SetVal();
        
        //if ((*ExternEE_Operation.ExternEE_Sda.GetVal)())
        if(ExtEE_Sda_GetVal())
        {
            btmp |= 0x01;
        }

        //(*ExternEE_Operation.ExternEE_Sck.ClrVal)();
        ExtEE_Sck_ClrVal();
    }

    //(*ExternEE_Operation.ExternEE_Sda.SetOutput)();
    ExtEE_Sda_SetOutput();

    return btmp;
}

//************************************************************/
// Function: get byte of ExternEE
// Trans para: point of data, address
//************************************************************/
uWord ExternEEIf_GetByte(uWord addr, uByte * p_data)
{    
    uWord absolute_addr;
    
    absolute_addr = ExternEEIf_CALC_ABSOLUTE_ADDR(addr);
    
    ExternEEIf_IICStart();
    ExternEEIf_WriteByte(EXTERNEEIF_ADDWR);
    ExternEEIf_IICAck();
    ExternEEIf_WriteByte(absolute_addr >> 8);
    ExternEEIf_IICAck();
    ExternEEIf_WriteByte(absolute_addr & 0xFF);
    ExternEEIf_IICAck();

    ExternEEIf_IICStart();
    ExternEEIf_WriteByte(EXTERNEEIF_ADDRD);

    ExternEEIf_IICAck();

    (*p_data) = ExternEEIf_ReadByte();

    ExternEEIf_IICNoAck();

    ExternEEIf_IICStop();

    return (uWord)EXTERNEEIF_ERR_OK;
}

//************************************************************/
// Function: set byte of ExternEE
// Trans para: point of data, address
//************************************************************/
uWord ExternEEIf_SetByte(uWord addr, uByte data)
{
    uByte rslt;
    uWord absolute_addr;
    
    absolute_addr = ExternEEIf_CALC_ABSOLUTE_ADDR(addr);
    
    ExternEEIf_IICStart();
    ExternEEIf_WriteByte(EXTERNEEIF_ADDWR);
    ExternEEIf_IICAck();
    ExternEEIf_WriteByte(absolute_addr >> 8);
    ExternEEIf_IICAck();
    ExternEEIf_WriteByte(absolute_addr & 0xFF);
    ExternEEIf_IICAck();

    ExternEEIf_WriteByte(data);
    ExternEEIf_IICAck();

    ExternEEIf_IICStop();

    rslt = (uWord)EXTERNEEIF_ERR_OK;

    return rslt;
}

uWord ExternEEIf_SetWord(uWord addr, uWord data)
{
    (void)addr;
    (void)data;
    return (uWord)EXTERNEEIF_ERR_OK;
}

uWord ExternEEIf_SetLong(uWord addr, uLWord data)
{
    (void)addr;
    (void)data;
    return (uWord)EXTERNEEIF_ERR_OK;
}

//************************************************************/
// Function: read array of ExternEE
// Trans para: point of data, address, leng.
//************************************************************/
uWord ExternEEIf_ReadArray(uByte* p_data, uWord addr, uByte dlen)
{
    uByte i;
    uWord absolute_addr;
    
    absolute_addr = ExternEEIf_CALC_ABSOLUTE_ADDR(addr);
    
    ExternEEIf_IICStart();
    ExternEEIf_WriteByte(EXTERNEEIF_ADDWR);
    ExternEEIf_IICAck();
    ExternEEIf_WriteByte(absolute_addr >> 8);
    ExternEEIf_IICAck();
    ExternEEIf_WriteByte(absolute_addr & 0xFF);
    ExternEEIf_IICAck();

    ExternEEIf_IICStart();
    ExternEEIf_WriteByte(EXTERNEEIF_ADDRD);

    for (i = 0; i < dlen; i++)
    {
        ExternEEIf_IICAck();

        *(p_data++) = ExternEEIf_ReadByte();
    }

    ExternEEIf_IICNoAck();

    ExternEEIf_IICStop();

    return (uWord)EXTERNEEIF_ERR_OK;
}

//************************************************************/
// Function: write array of ExternEE
// Trans para: point of data, address, leng.
//************************************************************/
uWord ExternEEIf_WriteArray(uByte* p_data, uWord addr, uByte dlen)
{
    uByte rslt;
    uByte i;
    uWord absolute_addr;
    
    absolute_addr = ExternEEIf_CALC_ABSOLUTE_ADDR(addr);
    
    ExternEEIf_IICStart();
    ExternEEIf_WriteByte(EXTERNEEIF_ADDWR);
    ExternEEIf_IICAck();
    ExternEEIf_WriteByte(absolute_addr >> 8);
    ExternEEIf_IICAck();
    ExternEEIf_WriteByte(absolute_addr & 0xFF);
    ExternEEIf_IICAck();

    for (i = 0; i < dlen; i++)
    {
        ExternEEIf_WriteByte(*p_data++);
        ExternEEIf_IICAck();
    }

    ExternEEIf_IICStop();

    rslt = (uWord)EXTERNEEIF_ERR_OK;

    return rslt;
}

