/* 自动生成文件，请勿修改 ！！！ Dawei.Zhou */
#include <stddef.h>
#include "vcb_vmb_msg_proc.h"

#if HOST_SYSTEM_BIG_ENDIAN
#define HOST_TO_NET2B(_dest_buf, _value) { (_dest_buf)[0] = (((_value) >> 8) & 0xFF);  (_dest_buf)[1] = ((_value) & 0xFF); }
#define HOST_TO_NET4B(_dest_buf, _value) { (_dest_buf)[0] = (((_value) >> 24) & 0xFF);  (_dest_buf)[1] = (((_value) >> 16) & 0xFF); \
                                           (_dest_buf)[2] = (((_value) >> 8) & 0xFF);  (_dest_buf)[3] = (((_value)) & 0xFF); }
#define NET_TO_HOST2B(_buf)              ( (((_buf)[0] << 8) & 0xFF00) | ((_buf)[1] & 0xFF) )
#define NET_TO_HOST4B(_buf)              ( (((_buf)[0] << 24) & 0xFF000000) | (((_buf)[1] << 16) & 0x0FF0000) | \
                                           (((_buf)[2] << 8)  & 0x0000FF00) | ((_buf)[3] & 0x000000FF))
#else
#define HOST_TO_NET2B(_dest_buf, _value) { (_dest_buf)[1] = (((_value) >> 8) & 0xFF);  (_dest_buf)[0] = ((_value) & 0xFF); } 
#define HOST_TO_NET4B(_dest_buf, _value) { (_dest_buf)[3] = (((_value) >> 24) & 0xFF);  (_dest_buf)[2] = (((_value) >> 16) & 0xFF); \
                                           (_dest_buf)[1] = (((_value) >> 8) & 0xFF);  (_dest_buf)[0] = (((_value)) & 0xFF); }
#define NET_TO_HOST2B(_buf)              ( (((_buf)[1] << 8) & 0xFF00) | ((_buf)[0] & 0xFF) )
#define NET_TO_HOST4B(_buf)              ( (((_buf)[3] << 24) & 0xFF000000) | (((_buf)[2] << 16) & 0x0FF0000) | \
                                           (((_buf)[1] << 8)  & 0x0000FF00) | ((_buf)[0] & 0x000000FF))
#endif
    
VCB_VMB_MSG_HANDLER_CB_T g_vcb_vmb_msg_handler_map[] = {
    {NULL, 0, 0, 0, 0}, 
    {NULL, 0, 0, 0, 0}, 
    {NULL, 0, 0, 0, 0}, 
    {NULL, 0, 0, 0, 0}, 
    {NULL, 0, 0, 0, 0}, 
    {NULL, 0, 0, 0, 0}, 
    {NULL, 0, 0, 0, 0}, 
    {NULL, 0, 0, 0, 0}, 
    {NULL, 0, 0, 0, 0}, 
    {NULL, 0, 0, 0, 0}, 
    {NULL, 0, 0, 0, 0}, // VCB_VMB_ID_VMB_MONITORING_DATA_UP_REQUEST
    {NULL, 0, 0, 0, 0}, // VCB_VMB_ID_VMB_MONITORING_DATA_UP_RESPONSE
    {NULL, 0, 0, 0, 0}, // VCB_VMB_ID_VMB_EXP_PRESSURE_UP_REQUEST
    {NULL, 0, 0, 0, 0}, // VCB_VMB_ID_VMB_EXP_PRESSURE_UP_RESPONSE
    {NULL, 0, 0, 0, 0}, // VCB_VMB_ID_VMB_TEST_DATA_REQUEST
    {NULL, 0, 0, 0, 0}, // VCB_VMB_ID_VMB_TEST_DATA_RESPONSE
    {NULL, 0, 0, 0, 0}  // Unknown(Error Type)
};
#define VCB_VMB_GET_HANDER_CB(_ID)         g_vcb_vmb_msg_handler_map[(_ID)-VCB_VMB_MESSAGE_ID_BASE]
#define VCB_VMB_MSG_HANDLER_COUNT           (sizeof(g_vcb_vmb_msg_handler_map)/sizeof(g_vcb_vmb_msg_handler_map[0]))
#define VCB_VMB_SEND_COUNT(_ID, _ret)      if ((_ret) == 0) { ++VCB_VMB_GET_HANDER_CB(_ID).send_count; }else { ++VCB_VMB_GET_HANDER_CB(_ID).send_e_count; }
#define VCB_VMB_UNKNOWN_HANDER_CB          g_vcb_vmb_msg_handler_map[VCB_VMB_MSG_HANDLER_COUNT-1]

#define VCB_VMB_CALL_RECV_MSG_PROCESS(_ID, _buf, _ret) do { \
            if ((_ret) == 0) \
            { \
                if (VCB_VMB_GET_HANDER_CB(_ID).handler != NULL) { VCB_VMB_GET_HANDER_CB(_ID).handler(_buf); } \
                ++VCB_VMB_GET_HANDER_CB(_ID).recv_count; \
            } \
            else \
            { \
                ++VCB_VMB_GET_HANDER_CB(_ID).recv_e_count; \
            } \
        } while(0) 

uint32_t g_vcb_vmb_msg_handler_map_count = VCB_VMB_MSG_HANDLER_COUNT; 


// 消息由外部回调函数处理，无需本子系统继续处理
#define VCB_VMB_MSG_PREPROCESS_CONTINUE  (0x10)    //  消息由外部回调函数处理，无需继续处理
int   (*g_vcb_vmb_msg_preprocess_hook)(uint8_t*, int) = NULL;

#define VCB_VMB_MSG_DO_PRO_PRECESSING \
    if (g_vcb_vmb_msg_preprocess_hook != NULL) \
    { \
        if (VCB_VMB_MSG_PREPROCESS_CONTINUE == g_vcb_vmb_msg_preprocess_hook(rawBuf, len + 1)) \
        { \
            return 0; \
        } \
    }

