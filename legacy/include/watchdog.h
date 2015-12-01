/**
 * @ingroup     r2c2
 * @{
 *
 * @file
 * @brief       Watchdog interface
 *
 * @author      Hauke Petersen <mail@haukepetersen.de>
 * @author      smlng <s@mlng.net>
 */

#ifndef R2C2_WD_H_
#define R2C2_WD_H_

#ifdef __cplusplus
 extern "C" {
#endif

/**
 * @brief   Initialize Peta and start the brain's thread
 */
void wd_init(void);

void wd_report(void);

void wd_activate(int state);

#ifdef __cplusplus
}
#endif

#endif /* WD_H_*/
/** @} */
