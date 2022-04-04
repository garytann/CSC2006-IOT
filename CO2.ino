#include "M5StickCPlus.h"
#include "Adafruit_SGP30.h"
#include <Wire.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_SGP30.h"
#include "BLEDevice.h"
#include "BLEServer.h" // Bluetooth Low Energy
#include "BLEUtils.h" // Bluetooth Low Energy
#include "BLE2902.h"

//define the bluetooh server name 
#define bleServerName "CO2_data"

//define service UUID 
#define SERVICE_UUID "91bad492-b950-4226-aa2b-4ede9fa42f59"

// init variables to be used
float CO2;
Adafruit_SGP30 sgp;
bool deviceConnected = false;

// init timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 60000; // 1min

static BLEUUID bmeServiceUUID("91bad492-b950-4226-aa2b-4ede9fa42f59");

//BLE characteristic and descriptor
BLECharacteristic bmeCarbonDioxideCharacteristics("cba1d466-344c-4be3-ab3f-189f80dd7518", BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor bmeCarbonDioxideDescriptor(BLEUUID((uint16_t)0x2904)); 

//Setup callbacks onConnect and onDisconnect
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

//setup bluetooh and start advertising
void setupBluetooth(){

  //create the BLE device
  BLEDevice::init(bleServerName);

  // Create the BLE Server
  BLEDevice::init(bleServerName);
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  //create BLE service
  BLEService *bmeService = pServer->createService(SERVICE_UUID);

  // setting bluetooth characteristics
  bmeService->addCharacteristic(&bmeCarbonDioxideCharacteristics);
    bmeCarbonDioxideDescriptor.setValue("BME Carbon Dioxide Value");
    bmeCarbonDioxideCharacteristics.addDescriptor(&bmeCarbonDioxideDescriptor);

  // Start the service
  bmeService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void setup(){
  // setting up of serial and M5 sticks parameters
  Serial.begin(115200);
  M5.begin();
  sgp.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.setCursor(0, 0, 2);
  setupBluetooth();
}

void loop() {
  
  // check for any devices connection
  if(deviceConnected){
    if((millis() - lastTime) > timerDelay){
      
      // measure the CO2
      sgp.IAQmeasure();
      float CO2 = sgp.eCO2;
      M5.Lcd.setCursor(0, 20, 2);
      M5.Lcd.printf("CO2: %2.1f", CO2);
      static char carbon[6];
      dtostrf(CO2, 6, 2, carbon);

      // setting the values into the bluetooth characteristic
      bmeCarbonDioxideCharacteristics.setValue(carbon);
      bmeCarbonDioxideCharacteristics.notify();
      
      Serial.print("Carbon Dioxide: ");
      Serial.println(CO2);
      lastTime = millis();
    }
  }
 
}
