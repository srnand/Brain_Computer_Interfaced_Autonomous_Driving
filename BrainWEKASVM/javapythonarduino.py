import serial
import time
#Data = serial.Serial("COM3", 9600)
time.sleep(5)
fo = open("brainfile.txt", "r+")
position = fo.seek(0, 0)
str = fo.read(16)
print str
#Data.write(str)
fo.close()
