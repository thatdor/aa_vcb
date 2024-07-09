/*
 * vcb_vmb_msg_fun.h
 *
 *  Created on: Mar 27, 2024
 *      Author: dawei.zhou
 */

#ifndef SYSTEM_MSG_BOUNDARY_VCB_VMB_VCB_VMB_MSG_FUN_H_
#define SYSTEM_MSG_BOUNDARY_VCB_VMB_VCB_VMB_MSG_FUN_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void vcb_vmb_uart_init();
int vcb_vmb_uart_msg_transmit(uint8_t* buf, uint32_t buf_len);
//int vcb_vmb_uart_msg_check_and_delay();
//int vcb_vmb_uart_msg_read_char(char* c);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_MSG_BOUNDARY_VCB_VMB_VCB_VMB_MSG_FUN_H_ */
