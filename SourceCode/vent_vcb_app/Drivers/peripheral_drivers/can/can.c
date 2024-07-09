/*
 * can.c
 *
 *  Created on: Nov 20, 2023
 *      Author: weizhuang.kong
 */
#include "can.h"
#include "main.h"

// 设置FIFO0过滤
uint8_t bsp_can1_filter_config(void)
{
    CAN_FilterTypeDef filter = {0};

    filter.FilterActivation = ENABLE;
    filter.FilterMode = CAN_FILTERMODE_IDMASK;
    filter.FilterScale = CAN_FILTERSCALE_16BIT;
    filter.FilterBank = 0;
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterIdLow = 0x123;
    filter.FilterIdHigh = 0;
    filter.FilterMaskIdLow = 0x3;
    filter.FilterMaskIdHigh = 0;

    HAL_CAN_ConfigFilter(&hcan1, &filter);

    return BSP_CAN_OK;
}

uint8_t bsp_can1_send_msg(uint32_t id_type, uint32_t basic_id, uint32_t ex_id, uint8_t *data, uint32_t data_len)
{
    uint8_t index = 0;
    uint32_t msg_box = 0;
    CAN_TxHeaderTypeDef send_msg_hdr;

    send_msg_hdr.StdId = basic_id;
    send_msg_hdr.ExtId = ex_id;
    send_msg_hdr.IDE = id_type;
    send_msg_hdr.RTR = CAN_RTR_DATA;
    send_msg_hdr.DLC = data_len;
    send_msg_hdr.TransmitGlobalTime = DISABLE;

    HAL_CAN_AddTxMessage(&hcan1, &send_msg_hdr, data, &msg_box);

    return BSP_CAN_OK;
}

uint8_t g_recv_data[8];
void bsp_can1_polling_recv_msg(void)
{
    CAN_RxHeaderTypeDef header;

    while (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) != 0)
    {
        if (__HAL_CAN_GET_FLAG(&hcan1, CAN_FLAG_FOV0) != RESET)
        {
            // printf("[CAN] FIFO0 overrun!\n");
        }

        HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &header, g_recv_data);
    }
}

void can_test(void)
{
    uint8_t send_data[8] = {1, 2, 3, 4, 5, 6, 7, 0xaa};

    HAL_CAN_Start(&hcan1);
    bsp_can1_filter_config();

    bsp_can1_send_msg(CAN_ID_STD, 11, 2, send_data, 8);
    vTaskDelay(2);
    bsp_can1_polling_recv_msg();
}

#define CAN1_FILTER_MODE_MASK_ENABLE 1 ///< CAN1过滤器模式选择：=1：屏蔽位模式  =0：屏蔽列表模式
#define CAN2_FILTER_MODE_MASK_ENABLE 1 ///< CAN2过滤器模式选择：=1：屏蔽位模式  =0：屏蔽列表模式

#define CAN1_BASE_ID 0x10F00266 ///< 主CAN过滤ID
#define CAN2_BASE_ID 0x10F0F126 ///< 从CAN过滤ID

#define CAN1_FILTER_BANK 0  ///< 主CAN过滤器组编号
#define CAN2_FILTER_BANK 14 ///< 从CAN过滤器组编号

/// CAN过滤器寄存器位宽类型定义
typedef union
{
    __IO uint32_t value;
    struct
    {
        uint8_t REV : 1;    ///< [0]    ：未使用
        uint8_t RTR : 1;    ///< [1]    : RTR（数据帧或远程帧标志位）
        uint8_t IDE : 1;    ///< [2]    : IDE（标准帧或扩展帧标志位）
        uint32_t EXID : 18; ///< [21:3] : 存放扩展帧ID
        uint16_t STID : 11; ///< [31:22]: 存放标准帧ID
    } Sub;
} CAN_FilterRegTypeDef;

// CAN_FMR寄存器位宽类型定义
typedef union
{
    __IO uint32_t value;
    struct
    {
        uint8_t FINIT : 1;
        uint8_t RESERVER_0 : 7;
        uint8_t CAN2SB : 6;
        uint32_t RESERVER_1 : 18;
    } Sub;
} FMR_TypeDef;

