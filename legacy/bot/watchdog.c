/**
 * @ingroup     r2c2
 * @{
 *
 * @file
 * @brief       High priority task that makes sure everything toes smoothly
 *
 * @author      Hauke Petersen <mail@haukepetersen.de>
 * @author      smlng <s@mlng.net>
 *
 * @}
 */

// standard
#include <stdint.h>
#include <stdio.h>
// riot
#include "board.h"
#include "thread.h"
#include "xtimer.h"
#include "periph/pwm.h"
//own
#include "watchdog.h"
#include "brain.h"
#include "r2c2_config.h"

static char wd_stack[THREAD_STACKSIZE_DEFAULT];

static int active = 1;
static volatile int pkt_count = 0;

static void _shutdown(void)
{
    brain_set_speed(0);
    /* HACK */
    pwm_set(CONF_ENGINE_PWM, CONF_ENGINE_PWM_CHAN, 0);
}

static void _be_happy(void)
{
    /* HACK */
    pwm_set(CONF_ENGINE_PWM, CONF_ENGINE_PWM_CHAN, 10000);
}

static void *_thread(void *arg)
{
    (void)arg;

    while (1) {
        xtimer_usleep(CONF_WD_INTERVAL);
        if (active && pkt_count < CONF_WD_THRESSHOLD) {
            _shutdown();
        }
        else {
            _be_happy();
        }
        pkt_count = 0;
    }

    /* never reached */
    return NULL;
}

void wd_init(void)
{
    /* start the thread */
    thread_create(wd_stack, sizeof(wd_stack), CONF_WD_PRIO, CREATE_STACKTEST,
                  _thread, NULL, "wd");
}

void wd_report(void)
{
    ++pkt_count;
}

void wd_activate(int state)
{
    active = state;
}
