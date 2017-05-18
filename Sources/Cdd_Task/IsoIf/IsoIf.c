 // IsoIf.c
#include "IsoIf.h"
#include "Iso_Macro.h"
#include "ISO_CTRL_POS.h"
#include "ISO_CTRL_NEG.h"
#include "ISO_POW.h" 

uByte  gby_flag_mos_pos;
uByte  gby_flag_mos_neg;
uByte  gby_flag_iso_mode;

uLWord glwd_iso_rslt_pos[3];
uLWord glwd_iso_rslt_neg[3];

uLWord glwd_sumv_acc;

uWord  gwd_u_mode_POS_ON;
uWord  gwd_u_mode_NEG_ON;
uWord  gwd_sumv_POS_ON;
uWord  gwd_sumv_NEG_ON;

uByte  gby_flg_iso_cnt;
uByte  gby_mode_convert_cnt;

uLWord  gwd_rslt;

GeneralReturnType IsoIf_PowerOn(eIso_IdNumType Iso_Id) 
{
    //const sIso_PropertyType *Iso_PropPtr;
    //const sIso_OperationType *Iso_OperaPtr;
    GeneralReturnType st;
    
    //Iso_PropPtr = GET_ISOPROPERTY((uByte)Iso_Id);
    //Iso_OperaPtr = GET_ISOOPERATION((uByte)Iso_Id);

    st = (GeneralReturnType)ERR_OK;
    
    ISO_POW_PutVal(FALSE);
    /*if(Iso_PropPtr->Iso_IdNum == Iso_Id) 
    {
        (*(Iso_OperaPtr->Ios_DoCtrlPowPtr))(FALSE);
    } 
    else 
    {
        st = (GeneralReturnType)ERR_ISO_ID_NUM_NOTMATCH;
    }*/
    return st;
}

GeneralReturnType IsoIf_PowerOff(eIso_IdNumType Iso_Id) 
{
    //const sIso_PropertyType *Iso_PropPtr;
    //const sIso_OperationType *Iso_OperaPtr;
    GeneralReturnType st;
    
    //Iso_PropPtr = GET_ISOPROPERTY((uByte)Iso_Id);
    //Iso_OperaPtr = GET_ISOOPERATION((uByte)Iso_Id);

    st = (GeneralReturnType)ERR_OK;
    ISO_POW_PutVal(TRUE);
    /*if(Iso_PropPtr->Iso_IdNum == Iso_Id) 
    {
        (*(Iso_OperaPtr->Ios_DoCtrlPowPtr))(TRUE);
    } 
    else 
    {
        st = (GeneralReturnType)ERR_ISO_ID_NUM_NOTMATCH;
    }*/
    return st;
}

static GeneralReturnType IsoIf_CtrlPosMos(eIso_IdNumType Iso_Id,bool val) 
{
    //const sIso_PropertyType *Iso_PropPtr;
    //const sIso_OperationType *Iso_OperaPtr;
    GeneralReturnType st;
    
    //Iso_PropPtr = GET_ISOPROPERTY((uByte)Iso_Id);
    //Iso_OperaPtr = GET_ISOOPERATION((uByte)Iso_Id);

    st = (GeneralReturnType)ERR_OK;
    ISO_CTRL_POS_PutVal(val);
    /*if(Iso_PropPtr->Iso_IdNum == Iso_Id) 
    {
        (*(Iso_OperaPtr->Iso_DoCtrlPosPtr))(val);
    } 
    else 
    {
        st = (GeneralReturnType)ERR_ISO_ID_NUM_NOTMATCH;
    }*/
    return st;
}

static GeneralReturnType IsoIf_CtrlNegMos(eIso_IdNumType Iso_Id,bool val) 
{
    //const sIso_PropertyType *Iso_PropPtr;
    //const sIso_OperationType *Iso_OperaPtr;
    GeneralReturnType st;
    
    //Iso_PropPtr = GET_ISOPROPERTY((uByte)Iso_Id);
    //Iso_OperaPtr = GET_ISOOPERATION((uByte)Iso_Id);

    st = (GeneralReturnType)ERR_OK;
    
    ISO_CTRL_NEG_PutVal(val);
    /*if(Iso_PropPtr->Iso_IdNum == Iso_Id) 
    {
        (*(Iso_OperaPtr->Iso_DoCtrlNegPtr))(val);
    } 
    else 
    {
        st = (GeneralReturnType)ERR_ISO_ID_NUM_NOTMATCH;
    }*/
    return st;
}

