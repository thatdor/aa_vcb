
#include <stddef.h>
#include "vcb_vmb_msg_proc.h"
#include "vcb_vmb_msg_boundary.h"
#include "main.h"
#include "cmsis_os.h"
#include "vcb_vmb_msg_fun.h"
#include "../system/common/utility/crc.h"
// 实现消息相关函数
// Todo


///////////////////////////////////
// 修改下列代码
// 定义宏
#define VCB_VMB_DELAY_MS(_xx)                (osDelay(_xx))   // 延时函数，参数单位为ms
#define VCB_VMB_GET_CURRENT_TICK(_xx)        (HAL_GetTick())   // 获取当前系统的tick，一般以ms为单位（QDateTime::currentMSecsSinceEpoch()， HAL_GetTick())
#define VCB_VMB_DELAY_INTERVAL               (VCB_VMB_DELAY_MS(2))   // 两条消息之间的发送延时
#define VCB_VMB_COM_OPENED(_parm)            (1)   // 串口是否打开, 嵌入式可以写为1，但是在Win/Linux需要对文件句柄进行判断
#define VCB_VMB_COM_READ(_parm, _rchar)      (1) // Linux/Win: read(fd, &(_rchar), 1);  嵌入式：从中断缓冲区里面获取一个字符，需要判断缓冲区是否有数据
#define VCB_VMB_COM_CHECK_AND_DELAY(_parm)
DEV_COM_MSG_SEND_CFG_T g_vcb_vmb_send_msg_cfg[] = {
        // 在这里添加需要发送的函数调用
        //{
        //        .interval = 100,  /*1s 周期性检查配置消息并下发: 1s*/
        //        .send_parm = 0,
        //        .sendfn = NULL    /* 持续更新配置 */
        // }
};

// 实现消息相关函数

// 串口消息的完整性校验
uint16_t VCB_VMB_msg_calc_crc(uint8_t* buf, int32_t len)
{
    // 实现函数用于该串口的消息校验
    return crc16_checksum(buf, len);
}

// 串口消息的发送
int VCB_VMB_msg_uart_send(uint8_t* buf, int32_t len)
{
    // 实现函数用于该串口的消息发送
    return vcb_vmb_uart_msg_transmit(buf, len);
}


//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
/// 下面为自动生成代码（一般情况下无需修改）
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
#define VCB_VMB_MSG_SEND_CFG(i)   g_vcb_vmb_send_msg_cfg[i]
#define VCB_VMB_SEND_MSG_COUNT    (sizeof(g_vcb_vmb_send_msg_cfg) / sizeof(g_vcb_vmb_send_msg_cfg[0]))
// 记录系统最后一次接收到正确消息的Tick
uint32_t g_vcb_vmb_last_recv_correct_msg_tick = 0;

// 接收消息任务中，消息完成后的回调函数
void (*g_vcb_vmb_msg_proc_after_callback_fn)(void*) = NULL;

void vcb_vmb_register_after_msg_proc_callback(void (*fn)(void*))
{
    g_vcb_vmb_msg_proc_after_callback_fn = fn;
}

// 发送
void vcb_vmb_send_msg_init(int interval)
{
    int i = 0;
    int count = VCB_VMB_SEND_MSG_COUNT;

    for (i = 0; i < count; ++i)
    {
        VCB_VMB_MSG_SEND_CFG(i).interval_remaining = i * interval;
        VCB_VMB_MSG_SEND_CFG(i).send_count = 0;
        VCB_VMB_MSG_SEND_CFG(i).send_error = 0;
    }
}

// 任务中调用该消息实现对收到消息的解析
void vcb_vmb_msg_proc_entry(void *parm)
{
    char rchar = 0;
    size_t msg_len = 0;
    static uint8_t rcv_msg_buf[VCB_VMB_MSG_MESSAGE_BUF_LEN];
    int nread = 0;

    while (1)
    {
        if (!VCB_VMB_COM_OPENED(parm))
        {
            VCB_VMB_DELAY_MS(50);
            break;
        }

        // 检测是否有可以使用的消息
        VCB_VMB_COM_CHECK_AND_DELAY(parm);

        // 读取消息，直至字符缓冲区没有有效数据
        do
        {
            // 读取一个字符
            nread = VCB_VMB_COM_READ(parm, rchar);
            if (nread < 1)
            {
                // 没有有效的消息，跳出循环
                VCB_VMB_DELAY_MS(5);
                break;
            }
            rcv_msg_buf[msg_len] = rchar;

            /* Message length does not contain 0x7e */
            if (rchar == VCB_VMB_MSG_END_FLAG)
            {
                if (0 == VCB_VMB_receive(rcv_msg_buf, msg_len))
                {
                    g_vcb_vmb_last_recv_correct_msg_tick = VCB_VMB_GET_CURRENT_TICK();
                }
                msg_len = 0;
            }
            else
            {
                ++msg_len;
                if (msg_len >= sizeof(rcv_msg_buf))
                {
                    msg_len = 0;
                }
            }
        } while(1);
        // 接收缓冲区中的消息处理完毕，前暂时未收到新数据
        if (g_vcb_vmb_msg_proc_after_callback_fn != NULL)
        {
            g_vcb_vmb_msg_proc_after_callback_fn(parm);
        }
    }
}

