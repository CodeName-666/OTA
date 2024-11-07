#ifndef OTA_HPP
#define OTA_HPP

#include "ota_types.h"


class OTA : public ArduinoOTAClass {

    public:
        
        /**
         * @brief Construct a new OTA object
         * 
         * @param[in] ota_config  Reference to a OTA configuration
         * @param[in] wifi_config  Reference to a Wifif Configuration
         */
        explicit OTA(OTA_config_t& ota_config, WIFI_config_t& wifi_config);
        /**
         * @brief Methode to initialize the OTA objekc
         */
        void begin();

        /**
         * @brief 
         * 
         */
        bool enable(void);

        /**
         * @brief Methode to disable the OTA Updates.
         *  
         * This methode will disable the OTA update functionallity and also the WiFi AccessPoint will go down.
         */
        bool disable(void);


        /**
         * @brief Methode to check whether the OTA Updates is enabled or not
         * 
         * @return true OTA is enabled
         * @return false OTA is disabled
         */
        bool isEnabled(void);
        /**
         * @brief Cycleic Loop Methode
         * 
         * This methde should be called cyclic in the main loop.
         */
        void loop(void);
        /**
         * @brief Methode to check whether an OTA Update is ongoning
         * 
         * @note This status is implemented with an global flag. This should be implmented correctly
         * @return true  OTA Update is Ongoing
         * @return false No OTA Update at the moment
         */
        bool isOtaUpdateOngoing(void);

        WifiStatus_e getWifiStatus(void);

        void checkWifiStatus(void);
    private:
    
        /**
         * @brief Methode to enable the OTA Updates.
         * 
         * Wifi will start and it will be configured as an Access Point.
         */
        bool enableAccessPoint(void);
        
        /**
         * @brief Methode to enable the OTA Updates.
         * 
         * This methode enables everything is needed to allow OTA Updates. This means also Wifi will start and it will 
         * be configured as an Access Point.
         */
        bool enableStationMode(void);
        /**
         * @brief Methode to disable the WiFi connectiion and shutdown the Access Point
         * 
         * @return true WiFI disabled successfull
         * @return false Error happen during shutdown prozess. @note Error handling is not implemented yet.
         */
        bool disableAccessPoint(void);

        bool disableStationMode(void);

        /**
         * @brief 
         * 
         */
        void start(void);

        /**
         * @brief 
         * 
         */
        void finish(void);

        /**
         * @brief 
         * 
         * @param error 
         */
        void error(ota_error_t ota_error);

        /**
         * @brief 
         * 
         * @param bytes_written 
         * @param total_size 
         */
        void progress(uint32_t bytes_written, uint32_t total_size);

#if OTA_ENABLE_EVENTS == true
    public:
        EventSignal<ota_state_t> onStartStop;
        EventSignal<ota_error_t> onUpdateError;
        EventSignal<uint32_t> onUdateProgress;
#endif

    private:
        WIFI_config_t* m_wifi_config;
        OTA_config_t* m_ota_config;
        bool m_is_enabled;
        uint32_t m_old_progress;
        uint32_t m_progress;
        bool m_ota_update_ongoing;
        WifiStatus_e m_wifi_status;
};

#endif
