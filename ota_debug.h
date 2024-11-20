#ifndef OTA_DEBUG_IF_H
#define OTA_DEBUG_IF_H
#ifdef DBIF_MODULE_NAME
#undef DBIF_MODULE_NAME
#endif


#if defined (DEBUG)
/*--- Required DBIF Flags ------------------------------------------------*/
      
  #define DBIF_ENABLED

  #define DBIF_LOGLEVEL                             LOG_DEBUG_0

  #define DBIF_MODULE_NAME                          "OTA"



/**
 * @brief Init Macro to initialze the debug interface
 * 
 */
//#define DBIF_INIT(args...)                      Serial.begin(args)
    
    
/**
  * @brief Print function interface definition for the whole debug interface
  * This flag is the main debug fuction definition. This means, this flag 
  * combines DBIF_STREAM and DBIF_PRINT to
  *          DBIF_STREAM.DBIF_PRINT
  * If a other print function is required, this flag can be used.
  */
  //#define DBIF_PRINTF_FUN                          Serial.printf
  
  /**
  * @brief Timestamp defintion function
  * This flag can be used to modifie the timestamp funciton, which will be written 
  * each time the DBIF_LOG... function is called.
  */
  //#define DBIF_TIMESTAMP_FUN                        millis()

  /**
  * @brief Debug function output enable flag
  * Flag to ouput the function name where debug fucntion was called.
  */
  //#define DBIF_FUNCTION_OUTPUT_ENABLE   

    /**
   * @brief  Debug Macro ouput on change
   * This flag enables the debug output on change functionality
   */
  //#define DBIF_ON_CHANGE_ENABLE

  #include "Debug_If.h"
#else 

  #define DBIF_INIT(...) do {} while(0)

  #define DBIF_LOG_DEBUG_3(...)  do {} while(0)
  #define DBIF_LOG_DEBUG_3_ONCE(...) do {} while(0)

  #define DBIF_LOG_DEBUG_2(...)  do {} while(0)
  #define DBIF_LOG_DEBUG_2_ONCE(...) do {} while(0)

  #define DBIF_LOG_DEBUG_1(...)  do {} while(0)
  #define DBIF_LOG_DEBUG_1_ONCE(...) do {} while(0)

  #define DBIF_LOG_DEBUG_0(...)  do {} while(0)
  #define DBIF_LOG_DEBUG_0_ONCE(...) do {} while(0)

  #define DBIF_LOG_INFO(...)  do {} while(0)
  #define DBIF_LOG_INFO_ONCE(...) do {} while(0)

  #define DBIF_LOG_WARN(...)  do {} while(0)
  #define DBIF_LOG_WARN_ONCE(...)  do {} while(0)

  #define DBIF_LOG_ERROR(...)  do {} while(0)
  #define DBIF_LOG_ERROR_ONCE(OC_VAR,...) do {} while(0)

  #define DBIF_SET_ON_CHANGE_VAR(...) do {} while(0)
  #define DBIF_GET_ON_CHANGE_VAR(...) do {} while(0)
  #define DBIF_ON_CHANGE_LOG_MACRO(...) do {} while(0)


#endif
#endif