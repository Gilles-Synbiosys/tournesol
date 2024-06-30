import serial
import time

ser = serial.Serial('/dev/ttyACM0', 115200)

while True:
    # write for 5 seconds "vx1.0;vy1.0;" and for 5 seconds "vx-1.0;vy-1.0;"
    ser.write(b'vx1.0;vy1.0;')
    print("vx1.0;vy1.0;")
    time.sleep(2)
    ser.write(b'vx-1.0;vy-1.0;')
    print("vx-1.0;vy-1.0;")
    time.sleep(2)