// 循环调用改消息实现串口消息的持续发送
// 应该限制消息的发送条数
// interval_ms: 该函数被调用的时间间隔
void vcb_vmb_msg_send_msg(int interval_ms)
{
    int ret = 0;
    size_t i = 0;
    size_t count = VCB_VMB_SEND_MSG_COUNT;

    for (i = 0; i < count; ++i)
    {
        if (VCB_VMB_MSG_SEND_CFG(i).interval_remaining <= interval_ms)
        {
            if ((i != 0) && (ret != DEV_COM_MSG_NO_MSG))
            {
                // 避免消息连续发送，从第二个消息开始延时
                VCB_VMB_DELAY_INTERVAL;
            }
            ret = VCB_VMB_MSG_SEND_CFG(i).sendfn(VCB_VMB_MSG_SEND_CFG(i).send_parm);
            VCB_VMB_MSG_SEND_CFG(i).interval_remaining = VCB_VMB_MSG_SEND_CFG(i).interval;
            if (ret == 0)     // 消息发送成功
            {
                VCB_VMB_MSG_SEND_CFG(i).send_count += 1;
            }
            else if (ret < 0) // 发送错误
            {
                VCB_VMB_MSG_SEND_CFG(i).send_error += 1;
            }
        }
        else
        {
            VCB_VMB_MSG_SEND_CFG(i).interval_remaining -= interval_ms;
        }
    }
}

// 返回发送侧配置的发送消息数组列表
// 返回列表和计数
DEV_COM_MSG_SEND_CFG_T*  vcb_vmb_msg_get_cfg(uint32_t *count)
{
    *count = VCB_VMB_SEND_MSG_COUNT;
    return g_vcb_vmb_send_msg_cfg;
}

// 返回模块支持的消息的总数
uint32_t vcb_vmb_get_msg_count()
{
    return g_vcb_vmb_msg_handler_map_count;
}

// 返回对应消息接受的总数和发生错误的次数
int vcb_vmb_get_recv_msg_stat(uint32_t msg_id, uint32_t  *count, uint32_t *err_count)
{
    if ((VCB_VMB_MESSAGE_ID_BASE <= msg_id  ) && (msg_id < (VCB_VMB_MESSAGE_ID_BASE + g_vcb_vmb_msg_handler_map_count)))
    {
        *count = g_vcb_vmb_msg_handler_map[msg_id - VCB_VMB_MESSAGE_ID_BASE].recv_count;
        *err_count = g_vcb_vmb_msg_handler_map[msg_id - VCB_VMB_MESSAGE_ID_BASE].recv_e_count;
        return 0;
    }
    return -1;
}

// 返回对应消息发送的次数和发送发生错误的次数
int vcb_vmb_get_send_msg_stat(uint32_t msg_id, uint32_t  *count, uint32_t *err_count)
{
    if ((VCB_VMB_MESSAGE_ID_BASE <= msg_id  ) && (msg_id < (VCB_VMB_MESSAGE_ID_BASE + g_vcb_vmb_msg_handler_map_count)))
    {
        *count = g_vcb_vmb_msg_handler_map[msg_id - VCB_VMB_MESSAGE_ID_BASE].send_count;
        *err_count = g_vcb_vmb_msg_handler_map[msg_id - VCB_VMB_MESSAGE_ID_BASE].send_e_count;
        return 0;
    }
    return -1;
}

// 返回最近一次收到正确消息的时间戳
uint32_t vcb_vmb_get_last_correct_msg_tick()
{
    return g_vcb_vmb_last_recv_correct_msg_tick;
}

//////////////////////////////////////////////////////////////
