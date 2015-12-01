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
#define CONF_CTL_INTERVAL       (20 * 1000)     /* 20ms */
#define CONF_CTL_BUTTON_TYPE    (1U)
#define CONF_CTL_AXIS_TYPE      (2U)

#define CONF_CTL_AXIS_STEERING  (3)
#define CONF_CTL_AXIS_FW        (2)
#define CONF_CTL_AXIS_BW        (5)
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* R2C2_CONFIG_H_ */
/** @} */
