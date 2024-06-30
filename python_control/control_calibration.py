import numpy as np
from pyPS4Controller.controller import Controller


class MyController(Controller):
    


    
    def __init__(self, **kwargs):
        Controller.__init__(self, **kwargs)
        self.max_x = 0
        self.max_y = 0
        self.max_z = 0
        self.max_rx = 0
        self.max_ry = 0
        self.max_rz = 0
    
    # read the left and right stick values and store the maximum value for each direction, and print them when x is pressed
    def on_x_press(self):

        print("X pressed")
        print("max_x: {}".format(self.max_x))
        print("max_y: {}".format(self.max_y))
        print("max_z: {}".format(self.max_z))
        print("max_rx: {}".format(self.max_rx))
        print("max_ry: {}".format(self.max_ry))
        print("max_rz: {}".format(self.max_rz))
        self.max_x = 0
        self.max_y = 0
        self.max_z = 0
        self.max_rx = 0
        self.max_ry = 0
        self.max_rz = 0

    
    def on_L3_up(self, value):
        if value > self.max_y:
            self.max_y = value
        #print("L3 up: {}".format(value))
    
    def on_L3_down(self, value):
        if value > self.max_y:
            self.max_y = value
        #print("L3 down: {}".format(value))

    def on_L3_left(self, value):
        if value > self.max_x:
            self.max_x = value
        #print("L3 left: {}".format(value))  
    
    def on_L3_right(self, value):    
        if value > self.max_x:
            self.max_x = value
        #print("L3 right: {}".format(value))
    
    def on_R3_up(self, value):   
        if value > self.max_ry:
            self.max_ry = value
        #print("R3 up: {}".format(value))
    
    def on_R3_down(self, value):    
        if value > self.max_ry:
            self.max_ry = value
        #print("R3 down: {}".format(value))
    
    def on_R3_left(self, value):    
        if value > self.max_rx:
            self.max_rx = value
        #print("R3 left: {}".format(value))
    
    def on_R3_right(self, value):  
        if value > self.max_rx:
            self.max_rx = value
        #print("R3 right: {}".format(value))
    
    def on_L3_x_at_rest(self):
        pass
        #print("L3 x at rest")
    
    def on_L3_y_at_rest(self):
        pass
        #print("L3 y at rest")
    
    def on_R3_x_at_rest(self):
        pass
        #print("R3 x at rest")
    
    def on_R3_y_at_rest(self):
        pass
        #print("R3 y at rest")

controller = MyController(interface="/dev/input/js0", connecting_using_ds4drv=False)
#controller.listen()

# Get the value from L3 and R3 and print it
controller.listen()