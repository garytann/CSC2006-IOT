import bluepy.btle as btle

p = btle.Peripheral("4C:75:25:CB:D2:B2")
svc = p.getServices()
s1 = p.getServiceByUUID(list(svc)[2].uuid)
c1 = s1.getCharacteristics()[0]   
c2 = s1.getCharacteristics()[1]   
 
while True:
    if p.waitForNotifications(10000):
        print("Humidity: ", c1.read().decode(), " Temperature Celsius: ", c2.read().decode() )
        #print(c1.read().decode())
        #print(c2.read().decode())