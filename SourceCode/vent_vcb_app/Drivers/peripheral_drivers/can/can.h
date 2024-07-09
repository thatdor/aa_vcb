/*
 * can1.h
 *
 *  Created on: Nov 20, 2023
 *      Author: weizhuang.kong
 */

#ifndef PERIPHERAL_DRIVERS_CAN_CAN_H_
#define PERIPHERAL_DRIVERS_CAN_CAN_H_
#ifdef __cplusplus
extern "C" {
#endif
#define BSP_CAN_OK 1


void can_test(void);
void bsp_can1_polling_recv_msg(void);
#ifdef __cplusplus
}
#endif
#endif /* PERIPHERAL_DRIVERS_CAN_CAN_H_ */
