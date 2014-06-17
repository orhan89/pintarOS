import sys
import serial

ser = serial.Serial('/dev/ttyUSB0')
ser.baudrate = 115200

def send():
    rawCLA = raw_input("CLA? ")
    rawINS = raw_input("INS? ")
    rawP1 = raw_input("P1? ")
    rawP2 = raw_input("P2? ")
    rawP3 = raw_input("P3? ")
    CLA = int(rawCLA,16)
    INS = int(rawINS,16)
    P1  = int(rawP1, 16)
    P2  = int(rawP2, 16)
    P3  = int(rawP3, 16)
    print CLA,INS,P1,P2,P3
    # CLA = '\x56'
    # INS = '\x52'
    # P1  = '\x53'
    # P2  = '\x54'
    # P3  = '\x55'
    ser.write(chr(CLA))
    ser.write(chr(INS))
    ser.write(chr(P1))
    ser.write(chr(P2))
    ser.write(chr(P3))

def send2():
    command = input("Command = ")
    print command
    ser.write(command[0])
    ser.write(command[1])
    ser.write(command[2])
    ser.write(command[3])
    ser.write(command[4])



