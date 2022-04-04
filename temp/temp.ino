#include "M5StickCPlus.h"
#include "Adafruit_SGP30.h"
#include "DHT12.h"
#include <Wire.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_SGP30.h"
#include "BLEDevice.h"
#include "BLEServer.h" // Bluetooth Low Energy
#include "BLEUtils.h" // Bluetooth Low Energy
#include "BLE2902.h"


#define temperatureCelsius

//define the bluetooh server name 
#define bleServerName "BME280"

#define SERVICE_UUID "91bad492-b950-4226-aa2b-4ede9fa42f60"

float temp; 
float hum;
DHT12 dht12;

bool deviceConnected = false;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

static BLEUUID bmeServiceUUID("91bad492-b950-4226-aa2b-4ede9fa42f59");

//BLE characteristic and descriptor
BLECharacteristic bmeTemperatureCelsiusCharacteristics("cba1d466-344c-4be3-ab3f-189f80dd7518", BLECharacteristic::PROPERTY_NOTIFY);
  BLEDescriptor bmeTemperatureCelsiusDescriptor(BLEUUID((uint16_t)0x2902));

//Humidity Charactertistic and descriptor
BLECharacteristic bmeHumidityCharacteristics("ca73b3ba-39f6-4ab3-91ae-186dc9577d99", BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor bmeHumidityDescriptor(BLEUUID((uint16_t)0x2903));


//Setup callbacks onConnect and onDisconnect
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

void setup(){

  Serial.begin(115200);
  setupDHT12();
  setupBluetooth();
  
}
void loop() {
  if(deviceConnected){
    if((millis() - lastTime) > timerDelay){
      temp = dht12.readTemperature();
      hum = dht12.readHumidity();
      M5.Lcd.setCursor(0,20,2);
      M5.Lcd.printf("Temp: %2.1f \n", temp);
      M5.Lcd.printf("Humi: %2.0f%% \n", hum);

      //notify temperature reading from dht12
      static char temperatureCTemp[6];
      dtostrf(temp, 6, 2, temperatureCTemp);
      bmeTemperatureCelsiusCharacteristics.setValue(temperatureCTemp);
      bmeTemperatureCelsiusCharacteristics.notify();
      Serial.print("Temperature Celsius: ");
      Serial.print(temp);
      Serial.print(" ºC");

      //notify humidity reading from hum
      static char humidityTemp[6];
      dtostrf(hum, 6, 2, humidityTemp);
      //Set humidity Characteristic value and notify connected client
      bmeHumidityCharacteristics.setValue(humidityTemp);
      bmeHumidityCharacteristics.notify();   
      Serial.print(" - Humidity: ");
      Serial.print(hum);
      Serial.println(" %");
      lastTime = millis();
    }
  }
 
}

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

 bmeService->addCharacteristic(&bmeTemperatureCelsiusCharacteristics);
    bmeTemperatureCelsiusDescriptor.setValue("BME temperature Celsius");
    bmeTemperatureCelsiusCharacteristics.addDescriptor(&bmeTemperatureCelsiusDescriptor);

  bmeService->addCharacteristic(&bmeHumidityCharacteristics);
  bmeHumidityDescriptor.setValue("BME humidity");
  bmeHumidityCharacteristics.addDescriptor(&bmeHumidityDescriptor);

  // Start the service
  bmeService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
  
}

//setup DHT12 sensor
void setupDHT12(){
  M5.begin();
  Wire.begin(0,26);
  M5.Lcd.setRotation(3);
}
