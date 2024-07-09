/*
 * heater.h
 *
 *  Created on: Apr 15, 2024
 *      Author: dawei.zhou
 */

#ifndef PERIPHERAL_DRIVERS_TC_HEATER_H_
#define PERIPHERAL_DRIVERS_TC_HEATER_H_

#ifdef __cplusplus
extern "C" {
#endif

void heater_clockrun(int16_t target_temp, int16_t current_temp);

#ifdef __cplusplus
}
#endif

#endif /* PERIPHERAL_DRIVERS_TC_HEATER_H_ */
