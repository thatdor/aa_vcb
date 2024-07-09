
#ifndef VCB_VMB_MSG_BOUNDARY_H__FILE_
#define VCB_VMB_MSG_BOUNDARY_H__FILE_
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __DEV_COM_MSG_SEND_CFG_T_
#define __DEV_COM_MSG_SEND_CFG_T_
typedef struct tagDevComMsgSendCfgTbl
{
    int16_t interval;            // 发送时间间隔, 初始化时候填写
    int16_t interval_remaining;  // 剩余时长
    uint32_t send_count;         // 发送消息计数
    uint32_t send_error;         // 发送错误计数
    uint32_t send_parm;          // 发送函数参数, 初始化时候填写
#define DEV_COM_MSG_NO_MSG   1   // 无需发送消息
    int  (*sendfn)(uint32_t);    // 发送函数，,  初始化时候填写  0：发送成功，<0 发送错误
}DEV_COM_MSG_SEND_CFG_T;
#endif

/*************************************************************************************/
/*                          消息接收                                                  */
/*************************************************************************************/
/* 
   消息处理的入口函数：
   在消息处理任务中调用该函数进行消息处理，该函数不会返回
 */
void vcb_vmb_msg_proc_entry(void *parm);
/*************************************************************************************/
/*                          消息发送                                                  */
/*************************************************************************************/
/*
   初始化消息发送函数列表
 */
void vcb_vmb_send_msg_init(int interval_ms);
/*
   注册：每接收到一条消息，且缓冲区暂时为空是的回调函数
 */
void vcb_vmb_register_after_msg_proc_callback(void (*fn)(void*));
/*
   周期消息发送的入口函数：
   在上位机进行周期调用，以发送周期消息，下位机所有的消息都是收到响应后的返回，不会主动发送消息
 */
void vcb_vmb_msg_send_msg(int interval_ms);

/*************************************************************************************
   下面提供一些工具函数：
*************************************************************************************/
/* 返回发送侧配置的下发消息配置列表 */
DEV_COM_MSG_SEND_CFG_T*  vcb_vmb_msg_get_cfg(uint32_t *count);
/* 返回支持的消息个数 */
uint32_t vcb_vmb_get_msg_count();
/* 返回接收消息的总数和错误总数 */
int      vcb_vmb_get_recv_msg_stat(uint32_t msg_id, uint32_t  *count, uint32_t *err_count);
/* 返回发送消息的总数和错误总数 */
int      vcb_vmb_get_send_msg_stat(uint32_t msg_id, uint32_t  *count, uint32_t *err_count);
/* 返回系统接收对应模块最近一次消息的时间戳 */
uint32_t vcb_vmb_get_last_correct_msg_tick();

#ifdef __cplusplus
}
#endif

/* class definition */
/* 如果需要定义cpp类，请在这里进行，否则编译错误 */


#endif
