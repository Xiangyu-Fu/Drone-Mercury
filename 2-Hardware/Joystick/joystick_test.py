import pygame
import time

pygame.init()
pygame.joystick.init()
done=False

while (done != True):
    for event in pygame.event.get():  # User did something
        if event.type == pygame.QUIT:  # If user clicked close
            done = True  # Flag that we are done so we exit this loop
    joystick_count = pygame.joystick.get_count()
    for i in range(joystick_count):
        joystick = pygame.joystick.Joystick(i)
        joystick.init()
        axes = joystick.get_numaxes()
        print('================')
        time.sleep(0.1)
        for i in range(axes):
            axis = joystick.get_axis(i)
            print(axis)
            
