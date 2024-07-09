/*
 * dac.h
 *
 *  Created on: Apr 15, 2024
 *      Author: dawei.zhou
 */

#ifndef PERIPHERAL_DRIVERS_DAC_DAC_H_
#define PERIPHERAL_DRIVERS_DAC_DAC_H_

#ifdef __cplusplus
extern "C" {
#endif

int dac_o2_set(uint16_t dac);
int dac_air_set(uint16_t dac);
int dac_vcm_set(uint16_t dac);

#ifdef __cplusplus
}
#endif

#endif /* PERIPHERAL_DRIVERS_DAC_DAC_H_ */