int VCB_VMB_msg_recv_handler_register(VCB_VMB_MSG_HANDLER_T fun, uint32_t msg_id)
{
    if ((msg_id >= VCB_VMB_MESSAGE_ID_BASE) &&
        (msg_id < (VCB_VMB_MESSAGE_ID_BASE + VCB_VMB_MSG_HANDLER_COUNT)))
    {
        VCB_VMB_GET_HANDER_CB(msg_id).handler = fun;
        VCB_VMB_GET_HANDER_CB(msg_id).recv_count   = 0;
        VCB_VMB_GET_HANDER_CB(msg_id).recv_e_count = 0;
        VCB_VMB_GET_HANDER_CB(msg_id).send_count   = 0;
        VCB_VMB_GET_HANDER_CB(msg_id).send_e_count = 0;
        return 0;
    }
    return 1;
}


int VCB_VMB_status_request_serialization(uint8_t* buffer, void* message)
{
    int i = 0;
    int len = 3;
    VCB_VMB_STATUS_REQUEST_T* msg = (VCB_VMB_STATUS_REQUEST_T*)message;

    /* head */
    buffer[0] = VCB_VMB_ID_STATUS_REQUEST;

    len = 3;

    /* message body */
    /* 接受客户端的ID，消息匹配才接收 */
    /* uint8_t : client_id */
    buffer[len] = (msg->client_id);
    len += 1;
     
    /* 0x1 设备版本信息， 0x02 开始升级（只有开始升级的时候下面三个字段才有效） */
    /* uint32_t : request_id */
    HOST_TO_NET4B(&(buffer[len]), msg->request_id);
    len += 4;
    
    /* 开始升级时向下位机下发app的长度和crc */
    /* uint32_t : app_len */
    HOST_TO_NET4B(&(buffer[len]), msg->app_len);
    len += 4;
    
    /* uint16_t : app_crc */
    HOST_TO_NET2B(&(buffer[len]), msg->app_crc);
    len += 2;
    
    /* 开始升级时通知下位机app版本号，如v1.0 */
    /* uint8_t[8] : app_ver */
    for (i = 0; i < 8; ++i)
    {
        buffer[len] = (msg->app_ver[i]);
        len += 1;
    }

    /* message length */
    HOST_TO_NET2B(&buffer[1], len)

    return len;
}

int VCB_VMB_status_request_deserialization(uint8_t* buffer, void* message)
{
    int i = 0;
    int len = 3;
    int msglen = NET_TO_HOST2B(&buffer[1]);
    VCB_VMB_STATUS_REQUEST_T* msg = (VCB_VMB_STATUS_REQUEST_T*)message;

    /* construct message body */
    /* 接受客户端的ID，消息匹配才接收 */
    /* uint8_t : client_id */
    msg->client_id = buffer[len];
    len += 1;
    
    if (msglen < len)
    { 
        return -2;
    }

    /* 0x1 设备版本信息， 0x02 开始升级（只有开始升级的时候下面三个字段才有效） */
    /* uint32_t : request_id */
    msg->request_id = NET_TO_HOST4B(&buffer[len]);
    len += 4;
    
    if (msglen < len)
    { 
        return -2;
    }

    /* 开始升级时向下位机下发app的长度和crc */
    /* uint32_t : app_len */
    msg->app_len = NET_TO_HOST4B(&buffer[len]);
    len += 4;
    
    if (msglen < len)
    { 
        return -2;
    }

    /* uint16_t : app_crc */
    msg->app_crc = NET_TO_HOST2B(&buffer[len]);
    len += 2;
    
    if (msglen < len)
    { 
        return -2;
    }

    /* 开始升级时通知下位机app版本号，如v1.0 */
    /* uint8_t[8] : app_ver */
    for (i = 0; i < 8; ++i)
    {
        msg->app_ver[i] = buffer[len];
        len += 1;
    }
    if (msglen < len)
    { 
        return -2;
    }

    return 0;
}

int VCB_VMB_status_response_serialization(uint8_t* buffer, void* message)
{
    int i = 0;
    int len = 3;
    VCB_VMB_STATUS_RESPONSE_T* msg = (VCB_VMB_STATUS_RESPONSE_T*)message;

    /* head */
    buffer[0] = VCB_VMB_ID_STATUS_RESPONSE;

    len = 3;

    /* message body */
    /* 客户端的ID */
    /* uint8_t : client_id */
    buffer[len] = (msg->client_id);
    len += 1;
    
    /* 设备状态标记位， 0x01 bootloader当前处于bootload中， 0x02 application当前处于app中， 只要是bootloader模式就可以升级，其它错误状态 */
    /* uint8_t : status */
    buffer[len] = (msg->status);
    len += 1;
    
    /* 下位机boot版本号，如v1.0 */
    /* uint8_t[8] : boot_ver */
    for (i = 0; i < 8; ++i)
    {
        buffer[len] = (msg->boot_ver[i]);
        len += 1;
    }

    /* 下位机app版本号，如v1.0 */
    /* uint8_t[8] : app_ver */
    for (i = 0; i < 8; ++i)
    {
        buffer[len] = (msg->app_ver[i]);
        len += 1;
    }

    /* 返回的硬件版本号 如XXXX 2021000001 */
    /* uint8_t[16] : hw_info */
    for (i = 0; i < 16; ++i)
    {
        buffer[len] = (msg->hw_info[i]);
        len += 1;
    }

    /* message length */
    HOST_TO_NET2B(&buffer[1], len)

    return len;
}

