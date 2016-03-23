#!/usr/bin/env python
import socket
import subprocess
import sys

R2C2_COMM_MSGCTL = 0xEE
R2C2_COMM_ADDR = ''
R2C2_COMM_PORT = 2409
R2C2_COMM_BUFSIZE = 64

R2C2_FREQ_PWM = 128
R2C2_GPIO_PWM_ENGINE = 14
R2C2_GPIO_PIN_FWD =    13
R2C2_GPIO_PIN_RWD =    12

R2C2_GPIO_PIN_LEFT =  8
R2C2_GPIO_PIN_RIGHT = 7
R2C2_GPIO_PWM_STEER = 6

def _brain_init_sock(host, port):
    bsock =  socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        bsock.bind( (host, port) )
    except:
        logging.critical ("Failed to bind socket!")
        sys.exit(1)
    return bsock

def _brain_set_speed(speed):
    if speed < 0:
        speed_rel = (speed / 1024)*(-1)
        subprocess.call(["fast-gpio", "set", str(R2C2_GPIO_PIN_FWD), "0"])
        subprocess.call(["fast-gpio", "set", str(R2C2_GPIO_PIN_RWD), "1"])
    else:
        speed_rel = (speed / 1024)
        subprocess.call(["fast-gpio", "set", str(R2C2_GPIO_PIN_FWD), "1"])
        subprocess.call(["fast-gpio", "set", str(R2C2_GPIO_PIN_RWD), "0"])
    subprocess.call(["fast-gpio", "pwm", str(R2C2_GPIO_PWM_ENGINE), str(R2C2_FREQ_PWM), str(speed_rel)])

def _brain_set_steer(steer):
    steer_rel = (speed / 1024)
    if steer < 0:
        steer_rel = steer_rel*(-1)
        subprocess.call(["fast-gpio", "set", str(R2C2_GPIO_PIN_RIGHT), "0"])
        subprocess.call(["fast-gpio", "set", str(R2C2_GPIO_PIN_LEFT), "1"])
    else:
        subprocess.call(["fast-gpio", "set", str(R2C2_GPIO_PIN_RIGHT), "1"])
        subprocess.call(["fast-gpio", "set", str(R2C2_GPIO_PIN_LEFT), "0"])
    subprocess.call(["fast-gpio", "pwm", str(R2C2_GPIO_PWM_STEER), str(R2C2_FREQ_PWM), str(steer_rel)])

def _brain_init_gpios():
    """init gpios"""
    subprocess.call(["fast-gpio", " set-output", str(R2C2_GPIO_PIN_FWD)])
    subprocess.call(["fast-gpio", " set-output", str(R2C2_GPIO_PIN_RWD)])
    subprocess.call(["fast-gpio", " set-output", str(R2C2_GPIO_PWM_ENGINE)])
    subprocess.call(["fast-gpio", "set", str(R2C2_GPIO_PIN_FWD), "0"])
    subprocess.call(["fast-gpio", "set", str(R2C2_GPIO_PIN_RWD), "0"])
    subprocess.call(["fast-gpio", "pwm", str(R2C2_GPIO_PWM_ENGINE), str(R2C2_FREQ_PWM), "0"])
    subprocess.call(["fast-gpio", " set-output", str(R2C2_GPIO_PIN_LEFT)])
    subprocess.call(["fast-gpio", " set-output", str(R2C2_GPIO_PIN_RIGHT)])
    subprocess.call(["fast-gpio", " set-output", str(R2C2_GPIO_PWM_STEER)])
    subprocess.call(["fast-gpio", "set", str(R2C2_GPIO_PIN_LEFT), "0"])
    subprocess.call(["fast-gpio", "set", str(R2C2_GPIO_PIN_RIGHT), "0"])
    subprocess.call(["fast-gpio", "pwm", str(R2C2_GPIO_PWM_STEER), str(R2C2_FREQ_PWM), "0"])

def brain_loop(lhost, lport):
    _brain_init_gpios()
    bsock = _brain_init_sock(lhost, lport)
    while True:
        data, addr = bsock.recvfrom(R2C2_COMM_BUFSIZE)
        mtype, speed, steer, buttons = unpack('HIII', data)
        if mtype == R2C2_COMM_MSGCTL:
            _brain_set_speed(speed)
            _brain_set_steer(steer)

def main():
    """The main loop"""
    brain_loop(R2C2_COMM_ADDR, R2C2_COMM_PORT)

if __name__ == "__main__":
    main()
