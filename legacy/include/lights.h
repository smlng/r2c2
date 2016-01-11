/**
 * @ingroup     r2c2
 * @{
 *
 * @file
 * @brief       Lights configuration options
 *
 * @author      smlng <s@mlng.net>
 */

#ifndef R2C2_LIGHTS_H_
#define R2C2_LIGHTS_H_

#ifdef __cplusplus
extern "C" {
#endif

void check_lights(uint16_t state);
kernel_pid_t lights_init(void);
void lights_reset (void);

#ifdef __cplusplus
}
#endif

#endif /* R2C2_LIGHTS_H_ */
/** @} */