int VCB_VMB_status_response_deserialization(uint8_t* buffer, void* message)
{
    int i = 0;
    int len = 3;
    int msglen = NET_TO_HOST2B(&buffer[1]);
    VCB_VMB_STATUS_RESPONSE_T* msg = (VCB_VMB_STATUS_RESPONSE_T*)message;

    /* construct message body */
    /* 客户端的ID */
    /* uint8_t : client_id */
    msg->client_id = buffer[len];
    len += 1;
    
    if (msglen < len)
    { 
        return -2;
    }

    /* 设备状态标记位， 0x01 bootloader当前处于bootload中， 0x02 application当前处于app中， 只要是bootloader模式就可以升级，其它错误状态 */
    /* uint8_t : status */
    msg->status = buffer[len];
    len += 1;
    
    if (msglen < len)
    { 
        return -2;
    }

    /* 下位机boot版本号，如v1.0 */
    /* uint8_t[8] : boot_ver */
    for (i = 0; i < 8; ++i)
    {
        msg->boot_ver[i] = buffer[len];
        len += 1;
    }
    if (msglen < len)
    { 
        return -2;
    }

    /* 下位机app版本号，如v1.0 */
    /* uint8_t[8] : app_ver */
    for (i = 0; i < 8; ++i)
    {
        msg->app_ver[i] = buffer[len];
        len += 1;
    }
    if (msglen < len)
    { 
        return -2;
    }

    /* 返回的硬件版本号 如XXXX 2021000001 */
    /* uint8_t[16] : hw_info */
    for (i = 0; i < 16; ++i)
    {
        msg->hw_info[i] = buffer[len];
        len += 1;
    }
    if (msglen < len)
    { 
        return -2;
    }

    return 0;
}

int VCB_VMB_update_data_request_serialization(uint8_t* buffer, void* message)
{
    int i = 0;
    int len = 3;
    VCB_VMB_UPDATE_DATA_REQUEST_T* msg = (VCB_VMB_UPDATE_DATA_REQUEST_T*)message;

    /* head */
    buffer[0] = VCB_VMB_ID_UPDATE_DATA_REQUEST;

    len = 3;

    /* message body */
    /* 客户端的ID，CB的ID为2，ECC的ID为3 */
    /* uint8_t : client_id */
    buffer[len] = (msg->client_id);
    len += 1;
    
    /* 是否是第一个最后一个 0x01 第一个 0x02最后一个 0x0 中间消息 */
    /* uint8_t : boundary_flag */
    buffer[len] = (msg->boundary_flag);
    len += 1;
    
    /* 消息内容的有效长度 */
    /* uint16_t : msg_len */
    HOST_TO_NET2B(&(buffer[len]), msg->msg_len);
    len += 2;
    
    /* 消息的序列号 */
    /* uint32_t : seq_id */
    HOST_TO_NET4B(&(buffer[len]), msg->seq_id);
    len += 4;
    
    /* 当前数据块的地址 */
    /* uint32_t : address */
    HOST_TO_NET4B(&(buffer[len]), msg->address);
    len += 4;
    
    /* 程序数据 */
    /* uint8_t[128] : app_data */
    for (i = 0; i < 128; ++i)
    {
        buffer[len] = (msg->app_data[i]);
        len += 1;
    }

    /* message length */
    HOST_TO_NET2B(&buffer[1], len)

    return len;
}

int VCB_VMB_update_data_request_deserialization(uint8_t* buffer, void* message)
{
    int i = 0;
    int len = 3;
    int msglen = NET_TO_HOST2B(&buffer[1]);
    VCB_VMB_UPDATE_DATA_REQUEST_T* msg = (VCB_VMB_UPDATE_DATA_REQUEST_T*)message;

    /* construct message body */
    /* 客户端的ID，CB的ID为2，ECC的ID为3 */
    /* uint8_t : client_id */
    msg->client_id = buffer[len];
    len += 1;
    
    if (msglen < len)
    { 
        return -2;
    }

    /* 是否是第一个最后一个 0x01 第一个 0x02最后一个 0x0 中间消息 */
    /* uint8_t : boundary_flag */
    msg->boundary_flag = buffer[len];
    len += 1;
    
    if (msglen < len)
    { 
        return -2;
    }

    /* 消息内容的有效长度 */
    /* uint16_t : msg_len */
    msg->msg_len = NET_TO_HOST2B(&buffer[len]);
    len += 2;
    
    if (msglen < len)
    { 
        return -2;
    }

    /* 消息的序列号 */
    /* uint32_t : seq_id */
    msg->seq_id = NET_TO_HOST4B(&buffer[len]);
    len += 4;
    
    if (msglen < len)
    { 
        return -2;
    }

    /* 当前数据块的地址 */
    /* uint32_t : address */
    msg->address = NET_TO_HOST4B(&buffer[len]);
    len += 4;
    
    if (msglen < len)
    { 
        return -2;
    }

    /* 程序数据 */
    /* uint8_t[128] : app_data */
    for (i = 0; i < 128; ++i)
    {
        msg->app_data[i] = buffer[len];
        len += 1;
    }
    if (msglen < len)
    { 
        return -2;
    }

    return 0;
}

int VCB_VMB_update_data_response_serialization(uint8_t* buffer, void* message)
{
    int len = 3;
    VCB_VMB_UPDATE_DATA_RESPONSE_T* msg = (VCB_VMB_UPDATE_DATA_RESPONSE_T*)message;

    /* head */
    buffer[0] = VCB_VMB_ID_UPDATE_DATA_RESPONSE;

    len = 3;

    /* message body */
    /* 客户端的ID，CB的ID为2，ECC的ID为3 */
    /* uint8_t : client_id */
    buffer[len] = (msg->client_id);
    len += 1;
    
    /* 附加的状态信息，正常情况为0，非0为发生错误，代表错误码 */
    /* uint32_t : status */
    HOST_TO_NET4B(&(buffer[len]), msg->status);
    len += 4;
    
    /* 请求的序列号，如果状态正常，在收到序列号的基础上加1 */
    /* uint32_t : next_seq_id */
    HOST_TO_NET4B(&(buffer[len]), msg->next_seq_id);
    len += 4;
    
    /* message length */
    HOST_TO_NET2B(&buffer[1], len)

    return len;
}

