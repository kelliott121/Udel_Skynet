#!/usr/bin/python

import os
import sys
import serial
from serial.serialutil import SerialException
from PyQt4 import QtGui
from PyQt4 import QtCore

###
# Global variables
###
DC1 = "\x11" #Forward
DC2 = "\x12" #Reverse
DC3 = "\x13" #Left Turn
DC4 = "\x14" #Right Turn
EOT = "\x04"

DELAY = 10

###
# Robot
###
class Robot(object):

    def __init__(self):
        self.serial = None
        try:
            self.serial = serial.Serial('/dev/ttyACM1', 9600)
            print "Serial connection successful"
        except SerialException, e:
            print "Serial connection failed"

    def send_cmd(self, cmd):
        if self.serial:
            self.serial.flushOutput()
            send_str = cmd + chr(DELAY) + EOT
            self.serial.write(send_str)
            print "Sending: " + send_str
        else:
            print "Can't send without serial connection"

    def forward(self):
        self.send_cmd(DC1)

    def reverse(self):
        self.send_cmd(DC2)

    def left_turn(self):
        self.send_cmd(DC3)

    def right_turn(self):
        self.send_cmd(DC4)

###
# MainWindow
###
class MainWindow(QtGui.QMainWindow):

    def __init__(self):
        super(MainWindow, self).__init__()
        self.robot = Robot()
        self.initUI()

    def initUI(self):
        self.setupLayout()
        self.resize(400, 100)
        self.center()
        self.setWindowTitle('Robot Control Center')
        self.show()

    def center(self):
        cp = QtGui.QDesktopWidget().availableGeometry().center()
        self.frameGeometry().moveCenter(cp)
        self.move(self.frameGeometry().topLeft())

    def setupLayout(self):        
        font = QtGui.QFont("Helvetica", 12)

        self.l1 = QtGui.QLabel("For how many tenths of a second? ")
        self.l1.setFont(font)

        self.c1 = QtGui.QComboBox()
        self.c1.setFont(font)
        for i in range(0,255):
            self.c1.insertItem(i, str(i+1))
        self.c1.setCurrentIndex(9)
        self.c1.currentIndexChanged.connect(self.update_delay)

        self.b1 = QtGui.QPushButton("Forward")
        self.b1.setFont(font)
        self.b1.clicked.connect(self.robot.forward)

        self.b2 = QtGui.QPushButton("Reverse")
        self.b2.setFont(font)
        self.b2.clicked.connect(self.robot.reverse)

        self.b3 = QtGui.QPushButton("Left Turn")
        self.b3.setFont(font)
        self.b3.clicked.connect(self.robot.left_turn)

        self.b4 = QtGui.QPushButton("Right Turn")
        self.b4.setFont(font)
        self.b4.clicked.connect(self.robot.right_turn)

        grid = QtGui.QGridLayout()
        grid.addWidget(self.b1, 0, 0)
        grid.addWidget(self.b2, 0, 1)
        grid.addWidget(self.b3, 0, 2)
        grid.addWidget(self.b4, 0, 3)
        grid.addWidget(self.l1, 1, 0, 1, 3)
        grid.addWidget(self.c1, 1, 3)
        
        main_widget = QtGui.QWidget()
        main_widget.setLayout(grid)
        
        self.setCentralWidget(main_widget)

    def update_delay(self):
        global DELAY
        DELAY = self.c1.currentIndex() + 1

###
# Main
###        
def main():
    app = QtGui.QApplication(sys.argv)
    ex = MainWindow()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()  
