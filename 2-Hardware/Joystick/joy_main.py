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
        self.joy_val = ""

        # initialization for serial
        self.ser = serial.Serial(
            port='/dev/ttyUSB0',
            baudrate=9600,
            timeout=1
        )
        self.ser.isOpen()
        self.done=False

    def JoystickRead(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                self.done = True

        self.joy_val = ""
        for i in range(self.axes):
            axis = int(round(self.joystick.get_axis(i), 2)*100) + 100
            axis_str = str(axis).zfill(3)
            self.joy_val = self.joy_val + axis_str

        #print("joy_val: ", self.joy_val)

    def SerialWriteAndRead(self):
            self.ser.write(self.joy_val.encode())
            # print(axis_str)
            received_data = self.ser.readline()
            print(received_data, '\n')


if __name__ == '__main__':
    joystick = Joystick()
    while (joystick.done != True):
        joystick.JoystickRead()
        joystick.SerialWriteAndRead()
        time.sleep(0.05)
            
