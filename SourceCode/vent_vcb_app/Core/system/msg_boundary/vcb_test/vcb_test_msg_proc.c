/* 自动生成文件，请勿修改 ！！！ Dawei.Zhou */
#include <stddef.h>
#include "vcb_test_msg_proc.h"

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
    
VCB_TEST_MSG_HANDLER_CB_T g_vcb_test_msg_handler_map[] = {
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
    {NULL, 0, 0, 0, 0}, // VCB_TEST_ID_VCB_CONFIGURE_REQUESET
    {NULL, 0, 0, 0, 0}, // VCB_TEST_ID_VCB_CONFIGURE_RESPONSE
    {NULL, 0, 0, 0, 0}, // VCB_TEST_ID_VCB_MONITORING_DATA_UP_REQUEST
    {NULL, 0, 0, 0, 0}, // VCB_TEST_ID_VCB_MONITORING_DATA_UP_RESPONSE
    {NULL, 0, 0, 0, 0}  // Unknown(Error Type)
};
#define VCB_TEST_GET_HANDER_CB(_ID)         g_vcb_test_msg_handler_map[(_ID)-VCB_TEST_MESSAGE_ID_BASE]
#define VCB_TEST_MSG_HANDLER_COUNT           (sizeof(g_vcb_test_msg_handler_map)/sizeof(g_vcb_test_msg_handler_map[0]))
#define VCB_TEST_SEND_COUNT(_ID, _ret)      if ((_ret) == 0) { ++VCB_TEST_GET_HANDER_CB(_ID).send_count; }else { ++VCB_TEST_GET_HANDER_CB(_ID).send_e_count; }
#define VCB_TEST_UNKNOWN_HANDER_CB          g_vcb_test_msg_handler_map[VCB_TEST_MSG_HANDLER_COUNT-1]

#define VCB_TEST_CALL_RECV_MSG_PROCESS(_ID, _buf, _ret) do { \
            if ((_ret) == 0) \
            { \
                if (VCB_TEST_GET_HANDER_CB(_ID).handler != NULL) { VCB_TEST_GET_HANDER_CB(_ID).handler(_buf); } \
                ++VCB_TEST_GET_HANDER_CB(_ID).recv_count; \
            } \
            else \
            { \
                ++VCB_TEST_GET_HANDER_CB(_ID).recv_e_count; \
            } \
        } while(0) 

uint32_t g_vcb_test_msg_handler_map_count = VCB_TEST_MSG_HANDLER_COUNT; 


// 消息由外部回调函数处理，无需本子系统继续处理
#define VCB_TEST_MSG_PREPROCESS_CONTINUE  (0x10)    //  消息由外部回调函数处理，无需继续处理
int   (*g_vcb_test_msg_preprocess_hook)(uint8_t*, int) = NULL;

#define VCB_TEST_MSG_DO_PRO_PRECESSING \
    if (g_vcb_test_msg_preprocess_hook != NULL) \
    { \
        if (VCB_TEST_MSG_PREPROCESS_CONTINUE == g_vcb_test_msg_preprocess_hook(rawBuf, len + 1)) \
        { \
            return 0; \
        } \
    }

int VCB_TEST_msg_recv_handler_register(VCB_TEST_MSG_HANDLER_T fun, uint32_t msg_id)
{
    if ((msg_id >= VCB_TEST_MESSAGE_ID_BASE) &&
        (msg_id < (VCB_TEST_MESSAGE_ID_BASE + VCB_TEST_MSG_HANDLER_COUNT)))
    {
        VCB_TEST_GET_HANDER_CB(msg_id).handler = fun;
        VCB_TEST_GET_HANDER_CB(msg_id).recv_count   = 0;
        VCB_TEST_GET_HANDER_CB(msg_id).recv_e_count = 0;
        VCB_TEST_GET_HANDER_CB(msg_id).send_count   = 0;
        VCB_TEST_GET_HANDER_CB(msg_id).send_e_count = 0;
        return 0;
    }
    return 1;
}


