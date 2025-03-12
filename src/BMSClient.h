#ifndef BMSClient_h
#define BMSClient_h

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEClient.h>
#include <vector>

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

    BMSClient(const char* deviceAddress);
    ~BMSClient();

    bool connect();
    void disconnect();
    bool isConnected();
    void update();
    const BMSData& getData() const;

    // Getter methods
    float getTotalVoltage() const;
    float getCurrent() const;
    uint8_t getSOC() const;
    String getSOH() const;
    // ... (Alle Getter hier deklarieren)

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

    void parseNotificationData(uint8_t* pData, size_t length);
    String bytesToHexString(uint8_t* data, int start, int length, bool reverse) const;
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