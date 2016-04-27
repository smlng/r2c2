/**
 * @defgroup    r2c2
 * @brief       Firmware for R2C2, the RIOT Radio Controlled Car
 * @{
 *
 * @file
 * @brief       Bootstraps the R2C2 firmware and run the shell
 *
 * @author      Hauke Petersen <mail@haukepetersen.de>
 * @author      smlng <s@mlng.net>
 *
 * @}
 */

// standard
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
// riot
#include "shell.h"
#include "board.h"
// own
#include "brain.h"
#include "watchdog.h"
#include "r2c2_config.h"

#define SHELL_BUFSIZE       (64U)

static int _speed(int argc, char **argv)
{
    int16_t speed;

    if (argc < 2) {
        printf("usage: %s SPEED [-1023 to 1023]\n", argv[0]);
        return 1;
    }

    speed = (int16_t)atoi(argv[1]);
    if (speed < -0x03ff || speed > 0x03ff) {
        puts("speed value out of range\n");
        return 1;
    }
    brain_set_speed(speed);
    return 0;
}

static int _steer(int argc, char **argv)
{
    int16_t dir;

    if (argc < 2) {
        printf("usage: %s DIR [-1023 to 1023]\n", argv[0]);
        return 1;
    }

    dir = (int16_t)atoi(argv[1]);
    printf("read steering value: %i\n", dir);
    if (dir < -0x03ff || dir > 0x03ff) {
        puts("dir value out of range\n");
        return 1;
    }
    brain_steer(dir);
    return 0;
}

static int _switch(int argc, char **argv)
{
    uint16_t button;
    if (argc < 2) {
        printf("usage: %s <button>\n", argv[0]);
        return 1;
    }
    button = (uint16_t)atoi(argv[1]);
    brain_buttons(button);
    return 0;
}

static int _debug(int argc, char **argv)
{
    int state;

    if (argc < 2) {
        printf("usage: %s <1|0>\n", argv[0]);
        return 1;
    }
    state = atoi(argv[1]);
    if (state) {
        wd_activate(0);
    }
    else {
        wd_activate(1);
    }
    return 0;
}

/**
 * @brief   Define some shell commands for testing the brain
 */
static const shell_command_t _commands[] = {
    { "speed", "set speed", _speed },
    { "steer", "set direction", _steer },
    { "switch", "trigger switches", _switch },
    { "debug", "enable debug mode", _debug },
    { NULL, NULL, NULL }
};

int main(void)
{
    LED0_OFF;

    /* define buffer to be used by the shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];

    /* initialize (and run) the brain */
    puts("initializing the brain");
    brain_init();

    /* run the shell for debugging purposes */
    puts("running the shell");
    shell_run(_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    /* should never be reached */
    return 0;
}
