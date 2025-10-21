import serial

# Set up serial communication with Arduino
arduinoComPort = "COM4"
baudRate = 9600
serialPort = serial.Serial(arduinoComPort, baudRate, timeout=1)

# Initialize data structures
grid = []
row = []
invert_row = 0

# Continuously read data from Arduino
while True:
    lineStr = serialPort.readline().decode()
    print(lineStr)

    if lineStr:
        split_data = lineStr.split(" ")
        servo1 = int(split_data[0])
        servo2 = int(split_data[1])
        sensor_str = split_data[2].split("\r\n")
        sensor_data = int(sensor_str[0])
        line_data = int(sensor_data)

        # Organize data into rows based on servo1 position
        if servo1 == invert_row:
            if len(grid) % 2 == 0:
                grid.append(row)
            else:
                row.reverse()
                grid.append(row)
            row = []
        else:
            row.extend([line_data])

        invert_row = servo1
        print(grid)
