#ifndef OTA_HPP
#define OTA_HPP

#include "ota_types.h"

/**
 * @class OTA
 * @brief Class for managing Over-The-Air (OTA) updates.
 *
 * The OTA class handles the setup, execution, and management of OTA updates
 * for devices based on the ArduinoOTAClass. It provides methods to enable or
 * disable OTA functionality, manage WiFi connections, and handle update events.
 */
class OTA : public ArduinoOTAClass
{

public:
    /**
     * @brief Constructs a new OTA object.
     *
     * Initializes the OTA object with the provided OTA configuration and WiFi configuration.
     *
     * @param[in] ota_config Reference to an OTA configuration structure.
     * @param[in] wifi_config Reference to a WiFi configuration structure.
     */
    explicit OTA(OTA_config_t &ota_config, WIFI_config_t &wifi_config);

    /**
     * @brief Initializes the OTA service.
     *
     * Sets up the necessary configurations and prepares the OTA service to begin.
     * This method should be called before enabling the OTA functionality.
     */
    void begin();

    /**
     * @brief Enables the OTA update functionality.
     *
     * Depending on the WiFi configuration, this method will start WiFi in either
     * Access Point or Station mode, allowing OTA updates to be received.
     *
     * @return true if the OTA service was successfully enabled.
     * @return false if there was an error enabling the OTA service.
     */
    bool enable(void);

    /**
     * @brief Disables the OTA update functionality.
     *
     * Shuts down the WiFi connection or Access Point and disables the OTA update functionality.
     *
     * @return true if the OTA service was successfully disabled.
     * @return false if there was an error disabling the OTA service.
     */
    bool disable(void);

    /**
     * @brief Checks whether OTA updates are enabled.
     *
     * @return true if OTA is enabled.
     * @return false if OTA is disabled.
     */
    bool isEnabled(void);

    /**
     * @brief Cyclic loop method to handle OTA updates.
     *
     * This method should be called periodically in the main loop to handle OTA updates
     * and maintain WiFi connections.
     */
    void loop(void);

    /**
     * @brief Checks whether an OTA update is ongoing.
     *
     * @note This status is implemented with a global flag and should be handled carefully.
     *
     * @return true if an OTA update is in progress.
     * @return false if no OTA update is currently happening.
     */
    bool isOtaUpdateOngoing(void);

    /**
     * @brief Retrieves the current WiFi status.
     *
     * @return The current WiFi status as a `WifiStatus_e` enumeration.
     */
    WifiStatus_e getWifiStatus(void);

    /**
     * @brief Checks the WiFi status and updates internal status if it has changed.
     *
     * Compares the current WiFi status with the stored status and updates it if there is a change.
     * It also logs the new status for debugging purposes.
     */
    void checkWifiStatus(void);

private:
    /**
     * @brief Enables WiFi in Access Point mode for OTA updates.
     *
     * Configures and starts WiFi as an Access Point, allowing devices to connect
     * directly to the device for OTA updates.
     *
     * @return true if the Access Point was successfully enabled.
     * @return false if there was an error enabling the Access Point.
     */
    bool enableAccessPoint(void);

    /**
     * @brief Enables WiFi in Station mode for OTA updates.
     *
     * Connects to an existing WiFi network as a station to receive OTA updates.
     *
     * @return true if Station mode was successfully enabled.
     * @return false if there was an error enabling Station mode.
     */
    bool enableStationMode(void);

    /**
     * @brief Disables the WiFi Access Point mode.
     *
     * Shuts down the WiFi Access Point, preventing any further connections for OTA updates.
     *
     * @return true if the Access Point was successfully disabled.
     * @return false if there was an error disabling the Access Point.
     */
    bool disableAccessPoint(void);

    /**
     * @brief Disables WiFi Station mode.
     *
     * Disconnects from the WiFi network and stops the WiFi module.
     *
     * @return true if Station mode was successfully disabled.
     * @return false if there was an error disabling Station mode.
     */
    bool disableStationMode(void);

    /**
     * @brief Callback function called when an OTA update starts.
     *
     * Sets internal flags, emits events or calls configured callbacks to handle the start of an OTA update.
     */
    void start(void);

    /**
     * @brief Callback function called when an OTA update finishes.
     *
     * Resets progress variables, emits events or calls configured callbacks to handle the completion of an OTA update.
     */
    void finish(void);

    /**
     * @brief Callback function called when an OTA update encounters an error.
     *
     * Handles errors that occur during the OTA update process by emitting events or calling error callbacks.
     *
     * @param ota_error The error code representing the type of error that occurred.
     */
    void error(ota_error_t ota_error);

    /**
     * @brief Callback function called to report OTA update progress.
     *
     * Calculates the progress percentage and handles progress updates by emitting events or calling progress callbacks.
     *
     * @param bytes_written The number of bytes written so far.
     * @param total_size The total size of the update in bytes.
     */
    void progress(uint32_t bytes_written, uint32_t total_size);

#if OTA_ENABLE_EVENTS == true
public:
    /**
     * @brief Event signal for OTA start and stop events.
     *
     * This event is emitted when an OTA update starts or stops, allowing external handlers to respond accordingly.
     */
    EventSignal<ota_state_t> onStartStop;

    /**
     * @brief Event signal for OTA update errors.
     *
     * This event is emitted when an error occurs during an OTA update, providing the error code.
     */
    EventSignal<ota_error_t> onUpdateError;

    /**
     * @brief Event signal for OTA update progress.
     *
     * This event is emitted periodically to report the progress of an OTA update in percentage.
     */
    EventSignal<uint32_t> onUdateProgress;
#endif

private:
    /**
     * @brief Pointer to the WiFi configuration.
     *
     * Stores a pointer to the WiFi configuration structure provided during initialization.
     */
    WIFI_config_t *m_wifi_config;

    /**
     * @brief Pointer to the OTA configuration.
     *
     * Stores a pointer to the OTA configuration structure provided during initialization.
     */
    OTA_config_t *m_ota_config;

    /**
     * @brief Flag indicating whether OTA updates are enabled.
     *
     * Set to `true` when OTA updates are enabled and `false` when disabled.
     */
    bool m_is_enabled;

    /**
     * @brief Stores the previous progress percentage.
     *
     * Used to track progress updates and avoid redundant progress reporting.
     */
    uint32_t m_old_progress;

    /**
     * @brief Stores the current progress percentage.
     *
     * Represents the current progress of an ongoing OTA update.
     */
    uint32_t m_progress;

    /**
     * @brief Flag indicating whether an OTA update is ongoing.
     *
     * Set to `true` when an OTA update is in progress and `false` otherwise.
     */
    bool m_ota_update_ongoing;

    /**
     * @brief Stores the current WiFi status.
     *
     * Holds the current status of the WiFi connection, as a `WifiStatus_e` enumeration value.
     */
    WifiStatus_e m_wifi_status;
};

#endif // OTA_HPP
