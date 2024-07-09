/*
 * vcb_test_msg_fun.h
 *
 *  Created on: Mar 27, 2024
 *      Author: dawei.zhou
 */

#ifndef SYSTEM_MSG_BOUNDARY_VCB_TEST_VCB_TEST_MSG_FUN_H_
#define SYSTEM_MSG_BOUNDARY_VCB_TEST_VCB_TEST_MSG_FUN_H_


#ifdef __cplusplus
extern "C" {
#endif


void vcb_test_uart_init();
void vcb_test_uart_msg_proc_task_entry();
void vcb_test_uart_msg_send_cb_monitoring_data(void *parm, uint32_t len);
int  vcb_test_uart_msg_check_and_delay();
int  vcb_test_uart_msg_read_char(char* c);
int  vcb_test_uart_msg_transmit(uint8_t* buf, uint32_t buf_len);



#ifdef __cplusplus
}
#endif


#endif /* SYSTEM_MSG_BOUNDARY_VCB_TEST_VCB_TEST_MSG_FUN_H_ */
