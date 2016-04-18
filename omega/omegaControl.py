#!/usr/bin/env python
from OnionGpio import *
import time

# global defines
R2C2_FREQ_PWM = 128
R2C2_GPIO_PWM_ENGINE = 14
R2C2_GPIO_PIN_FWD =    13
R2C2_GPIO_PIN_RWD =    12

R2C2_GPIO_PIN_LEFT =   8
R2C2_GPIO_PIN_RIGHT =  7
R2C2_GPIO_PWM_STEER =  6

MILLISECOND=1.0/1000.0
def control_init():
    gpios = dict()
    # init
    gpios['engine'] = OnionGpio(R2C2_GPIO_PWM_ENGINE)
    gpios['forward'] = OnionGpio(R2C2_GPIO_PIN_FWD)
    gpios['backward'] = OnionGpio(R2C2_GPIO_PIN_RWD)
    gpios['steer'] = OnionGpio(R2C2_GPIO_PWM_STEER)
    gpios['right'] = OnionGpio(R2C2_GPIO_PIN_RIGHT)
    gpios['left'] = OnionGpio(R2C2_GPIO_PIN_LEFT)
    # set to output
    gpios['engine'].setOutputDirection()
    gpios['forward'].setOutputDirection()
    gpios['backward'].setOutputDirection()
    gpios['steer'].setOutputDirection()
    gpios['right'].setOutputDirection()
    gpios['left'].setOutputDirection()
    # set all to zero
    gpios['engine'].setValue(0)
    gpios['forward'].setValue(0)
    gpios['backward'].setValue(0)
    gpios['steer'].setValue(0)
    gpios['right'].setValue(0)
    gpios['left'].setValue(0)
    pass

def control_loop(gpio_list):
    while True:
        time.sleep(MILLISECOND)

def main():
    """The main loop"""

if __name__ == "__main__":
    main()
