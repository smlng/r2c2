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

#ifdef RIOT_VERSION
/**
 * @brief   Configure the steering ENGINE
 * @{
 */
#define CONF_STEERING_PWM          (PWM_1)
#define CONF_STEERING_PWM_CHAN     (0U)
#define CONF_STEERING_FREQ         (1000U)
#define CONF_STEERING_RES          (0x03ff)    /* results in ~10MHz timer freq */
#define CONF_STEERING_DIRA         GPIO_PIN(PA,22)
#define CONF_STEERING_DIRB         GPIO_PIN(PA,23)
/** @} */

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
#define CONF_COMM_PAN           (0x23)
#define CONF_COMM_CHAN          (16U)
#define CONF_COMM_PORT          "2409"
#define CONF_COMM_MSGCTL        (0xee)
#define CONF_COMM_MSGLEN        (6U)
/** @} */

/**
 * @brief   controller configuration
 * @{
 */
#define CONF_CTL_INTERVAL           (20 * 1000)     /* 20ms */

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
