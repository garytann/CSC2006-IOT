from bluepy.btle import DefaultDelegate, Scanner, BTLEException
import sys
import struct
from datetime import datetime

class ScanDelegate(DefaultDelegate):
    def __init__(self): # コンストラクタ
        DefaultDelegate.__init__(self)
        self.lastseq = None
        self.lasttime = datetime.fromtimestamp(0)

    def handleDiscovery(self, dev, isNewDev, isNewData):
        if isNewDev or isNewData: # 新しいデバイスまたは新しいデータ
            print(dev.getScanData())
            # for (adtype, desc, value) in dev.getScanData():
            #     print(adtype," ",desc," ",value)
            # for (adtype, desc, value) in dev.getScanData(): # データの数だけ繰り返す
            #     if desc == 'Manufacturer' and value[0:4] == 'ffff': # テスト用companyID
            #         __delta = datetime.now() - self.lasttime
            #         # アドバタイズする10秒の間に複数回測定されseqが加算されたものは捨てる（最初に取得された１個のみを使用する）
            #         if value[4:6] != self.lastseq and __delta.total_seconds() > 11:
            #             self.lastseq = value[4:6] # Seqと時刻を保存
            #             self.lasttime = datetime.now()
            #             (temp, humid, press, volt) = struct.unpack('<hhhh', bytes.fromhex(value[6:])) # hは2Byte整数（４つ取り出す）
            #             print('温度= {0} 度、 湿度= {1} %、 気圧 = {2} hPa、 電圧 = {3} V'.format( temp / 100, humid / 100, press, volt/100))

if __name__ == "__main__":
    scanner = Scanner().withDelegate(ScanDelegate())
    while True:
        try:
            scanner.scan(5.0) # スキャンする。デバイスを見つけた後の処理はScanDelegateに任せる
        except BTLEException:
            ex, ms, tb = sys.exc_info()
            print('BLE exception '+str(type(ms)) + ' at ' + sys._getframe().f_code.co_name)