static GeneralReturnType IsoIf_RdPosAdValue(eIso_IdNumType Iso_Id,uWord *PosAdValuePtr) 
{
    //const sIso_PropertyType *Iso_PropPtr;
    //const sIso_OperationType *Iso_OperaPtr;
    GeneralReturnType st;
    
    //Iso_PropPtr = GET_ISOPROPERTY((uByte)Iso_Id);
    //Iso_OperaPtr = GET_ISOOPERATION((uByte)Iso_Id);

    st = (GeneralReturnType)ERR_OK;
    st = Iso_GetPosAdValue(PosAdValuePtr);
    /*if(Iso_PropPtr->Iso_IdNum == Iso_Id) 
    {
        st = (*(Iso_OperaPtr->Iso_GetAdValPosPtr))(PosAdValuePtr);
    } 
    else 
    {
        st = (GeneralReturnType)ERR_ISO_ID_NUM_NOTMATCH;
    }*/
    return st;
}

static GeneralReturnType IsoIf_RdNegAdValue(eIso_IdNumType Iso_Id,uWord *NegAdValuePtr) 
{
    //const sIso_PropertyType *Iso_PropPtr;
    //const sIso_OperationType *Iso_OperaPtr;
    GeneralReturnType st;
    
    //Iso_PropPtr = GET_ISOPROPERTY((uByte)Iso_Id);
    //Iso_OperaPtr = GET_ISOOPERATION((uByte)Iso_Id);

    st = (GeneralReturnType)ERR_OK;
    st = Iso_GetNegAdValue(NegAdValuePtr);
    /*if(Iso_PropPtr->Iso_IdNum == Iso_Id) 
    {
        st = (*(Iso_OperaPtr->Iso_GetAdValNegPtr))(NegAdValuePtr);
    } 
    else 
    {
        st = (GeneralReturnType)ERR_ISO_ID_NUM_NOTMATCH;
    }*/
    return st;
}

GeneralReturnType IsoIf_CloseMos(eIso_IdNumType Iso_Id,uByte type)
{
  GeneralReturnType st;
  st = (GeneralReturnType)ERR_OK; 
  
    if(ISO1 == Iso_Id)
    {
      switch(type)
      {
          case OPT_MOS_POS:
          {
              st = IsoIf_CtrlPosMos(Iso_Id,FALSE);
              if(st != ERR_OK) 
              {
                  return st;
              }
              gby_flag_mos_pos = FLAG_CLOSED;
              
              break; 
          }
          case OPT_MOS_NEG:
          {
              st = IsoIf_CtrlNegMos(Iso_Id,FALSE);
              if(st != ERR_OK) 
              {
                  return st;
              }
              gby_flag_mos_neg = FLAG_CLOSED;
              
              break; 
          }
          default:
          {
              break;
          }
      }
      return st;
   }
   else
   {
     return (GeneralReturnType)ERR_ISO_ID_NUM_NOTMATCH; 
   }
}

GeneralReturnType IsoIf_OpenMos(eIso_IdNumType Iso_Id,uByte type)
{ 

   GeneralReturnType st;   
   st = (GeneralReturnType)ERR_OK;
    
   if(ISO1 == Iso_Id)
   {
    switch(type)
    {
        case OPT_MOS_POS:
        {
            st = IsoIf_CtrlPosMos(Iso_Id,TRUE);
            if(st != ERR_OK) 
            {
                return st;
            }
            gby_flag_mos_pos = FLAG_OPEN;
            
            break; 
        }
        case OPT_MOS_NEG:
        {
            st = IsoIf_CtrlNegMos(Iso_Id,TRUE); 
            if(st != ERR_OK) 
            {
                return st;
            }
            gby_flag_mos_neg = FLAG_OPEN;
            
            break; 
        }
        default:
        {
            break;
        }
    }
    return st;
}
  else
 {
   return (GeneralReturnType)ERR_ISO_ID_NUM_NOTMATCH; 
 }
}

