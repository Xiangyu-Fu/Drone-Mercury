import pygame
import time
import serial

class Joystick:
    def __init__(self):
        # initialization for joystick
        pygame.init()
        pygame.joystick.init()
        self.joystick_count = pygame.joystick.get_count()
        self.joystick = pygame.joystick.Joystick(0)
        self.joystick.init()
        self.axes = self.joystick.get_numaxes()

        # initialization for serial
        self.ser = serial.Serial(
            port='/dev/ttyUSB0',
            baudrate=9600,
            parity=serial.PARITY_ODD,
            stopbits=serial.STOPBITS_TWO,
            bytesize=serial.SEVENBITS
        )
        self.ser.isOpen()
        self.done=False

    def JoystickRead(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                self.done = True

        for i in range(self.axes):
            axis = int(round(self.joystick.get_axis(i), 2)*100) + 100
            axis_str = str(axis).zfill(3)
            self.ser.write(axis_str.encode('utf-8'))
            print(axis_str)
            received_data = self.ser.readline().decode() # read a byte
        
        print(received_data)    

if __name__ == '__main__':
    joystick = Joystick()
    while (joystick.done != True):
        joystick.JoystickRead()
        time.sleep(1)
            
