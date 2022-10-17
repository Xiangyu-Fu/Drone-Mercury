import serial 
import time

ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=9600,
    parity=serial.PARITY_ODD,
    stopbits=serial.STOPBITS_TWO,
    bytesize=serial.SEVENBITS
)

ser.isOpen()

# read a string
while 1:
    trans_data = "Hello World"
    ser.write(trans_data.encode('utf-8')) # write a string
    received_data = ser.readline().decode() # read a byte

    print(received_data)

