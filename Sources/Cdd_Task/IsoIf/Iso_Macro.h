#ifndef MACROIF_H
#define MACROIF_H

#define SUMV_RATIO_NC1    0x00    //0b0000 0000
#define SUMV_RATIO_10mV   0x04    //0b0000 0100
#define SUMV_RATIO_100mV  0x08    //0b0000 1000
#define SUMV_RATIO_MASK   0x0C    //0b0000 1100

#define ISO_NEG                        0x08
#define ISO_POS                        0x09
#define ADC_STOP                       0x1F

/* define iso error code */
#define ISO_NORMAL                     0
#define CNVT_SEQ_ERR                   0x8000   /* Conversion sequence not completed */

/* define type for different opt_mos control */
#define OPT_MOS_POS                    0x01
#define OPT_MOS_NEG                    0x02

/* define check mode for isolation */
#define ISO_MODE_POS_OFF_NEG_OFF       0x00 
#define ISO_MODE_POS_OFF_NEG_ON        0x01 
#define ISO_MODE_POS_ON_NEG_OFF        0x02 
#define ISO_MODE_POS_ON_NEG_ON         0x03
#define ISO_MODE_ERROR                 0xFF  

/* define AD read statue for ISO */
#define FLAG_ISO_FAIL                  0x00
#define FLAG_ISO_OK                    0x01
#define FLAG_ISO_DONE_ONCE             0x10

/* isolation rank divide */
#define LEVEL_NUM                      2

/* define release configuration for ISO_INIT_CONFIG */
#define ISO_GAIN  	                   1
#define ISO_OFFSET 	                   0

/* define for mosfet status */
#define FLAG_OPEN                      0
#define FLAG_CLOSED                    1

/* define sample counter */
#define SAMPLE                         5
#define RERULT_NUM                     20
#define ASYMMETRIC_CHECK_NUM           60

/* define isolation check parameter */
#define ISO_CHECK_SUMV_THRD            1000 //100V, 0.1 resolution
#define ISO_CHECK_SUMV_THRD_100mV      1000  //100V, 0.1 resolution
#define ISO_CHECK_SUMV_THRD_10mV       10000 //100V, 0.01 resolution
#define ISO_FAULT_LEVEL1               500  // 500¦¸/V
#define ISO_FAULT_LEVEL2               100  // 100¦¸/V

#define POS_GAIN                     4450   //  5386 //2k 7121   // 4*820   
#define NEG_GAIN                     4450  //    5344 //7095   
#define POS_GAIN1                      445
#define NEG_GAIN1                      445 
#define R_BRANCH                       3285

#define ISO_CALC_DET_SUMV_THERD        1000  // 10V
#define ISO_CALC_DET_SUMV_THERD1        100

#define ISO_MODE1_THRED1_GAIN          25   // 400V-->16V
#define ISO_MODE1_THRED1_GAIN1        2  // 400V-->16V
#define ISO_MODE1_THRED2_GAIN          65   // 400V-->5V  
#define ISO_MODE1_THRED2_GAIN1        5  // 400V-->5V 


#endif