import serial 

ser = serial.Serial('/dev/ttyUSB0')
print(ser.name)         # check which port was really used

# read a string
while 1:
    print(ser.readline())
