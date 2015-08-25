#! /usr/bin/env python

import socket

UDP_IP = "127.0.0.1"
UDP_PORT = 12344
TEST_MESSAGE = "HELLO WORLD"

print "Setting up socket..."
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

print "Sending message"
sock.sendto(TEST_MESSAGE, (UDP_IP, UDP_PORT))

print "DONE"
