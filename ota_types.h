#ifndef OTA_TYPES_H
#define OTA_TYPES_H

#include "ArduinoOTA.h"


#ifndef OTA_ENABLE
    #define OTA_ENABLE                                  true
#endif

#ifndef OTA_ENABLE_EVENTS
    #define OTA_ENABLE_EVENTS                           true
#endif


#if (OTA_ENABLE_EVENTS == true) 
    #include "Event.h"
#else 

using OTA_StartCallback = ArduinoOTAClass::THandlerFunction;
using OTA_FinishCallback = ArduinoOTAClass::THandlerFunction;
using OTA_ErrorCallback = ArduinoOTAClass::THandlerFunction_Error; 
using OTA_ProgressCallback = ArduinoOTAClass::THandlerFunction_Progress;

#endif

enum WifiType_e 
{
    WIFI_AS_AP_MODE   = 0x00,
    WIFI_AS_STA_MODE  = 0x01
};


typedef wl_status_t WifiStatus_e;

static inline const char* Ota_WifiStatus2Str(WifiStatus_e status)
{
    switch(status)
    {
        case WL_NO_SHIELD     :
            return "WL_NO_SHIELD";
        case WL_IDLE_STATUS   :
            return "WL_IDLE_STATUS";
        case WL_NO_SSID_AVAIL :
            return "WL_NO_SSID_AVAIL";
        case WL_SCAN_COMPLETED:
            return "WL_SCAN_COMPLETED";
        case WL_CONNECTED     :
            return "WL_CONNECTED";
        case WL_CONNECT_FAILED:
            return "WL_CONNECT_FAILED";
        case WL_CONNECTION_LOST:
            return "WL_CONNECTION_LOST";
        case WL_DISCONNECTED   :
            return "WL_DISCONNECTED";   
    }

    return "WifiStatus_e UNDEF";
}

static inline const char* Ota_WifiType2Str(WifiType_e type)
{
    switch(type)
    {
        case WIFI_AS_AP_MODE :
            return "WIFI_AS_AP_MODE";    
        case WIFI_AS_STA_MODE:
            return "WIFI_AS_STA_MODE";
    }
    return "WifiType_e UNDEF";
}

/**
 * @brief Sturct for the Wifi Configuration
 * 
 */
struct WIFI_config_t
{
    const char* ssid;            //!< SSID of the Access Point or Wifi to connect. This name will be also used for OTA Hostname
    const char* password;        //!< Accss Point Password
    WifiType_e type;             //!< Use Wifi Settings as AccessPoint or as Wifi Client: True = AP/False = Client                            
    uint8_t channel;             //!< WiFi Channel number of the Access Point
    bool isHidden;               //!< Flag to hide the WiFi
    uint8_t maxConnections;      //!< Maximum number of allowed connection to the Access Point.

};

struct OTA_config_t
{
    const char* ota_device_name;            //!< Name of the ESP Device used in the WIFI Network
    const char* ota_password;               //!< Authentification password for the OTA Update
    bool reboot_on_success;                 //!< Reboot flag after OTA Update
    bool ota_enabled;                       //!< General enable flag of the OTA Updates on StartUp. This also enabled the WIFI Connection 
    uint16_t port;                          //!< Upload Port for the OTA Update

#if OTA_ENABLE_EVENTS == false 
    OTA_StartCallback onStartCbk;           //!< Pointer to a callback which shows the start of an OTA Update
    OTA_FinishCallback onFinishCbk;         //!< Pointer to a callback which shows the end of a OTA Updtate
    OTA_ErrorCallback onErrorCbk;           //!< Pointer to a callbeck which will be called when an error occurrs during an ota update
    OTA_ProgressCallback onProgressCbk;     //!< Pointer to a cyclic called callback which contains information about the status
#endif

};



#endif