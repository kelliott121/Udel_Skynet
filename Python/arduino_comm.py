#!/usr/bin/python

import os
import sys
import serial
from serial.serialutil import SerialException

###
# Global variables
###
SER = None

###
# Functions
###
def init_serial():
    global SER
    try:
        SER = serial.Serial('/dev/ttyACM0', 9600)
        print "Connection successful \n"
    except SerialException, e:
        print "Connection failed \n"

def test_send():
    SER.flushOutput()
    SER.write("Hi little buddy!")

def get_selection():
    print "Select an option: "
    print "  1. Send serial to arduino"
    print "  2. Quit"
    return input("Enter your selection: ")

###
# Main
###        
def main():
    sel = 0
    init_serial()
    if SER:
        while sel != 2:
            sel = get_selection()
            print ""
            if sel == 1:
                test_send()

if __name__ == '__main__':
    main()  
