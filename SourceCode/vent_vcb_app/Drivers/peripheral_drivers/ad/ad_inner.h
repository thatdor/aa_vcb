/*
 * ad_inner.h
 *
 *  Created on: Apr 17, 2024
 *      Author: dawei.zhou
 */

#ifndef PERIPHERAL_DRIVERS_AD_AD_INNER_H_
#define PERIPHERAL_DRIVERS_AD_AD_INNER_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum enAD_INNER_CH
{
    AD_INNER_CH_12V_VAL,
    AD_INNER_CH_7V_VAL,
    AD_INNER_CH_3V3_VAL,
    AD_INNER_CH_5V_VAL,
    AD_INNER_CH_MAX
}AD_INNER_CH_E;

#ifdef __cplusplus
}
#endif

#endif /* PERIPHERAL_DRIVERS_AD_AD_INNER_H_ */
