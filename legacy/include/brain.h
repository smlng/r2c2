/**
 * @ingroup     r2c2
 * @{
 *
 * @file
 * @brief       Interfaces of R2C2s brain
 *
 * @author      Hauke Petersen <mail@haukepetersen.de>
 * @author      smlng <s@mlng.net>
 */

#ifndef R2C2_BRAIN_H_
#define R2C2_BRAIN_H_

#ifdef __cplusplus
 extern "C" {
#endif

/**
 * @brief   Initialize Peta and start the brain's thread
 */
void brain_init(void);

void comm_init(void);

void brain_set_speed(int16_t speed);

void brain_steer(int16_t dir);

void brain_switches(uint8_t state);

#ifdef __cplusplus
}
#endif

#endif /* R2C2_BRAIN_H_*/
/** @} */
