import serial
import numpy as np
import traceback

BAUD_RATE = 460800          # UART baud rate

# Commands
PIXELS = b'p'
DIFF = b'd'
GRAY = b'g'
EDGE = b'e'
SCCB_WRITE = b'w'
SCCB_READ = b'r'
COMMANDS = (PIXELS, DIFF, GRAY, EDGE)
COMMANDS_GRAY = (GRAY, EDGE)

# OV7670 registers
BRIGHTNESS_ADDR = 0x55
CONTRAS_ADDR = 0x56

# Image size
QQVGA_WIDTH = 160
QQVGA_HEIGHT = 120

NUM_SAMPLES = {"q": QQVGA_WIDTH * QQVGA_HEIGHT * 2,
               "m": 128 * 128 * 2,
               "s": 32 * 32 * 2}

SHAPE = {"q": (QQVGA_HEIGHT, QQVGA_WIDTH, 3),
         "m": (128, 128, 3),
         "s": (32, 32, 3)}

def yuv2rgb(y, u, v):

    def clip(x):
        y = x
        if x > 255:
            y = 255
        elif x < 0:
            y = 0
        return y

    c = y - 16
    d = u - 128
    e = v - 128
    r = clip(( 298 * c + 409 * e + 128 ) >> 8)
    g = clip(( 298 * c - 100 * d - 208 * e + 128 >> 8))
    b = clip(( 298 * c + 516 *d + 128) >> 8)
    
    return (r, g, b)

class Interface:
    
    def __init__(self, port, size):
        # Serial interface
        self.port = port
        self.size = size
        try:
            self.ser = serial.Serial(self.port, BAUD_RATE, timeout=3)
        except:
            print('*** Serial connection failure!')
            traceback.print_exc()

    def close(self):
        if self.ser:
            self.ser.close()
                
    # As an application processor, send a command
    # then receive and process the output.
    def read(self, cmd):

        data = []
        try:
            self.ser.write(cmd+b'\n')
            print(cmd)
            if cmd in COMMANDS_GRAY:
                rx = self.ser.read(int(NUM_SAMPLES[self.size]/2))
                print(int(NUM_SAMPLES[self.size]/2))
                for lsb in rx:
                    d =  int.from_bytes([lsb], byteorder='big', signed=False)
                    data.append(d << 2)
                data = np.array(data, dtype=np.int).reshape(SHAPE[self.size][0], SHAPE[self.size][1])
            else:
                rx = self.ser.read(NUM_SAMPLES[self.size])
                print(NUM_SAMPLES[self.size])
                #print(rx)
                rx = zip(rx[0::4], rx[1::4], rx[2::4], rx[3::4])
                for y0, v, y1, u in rx:
                    data.append(yuv2rgb(y0, u, v))
                    data.append(yuv2rgb(y1, u, v))
                data = np.array(data, dtype=np.int).reshape(*SHAPE[self.size])
            print(data.shape)
        except:
            traceback.print_exc()

        return data
    
    def sccb_write(self, reg_addr, reg_value):
        reg_addr = str(int(reg_addr, 16))
        try:
            value = int(reg_addr) * 256 + int(reg_value)
            cmd_line = SCCB_WRITE + bytes(str(value), encoding='ascii') + b'\n'
            self.ser.write(cmd_line)
            print(cmd_line)
        except:
            traceback.print_exc()

    def sccb_read(self, reg_addr):
        reg_addr = str(int(reg_addr, 16))
        reg_value = 0
        addr = bytes(reg_addr, encoding='ascii')
        try:
            self.ser.write(SCCB_READ + addr + b'\n')
            print(SCCB_READ + addr + b'\n')
            rx = self.ser.read(1)
            reg_value = int(rx[0])
            print("{0:08b}".format(reg_value))
        except:
            traceback.print_exc()
        return reg_value