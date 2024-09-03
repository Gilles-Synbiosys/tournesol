import struct
import serial
import time

# Calculate checksum function
def calculate_checksum(data_bytes):
    return sum(data_bytes) % 256

# Configure the serial connection (adjust the port as needed)
ser = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=1)  # Replace with your port (e.g., COM3 on Windows)

# Wait for the connection to establish
time.sleep(2)

# Function to send a packet
def send_packet(floats, integer):
    if len(floats) != 6:
        raise ValueError("There must be exactly 6 float values.")

    # Pack the floats and integer into a binary format
    packet = struct.pack('<6fI', *floats, integer)
    
    # Calculate checksum
    checksum = calculate_checksum(packet)
    
    # Add checksum to the packet
    packet += struct.pack('B', checksum)
    
    # Send the packet over the serial connection
    ser.write(packet)

# Example usage
floats_to_send = [1.1, 2.2, 3.3, 4.4, 5.5, 6.6]
int_to_send = 42


# In the turret application the only 2 floats are necessary (2 coordinate or 2 velocities)
# maybe 4 (pos and vel simultaneously). But having 6 float can enable PID value change for
# both velocities and positions. The integer is used to select the mode to change
# For instance 0 for position, 1 for velocity, 2 for position and velocity, 3 for PID values,
# 4 for changing PID value

send_packet(floats_to_send, int_to_send)
print("Packet sent.")

# Close the serial connection
ser.close()