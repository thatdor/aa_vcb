/*
 * vcb_test_msg_fun.cpp
 *
 *  Created on: Mar 27, 2024
 *      Author: dawei.zhou
 */
#include <stdint.h>
#include <string.h>
#include "main.h"
#include "cmsis_os.h"
#include "vcb_test_msg_fun.h"
#include "vcb_test_msg_boundary.h"
#include "vcb_test_msg_proc.h"
#include "../system/ventilator/vcb_test/VCBTestBoundary.h"
#include "../../Drivers/peripheral_drivers/uart/uart.h"

extern UART_CB  g_uart_vcb_test;

void vcb_test_uart_msg_vcb_configure_requeset_proc(void *rmsg)
{
    VCB_TEST_VCB_CONFIGURE_REQUESET_T* msg = (VCB_TEST_VCB_CONFIGURE_REQUESET_T*)rmsg;

    VCBTestBoundary::getInstanceRef().procConfigData(static_cast<uint8_t*>((void*)msg->raw_data), sizeof(msg->raw_data));
}

void vcb_test_uart_msg_vcb_monitoring_data_up_request_proc(void *rmsg)
{
    VCB_TEST_VCB_MONITORING_DATA_UP_REQUEST_T* msg = (VCB_TEST_VCB_MONITORING_DATA_UP_REQUEST_T*)rmsg;

    if (msg->status & VCB_TEST_VCB_MONITORING_DATA_UP_REQUEST_UPLOAD_ENABLE)
    {
        VCBTestBoundary::getInstanceRef().enableMonitorDataTransmit(true);
    }
    else if (msg->status & VCB_TEST_VCB_MONITORING_DATA_UP_REQUEST_UPLOAD_DISABLE)
    {
        VCBTestBoundary::getInstanceRef().enableMonitorDataTransmit(false);
    }
}

VCB_TEST_VCB_MONITORING_DATA_UP_RESPONSE_T g_vb_test_uart_up_smsg_buf = {0};
void vcb_test_uart_msg_send_cb_monitoring_data(void *parm, uint32_t len)
{

    memcpy(g_vb_test_uart_up_smsg_buf.raw_data, parm, len);

    VCB_TEST_send_vcb_monitoring_data_up_response(&g_vb_test_uart_up_smsg_buf);
}

void vcb_test_uart_init()
{
    vcb_test_register_after_msg_proc_callback(nullptr);

    VCB_TEST_msg_recv_handler_register(vcb_test_uart_msg_vcb_configure_requeset_proc, VCB_TEST_ID_VCB_CONFIGURE_REQUESET);
    VCB_TEST_msg_recv_handler_register(vcb_test_uart_msg_vcb_monitoring_data_up_request_proc, VCB_TEST_ID_VCB_MONITORING_DATA_UP_REQUEST);

    UART_MSG_CB_INIT(&g_uart_vcb_test);
}

void vcb_test_uart_msg_proc_task_entry()
{
    vcb_test_msg_proc_entry(nullptr);
}

int vcb_test_uart_msg_check_and_delay()
{
    UART_MSG_INVALID_EXT(&g_uart_vcb_test, 50);
}

int vcb_test_uart_msg_read_char(char* c)
{
    UART_MSG_GET_CHAR(&g_uart_vcb_test, c);
}

int  vcb_test_uart_msg_transmit(uint8_t* buf, uint32_t buf_len)
{
	int timeout = 20;
    int ret = HAL_UART_Transmit_IT(g_uart_vcb_test.uart, buf, buf_len);

    while(g_uart_vcb_test.uart->gState != HAL_UART_STATE_READY)
    {
        osDelay(1);
        if (--timeout < 0)
        {
            ret = -1;
            break;
        }
    }
    return ret;

    //return HAL_UART_Transmit(g_uart_vcb_test.uart, buf, buf_len, 10);
}
