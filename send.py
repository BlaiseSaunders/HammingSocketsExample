#!/usr/bin/python

import socket
import sys
import subprocess

from PyQt4.QtCore import pyqtSlot
from PyQt4.QtGui import *

# Configuration settings
UDP_IP = "127.0.0.1"
UDP_PORT = 6969

# Simple to binary function
get_bin = lambda x: format(x, 'b')

# Create the socket we'll be sending data through
# and make it datagram and IP
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Qt4 Setup
a = QApplication(sys.argv)
w = QMainWindow()
w.resize(320, 150)
w.setWindowTitle("Hamming Sender")

# Setup the widgets we'll be working with in Qt
textbox = QLineEdit(w)
textbox.move(20, 20)
textbox.resize(280,40)

button = QPushButton('Click me', w)
button.move(20,80)


# Function to call when the button is clicked, works with the users input
@pyqtSlot()
def on_click():
	text = textbox.text()
	if not text.isalnum(): # Validate the input
		textbox.setText("Alphanumeric only please!")
		return

	for c in text:
		# Simple conversion to binary
		c = ord(c)
		print(c)
		bin = get_bin(c)
		print(bin) # Some debugging output

		# User our previous hamming program to convert each char
		# for transmission over the network
		ham_encoded = subprocess.Popen("./hamming_tool -g %s " % bin, shell=True,
		                               stdout=subprocess.PIPE).stdout.read().rstrip()

		print(ham_encoded)

		# Send it off through the UDP socket
		sock.sendto((ham_encoded) , (UDP_IP, UDP_PORT))

	# Finally send NULL to notify the server we've finished
	# sending this chunk
	sock.sendto(('\0'.encode()) , (UDP_IP, UDP_PORT))

	textbox.setText("Sent ;)") # Let the user know the good news


# Register the callback
button.clicked.connect(on_click)

# Run the Qt GUI
w.show()
sys.exit(a.exec_()) # Return the exit status of Qt
