#ifndef BMSClient_h
#define BMSClient_h

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEClient.h>
#include <vector>
#include <Arduino.h>

class BMSClient {
public:
    struct BMSData {
        float totalVoltage;
        float cellVoltageSum;
        float current;
        int16_t mosfetTemp;
        int16_t cellTemp;
        uint8_t soc;
        String soh;
        float remainingAh;
        float fullCapacityAh;
        String protectionState;
        String heatState;
        String balanceMemory;
        String failureState;
        String balancingState;
        String batteryState;
        uint32_t dischargesCount;
        float dischargesAhCount;
        std::vector<float> cellVoltages;
    };

    BMSClient();
    ~BMSClient();

    void init(const char* deviceAddress);
    bool connect();
    void disconnect();
    bool isConnected();
    void update();
    const BMSData& getData() const;

    // Getter methods
    float getTotalVoltage() const;
    float getCellVoltageSum() const;
    float getCurrent() const;
    int16_t getMosfetTemp() const;
    int16_t getCellTemp() const;
    uint8_t getSOC() const;
    String getSOH() const;
    float getRemainingAh() const;
    float getFullCapacityAh() const;
    String getProtectionState() const;
    String getHeatState() const;
    String getBalanceMemory() const;
    String getFailureState() const;
    String getBalancingState() const;
    String getBatteryState() const;
    uint32_t getDischargesCount() const;
    float getDischargesAhCount() const;
    std::vector<float> getCellVoltages() const;

private:
    static BLEUUID serviceUUID;
    static BLEUUID writeUUID;
    static BLEUUID notifyUUID;
    
    BLEAddress deviceAddress;
    BLEClient* pClient;
    BLERemoteCharacteristic* pWriteCharacteristic;
    BLERemoteCharacteristic* pNotifyCharacteristic;
    BMSData currentData;
    bool connected;
    unsigned long lastQuery;

    void parseNotificationData(uint8_t* pData, size_t length);
    String bytesToHexString(uint8_t* data, int start, int length, bool reverse = true) const;
    String bytesToBinaryString(uint8_t* data, int start, int length) const;

    class MyClientCallback : public BLEClientCallbacks {
    public:
        MyClientCallback(BMSClient& owner) : owner(owner) {}
        void onConnect(BLEClient* pclient);
        void onDisconnect(BLEClient* pclient);
    private:
        BMSClient& owner;
    };
};

#endif
