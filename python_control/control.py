import numpy as np
import time
from pyPS4Controller.controller import Controller
import serial

class MyController(Controller):
    def __init__(self, **kwargs):
        Controller.__init__(self, **kwargs)
        self.ser = serial.Serial('/dev/ttyACM0', 115200)
        self.vx = 0
        self.vy = 0
        self.scale = 32767/10.0
        self.threshold = 0.5

    def send_serial(self):
        self.ser.write("vx{:.2f};vy{:.2f};".format(self.vx, self.vy).encode())
        print("vx{:.2f};vy{:.2f};".format(self.vx, self.vy))

    def on_L3_up(self, value):
        self.vy = value/self.scale
        if abs(self.vy) < self.threshold:
            self.vy = 0.0
        self.send_serial()
    
    def on_L3_down(self, value):
        self.vy = value/self.scale
        if abs(self.vy) < self.threshold:
            self.vy = 0.0
        self.send_serial()
    
    def on_L3_left(self, value):
        self.vx = value/self.scale
        if abs(self.vx) < self.threshold:
            self.vx = 0.0
        self.send_serial()
    
    def on_L3_right(self, value):
        self.vx = value/self.scale
        if abs(self.vx) < self.threshold:
            self.vx = 0.0
        self.send_serial()
    
    def on_R2_press(self, value):
        self.scale = 32767/1.0
        self.threshold = 0.05

    def on_R2_release(self):
        self.scale = 32767/10.0
        self.threshold = 0.5


controller = MyController(interface="/dev/input/js0", connecting_using_ds4drv=False)
#controller.listen()

# Get the value from L3 and R3 and print it
controller.listen()
    