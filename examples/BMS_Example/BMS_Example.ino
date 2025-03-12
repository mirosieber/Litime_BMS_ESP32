#include <BMSClient.h>

BMSClient bmsClient;

void printCellVoltages(const std::vector<float>& cells) {
    Serial.println("Cell Voltages:");
    for(size_t i = 0; i < cells.size(); i++) {
        Serial.printf("  Cell %2d: %.3fV\n", i+1, cells[i]);
    }
}

void setup() {
    Serial.begin(115200);
    delay(3000);
    Serial.println("BMS Client Example");
    
    bmsClient.init("C8:47:80:18:E1:15");  // MAC-Adresse über init()
    
    if(bmsClient.connect()) {
        Serial.println("Connected to BMS!");
    } else {
        Serial.println("Connection failed!");
    }
}

void loop() {
    if(bmsClient.isConnected()) {
        bmsClient.update();
        
        Serial.println("\n=== BMS Data ===");
        Serial.printf("Total Voltage:    %.2f V\n", bmsClient.getTotalVoltage());
        Serial.printf("Current:          %.2f A\n", bmsClient.getCurrent());
        Serial.printf("SOC:              %d %%\n", bmsClient.getSOC());
        Serial.printf("SOH:              %s\n", bmsClient.getSOH().c_str());
        Serial.printf("MOSFET Temp:      %d °C\n", bmsClient.getMosfetTemp());
        Serial.printf("Cell Temp:        %d °C\n", bmsClient.getCellTemp());
        Serial.printf("Remaining:        %.2f Ah\n", bmsClient.getRemainingAh());
        Serial.printf("Full Capacity:    %.2f Ah\n", bmsClient.getFullCapacityAh());
        Serial.printf("Protection State: %s\n", bmsClient.getProtectionState().c_str());
        Serial.printf("Balancing:        %s\n", bmsClient.getBalancingState().c_str());
        Serial.printf("Discharge Cycles: %u\n", bmsClient.getDischargesCount());
        
        printCellVoltages(bmsClient.getCellVoltages());
        Serial.println("==================\n");
    }else {
      if(bmsClient.connect()) {
        Serial.println("Connected to BMS!");
      } else {
        Serial.println("Connection failed!");

      }
    }
    delay(2000);
}