int VCB_VMB_update_data_response_deserialization(uint8_t* buffer, void* message)
{
    int len = 3;
    int msglen = NET_TO_HOST2B(&buffer[1]);
    VCB_VMB_UPDATE_DATA_RESPONSE_T* msg = (VCB_VMB_UPDATE_DATA_RESPONSE_T*)message;

    /* construct message body */
    /* 客户端的ID，CB的ID为2，ECC的ID为3 */
    /* uint8_t : client_id */
    msg->client_id = buffer[len];
    len += 1;
    
    if (msglen < len)
    { 
        return -2;
    }

    /* 附加的状态信息，正常情况为0，非0为发生错误，代表错误码 */
    /* uint32_t : status */
    msg->status = NET_TO_HOST4B(&buffer[len]);
    len += 4;
    
    if (msglen < len)
    { 
        return -2;
    }

    /* 请求的序列号，如果状态正常，在收到序列号的基础上加1 */
    /* uint32_t : next_seq_id */
    msg->next_seq_id = NET_TO_HOST4B(&buffer[len]);
    len += 4;
    
    if (msglen < len)
    { 
        return -2;
    }

    return 0;
}
int VCB_VMB_vmb_monitoring_data_up_request_serialization(uint8_t* buffer, void* message)
{
    int len = 3;
    VCB_VMB_VMB_MONITORING_DATA_UP_REQUEST_T* msg = (VCB_VMB_VMB_MONITORING_DATA_UP_REQUEST_T*)message;

    /* head */
    buffer[0] = VCB_VMB_ID_VMB_MONITORING_DATA_UP_REQUEST;

    len = 3;

    /* message body */
    /* 气源压力监控数据上送请求，不关闭，无需发送 */
    /* macro : UPLOAD_ENABLE          0x01 // 上送使能（默认状态） */
    
    /* macro : UPLOAD_DISABLE         0x02 // 上送停止            */
    
    /* macro : ENERGIZE_SAFE_VALVE    0x04 // 激活安全阀 */
    
    /* macro : DE_ENERGIZE_SAFE_VALVE 0x08 // 激活安全阀 */
    
    /* uint8_t : status */
    buffer[len] = (msg->status);
    len += 1;
    
    /* message length */
    HOST_TO_NET2B(&buffer[1], len)

    return len;
}

int VCB_VMB_vmb_monitoring_data_up_request_deserialization(uint8_t* buffer, void* message)
{
    int len = 3;
    int msglen = NET_TO_HOST2B(&buffer[1]);
    VCB_VMB_VMB_MONITORING_DATA_UP_REQUEST_T* msg = (VCB_VMB_VMB_MONITORING_DATA_UP_REQUEST_T*)message;

    /* construct message body */
    /* 气源压力监控数据上送请求，不关闭，无需发送 */
    /* macro : UPLOAD_ENABLE          0x01 // 上送使能（默认状态） */
    
    /* macro : UPLOAD_DISABLE         0x02 // 上送停止            */
    
    /* macro : ENERGIZE_SAFE_VALVE    0x04 // 激活安全阀 */
    
    /* macro : DE_ENERGIZE_SAFE_VALVE 0x08 // 激活安全阀 */
    
    /* uint8_t : status */
    msg->status = buffer[len];
    len += 1;
    
    if (msglen < len)
    { 
        return -2;
    }

    return 0;
}

int VCB_VMB_vmb_monitoring_data_up_response_serialization(uint8_t* buffer, void* message)
{
    int len = 3;
    VCB_VMB_VMB_MONITORING_DATA_UP_RESPONSE_T* msg = (VCB_VMB_VMB_MONITORING_DATA_UP_RESPONSE_T*)message;

    /* head */
    buffer[0] = VCB_VMB_ID_VMB_MONITORING_DATA_UP_RESPONSE;

    len = 3;

    /* message body */
    /* 每200ms发送一次上送数据 */
    /* vmb系统的状态 */
    /* uint32_t : status_bits */
    HOST_TO_NET4B(&(buffer[len]), msg->status_bits);
    len += 4;
    
    /* int32_t : air_supply_pressure */
    HOST_TO_NET4B(&(buffer[len]), msg->air_supply_pressure);
    len += 4;
    
    /* int32_t : o2_supply_pressure */
    HOST_TO_NET4B(&(buffer[len]), msg->o2_supply_pressure);
    len += 4;
    
    /* int16_t : pht_pressure */
    HOST_TO_NET2B(&(buffer[len]), msg->pht_pressure);
    len += 2;
    
    /* int16_t : pht_temperature */
    HOST_TO_NET2B(&(buffer[len]), msg->pht_temperature);
    len += 2;
    
    /* int16_t : pht_humidity */
    HOST_TO_NET2B(&(buffer[len]), msg->pht_humidity);
    len += 2;
    
    /* IIC Error */
    /* uint32_t : exp_flow_error_count */
    HOST_TO_NET4B(&(buffer[len]), msg->exp_flow_error_count);
    len += 4;
    
    /* uint32_t : exp_flow_reset_count */
    HOST_TO_NET4B(&(buffer[len]), msg->exp_flow_reset_count);
    len += 4;
    
    /* uint32_t : pht_error_count */
    HOST_TO_NET4B(&(buffer[len]), msg->pht_error_count);
    len += 4;
    
    /* uint32_t : pht_reset_count */
    HOST_TO_NET4B(&(buffer[len]), msg->pht_reset_count);
    len += 4;
    
    /* message length */
    HOST_TO_NET2B(&buffer[1], len)

    return len;
}

