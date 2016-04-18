#!/usr/bin/env python

import omegaControl
import omegaNetwork

def main():
    """The main loop"""
    _brain_init_gpios()
    #brain_loop(R2C2_COMM_ADDR, R2C2_COMM_PORT)
    brain_demo()

if __name__ == "__main__":
    main()
