#!/usr/bin/env python
import socket
import json

# global defines
R2C2_COMM_MSGCTL = 0xEE
R2C2_COMM_ADDR = ''
R2C2_COMM_PORT = 2409
R2C2_COMM_BUFSIZE = 512

def network_init(host,port):
    print "CALL network_init\n"
    nsock =  socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        nsock.bind( (host, port) )
    except:
        logging.critical ("Failed to bind socket!")
        sys.exit(1)
    return nsock

def network_loop(host, port):
    print "CALL network_loop\n"
    nsock = network_init(host, port)
    while True:
        rawdata, addr = nsock.recvfrom(R2C2_COMM_BUFSIZE)
        print "> received message from " + addr + "\n"
        try:
            ctldata = json.loads(rawdata)
        except Exception, e:
            print "ERROR"
        else:
            pass

def main():
    """The main loop"""

if __name__ == "__main__":
    main()
