import bluepy.btle as btle
import os
import time
import sys
import paho.mqtt.client as mqtt
import json



THINGSBOARD_HOST = 'demo.thingsboard.io'
ACCESS_TOKEN = 'vsydo22hiEvrYXeg9Kmw'

INTERVAL=2

sensor_data = {'temperature': 0, 'humidity': 0, 'co2':0}

next_reading = time.time() 

client = mqtt.Client()

# Set access token
client.username_pw_set(ACCESS_TOKEN)

# Connect to ThingsBoard using default MQTT port and 60 seconds keepalive interval
client.connect(THINGSBOARD_HOST, 1883, 60)

#client.loop_start()


#p = btle.Peripheral("4C:75:25:CB:D2:B2")
#Gary
p = btle.Peripheral("4C:75:25:CB:80:A2")
svc = p.getServices()
s1 = p.getServiceByUUID(list(svc)[2].uuid)
c1 = s1.getCharacteristics()[0]   
c2 = s1.getCharacteristics()[1]


p2 = btle.Peripheral("4C:75:25:CB:89:9A")

svc2 = p2.getServices()
s2 = p2.getServiceByUUID(list(svc2)[2].uuid)
co2 = s2.getCharacteristics()[0]   

 
while True:
    if p.waitForNotifications(20000) or p2.waitForNotifications(20000):
        print("Humidity: ", c1.read().decode(), " Temperature Celsius: ", c2.read().decode(), " CO2: ", co2.read().decode())
        sensor_data['temperature'] = c1.read().decode()
        sensor_data['humidity'] = c2.read().decode()
        sensor_data['co2'] = co2.read().decode()
        client.publish('v1/devices/me/telemetry', json.dumps(sensor_data), 1)
        #print(c1.read().decode())
        #print(c2.read().decode())