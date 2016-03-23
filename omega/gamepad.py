#!/usr/bin/env python
import argparse
import sys

def ctl_init(dev):
    pipe = open(dev, 'r')
    ready  = False
    buf = []
    num_btns = 0
    num_axis = 0
    while not ready:
        buf = pipe.read(8)
        actions = "["
        actions += ' '.join('%02X' % ord(c) for c in buf)
        actions += "]"
        print actions
        if ord(buf[6]) == 0x81:
            num_btns += 1
        if ord(buf[6]) == 0x82:
            num_axis += 1
        if ord(buf[5]) == 0x80:
            ready = True
    print "Found gamepad with %d buttons and %d axis\n" % (num_btns, num_axis)

def main():
    #parser = argparse.ArgumentParser(description='', epilog='')
    #parser.add_argument('-d', '--device', help='Input device',
    #                    type=str, required=True)
    #args = vars(parser.parse_args())
    #gamepad_dev = args['device']
    gamepad_dev = '/dev/bus/usb/001/003'
    ctl_init(gamepad_dev)

if __name__ == "__main__":
    main()
