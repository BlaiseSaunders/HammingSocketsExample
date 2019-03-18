# Overview:
	Simple C/Python program for sending hamming code encoded chars
	through UDP


[vidya](https://raw.githubusercontent.com/DavidoRotho/HammingSocketsExample/master/example.gif)](demo vid)
	
	
# Files:
	get.py - Python UDP Server
	send.py - Qt4 Python Sender Tool
	hamming.c - Source for `hamming_tool`
	Makefile - What it says on the box

# How to run:
	## Requirements:
		Python 3
		A C compiler (gcc is recommended (STILL FASTEST))
		Qt4
		Python-Qt4
		Make (optional)
	## Tested on:
		Arch GNU/Linux
			Linux mugi 4.9.6-1-ARCH #1 SMP PREEMPT Thu Jan 26 09:22:26 CET 2017 x86_64 GNU/Linux


	## Quick Start:
		If you can get to a bourne style shell
		`make && ./get.py & ./send.py > /dev/null 2>&1 && killall get.py`

	## Getting started:
		First, compile the hamming tool with either your C compiler or
		the provided makefile

		Then, run `get.py` which is the server, this will listen and output
		anything it recieves on the defined port and attmept to decode
		anything it recieves as 7,4 hamming code

		Then, run `send.py` which is the client, upon hitting the `click me`
		button, any data in the text box will be encoded with 7,4 hamming
		and sent to the server



	## Note:
		All the hamming work is done by the C `hamming_tool`, the python will
		fail without it

		If a hamming check fails, `hamming_tool` will output to STDERR which
		is piped to STDOUT by `get.py`


