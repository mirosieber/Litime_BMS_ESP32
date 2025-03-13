# Litime BMS Client Library

Inspiriert an:
https://github.com/calledit/LiTime_BMS_bluetooth


[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Eine Arduino/ESP32-Bibliothek zur Kommunikation mit Batteriemanagementsystemen (BMS) der firma Litime über Bluetooth Low Energy (BLE). Diese Bibliothek ermöglicht den Zugriff auf wichtige Batteriedaten wie Spannung, Strom, Temperatur, Ladezustand (SOC) und vieles mehr.

## Features

- **Einfache Integration**: Einfache API für den Zugriff auf BMS-Daten
- **Umfangreiche Daten**: Zugriff auf über 20 verschiedene Batterieparameter
- **Automatische Verbindungsverwaltung**: Automatisches Reconnect bei Verbindungsabbruch
- **Echtzeitdaten**: Regelmäßige Aktualisierung der Batteriedaten
- **Plattformunabhängig**: Kompatibel mit allen ESP32-Boards
- **Detaillierte Zellinformationen**: Einzelne Zellspannungen und Balancierungsstatus

## Installation

1. Laden Sie die neueste Version der Bibliothek als ZIP-Datei herunter
2. Öffnen Sie die Arduino IDE
3. Gehen Sie zu `Sketch` -> `Bibliothek einbinden` -> `.ZIP-Bibliothek hinzufügen...`
4. Wählen Sie die heruntergeladene ZIP-Datei aus
5. Die Bibliothek ist nun installiert und kann verwendet werden

## Verwendung

### Grundlegende Initialisierung

```cpp
#include <BMSClient.h>

// BMS MAC-Adresse
BMSClient bmsClient;

void setup() {
    Serial.begin(115200);
    bmsClient.init("C8:47:80:18:E1:15");
    if(bmsClient.connect()) {
        Serial.println("Connected to BMS!");
    }
}

void loop() {
    if(bmsClient.isConnected()) {
        bmsClient.update();
        
        // Daten abrufen und anzeigen
        Serial.printf("Total Voltage: %.2f V\n", bmsClient.getTotalVoltage());
        Serial.printf("SOC: %d %%\n", bmsClient.getSOC());
    }
    delay(1000);
}