int VCB_VMB_vmb_monitoring_data_up_response_deserialization(uint8_t* buffer, void* message)
{
    int len = 3;
    int msglen = NET_TO_HOST2B(&buffer[1]);
    VCB_VMB_VMB_MONITORING_DATA_UP_RESPONSE_T* msg = (VCB_VMB_VMB_MONITORING_DATA_UP_RESPONSE_T*)message;

    /* construct message body */
    /* 每200ms发送一次上送数据 */
    /* vmb系统的状态 */
    /* uint32_t : status_bits */
    msg->status_bits = NET_TO_HOST4B(&buffer[len]);
    len += 4;
    
    if (msglen < len)
    { 
        return -2;
    }

    /* int32_t : air_supply_pressure */
    msg->air_supply_pressure = NET_TO_HOST4B(&buffer[len]);
    len += 4;
    
    if (msglen < len)
    { 
        return -2;
    }

    /* int32_t : o2_supply_pressure */
    msg->o2_supply_pressure = NET_TO_HOST4B(&buffer[len]);
    len += 4;
    
    if (msglen < len)
    { 
        return -2;
    }

    /* int16_t : pht_pressure */
    msg->pht_pressure = NET_TO_HOST2B(&buffer[len]);
    len += 2;
    
    if (msglen < len)
    { 
        return -2;
    }

    /* int16_t : pht_temperature */
    msg->pht_temperature = NET_TO_HOST2B(&buffer[len]);
    len += 2;
    
    if (msglen < len)
    { 
        return -2;
    }

    /* int16_t : pht_humidity */
    msg->pht_humidity = NET_TO_HOST2B(&buffer[len]);
    len += 2;
    
    if (msglen < len)
    { 
        return -2;
    }

    /* IIC Error */
    /* uint32_t : exp_flow_error_count */
    msg->exp_flow_error_count = NET_TO_HOST4B(&buffer[len]);
    len += 4;
    
    if (msglen < len)
    { 
        return -2;
    }

    /* uint32_t : exp_flow_reset_count */
    msg->exp_flow_reset_count = NET_TO_HOST4B(&buffer[len]);
    len += 4;
    
    if (msglen < len)
    { 
        return -2;
    }

    /* uint32_t : pht_error_count */
    msg->pht_error_count = NET_TO_HOST4B(&buffer[len]);
    len += 4;
    
    if (msglen < len)
    { 
        return -2;
    }

    /* uint32_t : pht_reset_count */
    msg->pht_reset_count = NET_TO_HOST4B(&buffer[len]);
    len += 4;
    
    if (msglen < len)
    { 
        return -2;
    }

    return 0;
}

int VCB_VMB_vmb_exp_pressure_up_request_serialization(uint8_t* buffer, void* message)
{
    int len = 3;
    VCB_VMB_VMB_EXP_PRESSURE_UP_REQUEST_T* msg = (VCB_VMB_VMB_EXP_PRESSURE_UP_REQUEST_T*)message;

    /* head */
    buffer[0] = VCB_VMB_ID_VMB_EXP_PRESSURE_UP_REQUEST;

    len = 3;

    /* message body */
    /* 压力监控数据上送请求，不关闭，无需发送 */
    /* macro : UPLOAD_ENABLE          0x01 // 上送使能（默认状态） */
    
    /* macro : UPLOAD_DISABLE         0x02 // 上送停止            */
    
    /* uint8_t : status */
    buffer[len] = (msg->status);
    len += 1;
    
    /* message length */
    HOST_TO_NET2B(&buffer[1], len)

    return len;
}

int VCB_VMB_vmb_exp_pressure_up_request_deserialization(uint8_t* buffer, void* message)
{
    int len = 3;
    int msglen = NET_TO_HOST2B(&buffer[1]);
    VCB_VMB_VMB_EXP_PRESSURE_UP_REQUEST_T* msg = (VCB_VMB_VMB_EXP_PRESSURE_UP_REQUEST_T*)message;

    /* construct message body */
    /* 压力监控数据上送请求，不关闭，无需发送 */
    /* macro : UPLOAD_ENABLE          0x01 // 上送使能（默认状态） */
    
    /* macro : UPLOAD_DISABLE         0x02 // 上送停止            */
    
    /* uint8_t : status */
    msg->status = buffer[len];
    len += 1;
    
    if (msglen < len)
    { 
        return -2;
    }

    return 0;
}

int VCB_VMB_vmb_exp_pressure_up_response_serialization(uint8_t* buffer, void* message)
{
    int len = 3;
    VCB_VMB_VMB_EXP_PRESSURE_UP_RESPONSE_T* msg = (VCB_VMB_VMB_EXP_PRESSURE_UP_RESPONSE_T*)message;

    /* head */
    buffer[0] = VCB_VMB_ID_VMB_EXP_PRESSURE_UP_RESPONSE;

    len = 3;

    /* message body */
    /* 每2ms发送一次上送数据 */
    /* AD7689BCPZRL7 -> MPXV2010GP  分辨率: 16bit  0 kPa to 10 kPa 准确性：0.5 % */
    /* int32_t : exp_pressure */
    HOST_TO_NET4B(&(buffer[len]), msg->exp_pressure);
    len += 4;
    
    /* ml/min */
    /* int32_t : exp_flow_rate */
    HOST_TO_NET4B(&(buffer[len]), msg->exp_flow_rate);
    len += 4;
    
    /* message length */
    HOST_TO_NET2B(&buffer[1], len)

    return len;
}

int VCB_VMB_vmb_exp_pressure_up_response_deserialization(uint8_t* buffer, void* message)
{
    int len = 3;
    int msglen = NET_TO_HOST2B(&buffer[1]);
    VCB_VMB_VMB_EXP_PRESSURE_UP_RESPONSE_T* msg = (VCB_VMB_VMB_EXP_PRESSURE_UP_RESPONSE_T*)message;

    /* construct message body */
    /* 每2ms发送一次上送数据 */
    /* AD7689BCPZRL7 -> MPXV2010GP  分辨率: 16bit  0 kPa to 10 kPa 准确性：0.5 % */
    /* int32_t : exp_pressure */
    msg->exp_pressure = NET_TO_HOST4B(&buffer[len]);
    len += 4;
    
    if (msglen < len)
    { 
        return -2;
    }

    /* ml/min */
    /* int32_t : exp_flow_rate */
    msg->exp_flow_rate = NET_TO_HOST4B(&buffer[len]);
    len += 4;
    
    if (msglen < len)
    { 
        return -2;
    }

    return 0;
}

int VCB_VMB_vmb_test_data_request_serialization(uint8_t* buffer, void* message)
{
    int len = 3;
    VCB_VMB_VMB_TEST_DATA_REQUEST_T* msg = (VCB_VMB_VMB_TEST_DATA_REQUEST_T*)message;

    /* head */
    buffer[0] = VCB_VMB_ID_VMB_TEST_DATA_REQUEST;

    len = 3;

    /* message body */
    /* 测试数据上送请求，测试期间使用 */
    /* 有效消息的高字节必须为0xA5 */
    /* macro : STATUS_VALID_MASK     0xA500 */
    
    /* uint16_t : status */
    HOST_TO_NET2B(&(buffer[len]), msg->status);
    len += 2;
    
    /* message length */
    HOST_TO_NET2B(&buffer[1], len)

    return len;
}

