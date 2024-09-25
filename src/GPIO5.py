#nomralement il ne va pas ici mais je n'ai pas trouvé d'autre endroit pour le moment


import serial
import serial.tools.list_ports
import threading

def init(port:str = None) :

    global ser

    if port:
        ser = serial.Serial()
        ser.baudrate = 115200
        ser.port = port
        return

    devList = serial.tools.list_ports.comports()

    if devList == []:
        raise Exception("No devices Found")

    MCUFound = 0

    for dev in devList:
        print("\n", dev.device, dev.pid)
        if dev.pid:
            print("ok")
            MCUFound = 1
            break
        else:
            print("Device not a microcontroler")
            continue

    if not MCUFound:
        raise Exception("No microcontroler found")

    ser = serial.Serial()
    ser.baudrate = 115200
    ser.port = dev.device

def send(data:str):
    th = threading.Thread(target=lambda: thread(data)).start()

def thread(data):
    ser.open()
    ser.write(data.encode())
    print(ser.readline()[:-2].decode())
    ser.send_break()
    ser.close()
    print("end")

if __name__ == "__main__":
    init("COM5")
    send("FDR")