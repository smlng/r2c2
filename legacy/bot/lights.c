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
static int count_indicator = CONF_LIGHTS_ICOUNT_INIT;
static int state_indicator_left = 0;
static int state_indicator_right = 0;

static char tlights_stack[THREAD_STACKSIZE_DEFAULT];

/* private stuff */
void check_lights(uint16_t state)
{
    puts("CALL check_lights");
    state_hlo = _chk_bit(&state, CONF_CTL_BUTTON_SQUARE);
    state_hli = _chk_bit(&state, CONF_CTL_BUTTON_CIRCLE);
    if (_chk_bit(&state, CONF_CTL_BUTTON_R2)) {
        state_flash = CONF_LIGHTS_FLASH;
    }
    state_indicator_left  = _chk_bit(&state, CONF_CTL_BUTTON_L1|CONF_CTL_BUTTON_TRIANGLE);
    state_indicator_right = _chk_bit(&state, CONF_CTL_BUTTON_R1|CONF_CTL_BUTTON_TRIANGLE);
}

static void _toggle_light(int *state, int light)
{
    if (*state) {
        if (!gpio_read(light)) {
            gpio_set(light);
        }
    }
    else {
        gpio_clear(light);
    }
}

static void _toggle_light_pwm(int *state)
{
    if (*state) {
        if (*state < 2) {
            pwm_set(CONF_LIGHTS_PWM, CONF_LIGHTS_PWM_CHAN, CONF_LIGHTS_PWM_MAX);
            *state = 2;
        }
    }
    else {
        pwm_set(CONF_LIGHTS_PWM, CONF_LIGHTS_PWM_CHAN, 0);
    }
}

static void *_thread_lights(void *arg)
{
    puts("CALL _thread_lights");
    (void)arg;
    while(1) {
        count_indicator--;
        xtimer_usleep(CONF_LIGHTS_INTERVAL);
        if (state_flash) {
            gpio_set(CONF_LIGHTS_HLO);
            //gpio_set(CONF_LIGHTS_HLO);
            pwm_set(CONF_LIGHTS_PWM, CONF_LIGHTS_PWM_CHAN, CONF_LIGHTS_PWM_MAX);
            --state_flash;
        }
        else {
            _toggle_light(&state_hlo,CONF_LIGHTS_HLO);
            _toggle_light_pwm(&state_hli);
        }
        if (!count_indicator) {
            count_indicator = CONF_LIGHTS_ICOUNT_INIT;
            state_indicator_left  ? gpio_toggle(CONF_LIGHTS_ILEFT)  : gpio_clear(CONF_LIGHTS_ILEFT);
            state_indicator_right ? gpio_toggle(CONF_LIGHTS_IRIGHT) : gpio_clear(CONF_LIGHTS_IRIGHT);
        }
    }
    return NULL;
}

/* public functions */
kernel_pid_t lights_init(void)
{
    puts("CALL lights_init");
    gpio_init(CONF_LIGHTS_HLO,    GPIO_OUT);
    gpio_init(CONF_LIGHTS_ILEFT,  GPIO_OUT);
    gpio_init(CONF_LIGHTS_IRIGHT, GPIO_OUT);
    //gpio_init(CONF_LIGHTS_HLO, GPIO_DIR_OUT, GPIO_NOPULL);
    if (pwm_init(CONF_LIGHTS_PWM, CONF_LIGHTS_PWM_CHAN,
                 CONF_LIGHTS_FREQ, CONF_LIGHTS_RES) < 0) {
        puts("ERROR lights_init: init lights PWM\n");
        return -1;
    }

    lights_reset();
    /* start the thread */
    kernel_pid_t pid = thread_create(   tlights_stack, sizeof(tlights_stack),
                            CONF_LIGHTS_PRIO, THREAD_CREATE_STACKTEST,
                            _thread_lights, NULL, "lights");
    return pid;
}

void lights_reset (void)
{
    puts("CALL lights_reset");
    gpio_clear(CONF_LIGHTS_HLO);
    gpio_clear(CONF_LIGHTS_ILEFT);
    gpio_clear(CONF_LIGHTS_IRIGHT);
    //pwm_set(CONF_LIGHTS_PWM, CONF_LIGHTS_PWM_CHAN, 0);
    pwm_set(CONF_LIGHTS_PWM, CONF_LIGHTS_PWM_CHAN, CONF_LIGHTS_PWM_MAX);
}