int VCB_VMB_vmb_test_data_request_deserialization(uint8_t* buffer, void* message)
{
    int len = 3;
    int msglen = NET_TO_HOST2B(&buffer[1]);
    VCB_VMB_VMB_TEST_DATA_REQUEST_T* msg = (VCB_VMB_VMB_TEST_DATA_REQUEST_T*)message;

    /* construct message body */
    /* 测试数据上送请求，测试期间使用 */
    /* 有效消息的高字节必须为0xA5 */
    /* macro : STATUS_VALID_MASK     0xA500 */
    
    /* uint16_t : status */
    msg->status = NET_TO_HOST2B(&buffer[len]);
    len += 2;
    
    if (msglen < len)
    { 
        return -2;
    }

    return 0;
}

int VCB_VMB_vmb_test_data_response_serialization(uint8_t* buffer, void* message)
{
    int i = 0;
    int len = 3;
    VCB_VMB_VMB_TEST_DATA_RESPONSE_T* msg = (VCB_VMB_VMB_TEST_DATA_RESPONSE_T*)message;

    /* head */
    buffer[0] = VCB_VMB_ID_VMB_TEST_DATA_RESPONSE;

    len = 3;

    /* message body */
    /* 必须收到请求才会发送 */
    /* uint8_t : id */
    buffer[len] = (msg->id);
    len += 1;
    
    /* uint8_t[32] : data */
    for (i = 0; i < 32; ++i)
    {
        buffer[len] = (msg->data[i]);
        len += 1;
    }

    /* message length */
    HOST_TO_NET2B(&buffer[1], len)

    return len;
}

int VCB_VMB_vmb_test_data_response_deserialization(uint8_t* buffer, void* message)
{
    int i = 0;
    int len = 3;
    int msglen = NET_TO_HOST2B(&buffer[1]);
    VCB_VMB_VMB_TEST_DATA_RESPONSE_T* msg = (VCB_VMB_VMB_TEST_DATA_RESPONSE_T*)message;

    /* construct message body */
    /* 必须收到请求才会发送 */
    /* uint8_t : id */
    msg->id = buffer[len];
    len += 1;
    
    if (msglen < len)
    { 
        return -2;
    }

    /* uint8_t[32] : data */
    for (i = 0; i < 32; ++i)
    {
        msg->data[i] = buffer[len];
        len += 1;
    }
    if (msglen < len)
    { 
        return -2;
    }

    return 0;
}

int VCB_VMB_message_stuff(uint8_t id, void* message, uint8_t* send_buffer)
{
    int i = 0;
    int send_len = 0;
    int len = 0;
    uint16_t crc = 0;
    static uint8_t raw_buffer[VCB_VMB_MSG_MESSAGE_BUF_LEN];

    switch (id)
    {

    case VCB_VMB_ID_STATUS_REQUEST:
        len = VCB_VMB_status_request_serialization(raw_buffer, message);
        break;
    case VCB_VMB_ID_STATUS_RESPONSE:
        len = VCB_VMB_status_response_serialization(raw_buffer, message);
        break;
    case VCB_VMB_ID_UPDATE_DATA_REQUEST:
        len = VCB_VMB_update_data_request_serialization(raw_buffer, message);
        break;
    case VCB_VMB_ID_UPDATE_DATA_RESPONSE:
        len = VCB_VMB_update_data_response_serialization(raw_buffer, message);
        break;
    case VCB_VMB_ID_VMB_MONITORING_DATA_UP_REQUEST:
        len = VCB_VMB_vmb_monitoring_data_up_request_serialization(raw_buffer, message);
        break;
    case VCB_VMB_ID_VMB_MONITORING_DATA_UP_RESPONSE:
        len = VCB_VMB_vmb_monitoring_data_up_response_serialization(raw_buffer, message);
        break;
    case VCB_VMB_ID_VMB_EXP_PRESSURE_UP_REQUEST:
        len = VCB_VMB_vmb_exp_pressure_up_request_serialization(raw_buffer, message);
        break;
    case VCB_VMB_ID_VMB_EXP_PRESSURE_UP_RESPONSE:
        len = VCB_VMB_vmb_exp_pressure_up_response_serialization(raw_buffer, message);
        break;
    case VCB_VMB_ID_VMB_TEST_DATA_REQUEST:
        len = VCB_VMB_vmb_test_data_request_serialization(raw_buffer, message);
        break;
    case VCB_VMB_ID_VMB_TEST_DATA_RESPONSE:
        len = VCB_VMB_vmb_test_data_response_serialization(raw_buffer, message);
        break;

    default:
        return -1;
    }

    crc = VCB_VMB_msg_calc_crc(raw_buffer, len);
    HOST_TO_NET2B(&raw_buffer[len], crc);
    len += 2;
    for (i = 0; i < len; ++i)
    {
        if (raw_buffer[i] == VCB_VMB_MSG_END_FLAG)
        {
            send_buffer[send_len++] = VCB_VMB_MSG_ESC_CHAR;
            send_buffer[send_len++] = VCB_VMB_MSG_ESC_7E;
        }
        else if (raw_buffer[i] == VCB_VMB_MSG_ESC_CHAR)
        {
            send_buffer[send_len++] = VCB_VMB_MSG_ESC_CHAR;
            send_buffer[send_len++] = VCB_VMB_MSG_ESC_5C;
        }
        else
        {
            send_buffer[send_len++] = raw_buffer[i];
        }
    }
    send_buffer[send_len++] = VCB_VMB_MSG_END_FLAG;

    return send_len;
}

