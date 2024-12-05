#include "ota.h"
#include "ota_debug.h"

#if ESP32
#include "WiFi.h"
#elif ESP8266
#include "ESP8266WiFi.h"
#else
#error "NOT Supported at the moment"
#endif

OTA::OTA(OTA_config_t &ota_config, WIFI_config_t &wifi_config) : ArduinoOTAClass(),
                                                                 m_wifi_config(&wifi_config),
                                                                 m_ota_config(&ota_config),
                                                                 m_is_enabled(false),
                                                                 m_old_progress(0),
                                                                 m_progress(0),
                                                                 m_ota_update_ongoing(false)
{
}

void OTA::begin(void)
{
    DBIF_INIT(115200);
    DBIF_LOG_INFO("Wifi Config:");
    DBIF_LOG_INFO("SSID: %s", wifi_config.ssid);
    DBIF_LOG_INFO("PSW: %s", wifi_config.ssid);
    DBIF_LOG_INFO("TYPE: %s", Ota_WifiType2Str(wifi_config.type));
    DBIF_LOG_INFO("CHANNEL: %i", wifi_config.channel);
    DBIF_LOG_INFO("MAX CONNECTION: %i", wifi_config.maxConnections);
    DBIF_LOG_INFO("HIDDEN: %i", wifi_config.isHidden);

    DBIF_LOG_INFO("OTA Config:");
    DBIF_LOG_INFO("DEVICE NAME: %s", ota_config.ota_device_name);
    DBIF_LOG_INFO("PORT: %i", ota_config.port);
    DBIF_LOG_INFO("PSW %s", ota_config.ota_password);
    DBIF_LOG_INFO("ENABLED: %i", ota_config.ota_enabled);
    DBIF_LOG_INFO("REBOOT: %i", ota_config.reboot_on_success);

    setHostname(m_ota_config->ota_device_name);
    setPassword(m_ota_config->ota_password);
    setPort(m_ota_config->port);
#if ESP32
    setMdnsEnabled(true);
#endif
    setRebootOnSuccess(m_ota_config->reboot_on_success);

    onStart([this]()
            { start(); });

    onEnd([this]()
          { finish(); });

    onError([this](ota_error_t ota_error)
            { error(ota_error); });

    onProgress([this](uint32_t bytes_written, uint32_t total_size)
               { progress(bytes_written, total_size); });

    if (m_ota_config->ota_enabled == true)
    {
        enable();
    }

    ArduinoOTAClass::begin();
}

bool OTA::enableAccessPoint(void)
{
    bool enabled = WiFi.softAP(m_wifi_config->ssid,
                               m_wifi_config->password,
                               m_wifi_config->channel,
                               m_wifi_config->isHidden,
                               m_wifi_config->maxConnections);

    if (enabled)
    {
        m_is_enabled = true;
        DBIF_LOG_INFO("OTA Access Point Started");
    }
    else
    {
        m_is_enabled = false;
        DBIF_LOG_ERROR("Could not start Access Point");
    }
    return enabled;
}

bool OTA::enableStationMode(void)
{
    m_wifi_status = WiFi.begin(m_wifi_config->ssid, m_wifi_config->password, m_wifi_config->channel);
    m_wifi_status != WL_CONNECT_FAILED ? m_is_enabled = true : m_is_enabled = false;
    WiFi.setAutoConnect(true);

    DBIF_LOG_INFO("WIFI Connection Status: %i", (uint16_t)m_wifi_status);
    return m_is_enabled;
}

bool OTA::enable(void)
{
    bool ret;
    switch (m_wifi_config->type)
    {
    case WIFI_AS_AP_MODE:
        ret = enableAccessPoint();
        break;
    case WIFI_AS_STA_MODE:
    default:
        ret = enableStationMode();
        break;
    }
    return ret;
}

bool OTA::disable(void)
{
    switch (m_wifi_config->type)
    {
    case WIFI_AS_AP_MODE:
        return disableAccessPoint();
        break;
    case WIFI_AS_STA_MODE:
    default:
        return disableStationMode();
        break;
    }
}

bool OTA::isEnabled(void)
{
    switch (m_wifi_config->type)
    {
    case WIFI_AS_AP_MODE:
        return m_is_enabled;
        break;
    case WIFI_AS_STA_MODE:
    default:
        return WiFi.isConnected();
        break;
    }
}

bool OTA::disableAccessPoint(void)
{
    return WiFi.softAPdisconnect(true);
}

bool OTA::disableStationMode(void)
{
    return WiFi.disconnect(true);
}

void OTA::loop(void)
{
    if (isEnabled())
    {
        ArduinoOTAClass::handle();
        checkWifiStatus();
    }
}

bool OTA::isOtaUpdateOngoing(void)
{
    return m_ota_update_ongoing;
}

void OTA::start(void)
{
    m_ota_update_ongoing = true;

#if OTA_ENABLE_EVENTS == true
    EVENT_EMIT(onStartStop, OTA_RUNUPDATE);
#else
    if (m_ota_config->onStartCbk != nullptr)
    {
        m_ota_config->onStartCbk();
    }
#endif
    m_old_progress = 0;
    m_progress = 0;

    DBIF_LOG_INFO("Start OTA UPDATE");
}

void OTA::finish(void)
{
    m_progress = 100;
    m_old_progress = 0;

#if OTA_ENABLE_EVENTS == true
    EVENT_EMIT(onUdateProgress, m_progress);
    EVENT_EMIT(onStartStop, OTA_IDLE);
#else
    if (m_ota_config->onFinishCbk != nullptr)
    {
        m_ota_config->onFinishCbk();
    }
#endif

    m_ota_update_ongoing = false;
}

void OTA::error(ota_error_t ota_error)
{

#if OTA_ENABLE_EVENTS == true
    EVENT_EMIT(onUpdateError, ota_error);
#else
    if (m_ota_config->onErrorCbk != nullptr)
    {
        m_ota_config->onErrorCbk(ota_error);
    }
#endif

    m_ota_update_ongoing = false;
}

void OTA::progress(uint32_t bytes_written, uint32_t total_size)
{
    m_progress = (bytes_written / (total_size / 100));

    if (m_progress > m_old_progress)
    {

#if OTA_ENABLE_EVENTS == true
        EVENT_EMIT(onUdateProgress, m_progress);
#endif

        m_old_progress = m_progress;
        DBIF_LOG_INFO("OTA in Progress: %i%% of %i bytes\n", m_progress, total_size);
    }
#if OTA_ENABLE_EVENTS == false
    if (m_ota_config->onProgressCbk != nullptr)
    {
        m_ota_config->onProgressCbk(bytes_written, total_size);
    }
#endif
    m_ota_update_ongoing = true;
}

WifiStatus_e OTA::getWifiStatus(void)
{
    return (WifiStatus_e)m_wifi_status;
}

void OTA::checkWifiStatus(void)
{
    if (WiFi.status() != m_wifi_status)
    {
        m_wifi_status = WiFi.status();
        DBIF_LOG_INFO("WIFI Status  %s", Ota_WifiStatus2Str(m_wifi_status));
    }
}