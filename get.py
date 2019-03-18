#!/usr/bin/python

import socket
import sys
import subprocess

# Network settings
UDP_IP = "127.0.0.1"
UDP_PORT = 6969

# Create a socket for recieving data from
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Bind to the socket
sock.bind((UDP_IP, UDP_PORT))


# String we'll be storing input in, ready to output
out = ""

while True:
	# Blocking call that waits to recieve data from client
	data, addr = sock.recvfrom(1024)
	# Check for the end of transmission NULL byte
	# (should probs just use EOT lmao but oh well)
	if b'\x00' in data:
		# Print out what they have to say and reset
		print("End transmission\n")
		print("User said:")
		print(out)
		out = ""
		continue

	# Otherwise, prep the data for decoding
	string = data.decode()
	print("gotem: " + string) # Debugging output

	# Decode with C hamming program
	tmp = ""
	ham_decoded = subprocess.Popen("./hamming_tool -c %s " % string, shell=True,
	                               stdout=subprocess.PIPE,
	                               stderr=sys.stdout
	                               ).stdout.read().rstrip()

	# Convert output to UTF-8 for further working with
	ham_decoded = ham_decoded.decode('utf-8')


	# Decode bytes to char
	print("hamd: " + ham_decoded)
	num = int(ham_decoded, 2)
	char = chr(num)
	print("char: " + char)
	# Add char to string for printing out at the end of transmission
	out += char

