/*
 * uart.c
 *
 *  Created on: Mar 27, 2024
 *      Author: dawei.zhou
 */
#include "main.h"
#include "cmsis_os.h"
#include "uart.h"
#include "../system/msg_boundary/vcb_vmb/vcb_vmb_msg_proc.h"

// VCB_VMB_串口
UART_CB g_uart_vcb_vmb = {
    .buf_sem = NULL,
    .uart = &huart6,
    .irq = USART6_IRQn,
    .rx_buf_head = 0,
    .rx_buf_tail = 0,
    .it_flag = 0,
};

UART_CB g_uart_vcb_test = {
    .buf_sem = NULL,
    .uart = &huart1,
    .irq = USART1_IRQn,
    .rx_buf_head = 0,
    .rx_buf_tail = 0,
    .it_flag = 0,
};

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART6)
    {
        UART_MSG_PROC_MSG_IN_INTERRUPTS(&g_uart_vcb_vmb, VCB_VMB_receive);
    }
    else if (huart->Instance == USART1)
    {
        UART_MSG_PROC_INT_MSG(&g_uart_vcb_test);
    }
}
