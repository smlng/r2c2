/**
 * @ingroup    r2c2
 * @{
 *
 * @file
 * @brief       controll tool runs on linux
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
// addons
#include <fcntl.h>
#include <sys/stat.h>
#include <err.h>
#include <pthread.h>
// network
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
// own
#include "r2c2_config.h"

pthread_mutex_t msg_lock;
static volatile r2c2_msg_t r2c2_msg;

static int js;
static int sendfd = -1;

static int ctl_init(const char *dev)
{
    int fd;
    uint8_t buf[8];
    uint8_t num_btns = 0;
    uint8_t num_axes = 0;

    fd = open(dev, O_RDONLY);
    if (fd < 0) {
        return fd;
    }

    do {
        read(fd, buf, 8);
        printf("0:%#x 1:%#x 2:%#x 3:%#x 4:%#x 5:%#x 6:%#x 7:%#x\n",
                buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7]);
        if (buf[6] == 0x81) {
            ++num_btns;
        }
        if (buf[6] == 0x82) {
            ++num_axes;
        }
    } while (buf[5] != 0x80);

    printf("Found Joystick: %i buttons, %i axes\n", num_btns, num_axes);
    return fd;
}

static int r2c2_send(char *addr_str, char *port_str, char *data, size_t datalen)
{
    struct addrinfo hints, *res;
    int ret;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_DGRAM;

    if ((ret = getaddrinfo(addr_str, port_str, &hints, &res)) != 0) {
        puts("ERROR r2c2_send: getaddrinfo");
        return (-1);
    }

    if(sendfd < 0) {
        // loop through all the results and make a socket
        if ((sendfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol)) < 0) {
            puts("ERROR r2c2_send: create socket");
            return (-1);
        }
    }

    if ((ret=sendto(sendfd,data,datalen,0,res->ai_addr,res->ai_addrlen)) < 0) {
        printf("ERROR r2c2_send: sending data: %d\n", ret);
    }

    freeaddrinfo(res);

    return ret;
}

static void *ctl_read(void *arg)
{
    ssize_t n;
    uint8_t buf[8];
    int16_t steering = 0;
    int16_t engine = 0;
    uint16_t buttons = 0;
    int newdata = 1;

    printf("\n     steering    engine\n");
    while (1) {
        // output current controller values
        if (newdata) {
            newdata = 0;
            /* scale values to actual control values */
            pthread_mutex_lock(&msg_lock);
            r2c2_msg.speed = (engine / 32);
            r2c2_msg.steer = (steering / 32);
            r2c2_msg.buttons = buttons;
            pthread_mutex_unlock(&msg_lock);
            printf("raw:  %7i   %7i\n", steering, engine);
            puts("");
            printf("        speed  steering  buttons\n");
            printf("ctrl  %7i   %7i  %7i\n",
                    r2c2_msg.speed, r2c2_msg.steer, r2c2_msg.buttons);
            printf("\033[F\033[F\033[F\033[F");
        }
        // read new input from controller
        n = read(js, buf, 8);
        if (n == 8) {
            if (buf[6] == CONF_CTL_BUTTON) {
                if (buf[4]) {
                    //printf ("INFO ctl_read: pressed button (%#x)\n", buf[7]);
                    if ((buf[7] == CONF_CTL_BUTTON_L2) | (buf[7] == CONF_CTL_BUTTON_R2)) {
                        _set_bit(&buttons, buf[7]);
                    }
                    else {
                        _tgl_bit(&buttons, buf[7]);
                    }
                }
                else {
                    //printf ("INFO ctl_read: released button (%#x)\n", buf[7]);
                    if ((buf[7] == CONF_CTL_BUTTON_L2) | (buf[7] == CONF_CTL_BUTTON_R2)) {
                        _clr_bit(&buttons, buf[7]);
                    }
                }
                newdata = 1;
            }
            else if (buf[6] == CONF_CTL_AXIS) {
                if (buf[7] == CONF_CTL_AXIS_STEERING) {
                    steering = ((int16_t)buf[5] << 8) | buf[4];
                    newdata = 1;
                }
                else if (buf[7] == CONF_CTL_AXIS_ENGINE) {
                    engine = ((int16_t)buf[5] << 8) | buf[4];
                    newdata = 1;
                }
            }
            else {
                puts("WARN ctl_read: unknown control type!\n");
            }
        }
    }

    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t ctl_thread;
    int res;

    if (argc < 3) {
        printf("usage: %s <controller> <remoteIP>\n", argv[0]);
        return 1;
    }

    js = ctl_init(argv[1]);
    if (js < 0) {
        puts("error: unable to open joystick");
        return 1;
    }

    /* start the reader thread */
    res = pthread_create(&ctl_thread, NULL, ctl_read, NULL);
    if (res < 0) {
        puts("error: unable to start reader thread");
        return 1;
    }

    char ctl_data[CONF_COMM_MSGLEN];
    pthread_mutex_lock(&msg_lock);
    r2c2_msg.type = CONF_COMM_MSGCTL;
    r2c2_msg.speed = 0;
    r2c2_msg.steer = 0;
    r2c2_msg.buttons = 0;
    pthread_mutex_unlock(&msg_lock);
    while (1) {
        usleep(CONF_CTL_INTERVAL);
        pthread_mutex_lock(&msg_lock);
        ctl_data[0] = r2c2_msg.type;
        memcpy(&(ctl_data[1]), &r2c2_msg.speed, 2);
        memcpy(&(ctl_data[3]), &r2c2_msg.steer, 2);
        memcpy(&(ctl_data[5]), &r2c2_msg.buttons, 2);
        pthread_mutex_unlock(&msg_lock);
        r2c2_send(argv[2], CONF_COMM_PORT, ctl_data, CONF_COMM_MSGLEN);
    }
    close(sendfd);
    pthread_join(ctl_thread, NULL);
    close(js);
    return 0;
}
