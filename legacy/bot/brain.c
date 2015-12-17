/**
 * @ingroup     r2c2
 * @{
 *
 * @file
 * @brief       The brain controls everything
 *
 * @author      Hauke Petersen <mail@haukepetersen.de>
 * @author      smlng <s@mlng.net>
 *
 * @}
 */

// standard
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// network
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
// riot
#include "board.h"
#include "kernel.h"
#include "net/gnrc/netapi.h"
#include "net/gnrc/netif.h"
#include "periph/gpio.h"
#include "periph/pwm.h"
#include "servo.h"
#include "thread.h"
// own
#include "brain.h"
#include "lights.h"
#include "watchdog.h"
#include "r2c2_config.h"

#define STACKSIZE           (THREAD_STACKSIZE_MAIN)
#define STACKPRIO           (THREAD_PRIORITY_MAIN - 2)

static char stack[STACKSIZE];

#define BRAIN_QUEUE_SIZE     (8)
static msg_t _brain_msg_queue[BRAIN_QUEUE_SIZE];

static void _dispatch(uint8_t *data, size_t len)
{
    int16_t speed, dir;
    uint16_t buttons;

    if (data[0] == CONF_COMM_MSGCTL && len == CONF_COMM_MSGLEN) {
        memcpy(&speed, &(data[1]), 2);
        memcpy(&dir, &(data[3]), 2);
        memcpy(&buttons, &(data[5]), 2);
        brain_set_speed(speed);
        brain_steer(dir);
        brain_buttons(buttons);
        printf("speed %d, dir %d\n", speed, dir);
        wd_report();
    } else {
        printf("unknown data, %d bytes.\n", len);
    }
}

static void *_brain_thread(void *arg)
{
    (void) arg;

    /* A message que is mandatory for a thread that registers at netreg */
    msg_init_queue(_brain_msg_queue, BRAIN_QUEUE_SIZE);

    struct sockaddr_in6 server_addr;
    uint16_t port;
    uint8_t recvbuf[5*CONF_COMM_MSGLEN];
    /* parse port */
    port = (uint16_t)atoi(CONF_COMM_PORT);
    if (port == 0) {
        puts("ERROR receiver: invalid port");
        return NULL;
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    memset(&server_addr.sin6_addr, 0, sizeof(server_addr.sin6_addr));
    server_addr.sin6_port = htons(port);

    int s = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (s < 0) {
        puts("ERROR receiver: create socket");
        return NULL;
    }
    if (bind(s,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) {
        puts("ERROR receiver: bind socket");
        close(s);
        return NULL;
    }
    printf(". started UDP server on port %" PRIu16 "\n", port);
    while (1) {
        int res;
        struct sockaddr_in6 src;
        memset(&src, 0, sizeof(src));
        memset(recvbuf, 0, sizeof(recvbuf));
        socklen_t src_len = sizeof(src);
        // blocking receive, waiting for data
        if ((res = recvfrom(s, recvbuf, sizeof(recvbuf), 0,
                            (struct sockaddr *)&src, &src_len)) < 0) {
            puts("ERROR receiver: recvfrom");
        }
        else if (res == 0) {
            puts("WARN receiver: Peer did shut down");
        }
        else {
            _dispatch(recvbuf, res);
            printf(". received data, %d bytes.\n", res);
        }
    }
    close(s);
    return NULL;
}

void comm_init(void)
{
    kernel_pid_t ifs[GNRC_NETIF_NUMOF];
    uint16_t pan = CONF_COMM_PAN;
    uint16_t chan = CONF_COMM_CHAN;

    /* get the PID of the first radio */
    if (gnrc_netif_get(ifs) <= 0) {
        puts("comm: ERROR during init, not radio found!\n");
        return;
    }

    /* initialize the radio */
    gnrc_netapi_set(ifs[0], NETOPT_NID, 0, &pan, 2);
    gnrc_netapi_set(ifs[0], NETOPT_CHANNEL, 0, &chan, 2);
}

void brain_init(void)
{
    /* initialize steering control */
    puts("+ init steering");
    gpio_init(CONF_STEERING_DIRA, GPIO_DIR_OUT, GPIO_NOPULL);
    gpio_init(CONF_STEERING_DIRB, GPIO_DIR_OUT, GPIO_NOPULL);
    if (pwm_init(CONF_STEERING_PWM, CONF_STEERING_PWM_CHAN,
                 CONF_STEERING_FREQ, CONF_STEERING_RES) < 0) {
        puts("ERROR brain_init: init steering PWM\n");
        return;
    }
    pwm_set(CONF_STEERING_PWM, CONF_STEERING_PWM_CHAN, 0);

    /* initialize ENGINE control */
    puts("+ init engine");
    gpio_init(CONF_ENGINE_DIRA, GPIO_DIR_OUT, GPIO_NOPULL);
    gpio_init(CONF_ENGINE_DIRB, GPIO_DIR_OUT, GPIO_NOPULL);
    if (pwm_init(CONF_ENGINE_PWM, CONF_ENGINE_PWM_CHAN,
                 CONF_ENGINE_FREQ, CONF_ENGINE_RES) < 0) {
        puts("ERROR brain_init: init engine PWM\n");
        return;
    }
    pwm_set(CONF_ENGINE_PWM, CONF_ENGINE_PWM_CHAN, 0);
    puts("+ init lights");
    lights_init();
    /* initialize the software watchdog */
    puts("+ init watchdog");
    wd_init();
    /* initializing network support */
    puts("+ init comm");
    comm_init();
    /* run brain thread */
    puts("+ run the brain");
    thread_create(stack, STACKSIZE, STACKPRIO, THREAD_CREATE_STACKTEST, _brain_thread,
                  NULL, "brain");
}

void brain_set_speed(int16_t speed)
{
    if (speed > 0) {
        gpio_set(CONF_ENGINE_DIRA);
        gpio_clear(CONF_ENGINE_DIRB);
    } else {
        gpio_clear(CONF_ENGINE_DIRA);
        gpio_set(CONF_ENGINE_DIRB);
        speed *= -1;
    }
    pwm_set(CONF_ENGINE_PWM, CONF_ENGINE_PWM_CHAN, speed);
}

void brain_steer(int16_t dir)
{
    if (dir > 0) {
        gpio_set(CONF_STEERING_DIRA);
        gpio_clear(CONF_STEERING_DIRB);
    } else {
        gpio_clear(CONF_STEERING_DIRA);
        gpio_set(CONF_STEERING_DIRB);
        dir *= -1;
    }
    pwm_set(CONF_STEERING_PWM, CONF_STEERING_PWM_CHAN, dir);
}

void brain_buttons(uint16_t buttons)
{
    if (_chk_bit(&buttons, CONF_CTL_BUTTON_SQUARE)) {
        toggle_headlights_outer();
    }
    if(_chk_bit(&buttons, CONF_CTL_BUTTON_CIRCLE)) {
        toggle_headlights_inner();
    }
    if(_chk_bit(&buttons, CONF_CTL_BUTTON_R2)) {
        flash_headlights();
    }
}