/// 设置CAN1的过滤器（主CAN）
static void CAN1_Filter_Config(void)
{
    CAN_FilterTypeDef sFilterConfig;
    CAN_FilterRegTypeDef IDH = {0};
    CAN_FilterRegTypeDef IDL = {0};

    IDH.Sub.IDE = 0;                              // 标准帧
    IDH.Sub.STID = 0;                             // 标准帧ID值
    IDH.Sub.EXID = (CAN1_BASE_ID >> 16) & 0xFFFF; // 扩展帧高16位ID值

    IDL.Sub.IDE = 1;                        // 扩展帧
    IDL.Sub.STID = 0;                       // 标准帧ID值
    IDL.Sub.EXID = (CAN1_BASE_ID & 0xFFFF); // 扩展帧低16位ID值

    sFilterConfig.FilterBank = CAN1_FILTER_BANK; // 设置过滤器组编号
#if CAN1_FILTER_MODE_MASK_ENABLE
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK; // 屏蔽位模式
#else
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST; // 列表模式
#endif
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;     // 32位宽
    sFilterConfig.FilterIdHigh = IDH.value;                // 标识符寄存器一ID高十六位，放入扩展帧位
    sFilterConfig.FilterIdLow = IDL.value;                 // 标识符寄存器一ID低十六位，放入扩展帧位
    sFilterConfig.FilterMaskIdHigh = IDH.value;            // 标识符寄存器二ID高十六位，放入扩展帧位
    sFilterConfig.FilterMaskIdLow = IDL.value;             // 标识符寄存器二ID低十六位，放入扩展帧位
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;     // 过滤器组关联到FIFO0
    sFilterConfig.FilterActivation = ENABLE;               // 激活过滤器
    sFilterConfig.SlaveStartFilterBank = CAN2_FILTER_BANK; // 设置CAN2的起始过滤器组（对于单CAN的CPU或从CAN此参数无效；对于双CAN的CPU此参数为从CAN的起始过滤器组编号）
    if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    {
        FMR_TypeDef regval = {0};
        regval.value = hcan1.Instance->FMR;
        printf("------ CAN1:> FMR:0x%0X  CAN2SB:0x%X  \r\n", regval.value, regval.Sub.CAN2SB);
    }
}


// CAN初始化
void CAN_Init(void)
{
    CAN1_Filter_Config();                                              // 初始化CNA1过滤器
    HAL_CAN_Start(&hcan1);                                             // 启动CAN1
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING); // 激活CAN1中断 接收FIFO0中待处理消息
}

/**
 * CAN数据传输
 * @param  buf    待发送的数据
 * @param  len    数据长度
 * @param  number CAN编号，=0：CAN1，=1：CAN2
 * @return        0：成功  other：失败
 */
uint8_t CAN_Transmit(const void *buf, uint32_t len, uint8_t number)
{
    uint32_t txmailbox = 0;
    uint32_t offset = 0;
    CAN_TxHeaderTypeDef hdr;

    hdr.IDE = CAN_ID_EXT;                                  // ID类型：扩展帧
    hdr.RTR = CAN_RTR_DATA;                                // 帧类型：数据帧
    hdr.StdId = 0;                                         // 标准帧ID,最大11位，也就是0x7FF
    hdr.ExtId = number == 0 ? CAN1_BASE_ID : CAN2_BASE_ID; // 扩展帧ID,最大29位，也就是0x1FFFFFFF
    hdr.TransmitGlobalTime = DISABLE;

    while (len != 0)
    {
        hdr.DLC = len > 8 ? 8 : len; // 数据长度
        if (HAL_CAN_AddTxMessage(&hcan1, &hdr, ((uint8_t *)buf) + offset, &txmailbox) != HAL_OK)
            return 1;
        offset += hdr.DLC;
        len -= hdr.DLC;
    }
    return 0;
}

uint8_t CAN1_RX_STA = 0; ///< CAN1数据接收标志：[7]:数据 [6:0]:未使用
uint8_t CAN2_RX_STA = 0; ///< CAN2数据接收标志：[7]:数据 [6:0]:未使用

uint8_t CAN1_RX_BUF[8]; ///< CAN1数据接收缓存
uint8_t CAN2_RX_BUF[8]; ///< CAN2数据接收缓存

uint8_t CAN1_TX_BUF[8]; ///< CAN1数据发送缓存
uint8_t CAN2_TX_BUF[8]; ///< CAN2数据发送缓存

/**
 * CAN FIFO0 数据接收中断回调函数
 * @param hcan CAN句柄
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    static CAN_RxHeaderTypeDef CAN_RX_HDR;

    // CAN1数据接收
    if (hcan->Instance == hcan1.Instance)
    {
        // 数据已经处理
        if ((CAN1_RX_STA & 0x80) == 0)
        {
            // 清空缓存
            memset(CAN1_RX_BUF, 0, 8);
            // 接收数据
            if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &CAN_RX_HDR, CAN1_RX_BUF) == HAL_OK) // 获得接收到的数据头和数据
            {
                CAN1_RX_STA |= 0x80;                                             // 标记接收到数据
                HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING); // 再次使能FIFO0接收中断
            }
        }
    }
    return;
}

// CAN数据处理函数
inline void CAN_RecvHandler(void)
{
    // CAN1有数据收到
    if (CAN1_RX_STA & 0x80)
    {
        int i = 0;
        memcpy(CAN1_TX_BUF, CAN1_RX_BUF, sizeof(CAN1_RX_BUF)); // 拷贝出数据
        CAN1_RX_STA = 0;                                       // 重置CAN1接收状态
        for (i = 0; i != 8; i++)
        {
            printf("CAN1_TX_BUF[%d]:0x%X\r\n", i, CAN1_TX_BUF[i]);
        }
        printf("\r\n\r\n");
    }

    // CAN2有数据收到
    if (CAN2_RX_STA & 0x80)
    {
        int i = 0;
        memcpy(CAN2_TX_BUF, CAN2_RX_BUF, sizeof(CAN2_RX_BUF)); // 拷贝出数据
        CAN2_RX_STA = 0;                                       // 重置CAN2接收状态
        for (i = 0; i != 8; i++)
        {
            printf("CAN2_TX_BUF[%d]:0x%X\r\n", i, CAN2_TX_BUF[i]);
        }
        printf("\r\n\r\n");
    }
}
