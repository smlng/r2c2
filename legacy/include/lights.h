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

void lights_init(void);
void lights_reset (void);
void flash_headlights (int);
void toggle_headlights_inner (int);
void toggle_headlights_outer (int);

#ifdef __cplusplus
}
#endif

#endif /* R2C2_LIGHTS_H_ */
/** @} */
