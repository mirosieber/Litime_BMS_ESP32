#include "BMSClient.h"

BLEUUID BMSClient::serviceUUID("0000ffe0-0000-1000-8000-00805f9b34fb");
BLEUUID BMSClient::writeUUID("0000ffe2-0000-1000-8000-00805f9b34fb");
BLEUUID BMSClient::notifyUUID("0000ffe1-0000-1000-8000-00805f9b34fb");
static uint8_t queryCommand[] = {0x00, 0x00, 0x04, 0x01, 0x13, 0x55, 0xAA, 0x17};

BMSClient::BMSClient() : 
    deviceAddress(BLEAddress("")),  // Korrektur 1: Initialisierung mit leerem BLEAddress
    pClient(nullptr),
    connected(false),
    lastQuery(0) {}

BMSClient::~BMSClient() {
    disconnect();
}

void BMSClient::init(const char* deviceAddress) {
    this->deviceAddress = BLEAddress(deviceAddress);
}

bool BMSClient::connect() {
    if(connected) return true;
    // Korrektur 2: empty() → isEmpty()
    if(deviceAddress.toString().empty()) return false;

    if(!BLEDevice::getInitialized()) {
        BLEDevice::init("ESP32-BMS-Client");
    }
    
    pClient = BLEDevice::createClient();
    pClient->setClientCallbacks(new MyClientCallback(*this));
    
    if(!pClient->connect(deviceAddress)) {
        Serial.println("Connection failed");
        return false;
    }

    BLERemoteService* pService = pClient->getService(serviceUUID);
    if(!pService) {
        Serial.println("Service not found");
        return false;
    }

    pWriteCharacteristic = pService->getCharacteristic(writeUUID);
    pNotifyCharacteristic = pService->getCharacteristic(notifyUUID);
    if(!pWriteCharacteristic || !pNotifyCharacteristic) {
        Serial.println("Characteristics not found");
        return false;
    }

    if(pNotifyCharacteristic->canNotify()) {
        pNotifyCharacteristic->registerForNotify(
            [this](BLERemoteCharacteristic* pBLERemoteCharacteristic, 
                  uint8_t* pData, size_t length, bool isNotify) {
                this->parseNotificationData(pData, length);
            });
    }

    connected = true;
    return true;
}

void BMSClient::disconnect() {
    if(pClient && pClient->isConnected()) {
        pClient->disconnect();
    }
    connected = false;
}

bool BMSClient::isConnected() {
    return connected;
}

void BMSClient::update() {
    if(connected && (millis() - lastQuery > 1000)) {
        pWriteCharacteristic->writeValue(queryCommand, sizeof(queryCommand), true);
        lastQuery = millis();
    }
}

void BMSClient::parseNotificationData(uint8_t* pData, size_t length) {
    if(length < 104) return;

    currentData.totalVoltage = (uint32_t(pData[11]) << 24 | pData[10] << 16 | pData[9] << 8 | pData[8]) / 1000.0f;
    currentData.cellVoltageSum = (uint32_t(pData[15]) << 24 | pData[14] << 16 | pData[13] << 8 | pData[12]) / 1000.0f;
    currentData.current = (int32_t(pData[51] << 24 | pData[50] << 16 | pData[49] << 8 | pData[48])) / 1000.0f;
    currentData.mosfetTemp = (int16_t)(pData[55] << 8 | pData[54]);
    currentData.cellTemp = (int16_t)(pData[53] << 8 | pData[52]);
    currentData.remainingAh = (pData[63] << 8 | pData[62]) / 100.0f;
    currentData.fullCapacityAh = (pData[65] << 8 | pData[64]) / 100.0f;
    currentData.protectionState = bytesToHexString(pData, 76, 4);
    currentData.heatState = bytesToHexString(pData, 68, 4);
    currentData.balanceMemory = bytesToHexString(pData, 72, 4);
    currentData.failureState = bytesToHexString(pData, 80, 3);
    currentData.balancingState = bytesToBinaryString(pData, 84, 4);
    currentData.batteryState = bytesToHexString(pData, 88, 2);
    currentData.soc = (pData[91] << 8 | pData[90]);
    currentData.soh = String((uint32_t(pData[95]) << 24 | pData[94] << 16 | pData[93] << 8 | pData[92])) + "%";
    currentData.dischargesCount = (uint32_t(pData[99]) << 24 | pData[98] << 16 | pData[97] << 8 | pData[96]);
    currentData.dischargesAhCount = (uint32_t(pData[103]) << 24 | pData[102] << 16 | pData[101] << 8 | pData[100]) / 1000.0f;

    currentData.cellVoltages.clear();
    for(int i = 16; i < 48; i += 2) {
        if(pData[i] == 0 && pData[i+1] == 0) continue;
        float voltage = (uint16_t(pData[i+1] << 8 | pData[i])) / 1000.0f;
        currentData.cellVoltages.push_back(voltage);
    }
}

String BMSClient::bytesToHexString(uint8_t* data, int start, int length, bool reverse) const {
    char buf[length * 2 + 1];
    if(reverse) {
        for(int i = 0; i < length; i++) {
            sprintf(&buf[i * 2], "%02x", data[start + length - 1 - i]);
        }
    } else {
        for(int i = 0; i < length; i++) {
            sprintf(&buf[i * 2], "%02x", data[start + i]);
        }
    }
    buf[length * 2] = '\0';
    return String(buf);
}

String BMSClient::bytesToBinaryString(uint8_t* data, int start, int length) const {
    String result;
    for(int i = length-1; i >= 0; i--) {
        for(int bit = 7; bit >= 0; bit--) {
            result += (data[start + i] & (1 << bit)) ? '1' : '0';
        }
    }
    return result;
}

// Getter Implementierungen
float BMSClient::getTotalVoltage() const { return currentData.totalVoltage; }
float BMSClient::getCellVoltageSum() const { return currentData.cellVoltageSum; }
float BMSClient::getCurrent() const { return currentData.current; }
int16_t BMSClient::getMosfetTemp() const { return currentData.mosfetTemp; }
int16_t BMSClient::getCellTemp() const { return currentData.cellTemp; }
uint8_t BMSClient::getSOC() const { return currentData.soc; }
String BMSClient::getSOH() const { return currentData.soh; }
float BMSClient::getRemainingAh() const { return currentData.remainingAh; }
float BMSClient::getFullCapacityAh() const { return currentData.fullCapacityAh; }
String BMSClient::getProtectionState() const { return currentData.protectionState; }
String BMSClient::getHeatState() const { return currentData.heatState; }
String BMSClient::getBalanceMemory() const { return currentData.balanceMemory; }
String BMSClient::getFailureState() const { return currentData.failureState; }
String BMSClient::getBalancingState() const { return currentData.balancingState; }
String BMSClient::getBatteryState() const { return currentData.batteryState; }
uint32_t BMSClient::getDischargesCount() const { return currentData.dischargesCount; }
float BMSClient::getDischargesAhCount() const { return currentData.dischargesAhCount; }
std::vector<float> BMSClient::getCellVoltages() const { return currentData.cellVoltages; }
const BMSClient::BMSData& BMSClient::getData() const { return currentData; }

// Client Callback Implementierungen
void BMSClient::MyClientCallback::onConnect(BLEClient* pclient) {
    owner.connected = true;
}

void BMSClient::MyClientCallback::onDisconnect(BLEClient* pclient) {
    owner.connected = false;
    BLEDevice::deinit(true);
}