int VCB_VMB_message_unstuff(uint8_t* buffer, int len, uint8_t *id)
{
    int i = 0;
    int ret = 0;
    int length = 0;
    uint16_t crc = 0;

    for (i = 0; i < len; ++i)
    {
        if (buffer[i] == VCB_VMB_MSG_ESC_CHAR)
        {
            buffer[length++] = buffer[i] | buffer[i + 1];
            ++i;
        }
        else
        {
            buffer[length++] = buffer[i];
        }
    }

    crc = VCB_VMB_msg_calc_crc(buffer, length - 2);
    // (((buffer[length - 2] << 8) & 0xff00) | buffer[length - 1]))
    if (crc != NET_TO_HOST2B(&buffer[length - 2]))  
    {
        return -1;
    }
    *id = buffer[0];

    return ret;
}

int VCB_VMB_receive(uint8_t *rawBuf, int len)
{
	int ret = 0;
	uint8_t msg_id = 0;
	static uint8_t msgbuf[VCB_VMB_MSG_MESSAGE_BUF_LEN];

    if (len < 5)
    {
        ++VCB_VMB_UNKNOWN_HANDER_CB.recv_e_count;;
        return -1;
    }

    VCB_VMB_MSG_DO_PRO_PRECESSING

	ret = VCB_VMB_message_unstuff(rawBuf, len, &msg_id);
	if (ret != 0)
	{
        ++VCB_VMB_UNKNOWN_HANDER_CB.recv_e_count;
		return ret;
	}
	switch (msg_id)
	{

    case VCB_VMB_ID_STATUS_REQUEST:
        len = VCB_VMB_status_request_deserialization(rawBuf, msgbuf);
        /* Message Process */
        VCB_VMB_CALL_RECV_MSG_PROCESS(VCB_VMB_ID_STATUS_REQUEST, msgbuf, len);
        /* Message Process End */
        break;
    case VCB_VMB_ID_STATUS_RESPONSE:
        len = VCB_VMB_status_response_deserialization(rawBuf, msgbuf);
        /* Message Process */
        VCB_VMB_CALL_RECV_MSG_PROCESS(VCB_VMB_ID_STATUS_RESPONSE, msgbuf, len);
        /* Message Process End */
        break;
    case VCB_VMB_ID_UPDATE_DATA_REQUEST:
        len = VCB_VMB_update_data_request_deserialization(rawBuf, msgbuf);
        /* Message Process */
        VCB_VMB_CALL_RECV_MSG_PROCESS(VCB_VMB_ID_UPDATE_DATA_REQUEST, msgbuf, len);
        /* Message Process End */
        break;
    case VCB_VMB_ID_UPDATE_DATA_RESPONSE:
        len = VCB_VMB_update_data_response_deserialization(rawBuf, msgbuf);
        /* Message Process */
        VCB_VMB_CALL_RECV_MSG_PROCESS(VCB_VMB_ID_UPDATE_DATA_RESPONSE, msgbuf, len);
        /* Message Process End */
        break;
    case VCB_VMB_ID_VMB_MONITORING_DATA_UP_REQUEST:
        len = VCB_VMB_vmb_monitoring_data_up_request_deserialization(rawBuf, msgbuf);
        /* Message Process */
        VCB_VMB_CALL_RECV_MSG_PROCESS(VCB_VMB_ID_VMB_MONITORING_DATA_UP_REQUEST, msgbuf, len);
        /* Message Process End */
        break;
    case VCB_VMB_ID_VMB_MONITORING_DATA_UP_RESPONSE:
        len = VCB_VMB_vmb_monitoring_data_up_response_deserialization(rawBuf, msgbuf);
        /* Message Process */
        VCB_VMB_CALL_RECV_MSG_PROCESS(VCB_VMB_ID_VMB_MONITORING_DATA_UP_RESPONSE, msgbuf, len);
        /* Message Process End */
        break;
    case VCB_VMB_ID_VMB_EXP_PRESSURE_UP_REQUEST:
        len = VCB_VMB_vmb_exp_pressure_up_request_deserialization(rawBuf, msgbuf);
        /* Message Process */
        VCB_VMB_CALL_RECV_MSG_PROCESS(VCB_VMB_ID_VMB_EXP_PRESSURE_UP_REQUEST, msgbuf, len);
        /* Message Process End */
        break;
    case VCB_VMB_ID_VMB_EXP_PRESSURE_UP_RESPONSE:
        len = VCB_VMB_vmb_exp_pressure_up_response_deserialization(rawBuf, msgbuf);
        /* Message Process */
        VCB_VMB_CALL_RECV_MSG_PROCESS(VCB_VMB_ID_VMB_EXP_PRESSURE_UP_RESPONSE, msgbuf, len);
        /* Message Process End */
        break;
    case VCB_VMB_ID_VMB_TEST_DATA_REQUEST:
        len = VCB_VMB_vmb_test_data_request_deserialization(rawBuf, msgbuf);
        /* Message Process */
        VCB_VMB_CALL_RECV_MSG_PROCESS(VCB_VMB_ID_VMB_TEST_DATA_REQUEST, msgbuf, len);
        /* Message Process End */
        break;
    case VCB_VMB_ID_VMB_TEST_DATA_RESPONSE:
        len = VCB_VMB_vmb_test_data_response_deserialization(rawBuf, msgbuf);
        /* Message Process */
        VCB_VMB_CALL_RECV_MSG_PROCESS(VCB_VMB_ID_VMB_TEST_DATA_RESPONSE, msgbuf, len);
        /* Message Process End */
        break;
    default:
		++VCB_VMB_UNKNOWN_HANDER_CB.recv_count;
        break;
	}
	return 0;
}



void VCB_VMB_msg_preprocessing_hook_register(int (*fun)(uint8_t*, int))
{
    g_vcb_vmb_msg_preprocess_hook = fun;
}

// Send Function for bootloader

int VCB_VMB_send_status_request(VCB_VMB_STATUS_REQUEST_T* msg)
{
    int ret = 0;
    uint8_t buf[VCB_VMB_MSG_MESSAGE_BUF_LEN] = {0};

    ret = VCB_VMB_message_stuff(VCB_VMB_ID_STATUS_REQUEST, msg, buf);
    if (ret > 0)
    {
        ret = VCB_VMB_msg_uart_send(buf, ret);
        VCB_VMB_SEND_COUNT(VCB_VMB_ID_STATUS_REQUEST, ret);
    }
    return ret;
}

