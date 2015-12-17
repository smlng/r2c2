// standard
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

static int js;
static int num_btns = 0;
static int num_axes = 0;

static int ctl_init(const char *dev)
{
    int fd;
    uint8_t buf[8];

    fd = open(dev, O_RDONLY);
    if (fd < 0) {
        return fd;
    }
    read(fd, buf, 8);
    while (buf[5] != 0x80) {
        printf("0:%#x 1:%#x 2:%#x 3:%#x 4:%#x 5:%#x 6:%#x 7:%#x\n",
                buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7]);
        if (buf[6] == 0x81) {
            ++num_btns;
        }
        if (buf[6] == 0x82) {
            ++num_axes;
        }
        read(fd, buf, 8);
    }

    printf("Found Joystick: %i buttons, %i axes\n", buttons, axes);
    return fd;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("usage: %s <controller> \n", argv[0]);
        return 1;
    }

    js = ctl_init(argv[1]);
    if (js < 0) {
        puts("error: unable to open joystick");
        return 1;
    }
    uint8_t buf[8];
    size_t n;
    while(1) {
        n = read(js,buf,8);
        printf("0:%#x 1:%#x 2:%#x 3:%#x 4:%#x 5:%#x 6:%#x 7:%#x\n",
                buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7]);
    }
    return 0;
}