GeneralReturnType IsoIf_ChangeMosMode(eIso_IdNumType Iso_Id,uByte mode)
{
   GeneralReturnType st;
    
   st = (GeneralReturnType)ERR_OK;
  if(ISO1 == Iso_Id ) 
  {
    switch (mode) 
    {
        case ISO_MODE_POS_ON_NEG_ON: 
        { 
            
            st = IsoIf_CloseMos(Iso_Id,OPT_MOS_POS);
            if(st != ERR_OK) 
            { 
                return st; 
            }
            st = IsoIf_CloseMos(Iso_Id,OPT_MOS_NEG); 
            if(st != ERR_OK) 
            {
               return st;
            }
            
            glwd_iso_rslt_pos[0] = 0;          
            glwd_iso_rslt_pos[1] = 0;
            glwd_iso_rslt_pos[2] = 0;
            
            glwd_iso_rslt_neg[0] = 0;
            glwd_iso_rslt_neg[1] = 0; 
            glwd_iso_rslt_neg[2] = 0;
            
            gby_flg_iso_cnt = 0;
            
            gby_flag_iso_mode = ISO_MODE_POS_ON_NEG_ON;
            
            break;
        }
        case ISO_MODE_POS_ON_NEG_OFF: 
        { 
            st = IsoIf_CloseMos(Iso_Id,OPT_MOS_POS);
            if(st != ERR_OK) 
            { 
                return st; 
            } 
            st = IsoIf_OpenMos(Iso_Id,OPT_MOS_NEG);
            if(st != ERR_OK) 
            {
                return st;
            }
           
            glwd_iso_rslt_pos[0] = 0;
            glwd_iso_rslt_pos[1] = 0;
            glwd_iso_rslt_pos[2] = 0;
            
            glwd_iso_rslt_neg[0] = 0;
            glwd_iso_rslt_neg[1] = 0; 
            glwd_iso_rslt_neg[2] = 0; 
            
            gby_flg_iso_cnt = 0;
            
            gby_flag_iso_mode = ISO_MODE_POS_ON_NEG_OFF;
            
            break;
        }
        case ISO_MODE_POS_OFF_NEG_ON: 
        {   
            st = IsoIf_OpenMos(Iso_Id,OPT_MOS_POS);
            if(st != ERR_OK) 
            {  
                return st;
            } 
            st = IsoIf_CloseMos(Iso_Id,OPT_MOS_NEG);
            if(st != ERR_OK) 
            {
                return st;
            }
            
            glwd_iso_rslt_pos[0] = 0;
            glwd_iso_rslt_pos[1] = 0;
            glwd_iso_rslt_pos[2] = 0;
            
            glwd_iso_rslt_neg[0] = 0;
            glwd_iso_rslt_neg[1] = 0; 
            glwd_iso_rslt_neg[2] = 0;
            
            gby_flg_iso_cnt = 0; 
            
            gby_flag_iso_mode = ISO_MODE_POS_OFF_NEG_ON;
            
            break;
        }
        case ISO_MODE_POS_OFF_NEG_OFF: 
        { 
        
            st = IsoIf_OpenMos(Iso_Id,OPT_MOS_POS);
            if(st != ERR_OK) 
            { 
                return st; 
            } 
            st = IsoIf_OpenMos(Iso_Id,OPT_MOS_NEG);
            if(st != ERR_OK) 
            {
                return st;
            }
           
            glwd_iso_rslt_pos[0] = 0;
            glwd_iso_rslt_pos[1] = 0;
            glwd_iso_rslt_pos[2] = 0;
            
            glwd_iso_rslt_neg[0] = 0;
            glwd_iso_rslt_neg[1] = 0; 
            glwd_iso_rslt_neg[2] = 0;
            
            gby_flg_iso_cnt = 0; 
            
            gby_flag_iso_mode = ISO_MODE_POS_OFF_NEG_OFF;
            
            break;
        }
        default:
        {
            break;
        }
    }
    return st;
}
else
 { 
   return (GeneralReturnType)ERR_ISO_ID_NUM_NOTMATCH;
 }

}

