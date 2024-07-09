/*
 * vcb_vmb_msg_fun.cpp
 *
 *  Created on: Mar 27, 2024
 *      Author: dawei.zhou
 */
#include <stdint.h>
#include "main.h"
#include "cmsis_os.h"
#include "vcb_vmb_msg_fun.h"
#include "vcb_vmb_msg_boundary.h"
#include "vcb_vmb_msg_proc.h"
#include "../system/ventilator/sensors/VMBProxy.h"
#include "../../Drivers/peripheral_drivers/uart/uart.h"


extern UART_CB  g_uart_vcb_vmb;


void vcb_vmb_uart_msg_vmb_monitoring_data_up_response_proc(void *rmsg)
{
    VCB_VMB_VMB_MONITORING_DATA_UP_RESPONSE_T* msg = (VCB_VMB_VMB_MONITORING_DATA_UP_RESPONSE_T*)rmsg;

    VMBProxy::getInstanceRef().setAirSupplyPressure(msg->air_supply_pressure);
    VMBProxy::getInstanceRef().setO2SupplyPressure(msg->o2_supply_pressure);
    VMBProxy::getInstanceRef().setStatusBits(msg->status_bits);

    VMBProxy::getInstanceRef().setExpFlowErrorCount(msg->exp_flow_error_count);
    VMBProxy::getInstanceRef().setExpFlowResetCount(msg->exp_flow_reset_count);
    VMBProxy::getInstanceRef().setPhtErrorCount(msg->pht_error_count);
    VMBProxy::getInstanceRef().setPhtResetCount(msg->pht_reset_count);
}

void vcb_msg_vmb_exp_pressure_up_request_proc(void *rmsg)
{
    VCB_VMB_VMB_EXP_PRESSURE_UP_RESPONSE_T* msg = (VCB_VMB_VMB_EXP_PRESSURE_UP_RESPONSE_T*)rmsg;

    VMBProxy::getInstanceRef().setExpPressure(msg->exp_pressure);
    VMBProxy::getInstanceRef().setExpFlow(msg->exp_flow_rate);
}

void vcb_vmb_uart_init()
{
    vcb_vmb_register_after_msg_proc_callback(nullptr);

    VCB_VMB_msg_recv_handler_register(vcb_vmb_uart_msg_vmb_monitoring_data_up_response_proc, VCB_VMB_ID_VMB_MONITORING_DATA_UP_RESPONSE);
    VCB_VMB_msg_recv_handler_register(vcb_msg_vmb_exp_pressure_up_request_proc, VCB_VMB_ID_VMB_EXP_PRESSURE_UP_RESPONSE);

    UART_MSG_CB_INIT_NO_SEM(&g_uart_vcb_vmb);
}

//int vcb_vmb_uart_msg_check_and_delay()
//{
//   UART_MSG_INVALID_EXT(&g_uart_vcb_vmb, 20);
//}
//
//int vcb_vmb_uart_msg_read_char(char* c)
//{
//   UART_MSG_GET_CHAR(&g_uart_vcb_vmb, c);
//}
//
int vcb_vmb_uart_msg_transmit(uint8_t* buf, uint32_t buf_len)
{
    return HAL_UART_Transmit_IT(g_uart_vcb_vmb.uart, buf, buf_len);
    //return HAL_UART_Transmit(g_uart_vcb_test.uart, buf, buf_len, 10);
}
