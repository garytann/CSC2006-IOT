#include "M5StickCPlus.h"
#include "Adafruit_SGP30.h"
#include "DHT12.h"
#include <Wire.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_SGP30.h"
#include "BLEDevice.h"

//define the bluetooh server name 
#define bleServerName "BME280_ESP32"

#define SERVICE_UUID "91bad492-b950-4226-aa2b-4ede9fa42f59"

//BLE characteristic and descriptor
static BLEUUID temperatureCharacteristicUUID("cba1d466-344c-4be3-ab3f-189f80dd7518");
static BLEDescriptor bmeTemperatureCelsiusDescriptor(BLEUUID((uint16_t)0x2902));

//Humidity Charactertistic and descriptor
static BLEUUID humidityCharacteristicUUID("ca73b3ba-39f6-4ab3-91ae-186dc9577d99");
static BLEDescriptor bmeHumidityDescriptor(BLEUUID((uint16_t)0x2903));

//Flags stating if should begin connecting and if the connection is up
static boolean doConnect = false;
static boolean connected = false;
bool deviceConnected = false;

float temp;
float hum;

DHT12 dht12;
//Adafruit_SGP30 sgp;

void setup(){
  M5.begin();
//  if (! sgp.begin()){
//    Serial.println("Sensor not found :(");
//    while (1);
//  }
  Wire.begin(0,26);
  M5.Lcd.setRotation(3);
  M5.Lcd.setCursor(0, 0, 2);
  Serial.begin(115200);
}
void loop() {
//  if (! sgp.IAQmeasure()) {
//    Serial.println("Measurement failed");
//    return;
//  }
//  float tmp = dht12.readTemperature();
//  float hum = dht12.readHumidity();
//  float CO2 = sgp.eCO2;
//  M5.Lcd.setCursor(0, 20, 2);
//  M5.Lcd.printf("Temp: %2.1f \n", tmp);
//  M5.Lcd.printf("Humi: %2.0f%% \n", hum);
//  Serial.print(CO2);

  readDHT12();
//  delay(1000);
}


void readDHT12(){
  temp = dht12.readTemperature();
  hum = dht12.readHumidity();
  M5.Lcd.printf("Temp: %2.1f \n", temp);
  M5.Lcd.printf("Humi: %2.0f%% \n", hum);
  delay(1000);
}