GeneralReturnType IsoIf_RdAdValue(eIso_IdNumType Iso_Id,uWord* VPosPtr, uWord* VNegPtr)
{
  if(ISO1 == Iso_Id)
  {
    if (VPosPtr!=NULL&&VNegPtr!=NULL)
    {
   
    uByte  i;
    uLWord result_neg = 0;
    uLWord result_pos = 0;
    uWord  max_neg = 0,min_neg = 0xFFFF,max_pos = 0,min_pos = 0xFFFF;
    uWord  AdValue;
    GeneralReturnType st;
    
    st = (GeneralReturnType)ERR_OK;
   /* if (SAMPLE > 20)
    {
        return FLAG_FAIL;
    }  */
        /* begin POS ADC sample */
        
    for (i = 0; i < SAMPLE; i++)
    {
        st = IsoIf_RdPosAdValue(Iso_Id,&AdValue);
        if(st != ERR_OK) 
        {
            return st;
        }
        result_pos += AdValue;
        
        if (max_pos <= AdValue)
        {
            max_pos = AdValue;
        }
        if (min_pos >= AdValue) 
        {
            min_pos = AdValue;
        }
    }
    
    /* begin NEG ADC sample */
    for (i = 0; i < SAMPLE; i++)
    {
        st = IsoIf_RdNegAdValue(Iso_Id,&AdValue);
        if(st != ERR_OK) 
        {
            return st;
        }
        result_neg += AdValue;
        
        if (max_neg <= AdValue)
        {
            max_neg = AdValue;
        }
        if (min_neg >= AdValue) 
        {
            min_neg = AdValue;
        }
    }
    
    result_neg = result_neg - max_neg - min_neg;
    result_neg /= (SAMPLE - 2);

    result_pos = result_pos - max_pos - min_pos;
    result_pos /= (SAMPLE - 2);

#ifdef __DEBUG_ISO

    result_opp = 1024;
    result_pos = 1024;

#endif

    //gwd_ad_pos = (uWord)result_pos;
    //gwd_ad_neg = (uWord)result_neg;

    //voltage threshold is 5V
//    if(result_pos <= 30)
//    {  /* prevent float pin */
//       *VPosPtr = 0;       
//    }
//    else
 //   {
       *VPosPtr = (uWord)result_pos;
//    }
    
    //voltage threshold is 5V
//    if(result_neg <= 30) 
//    {  /* prevent float pin */
//       *VNegPtr = 0;       
//    }
//    else
//    {
       *VNegPtr = (uWord)result_neg;
 //   } 

    //*p_err_code = ISO_NORMAL;

    return st;
}
  else 
  {
     return  (GeneralReturnType)ERR_ISO_VALUE_PTR_NULL;
  }
}  
else
{
  return (GeneralReturnType)ERR_ISO_ID_NUM_NOTMATCH;
} 
}

GeneralReturnType IsoIf_Init(void)
{
    GeneralReturnType st;
    
    st = (GeneralReturnType)ERR_OK;
    
    IsoIf_PowerOn(ISO1);
    
    st = IsoIf_OpenMos(ISO1,OPT_MOS_POS);
    if(st != ERR_OK) 
    {
        return st;  
    } 
    st = IsoIf_OpenMos(ISO1,OPT_MOS_NEG);
    if(st != ERR_OK) 
    {
        return st;
    }

    gby_flag_mos_pos = FLAG_OPEN;
    gby_flag_mos_neg = FLAG_OPEN;
    
    /* switch opt_mos to ON */
    
    st = IsoIf_CloseMos(ISO1,OPT_MOS_POS);
    if(st != ERR_OK) 
    {
        return st;  
    } 
    st = IsoIf_CloseMos(ISO1,OPT_MOS_NEG);
    if(st != ERR_OK) 
    {
        return st;
    }

    gby_flag_mos_pos = FLAG_CLOSED;
    gby_flag_mos_neg = FLAG_CLOSED;
    
    gby_flag_iso_mode = ISO_MODE_POS_ON_NEG_ON;

   // *p_err_code = ISO_NORMAL;
    
    glwd_iso_rslt_pos[0] = 0;
    glwd_iso_rslt_pos[1] = 0;
    glwd_iso_rslt_pos[2] = 0;
    
    glwd_iso_rslt_neg[0] = 0;
    glwd_iso_rslt_neg[1] = 0;
    glwd_iso_rslt_neg[2] = 0;
    
    glwd_sumv_acc = 0;

    gwd_u_mode_POS_ON = 0;
    gwd_u_mode_NEG_ON = 0;
    gwd_sumv_POS_ON = 0;
    gwd_sumv_NEG_ON = 0;

    //gwd_iso_RP = 0;
    //gwd_iso_RN = 0;

    gby_flg_iso_cnt = 0;
    gby_mode_convert_cnt = 0;   

    return st;  
}



