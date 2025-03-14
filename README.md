# ESP32 Litime BMS bluetooth Library

Inspired by:  
https://github.com/calledit/LiTime_BMS_bluetooth

An ESP32 library for communication with Litime Battery Management Systems (BMS) via Bluetooth Low Energy (BLE). This library allows access to important battery data such as voltage, current, temperature, state of charge (SOC), and much more.

Tested with Litime 24V 50Ah and ESP32 S3.

## Features

- **Easy Integration**: Simple API to access BMS data
- **Extensive Data**: Access to all available battery parameters
- **Automatic Connection Management**: Automatic reconnect in case of connection loss
- **Real-Time Data**: Regular updates of battery data

## Installation

1. Download the latest version of the library as a ZIP file
2. Open the Arduino IDE
3. Go to `Sketch` -> `Include Library` -> `Add .ZIP Library...`
4. Select the downloaded ZIP file
5. The library is now installed and can be used

## Usage

### Basic Initialization

```cpp
#include <BMSClient.h>

BMSClient bmsClient;

void setup() {
    Serial.begin(115200);
    bmsClient.init("AA:BB:CC:DD:EE:FF"); // replace with your BMS MAC Address!!
    if(bmsClient.connect()) {
        Serial.println("Connected to BMS!");
    }
}

void loop() {
    if(bmsClient.isConnected()) {
        bmsClient.update();
        
        // Retrieve and display data
        Serial.printf("Total Voltage: %.2f V\n", bmsClient.getTotalVoltage());
        Serial.printf("SOC: %d %%\n", bmsClient.getSOC());
    }
    delay(1000);
}
