# Litime BMS Bluetooth ESP32 Library

This repository contains an ESP32 library for communication with **Litime Battery Management Systems (BMS)** via **Bluetooth Low Energy (BLE)**. The library provides a simple interface to access essential battery data such as **voltage**, **current**, **temperature**, **state of charge (SOC)**, and more. 

The library is designed to work with the **Litime BMS** and has been tested with the **Litime 24V 50Ah** model and **ESP32 S3**.

## Features

- **Easy Integration**: A simple API for accessing BMS data with minimal setup.
- **Extensive Data**: Provides access to all available battery parameters.
- **Automatic Connection Management**: Automatically reconnects if the connection to the BMS is lost.
- **Real-Time Data**: Provides regular updates on the battery’s status.

## Installation

To install the library:

1. Download the latest version of the library as a ZIP file.
2. Open the **Arduino IDE**.
3. Go to **Sketch** -> **Include Library** -> **Add .ZIP Library...**.
4. Select the downloaded ZIP file.
5. The library is now installed and ready for use.

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