int VCB_TEST_status_request_serialization(uint8_t* buffer, void* message)
{
    int i = 0;
    int len = 3;
    VCB_TEST_STATUS_REQUEST_T* msg = (VCB_TEST_STATUS_REQUEST_T*)message;

    /* head */
    buffer[0] = VCB_TEST_ID_STATUS_REQUEST;

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

int VCB_TEST_status_request_deserialization(uint8_t* buffer, void* message)
{
    int i = 0;
    int len = 3;
    int msglen = NET_TO_HOST2B(&buffer[1]);
    VCB_TEST_STATUS_REQUEST_T* msg = (VCB_TEST_STATUS_REQUEST_T*)message;

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

int VCB_TEST_status_response_serialization(uint8_t* buffer, void* message)
{
    int i = 0;
    int len = 3;
    VCB_TEST_STATUS_RESPONSE_T* msg = (VCB_TEST_STATUS_RESPONSE_T*)message;

    /* head */
    buffer[0] = VCB_TEST_ID_STATUS_RESPONSE;

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

int VCB_TEST_status_response_deserialization(uint8_t* buffer, void* message)
{
    int i = 0;
    int len = 3;
    int msglen = NET_TO_HOST2B(&buffer[1]);
    VCB_TEST_STATUS_RESPONSE_T* msg = (VCB_TEST_STATUS_RESPONSE_T*)message;

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

int VCB_TEST_update_data_request_serialization(uint8_t* buffer, void* message)
{
    int i = 0;
    int len = 3;
    VCB_TEST_UPDATE_DATA_REQUEST_T* msg = (VCB_TEST_UPDATE_DATA_REQUEST_T*)message;

    /* head */
    buffer[0] = VCB_TEST_ID_UPDATE_DATA_REQUEST;

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

int VCB_TEST_update_data_request_deserialization(uint8_t* buffer, void* message)
{
    int i = 0;
    int len = 3;
    int msglen = NET_TO_HOST2B(&buffer[1]);
    VCB_TEST_UPDATE_DATA_REQUEST_T* msg = (VCB_TEST_UPDATE_DATA_REQUEST_T*)message;

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

int VCB_TEST_update_data_response_serialization(uint8_t* buffer, void* message)
{
    int len = 3;
    VCB_TEST_UPDATE_DATA_RESPONSE_T* msg = (VCB_TEST_UPDATE_DATA_RESPONSE_T*)message;

    /* head */
    buffer[0] = VCB_TEST_ID_UPDATE_DATA_RESPONSE;

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

int VCB_TEST_update_data_response_deserialization(uint8_t* buffer, void* message)
{
    int len = 3;
    int msglen = NET_TO_HOST2B(&buffer[1]);
    VCB_TEST_UPDATE_DATA_RESPONSE_T* msg = (VCB_TEST_UPDATE_DATA_RESPONSE_T*)message;

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
int VCB_TEST_vcb_configure_requeset_serialization(uint8_t* buffer, void* message)
{
    int i = 0;
    int len = 3;
    VCB_TEST_VCB_CONFIGURE_REQUESET_T* msg = (VCB_TEST_VCB_CONFIGURE_REQUESET_T*)message;

    /* head */
    buffer[0] = VCB_TEST_ID_VCB_CONFIGURE_REQUESET;

    len = 3;

    /* message body */
    /* 不定义具体字段 */
    /* int8_t[64] : raw_data */
    for (i = 0; i < 64; ++i)
    {
        buffer[len] = (msg->raw_data[i]);
        len += 1;
    }

    /* uint32_t : version */
    HOST_TO_NET4B(&(buffer[len]), msg->version);
    len += 4;
    
    /* message length */
    HOST_TO_NET2B(&buffer[1], len)

    return len;
}

int VCB_TEST_vcb_configure_requeset_deserialization(uint8_t* buffer, void* message)
{
    int i = 0;
    int len = 3;
    int msglen = NET_TO_HOST2B(&buffer[1]);
    VCB_TEST_VCB_CONFIGURE_REQUESET_T* msg = (VCB_TEST_VCB_CONFIGURE_REQUESET_T*)message;

    /* construct message body */
    /* 不定义具体字段 */
    /* int8_t[64] : raw_data */
    for (i = 0; i < 64; ++i)
    {
        msg->raw_data[i] = buffer[len];
        len += 1;
    }
    if (msglen < len)
    { 
        return -2;
    }

    /* uint32_t : version */
    msg->version = NET_TO_HOST4B(&buffer[len]);
    len += 4;
    
    if (msglen < len)
    { 
        return -2;
    }

    return 0;
}

int VCB_TEST_vcb_configure_response_serialization(uint8_t* buffer, void* message)
{
    int len = 3;
    VCB_TEST_VCB_CONFIGURE_RESPONSE_T* msg = (VCB_TEST_VCB_CONFIGURE_RESPONSE_T*)message;

    /* head */
    buffer[0] = VCB_TEST_ID_VCB_CONFIGURE_RESPONSE;

    len = 3;

    /* message body */
    /* uint32_t : version */
    HOST_TO_NET4B(&(buffer[len]), msg->version);
    len += 4;
    
    /* message length */
    HOST_TO_NET2B(&buffer[1], len)

    return len;
}

int VCB_TEST_vcb_configure_response_deserialization(uint8_t* buffer, void* message)
{
    int len = 3;
    int msglen = NET_TO_HOST2B(&buffer[1]);
    VCB_TEST_VCB_CONFIGURE_RESPONSE_T* msg = (VCB_TEST_VCB_CONFIGURE_RESPONSE_T*)message;

    /* construct message body */
    /* uint32_t : version */
    msg->version = NET_TO_HOST4B(&buffer[len]);
    len += 4;
    
    if (msglen < len)
    { 
        return -2;
    }

    return 0;
}

int VCB_TEST_vcb_monitoring_data_up_request_serialization(uint8_t* buffer, void* message)
{
    int len = 3;
    VCB_TEST_VCB_MONITORING_DATA_UP_REQUEST_T* msg = (VCB_TEST_VCB_MONITORING_DATA_UP_REQUEST_T*)message;

    /* head */
    buffer[0] = VCB_TEST_ID_VCB_MONITORING_DATA_UP_REQUEST;

    len = 3;

    /* message body */
    /* 监控数据上送请求，不关闭，无需发送 */
    /* macro : UPLOAD_ENABLE          0x01 // 上送使能（默认状态） */
    
    /* macro : UPLOAD_DISABLE         0x02 // 上送停止            */
    
    /* uint8_t : status */
    buffer[len] = (msg->status);
    len += 1;
    
    /* message length */
    HOST_TO_NET2B(&buffer[1], len)

    return len;
}

int VCB_TEST_vcb_monitoring_data_up_request_deserialization(uint8_t* buffer, void* message)
{
    int len = 3;
    int msglen = NET_TO_HOST2B(&buffer[1]);
    VCB_TEST_VCB_MONITORING_DATA_UP_REQUEST_T* msg = (VCB_TEST_VCB_MONITORING_DATA_UP_REQUEST_T*)message;

    /* construct message body */
    /* 监控数据上送请求，不关闭，无需发送 */
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

int VCB_TEST_vcb_monitoring_data_up_response_serialization(uint8_t* buffer, void* message)
{
    int i = 0;
    int len = 3;
    VCB_TEST_VCB_MONITORING_DATA_UP_RESPONSE_T* msg = (VCB_TEST_VCB_MONITORING_DATA_UP_RESPONSE_T*)message;

    /* head */
    buffer[0] = VCB_TEST_ID_VCB_MONITORING_DATA_UP_RESPONSE;

    len = 3;

    /* message body */
    /* 不区分数据格式，16个int32_t */
    /* int8_t[80] : raw_data */
    for (i = 0; i < 80; ++i)
    {
        buffer[len] = (msg->raw_data[i]);
        len += 1;
    }

    /* message length */
    HOST_TO_NET2B(&buffer[1], len)

    return len;
}

int VCB_TEST_vcb_monitoring_data_up_response_deserialization(uint8_t* buffer, void* message)
{
    int i = 0;
    int len = 3;
    int msglen = NET_TO_HOST2B(&buffer[1]);
    VCB_TEST_VCB_MONITORING_DATA_UP_RESPONSE_T* msg = (VCB_TEST_VCB_MONITORING_DATA_UP_RESPONSE_T*)message;

    /* construct message body */
    /* 不区分数据格式，16个int32_t */
    /* int8_t[80] : raw_data */
    for (i = 0; i < 80; ++i)
    {
        msg->raw_data[i] = buffer[len];
        len += 1;
    }
    if (msglen < len)
    { 
        return -2;
    }

    return 0;
}

int VCB_TEST_message_stuff(uint8_t id, void* message, uint8_t* send_buffer)
{
    int i = 0;
    int send_len = 0;
    int len = 0;
    uint16_t crc = 0;
    static uint8_t raw_buffer[VCB_TEST_MSG_MESSAGE_BUF_LEN];

    switch (id)
    {

    case VCB_TEST_ID_STATUS_REQUEST:
        len = VCB_TEST_status_request_serialization(raw_buffer, message);
        break;
    case VCB_TEST_ID_STATUS_RESPONSE:
        len = VCB_TEST_status_response_serialization(raw_buffer, message);
        break;
    case VCB_TEST_ID_UPDATE_DATA_REQUEST:
        len = VCB_TEST_update_data_request_serialization(raw_buffer, message);
        break;
    case VCB_TEST_ID_UPDATE_DATA_RESPONSE:
        len = VCB_TEST_update_data_response_serialization(raw_buffer, message);
        break;
    case VCB_TEST_ID_VCB_CONFIGURE_REQUESET:
        len = VCB_TEST_vcb_configure_requeset_serialization(raw_buffer, message);
        break;
    case VCB_TEST_ID_VCB_CONFIGURE_RESPONSE:
        len = VCB_TEST_vcb_configure_response_serialization(raw_buffer, message);
        break;
    case VCB_TEST_ID_VCB_MONITORING_DATA_UP_REQUEST:
        len = VCB_TEST_vcb_monitoring_data_up_request_serialization(raw_buffer, message);
        break;
    case VCB_TEST_ID_VCB_MONITORING_DATA_UP_RESPONSE:
        len = VCB_TEST_vcb_monitoring_data_up_response_serialization(raw_buffer, message);
        break;

    default:
        return -1;
    }

    crc = VCB_TEST_msg_calc_crc(raw_buffer, len);
    HOST_TO_NET2B(&raw_buffer[len], crc);
    len += 2;
    for (i = 0; i < len; ++i)
    {
        if (raw_buffer[i] == VCB_TEST_MSG_END_FLAG)
        {
            send_buffer[send_len++] = VCB_TEST_MSG_ESC_CHAR;
            send_buffer[send_len++] = VCB_TEST_MSG_ESC_7E;
        }
        else if (raw_buffer[i] == VCB_TEST_MSG_ESC_CHAR)
        {
            send_buffer[send_len++] = VCB_TEST_MSG_ESC_CHAR;
            send_buffer[send_len++] = VCB_TEST_MSG_ESC_5C;
        }
        else
        {
            send_buffer[send_len++] = raw_buffer[i];
        }
    }
    send_buffer[send_len++] = VCB_TEST_MSG_END_FLAG;

    return send_len;
}

int VCB_TEST_message_unstuff(uint8_t* buffer, int len, uint8_t *id)
{
    int i = 0;
    int ret = 0;
    int length = 0;
    uint16_t crc = 0;

    for (i = 0; i < len; ++i)
    {
        if (buffer[i] == VCB_TEST_MSG_ESC_CHAR)
        {
            buffer[length++] = buffer[i] | buffer[i + 1];
            ++i;
        }
        else
        {
            buffer[length++] = buffer[i];
        }
    }

    crc = VCB_TEST_msg_calc_crc(buffer, length - 2);
    // (((buffer[length - 2] << 8) & 0xff00) | buffer[length - 1]))
    if (crc != NET_TO_HOST2B(&buffer[length - 2]))  
    {
        return -1;
    }
    *id = buffer[0];

    return ret;
}

int VCB_TEST_receive(uint8_t *rawBuf, int len)
{
	int ret = 0;
	uint8_t msg_id = 0;
	static uint8_t msgbuf[VCB_TEST_MSG_MESSAGE_BUF_LEN];

    if (len < 5)
    {
        ++VCB_TEST_UNKNOWN_HANDER_CB.recv_e_count;;
        return -1;
    }

    VCB_TEST_MSG_DO_PRO_PRECESSING

	ret = VCB_TEST_message_unstuff(rawBuf, len, &msg_id);
	if (ret != 0)
	{
        ++VCB_TEST_UNKNOWN_HANDER_CB.recv_e_count;
		return ret;
	}
	switch (msg_id)
	{

    case VCB_TEST_ID_STATUS_REQUEST:
        len = VCB_TEST_status_request_deserialization(rawBuf, msgbuf);
        /* Message Process */
        VCB_TEST_CALL_RECV_MSG_PROCESS(VCB_TEST_ID_STATUS_REQUEST, msgbuf, len);
        /* Message Process End */
        break;
    case VCB_TEST_ID_STATUS_RESPONSE:
        len = VCB_TEST_status_response_deserialization(rawBuf, msgbuf);
        /* Message Process */
        VCB_TEST_CALL_RECV_MSG_PROCESS(VCB_TEST_ID_STATUS_RESPONSE, msgbuf, len);
        /* Message Process End */
        break;
    case VCB_TEST_ID_UPDATE_DATA_REQUEST:
        len = VCB_TEST_update_data_request_deserialization(rawBuf, msgbuf);
        /* Message Process */
        VCB_TEST_CALL_RECV_MSG_PROCESS(VCB_TEST_ID_UPDATE_DATA_REQUEST, msgbuf, len);
        /* Message Process End */
        break;
    case VCB_TEST_ID_UPDATE_DATA_RESPONSE:
        len = VCB_TEST_update_data_response_deserialization(rawBuf, msgbuf);
        /* Message Process */
        VCB_TEST_CALL_RECV_MSG_PROCESS(VCB_TEST_ID_UPDATE_DATA_RESPONSE, msgbuf, len);
        /* Message Process End */
        break;
    case VCB_TEST_ID_VCB_CONFIGURE_REQUESET:
        len = VCB_TEST_vcb_configure_requeset_deserialization(rawBuf, msgbuf);
        /* Message Process */
        VCB_TEST_CALL_RECV_MSG_PROCESS(VCB_TEST_ID_VCB_CONFIGURE_REQUESET, msgbuf, len);
        /* Message Process End */
        break;
    case VCB_TEST_ID_VCB_CONFIGURE_RESPONSE:
        len = VCB_TEST_vcb_configure_response_deserialization(rawBuf, msgbuf);
        /* Message Process */
        VCB_TEST_CALL_RECV_MSG_PROCESS(VCB_TEST_ID_VCB_CONFIGURE_RESPONSE, msgbuf, len);
        /* Message Process End */
        break;
    case VCB_TEST_ID_VCB_MONITORING_DATA_UP_REQUEST:
        len = VCB_TEST_vcb_monitoring_data_up_request_deserialization(rawBuf, msgbuf);
        /* Message Process */
        VCB_TEST_CALL_RECV_MSG_PROCESS(VCB_TEST_ID_VCB_MONITORING_DATA_UP_REQUEST, msgbuf, len);
        /* Message Process End */
        break;
    case VCB_TEST_ID_VCB_MONITORING_DATA_UP_RESPONSE:
        len = VCB_TEST_vcb_monitoring_data_up_response_deserialization(rawBuf, msgbuf);
        /* Message Process */
        VCB_TEST_CALL_RECV_MSG_PROCESS(VCB_TEST_ID_VCB_MONITORING_DATA_UP_RESPONSE, msgbuf, len);
        /* Message Process End */
        break;
    default:
		++VCB_TEST_UNKNOWN_HANDER_CB.recv_count;
        break;
	}
	return 0;
}



void VCB_TEST_msg_preprocessing_hook_register(int (*fun)(uint8_t*, int))
{
    g_vcb_test_msg_preprocess_hook = fun;
}

// Send Function for bootloader

int VCB_TEST_send_status_request(VCB_TEST_STATUS_REQUEST_T* msg)
{
    int ret = 0;
    uint8_t buf[VCB_TEST_MSG_MESSAGE_BUF_LEN] = {0};

    ret = VCB_TEST_message_stuff(VCB_TEST_ID_STATUS_REQUEST, msg, buf);
    if (ret > 0)
    {
        ret = VCB_TEST_msg_uart_send(buf, ret);
        VCB_TEST_SEND_COUNT(VCB_TEST_ID_STATUS_REQUEST, ret);
    }
    return ret;
}

int VCB_TEST_send_status_response(VCB_TEST_STATUS_RESPONSE_T* msg)
{
    int ret = 0;
    uint8_t buf[VCB_TEST_MSG_MESSAGE_BUF_LEN] = {0};

    ret = VCB_TEST_message_stuff(VCB_TEST_ID_STATUS_RESPONSE, msg, buf);
    if (ret > 0)
    {
        ret = VCB_TEST_msg_uart_send(buf, ret);
        VCB_TEST_SEND_COUNT(VCB_TEST_ID_STATUS_RESPONSE, ret);
    }
    return ret;
}

int VCB_TEST_send_update_data_request(VCB_TEST_UPDATE_DATA_REQUEST_T* msg)
{
    int ret = 0;
    uint8_t buf[VCB_TEST_MSG_MESSAGE_BUF_LEN] = {0};

    ret = VCB_TEST_message_stuff(VCB_TEST_ID_UPDATE_DATA_REQUEST, msg, buf);
    if (ret > 0)
    {
        ret = VCB_TEST_msg_uart_send(buf, ret);
        VCB_TEST_SEND_COUNT(VCB_TEST_ID_UPDATE_DATA_REQUEST, ret);
    }
    return ret;
}

int VCB_TEST_send_update_data_response(VCB_TEST_UPDATE_DATA_RESPONSE_T* msg)
{
    int ret = 0;
    uint8_t buf[VCB_TEST_MSG_MESSAGE_BUF_LEN] = {0};

    ret = VCB_TEST_message_stuff(VCB_TEST_ID_UPDATE_DATA_RESPONSE, msg, buf);
    if (ret > 0)
    {
        ret = VCB_TEST_msg_uart_send(buf, ret);
        VCB_TEST_SEND_COUNT(VCB_TEST_ID_UPDATE_DATA_RESPONSE, ret);
    }
    return ret;
}


// Send Function
int VCB_TEST_send_vcb_configure_requeset(VCB_TEST_VCB_CONFIGURE_REQUESET_T* msg)
{
    int ret = 0;
    uint8_t buf[VCB_TEST_MSG_MESSAGE_BUF_LEN] = {0};

    ret = VCB_TEST_message_stuff(VCB_TEST_ID_VCB_CONFIGURE_REQUESET, msg, buf);
    if (ret > 0)
    {
        ret = VCB_TEST_msg_uart_send(buf, ret);
        VCB_TEST_SEND_COUNT(VCB_TEST_ID_VCB_CONFIGURE_REQUESET, ret);
    }
    return ret;
}

int VCB_TEST_send_vcb_configure_response(VCB_TEST_VCB_CONFIGURE_RESPONSE_T* msg)
{
    int ret = 0;
    uint8_t buf[VCB_TEST_MSG_MESSAGE_BUF_LEN] = {0};

    ret = VCB_TEST_message_stuff(VCB_TEST_ID_VCB_CONFIGURE_RESPONSE, msg, buf);
    if (ret > 0)
    {
        ret = VCB_TEST_msg_uart_send(buf, ret);
        VCB_TEST_SEND_COUNT(VCB_TEST_ID_VCB_CONFIGURE_RESPONSE, ret);
    }
    return ret;
}

int VCB_TEST_send_vcb_monitoring_data_up_request(VCB_TEST_VCB_MONITORING_DATA_UP_REQUEST_T* msg)
{
    int ret = 0;
    uint8_t buf[VCB_TEST_MSG_MESSAGE_BUF_LEN] = {0};

    ret = VCB_TEST_message_stuff(VCB_TEST_ID_VCB_MONITORING_DATA_UP_REQUEST, msg, buf);
    if (ret > 0)
    {
        ret = VCB_TEST_msg_uart_send(buf, ret);
        VCB_TEST_SEND_COUNT(VCB_TEST_ID_VCB_MONITORING_DATA_UP_REQUEST, ret);
    }
    return ret;
}

int VCB_TEST_send_vcb_monitoring_data_up_response(VCB_TEST_VCB_MONITORING_DATA_UP_RESPONSE_T* msg)
{
    int ret = 0;
    uint8_t buf[VCB_TEST_MSG_MESSAGE_BUF_LEN] = {0};

    ret = VCB_TEST_message_stuff(VCB_TEST_ID_VCB_MONITORING_DATA_UP_RESPONSE, msg, buf);
    if (ret > 0)
    {
        ret = VCB_TEST_msg_uart_send(buf, ret);
        VCB_TEST_SEND_COUNT(VCB_TEST_ID_VCB_MONITORING_DATA_UP_RESPONSE, ret);
    }
    return ret;
}

