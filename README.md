# OTA Library

Eine Arduino-Bibliothek für die Verwaltung von Over-The-Air (OTA) Updates und WiFi-Funktionalität für ESP32 und ESP8266 Plattformen.

## Inhaltsverzeichnis

- [Einführung](#einführung)
- [Funktionen](#funktionen)
- [Installation](#installation)
- [Erste Schritte](#erste-schritte)
  - [Einbindung der Bibliothek](#einbindung-der-bibliothek)
  - [OTA und WiFi Konfiguration](#ota-und-wifi-konfiguration)
  - [Initialisierung der OTA Klasse](#initialisierung-der-ota-klasse)
  - [Aktivieren der OTA Funktionalität](#aktivieren-der-ota-funktionalität)
  - [Loop-Methode](#loop-methode)
- [Verwendung](#verwendung)
  - [OTA Events und Callback-Funktionen](#ota-events-und-callback-funktionen)
  - [Überprüfen des OTA Status](#überprüfen-des-ota-status)
  - [Deaktivieren der OTA Funktionalität](#deaktivieren-der-ota-funktionalität)
- [Beispiele](#beispiele)
  - [Einfaches OTA Beispiel](#einfaches-ota-beispiel)
  - [OTA mit Events und Fortschrittsanzeige](#ota-mit-events-und-fortschrittsanzeige)
- [Konfigurationsoptionen](#konfigurationsoptionen)
- [API Referenz](#api-referenz)
- [Lizenz](#lizenz)
- [Autoren](#autoren)
- [Beitrag leisten](#beitrag-leisten)
- [Support](#support)

## Einführung

Die OTA Bibliothek ermöglicht es Entwicklern, Over-The-Air Firmware-Updates auf ESP32 und ESP8266 Geräten einfach zu implementieren. Sie bietet eine einfache Schnittstelle zur Verwaltung von OTA Updates, WiFi-Verbindungen und Ereignisbehandlung während des Update-Prozesses.

## Funktionen

- **OTA Updates**: Einfache Implementierung von Over-The-Air Firmware-Updates.
- **WiFi Management**: Unterstützt sowohl Access Point als auch Station Mode für WiFi-Verbindungen.
- **Event Handling**: Ereignisgesteuerte Callback-Funktionen für Start, Fortschritt, Fehler und Abschluss von OTA Updates.
- **Anpassbare Konfiguration**: Flexible Einstellungen für WiFi und OTA über Konfigurationsstrukturen.
- **Unterstützung für ESP32 und ESP8266**: Kompatibel mit den gängigen ESP-Mikrocontroller-Plattformen.

## Installation

Um die OTA Bibliothek in Ihrem Projekt zu verwenden, können Sie sie über PlatformIO installieren oder manuell in Ihr Projekt einbinden.

### Installation über PlatformIO

Fügen Sie in Ihrer `platformio.ini` Datei folgende Zeile hinzu:

```ini
lib_deps =
    OTA
```

### Manuelle Installation

1. Laden Sie den Quellcode der Bibliothek von [GitHub](https://github.com/IhrRepository/OTA) herunter.
2. Kopieren Sie den Ordner in das `lib` Verzeichnis Ihres PlatformIO-Projekts.

## Erste Schritte

### Einbindung der Bibliothek

In Ihrer Hauptprogrammdatei (z.B. `main.cpp`) müssen Sie die OTA Bibliothek einbinden:

```cpp
#include "ota.h"
```

### OTA und WiFi Konfiguration

Definieren Sie die Konfigurationsstrukturen für OTA und WiFi:

```cpp
#include "ota_types.h"

WIFI_config_t wifi_config = {
    .ssid = "IhrSSID",
    .password = "IhrPasswort",
    .channel = 1,
    .type = WIFI_AS_STA_MODE, // Oder WIFI_AS_AP_MODE
    .isHidden = false,
    .maxConnections = 4
};

OTA_config_t ota_config = {
    .ota_device_name = "IhrGerätename",
    .port = 3232,
    .ota_password = "OTA_Passwort",
    .ota_enabled = true,
    .reboot_on_success = true
};
```

### Initialisierung der OTA Klasse

Erstellen Sie eine Instanz der OTA Klasse und initialisieren Sie sie:

```cpp
OTA ota(ota_config, wifi_config);

void setup() {
    // Initialisierung der seriellen Kommunikation für Debugging (optional)
    Serial.begin(115200);

    // Initialisierung der OTA Funktionalität
    ota.begin();
}
```

### Aktivieren der OTA Funktionalität

Aktivieren Sie die OTA Updates, indem Sie die `enable()` Methode aufrufen:

```cpp
void setup() {
    // ... vorherige Initialisierung

    // OTA aktivieren
    if (ota.enable()) {
        Serial.println("OTA Updates aktiviert.");
    } else {
        Serial.println("Fehler beim Aktivieren der OTA Updates.");
    }
}
```

### Loop-Methode

In Ihrer Hauptschleife müssen Sie die `loop()` Methode der OTA Klasse aufrufen, um den OTA Service und die WiFi-Verbindung aufrechtzuerhalten:

```cpp
void loop() {
    ota.loop();

    // Rest Ihres Codes
}
```

## Verwendung

### OTA Events und Callback-Funktionen

Die OTA Klasse unterstützt Ereignisse für Start, Fortschritt, Fehler und Abschluss von OTA Updates. Sie können eigene Callback-Funktionen definieren oder die bereitgestellten Events verwenden (wenn `OTA_ENABLE_EVENTS` aktiviert ist).

#### Verwendung von Events

```cpp
void onOtaStart(ota_state_t state) {
    Serial.println("OTA Update gestartet.");
}

void onOtaProgress(uint32_t progress) {
    Serial.printf("OTA Fortschritt: %u%%\n", progress);
}

void onOtaError(ota_error_t error) {
    Serial.printf("OTA Fehler: %d\n", error);
}

void setup() {
    // ... vorherige Initialisierung

    // Events abonnieren
    ota.onStartStop.attach(onOtaStart);
    ota.onUdateProgress.attach(onOtaProgress);
    ota.onUpdateError.attach(onOtaError);
}
```

#### Verwendung von Callback-Funktionen

Alternativ können Sie in der OTA Konfiguration Callback-Funktionen definieren (stellen Sie sicher, dass `OTA_ENABLE_EVENTS` auf `false` gesetzt ist):

```cpp
void onOtaStart() {
    Serial.println("OTA Update gestartet.");
}

void onOtaProgress(uint32_t bytes_written, uint32_t total_size) {
    uint32_t progress = (bytes_written * 100) / total_size;
    Serial.printf("OTA Fortschritt: %u%%\n", progress);
}

void onOtaError(ota_error_t error) {
    Serial.printf("OTA Fehler: %d\n", error);
}

void onOtaFinish() {
    Serial.println("OTA Update abgeschlossen.");
}

OTA_config_t ota_config = {
    // ... andere Konfigurationen
    .onStartCbk = onOtaStart,
    .onProgressCbk = onOtaProgress,
    .onErrorCbk = onOtaError,
    .onFinishCbk = onOtaFinish
};
```

### Überprüfen des OTA Status

Sie können den Status der OTA Updates und der WiFi-Verbindung überprüfen:

```cpp
if (ota.isOtaUpdateOngoing()) {
    Serial.println("OTA Update läuft.");
}

WifiStatus_e wifi_status = ota.getWifiStatus();
Serial.printf("WiFi Status: %d\n", wifi_status);
```

### Deaktivieren der OTA Funktionalität

Wenn Sie OTA Updates deaktivieren möchten:

```cpp
if (ota.disable()) {
    Serial.println("OTA Updates deaktiviert.");
} else {
    Serial.println("Fehler beim Deaktivieren der OTA Updates.");
}
```

## Beispiele

### Einfaches OTA Beispiel

```cpp
#include <Arduino.h>
#include "ota.h"
#include "ota_types.h"

WIFI_config_t wifi_config = {
    .ssid = "IhrSSID",
    .password = "IhrPasswort",
    .channel = 1,
    .type = WIFI_AS_STA_MODE,
    .isHidden = false,
    .maxConnections = 4
};

OTA_config_t ota_config = {
    .ota_device_name = "IhrGerätename",
    .port = 3232,
    .ota_password = "OTA_Passwort",
    .ota_enabled = true,
    .reboot_on_success = true
};

OTA ota(ota_config, wifi_config);

void setup() {
    Serial.begin(115200);
    ota.begin();
    ota.enable();
}

void loop() {
    ota.loop();
}
```

### OTA mit Events und Fortschrittsanzeige

```cpp
#include <Arduino.h>
#include "ota.h"
#include "ota_types.h"

WIFI_config_t wifi_config = {
    .ssid = "IhrSSID",
    .password = "IhrPasswort",
    .channel = 1,
    .type = WIFI_AS_STA_MODE,
    .isHidden = false,
    .maxConnections = 4
};

OTA_config_t ota_config = {
    .ota_device_name = "IhrGerätename",
    .port = 3232,
    .ota_password = "OTA_Passwort",
    .ota_enabled = true,
    .reboot_on_success = true
};

OTA ota(ota_config, wifi_config);

void onOtaStart(ota_state_t state) {
    Serial.println("OTA Update gestartet.");
}

void onOtaProgress(uint32_t progress) {
    Serial.printf("OTA Fortschritt: %u%%\n", progress);
}

void onOtaError(ota_error_t error) {
    Serial.printf("OTA Fehler: %d\n", error);
}

void setup() {
    Serial.begin(115200);
    ota.begin();
    ota.onStartStop.attach(onOtaStart);
    ota.onUdateProgress.attach(onOtaProgress);
    ota.onUpdateError.attach(onOtaError);
    ota.enable();
}

void loop() {
    ota.loop();
}
```

## Konfigurationsoptionen

### WiFi Konfiguration (`WIFI_config_t`)

| Feld             | Typ        | Beschreibung                                       |
|------------------|------------|----------------------------------------------------|
| `ssid`           | `char*`    | SSID des WiFi Netzwerks oder des Access Points     |
| `password`       | `char*`    | Passwort für das WiFi Netzwerk oder Access Point   |
| `channel`        | `uint8_t`  | WiFi Kanal (1-13)                                  |
| `type`           | `WifiType` | WiFi Modus: `WIFI_AS_STA_MODE` oder `WIFI_AS_AP_MODE` |
| `isHidden`       | `bool`     | Ob der Access Point versteckt sein soll            |
| `maxConnections` | `uint8_t`  | Maximale Anzahl an Verbindungen (nur AP Mode)      |

### OTA Konfiguration (`OTA_config_t`)

| Feld                | Typ                      | Beschreibung                                     |
|---------------------|--------------------------|--------------------------------------------------|
| `ota_device_name`   | `char*`                  | Gerätename für die OTA Updates                   |
| `port`              | `uint16_t`               | Port für OTA Updates (Standard: 3232)            |
| `ota_password`      | `char*`                  | Passwort für die OTA Authentifizierung           |
| `ota_enabled`       | `bool`                   | Ob OTA Updates aktiviert sind                    |
| `reboot_on_success` | `bool`                   | Ob das Gerät nach erfolgreichem Update neu startet |
| `onStartCbk`        | `void (*)()`             | Callback für Start des OTA Updates               |
| `onProgressCbk`     | `void (*)(uint32_t, uint32_t)` | Callback für Fortschritt des OTA Updates  |
| `onErrorCbk`        | `void (*)(ota_error_t)`  | Callback für Fehler während des OTA Updates      |
| `onFinishCbk`       | `void (*)()`             | Callback für Abschluss des OTA Updates           |

## API Referenz

Die detaillierte API Dokumentation finden Sie in den Doxygen-Kommentaren der Header-Datei `ota.h`. Sie enthält ausführliche Beschreibungen aller Klassen, Methoden und Parameter.

## Lizenz

Diese Bibliothek ist unter der [MIT Lizenz](LICENSE) lizenziert.

## Autoren

- **Seidel Christof** - *Initiale Arbeit* - [IhrGitHubUsername](https://github.com/CodeName-666/OTA.git)

## Beitrag leisten

Beiträge sind willkommen! Bitte lesen Sie die [CONTRIBUTING.md](CONTRIBUTING.md) für Details zum Ablauf von Pull Requests.

## Support

Bei Fragen oder Problemen öffnen Sie bitte ein Issue auf [GitHub](https://github.com/CodeName-666/OTA/issues).