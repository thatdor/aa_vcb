/*
 * valve.h
 *
 *  Created on: Apr 15, 2024
 *      Author: dawei.zhou
 */

#ifndef PERIPHERAL_DRIVERS_VALVE_VALVE_H_
#define PERIPHERAL_DRIVERS_VALVE_VALVE_H_


#ifdef __cplusplus
extern "C" {
#endif


typedef enum enVALVE_E
{
    VALVE_NO,
    VALVE_MIX,
    VALVE_SOL_INP,
    VALVE_SAFE,
    VALVE_MAX
}VALVE_E;


void valve_set(VALVE_E valve, int en);
int8_t valve_get_fb(VALVE_E valve);


#ifdef __cplusplus
}
#endif

#endif /* PERIPHERAL_DRIVERS_VALVE_VALVE_H_ */
