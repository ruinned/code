// EcuDebug.h
#ifndef ECUDEBUG_H
#define ECUDEBUG_H

/*
 * Define for Debug,when complete debug,must close this
 */
#define __DEBUG
#ifdef __DEBUG
//#warning "The program is working in debug model"
/*
 * Define for test task time
 * This permit use wdgm_Single_trig() for test task time
 * And close Wdgm_MainTrig();
 */  
  //#define __DEBUG_TEST_TIME
  #ifdef __DEBUG_TEST_TIME
  /* This open wdgm_Single_trig() in timer */
  //#define __DEBUG_TEST_TIMER_PERIOD 
  //#define __DEBUG_TEST_CANPERIOD
  //#define __DEBUG_TEST_MAIN_FUNC_TIME
  #endif /* __DEBUG_TEST_TIME */
  //#define __DEBUG_TEST_SOC
  #define __DEBUG_TEST_SOC_CLBR
  #define __DEBUG_CUR_CHECK_AD
  #define __DEBUG_CUR_CLBR
  /*
   * Test condition true or false
   * if condition is false, wait forever  
   */
  #define __ENABLE_ASSERT_ERROR_CNT
  #ifdef __ENABLE_ASSERT_ERROR_CNT
  extern unsigned char gby_AssertErrorCnt;
  #define __GET_ASSERT_ERROR_CNT()  (gby_AssertErrorCnt) // interface for get assert error cnt 
  #define __INCREASE_ASSERT_ERROR_CNT() \
  {\
    (gby_AssertErrorCnt < 0xFFUL)? \
    (gby_AssertErrorCnt++) : \
    (gby_AssertErrorCnt = gby_AssertErrorCnt);\
    }
  #else
   #define __INCREASE_ASSERT_ERROR_CNT()
   #define  __GET_ASSERT_ERROR_CNT()    (0) // assert no error cnt
  #endif /* end of #ifdef __ENABLE_ASSERT_ERROR_CNT */
  
  #define __ASSERT_TEST_
  #ifdef  __ASSERT_TEST_    
   #define __ASSERT( condition ) \
     if(! (condition) ) \
    {\
       __INCREASE_ASSERT_ERROR_CNT()\
       return 0xFFFFFFFFUL;\
       }     
  #else
  #define __ASSERT(condition)
  #endif /* end of ASSERT */      
  
  
#define USE_ANSI_C_ASSERT
#ifdef  USE_ANSI_C_ASSERT
#include <assert.h> /* Progam Can Use ANSI-C assert 
                     * Such as assert(exp);
                     */

//#define __DEBUG_DISABLE_PRODUCT_TASK                     
#ifdef __DEBUG_DISABLE_PRODUCT_TASK
#warning "Product Task has been disable!"
#endif 
//#define __DEBUG_TASK
#ifdef __DEBUG_TASK
#warning  "Debug Task has been enabled, please Open define "__DEBUG_DISABLE_PRODUCT_TASK" to disable product task"
#ifndef __DEBUG_DISABLE_PRODUCT_TASK
#error "Need open "__DEBUG_DISABLE_PRODUCT_TASK" to disable product task!!!!"
#endif
#endif
                        
#endif  
  
#else
  /*
   * Define for Release
   */
  /* close assert */
  #define __ASSERT(condition)
  
#endif /* __DEBUG */

#endif