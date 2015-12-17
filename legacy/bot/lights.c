// riot
#include "board.h"
#include "periph/gpio.h"
#include "thread.h"
#include "xtimer.h"
// own
#include "r2c2_config.h"
#include "lights.h"

static int state_hli = 0; // state of inner headlights, 0=OFF, 1=ON
static int state_hlo = 0; // state of outer headlights, 0=OFF, 1=ON
static int state_flash = 0;

static char tlights_stack[THREAD_STACKSIZE_DEFAULT];

static void _toggle_light(int *state, int light)
{
    if (*state) {
        if (*state < 2) {
            gpio_set(light);
            *state = 2;
        }
    }
    else {
        gpio_clear(light);
    }
}

static void *_thread_lights(void *arg)
{
    (void)arg;
    gpio_init(CONF_LIGHTS_HLI, GPIO_DIR_OUT, GPIO_NOPULL);
    gpio_init(CONF_LIGHTS_HLO, GPIO_DIR_OUT, GPIO_NOPULL);
    lights_reset();
    while(1) {
        xtimer_usleep(CONF_LIGHTS_INTERVAL);
        if (state_flash) {
            gpio_set(CONF_LIGHTS_HLI);
            gpio_set(CONF_LIGHTS_HLO);
            --state_flash;
        }
        else {
            _toggle_light(&state_hli,CONF_LIGHTS_HLI);
            _toggle_light(&state_hlo,CONF_LIGHTS_HLO);
        }
    }
    return NULL;
}

void lights_init(void)
{
    /* start the thread */
    thread_create(tlights_stack, sizeof(tlights_stack), CONF_LIGHTS_PRIO,
                    THREAD_CREATE_STACKTEST, _thread_lights, NULL, "lights");
}

void lights_reset (void)
{
    gpio_clear(CONF_LIGHTS_HLI);
    gpio_clear(CONF_LIGHTS_HLO);
}

void flash_headlights (int state)
{
    if (state) {
        state_flash = CONF_LIGHTS_FLASH;
    }
}

void toggle_headlights_inner (int state)
{
    state_hli = state;
}

void toggle_headlights_outer (int state)
{
    state_hlo = state;
}
