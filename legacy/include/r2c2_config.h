/**
 * @ingroup     r2c2
 * @{
 *
 * @file
 * @brief       Global configuration options
 *
 * @author      Hauke Petersen <mail@haukepetersen.de>
 * @author      smlng <s@mlng.net>
 */

#ifndef R2C2_CONFIG_H_
#define R2C2_CONFIG_H_

#ifdef RIOT_VERSION
#include "periph/gpio.h"
#include "periph/pwm.h"
#endif // RIOT_VERION

#ifdef __cplusplus
 extern "C" {
#endif

/* bit manipulation helper */
static inline void _clr_bit(uint16_t *x, const int b)
{
    *x &= ~(1U << b);
}
static inline void _set_bit(uint16_t *x, const int b)
{
    *x |= (1U << b);
}
static inline void _tgl_bit(uint16_t *x, const int b)
{
    *x ^= (1U << b);
}
static inline int _chk_bit(uint16_t *x, const int b)
{
    return ((*x >> b) & 1);
}

typedef struct {
    uint8_t     type;
    int16_t     speed;
    int16_t     steer;
    uint16_t    buttons;
} r2c2_msg_t;

#ifdef RIOT_VERSION
/**
 * @brief   Configure the main engine
 * @{
 */
#define CONF_ENGINE_PWM          (PWM_0)
#define CONF_ENGINE_PWM_CHAN     (0U)
#define CONF_ENGINE_FREQ         (1000U)
#define CONF_ENGINE_RES          (0x03ff)    /* results in ~10MHz timer freq */
#define CONF_ENGINE_DIRA         GPIO_PIN(PA,13)
#define CONF_ENGINE_DIRB         GPIO_PIN(PA,28)
/** @} */

/**
 * @brief   Configure the steering ENGINE
 * @{
 */
#define CONF_STEERING_PWM          (PWM_0)
#define CONF_STEERING_PWM_CHAN     (1U)
#define CONF_STEERING_FREQ         (1000U)
#define CONF_STEERING_RES          (0x03ff)    /* results in ~10MHz timer freq */
#define CONF_STEERING_DIRA         GPIO_PIN(PA,22)
#define CONF_STEERING_DIRB         GPIO_PIN(PA,23)
/** @} */

/**
 * @brief   Configure EXTRAS
 * @{
 */
#define CONF_HORN_PWM           (PWM_1)
#define CONF_HORN_PWM_CHAN      (1U)
#define CONF_HORN_FREQ          (500)
#define CONF_HORN_RES           (255)
#define CONF_HORN_PWM_MAX       ((unsigned int)200)

#define CONF_DISTANCE_ECHO      GPIO_PIN(PA,14)
#define CONF_DISTANCE_TRIG      GPIO_PIN(PB,22)
/** @} */

/**
 * @brief   Configure LIGHTS
 * @{
 */
#define CONF_LIGHTS_PWM         (PWM_1)
#define CONF_LIGHTS_PWM_CHAN    (0U)
#define CONF_LIGHTS_FREQ        (500)
#define CONF_LIGHTS_RES         (255)    /* results in ~10MHz timer freq */
#define CONF_LIGHTS_PWM_MAX     ((unsigned int)255)

#define CONF_LIGHTS_PRIO        (THREAD_PRIORITY_MAIN - 2)
#define CONF_LIGHTS_INTERVAL    (29*1000) // 29ms
#define CONF_LIGHTS_HLO         GPIO_PIN(PB,03)
//#define CONF_LIGHTS_HLO         GPIO_PIN(PB,22)
#define CONF_LIGHTS_FLASH       (7) // x-times CONF_LIGHTS_INTERVAL
#define CONF_LIGHTS_ILEFT       GPIO_PIN(PB,02) // left indicator
#define CONF_LIGHTS_IRIGHT      GPIO_PIN(PB,23) // right indicator
#define CONF_LIGHTS_ICOUNT_INIT (14)
/** @} */

/**
 * @brief   Watchdog configuration
 * @{
 */
#define CONF_WD_PRIO            (THREAD_PRIORITY_MAIN - 4)
#define CONF_WD_INTERVAL        (200 * 1000)        /* 200ms */
#define CONF_WD_THRESSHOLD      (3U)                /* at least 3 packets before
                                                     * shutdown */
/** @} */
#endif // RIOT_VERION

/**
 * @brief   Configure communication
 * @{
 */
#define CONF_COMM_PAN           (0x23) // lowpan ID
#define CONF_COMM_CHAN          (16U)  // channel
#define CONF_COMM_PORT          "2409"
#define CONF_COMM_MSGCTL        (0xee)
#define CONF_COMM_MSGLEN        (sizeof(r2c2_msg_t))
/** @} */

/**
 * @brief   controller configuration
 * @{
 */
#define CONF_CTL_INTERVAL           (41 * 1000)     /* 41ms */

#define CONF_CTL_AXIS               (2U)

#define CONF_CTL_AXIS_LSTICK_H      (0)
#define CONF_CTL_AXIS_LSTICK_V      (1)
#define CONF_CTL_AXIS_RSTICK_H      (2)
#define CONF_CTL_AXIS_RSTICK_V      (3)
#define CONF_CTL_AXIS_CROSS_H       (4)
#define CONF_CTL_AXIS_CROSS_V       (5)

#define CONF_CTL_AXIS_ENGINE        (1)
#define CONF_CTL_AXIS_STEERING      (2)

#define CONF_CTL_BUTTON             (1U)

#define CONF_CTL_BUTTON_TRIANGLE    (0)
#define CONF_CTL_BUTTON_CIRCLE      (1)
#define CONF_CTL_BUTTON_CROSS       (2)
#define CONF_CTL_BUTTON_SQUARE      (3)

#define CONF_CTL_BUTTON_L1          (4)
#define CONF_CTL_BUTTON_L2          (6)
#define CONF_CTL_BUTTON_R1          (5)
#define CONF_CTL_BUTTON_R2          (7)

#define CONF_CTL_BUTTON_SELECT      (8)
#define CONF_CTL_BUTTON_START       (9)
#define CONF_CTL_BUTTON_ENGINE      (10)
#define CONF_CTL_BUTTON_STEERING    (11)

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* R2C2_CONFIG_H_ */
/** @} */
