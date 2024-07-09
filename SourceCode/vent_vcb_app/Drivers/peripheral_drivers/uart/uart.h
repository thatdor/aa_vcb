/*
 * uart.h
 *
 *  Created on: Mar 27, 2024
 *      Author: dawei.zhou
 */

#ifndef PERIPHERAL_DRIVERS_UART_UART_H_
#define PERIPHERAL_DRIVERS_UART_UART_H_

typedef struct tagUART_CB
{
    osSemaphoreId_t     buf_sem;
    UART_HandleTypeDef* uart;
    IRQn_Type           irq;
    uint8_t    rx_int_buf[1];
#define USART_MESSAGE_BUFFER_SIZE       256 //缓存大小
    uint8_t             rx_buf[USART_MESSAGE_BUFFER_SIZE];
    volatile uint32_t   rx_buf_tail;
    volatile uint32_t   rx_buf_head;
    int                 it_flag;
}UART_CB;


// 初始化消息消息控制块：无信号量
#define UART_MSG_CB_INIT_NO_SEM(cb) do { \
    (cb)->buf_sem = NULL;    \
    (cb)->rx_buf_head = 0;   \
    (cb)->rx_buf_tail = 0;   \
    (cb)->it_flag = HAL_UART_Receive_IT((cb)->uart, (cb)->rx_int_buf, sizeof((cb)->rx_int_buf));   \
} while(0)

// 初始化消息消息控制块
#define UART_MSG_CB_INIT(cb) do {  \
    UART_MSG_CB_INIT_NO_SEM(cb); \
    (cb)->buf_sem = osSemaphoreNew(1, 1, NULL);   \
} while(0)

// 消息中断重置
#define UART_MSG_INT_RESET(cb)  do { \
    HAL_NVIC_SetPriority((cb)->irq, 5, 0); \
    HAL_NVIC_EnableIRQ((cb)->irq); \
    (cb)->it_flag = HAL_UART_Receive_IT((cb)->uart, (cb)->rx_int_buf, sizeof((cb)->rx_int_buf)); \
} while(0)

// 判定是否收到一条消息
#define UART_MSG_INVALID_EXT(cb, timeout) do { \
    if (osOK != osSemaphoreAcquire((cb)->buf_sem, (timeout))) \
    { \
        UART_MSG_INT_RESET(cb); \
        return 1; \
    } \
    return 0;  \
} while(0)

#define UART_MSG_INVALID(cb)  UART_MSG_INVALID_EXT(cb, 20)

// 从中断缓冲中读取一个字符
#define UART_MSG_GET_CHAR(cb, pchar)  do {   \
    if ( (cb)->rx_buf_head != (cb)->rx_buf_tail )  \
    {   \
        *pchar = (cb)->rx_buf[(cb)->rx_buf_head];   \
        ++(cb)->rx_buf_head;   \
        if ( (cb)->rx_buf_head >= sizeof((cb)->rx_buf) )   \
        {   \
            (cb)->rx_buf_head  = 0;   \
        }   \
        return 1;   \
    }   \
    return 0;   \
} while(0)

// 处理UART中断
#define UART_MSG_PROC_INT_MSG(cb)  do {  \
    (cb)->rx_buf[(cb)->rx_buf_tail] = (cb)->rx_int_buf[0];   \
    ++(cb)->rx_buf_tail;   \
    if ((cb)->rx_buf_tail >= USART_MESSAGE_BUFFER_SIZE)   \
    {   \
        (cb)->rx_buf_tail = 0;   \
    }   \
    (cb)->it_flag = HAL_UART_Receive_IT((cb)->uart, (cb)->rx_int_buf, sizeof((cb)->rx_int_buf)); \
    if ((cb)->rx_int_buf[0] == 0x7e)   \
    {   \
        osSemaphoreRelease((cb)->buf_sem);   \
    }   \
} while(0)

// 在中断中处理消息
#define UART_MSG_PROC_MSG_IN_INTERRUPTS(cb, callback_fun) do {\
        (cb)->rx_buf[(cb)->rx_buf_tail] = (cb)->rx_int_buf[0];   \
        ++(cb)->rx_buf_tail;   \
        if ((cb)->rx_buf_tail >= USART_MESSAGE_BUFFER_SIZE)   \
        {   \
            (cb)->rx_buf_tail = 0;   \
        }   \
        (cb)->it_flag = HAL_UART_Receive_IT((cb)->uart, (cb)->rx_int_buf, sizeof((cb)->rx_int_buf)); \
        if ((cb)->rx_int_buf[0] == 0x7e)   \
        { \
            callback_fun((cb)->rx_buf, (cb)->rx_buf_tail - 1); \
            (cb)->rx_buf_tail = 0; \
        } \
} while(0)



#endif /* PERIPHERAL_DRIVERS_UART_UART_H_ */
