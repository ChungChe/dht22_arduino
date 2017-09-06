import socket
from time import strftime
import time
import sys

UDP_IP = "192.168.0.101"
UDP_PORT = 15000

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

while True:
    data, addr = sock.recvfrom(1024)
    curr_time = strftime("%Y/%m/%d %H:%M:%S")
    #print("Received from {}, message: {}".format(addr, data))
    toks = data.split()
    temp = toks[0]
    humd = toks[1]
    gas = toks[2]
    print("{} {} {} {}".format(curr_time, temp, humd, gas))
    sys.stdout.flush()
