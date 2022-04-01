import bluepy.btle as btle
import os
import time
import sys
import paho.mqtt.client as mqtt
import json
from sense_hat import SenseHat




THINGSBOARD_HOST = 'demo.thingsboard.io'
ACCESS_TOKEN = 'V7V5E98eB4DHhlyAHeXz'

sensor_data = {'Temperature': 0, 'Humidity': 0, 'Carbon Dioxide': 0}


client = mqtt.Client()

# Set access token
client.username_pw_set(ACCESS_TOKEN)

# Connect to ThingsBoard using default MQTT port and 60 seconds keepalive interval
client.connect(THINGSBOARD_HOST, 1883, 60)

# Init sensehat
sense = SenseHat()

#p = btle.Peripheral("4C:75:25:CB:D2:B2")
#Gary
while True:
    try:
        pTempnHumidity = btle.Peripheral("4C:75:25:CB:80:A2")
        svcTempnHumidity = pTempnHumidity.getServices()
        s1 = pTempnHumidity.getServiceByUUID(list(svcTempnHumidity)[2].uuid)
        humidityCharacteristic = s1.getCharacteristics()[0]   
        temperatureCharacteristic = s1.getCharacteristics()[1]


        pCO2 = btle.Peripheral("4C:75:25:CB:89:9A")
        svcCO2 = pCO2.getServices()
        s2 = pCO2.getServiceByUUID(list(svcCO2)[2].uuid)
        co2Characteristic = s2.getCharacteristics()[0]   

        
        while True:
            if pTempnHumidity.waitForNotifications(15000) or pCO2.waitForNotifications(15000):
                temp = temperatureCharacteristic.read().decode()
                humid = humidityCharacteristic.read().decode()
                co2 = co2Characteristic.read().decode()
                print("Humidity: ", humid, " Temperature Celsius: ", temp, " CO2: ", co2)
                sensor_data['Temperature'] = temp
                sensor_data['Humidity'] = humid
                sensor_data['Carbon Dioxide'] = co2
                
                client.publish('v1/devices/me/telemetry', json.dumps(sensor_data), 1)
                with open("log.txt", "a+") as file_object:
                    # Append 'hello' at the end of file
                    file_object.write("Temp: {0}, Humidity: {1}, CO2: {2}\n".format(temp,humid,co2))

                if(float(co2) > 500.0):
                    sense.clear((255,255,255))
                else:
                    sense.clear((100,80,255))

                #print(c1.read().decode())
                #print(c2.read().decode())
    except KeyboardInterrupt:
        quit()