int VCB_VMB_send_status_response(VCB_VMB_STATUS_RESPONSE_T* msg)
{
    int ret = 0;
    uint8_t buf[VCB_VMB_MSG_MESSAGE_BUF_LEN] = {0};

    ret = VCB_VMB_message_stuff(VCB_VMB_ID_STATUS_RESPONSE, msg, buf);
    if (ret > 0)
    {
        ret = VCB_VMB_msg_uart_send(buf, ret);
        VCB_VMB_SEND_COUNT(VCB_VMB_ID_STATUS_RESPONSE, ret);
    }
    return ret;
}

int VCB_VMB_send_update_data_request(VCB_VMB_UPDATE_DATA_REQUEST_T* msg)
{
    int ret = 0;
    uint8_t buf[VCB_VMB_MSG_MESSAGE_BUF_LEN] = {0};

    ret = VCB_VMB_message_stuff(VCB_VMB_ID_UPDATE_DATA_REQUEST, msg, buf);
    if (ret > 0)
    {
        ret = VCB_VMB_msg_uart_send(buf, ret);
        VCB_VMB_SEND_COUNT(VCB_VMB_ID_UPDATE_DATA_REQUEST, ret);
    }
    return ret;
}

int VCB_VMB_send_update_data_response(VCB_VMB_UPDATE_DATA_RESPONSE_T* msg)
{
    int ret = 0;
    uint8_t buf[VCB_VMB_MSG_MESSAGE_BUF_LEN] = {0};

    ret = VCB_VMB_message_stuff(VCB_VMB_ID_UPDATE_DATA_RESPONSE, msg, buf);
    if (ret > 0)
    {
        ret = VCB_VMB_msg_uart_send(buf, ret);
        VCB_VMB_SEND_COUNT(VCB_VMB_ID_UPDATE_DATA_RESPONSE, ret);
    }
    return ret;
}


// Send Function
int VCB_VMB_send_vmb_monitoring_data_up_request(VCB_VMB_VMB_MONITORING_DATA_UP_REQUEST_T* msg)
{
    int ret = 0;
    uint8_t buf[VCB_VMB_MSG_MESSAGE_BUF_LEN] = {0};

    ret = VCB_VMB_message_stuff(VCB_VMB_ID_VMB_MONITORING_DATA_UP_REQUEST, msg, buf);
    if (ret > 0)
    {
        ret = VCB_VMB_msg_uart_send(buf, ret);
        VCB_VMB_SEND_COUNT(VCB_VMB_ID_VMB_MONITORING_DATA_UP_REQUEST, ret);
    }
    return ret;
}

int VCB_VMB_send_vmb_monitoring_data_up_response(VCB_VMB_VMB_MONITORING_DATA_UP_RESPONSE_T* msg)
{
    int ret = 0;
    uint8_t buf[VCB_VMB_MSG_MESSAGE_BUF_LEN] = {0};

    ret = VCB_VMB_message_stuff(VCB_VMB_ID_VMB_MONITORING_DATA_UP_RESPONSE, msg, buf);
    if (ret > 0)
    {
        ret = VCB_VMB_msg_uart_send(buf, ret);
        VCB_VMB_SEND_COUNT(VCB_VMB_ID_VMB_MONITORING_DATA_UP_RESPONSE, ret);
    }
    return ret;
}

int VCB_VMB_send_vmb_exp_pressure_up_request(VCB_VMB_VMB_EXP_PRESSURE_UP_REQUEST_T* msg)
{
    int ret = 0;
    uint8_t buf[VCB_VMB_MSG_MESSAGE_BUF_LEN] = {0};

    ret = VCB_VMB_message_stuff(VCB_VMB_ID_VMB_EXP_PRESSURE_UP_REQUEST, msg, buf);
    if (ret > 0)
    {
        ret = VCB_VMB_msg_uart_send(buf, ret);
        VCB_VMB_SEND_COUNT(VCB_VMB_ID_VMB_EXP_PRESSURE_UP_REQUEST, ret);
    }
    return ret;
}

int VCB_VMB_send_vmb_exp_pressure_up_response(VCB_VMB_VMB_EXP_PRESSURE_UP_RESPONSE_T* msg)
{
    int ret = 0;
    uint8_t buf[VCB_VMB_MSG_MESSAGE_BUF_LEN] = {0};

    ret = VCB_VMB_message_stuff(VCB_VMB_ID_VMB_EXP_PRESSURE_UP_RESPONSE, msg, buf);
    if (ret > 0)
    {
        ret = VCB_VMB_msg_uart_send(buf, ret);
        VCB_VMB_SEND_COUNT(VCB_VMB_ID_VMB_EXP_PRESSURE_UP_RESPONSE, ret);
    }
    return ret;
}

int VCB_VMB_send_vmb_test_data_request(VCB_VMB_VMB_TEST_DATA_REQUEST_T* msg)
{
    int ret = 0;
    uint8_t buf[VCB_VMB_MSG_MESSAGE_BUF_LEN] = {0};

    ret = VCB_VMB_message_stuff(VCB_VMB_ID_VMB_TEST_DATA_REQUEST, msg, buf);
    if (ret > 0)
    {
        ret = VCB_VMB_msg_uart_send(buf, ret);
        VCB_VMB_SEND_COUNT(VCB_VMB_ID_VMB_TEST_DATA_REQUEST, ret);
    }
    return ret;
}

int VCB_VMB_send_vmb_test_data_response(VCB_VMB_VMB_TEST_DATA_RESPONSE_T* msg)
{
    int ret = 0;
    uint8_t buf[VCB_VMB_MSG_MESSAGE_BUF_LEN] = {0};

    ret = VCB_VMB_message_stuff(VCB_VMB_ID_VMB_TEST_DATA_RESPONSE, msg, buf);
    if (ret > 0)
    {
        ret = VCB_VMB_msg_uart_send(buf, ret);
        VCB_VMB_SEND_COUNT(VCB_VMB_ID_VMB_TEST_DATA_RESPONSE, ret);
    }
    return ret;
}

