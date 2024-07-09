#ifndef VCB_VMB_MSG_PROC_H__FILE_
#define VCB_VMB_MSG_PROC_H__FILE_
/* 自动生成文件，请勿修改 ！！！ Dawei.Zhou */
#include <stdint.h> 

#ifdef __cplusplus
extern "C" {
#endif
#define HOST_SYSTEM_BIG_ENDIAN           (0)
// Escape character.
#define VCB_VMB_MSG_ESC_CHAR               (0x5C)
#define VCB_VMB_MSG_END_FLAG               (0x7E)
#define VCB_VMB_MSG_ESC_5C                 (0x4C)
#define VCB_VMB_MSG_ESC_7E                 (0x22)

#define VCB_VMB_MSG_MESSAGE_BUF_LEN        (300)
#define VCB_VMB_MESSAGE_ID_BASE            (0x14)


typedef void (*VCB_VMB_MSG_HANDLER_T)(void*);
typedef struct tagVCB_VMB_MSG_HANDLER_CB
{
    VCB_VMB_MSG_HANDLER_T handler;
    uint32_t recv_count;
    uint32_t recv_e_count;
    uint32_t send_count;
    uint32_t send_e_count;
} VCB_VMB_MSG_HANDLER_CB_T;
extern VCB_VMB_MSG_HANDLER_CB_T g_vcb_vmb_msg_handler_map[];
extern uint32_t g_vcb_vmb_msg_handler_map_count;
    
/* status_request */
#define VCB_VMB_ID_STATUS_REQUEST (20)
typedef struct tagVCB_VMB_STATUS_REQUEST
{
    /* 客户端的ID */
    uint8_t             client_id;
    /* 0x1 设备版本信息， 0x02 开始升级（只有开始升级的时候下面三个字段才有效） */
#define VCB_VMB_REQ_ID_INVALID       0x00
#define VCB_VMB_REQ_ID_DEV_INFO      0x01   /* boot状态下请求版本信息    */
#define VCB_VMB_REQ_ID_UPGRADE       0x02
    /* 新增，用于支持老设备升级：对端可以区分系统状态 */
#define VCB_VMB_REQ_ID_DEV_INFO_APP  0xF1   /* 应用程序状态下请求版本信息 */
#define VCB_VMB_REQ_ID_DEV_INFO_SERV 0xF2   /* 服务模式状态下请求版本信息 */
    uint32_t            request_id;
    /* 开始升级时向下位机下发app的长度和crc */
    uint32_t            app_len;
    uint16_t            app_crc;
    /* 开始升级时通知下位机app版本号，如v1.0 */
    uint8_t             app_ver[8];
} VCB_VMB_STATUS_REQUEST_T;


/* status_response */
#define VCB_VMB_ID_STATUS_RESPONSE (21)
typedef struct tagVCB_VMB_STATUS_RESPONSE
{
    /* 客户端的ID */
    uint8_t             client_id;
    /* 设备状态标记位， 0x01 bootloader当前处于bootload中， 0x02 application当前处于app中， 只要是bootloader模式就可以升级，其它错误状态 */
#define VCB_VMB_STATUS_BOOTLOADER   0x01
#define VCB_VMB_STATUS_APP          0x02
#define VCB_VMB_STATUS_UPGRAD_RDY   0x03  // 处于bootloader模式，且收到升级开始消息并准备好了
    uint8_t             status;
    /* 下位机boot版本号，如v1.0 */
    uint8_t             boot_ver[8];
    /* 下位机app版本号，如v1.0 */
    uint8_t             app_ver[8];
    /* 返回的硬件版本号 如XXXX 2021000001 */
    uint8_t             hw_info[16];
} VCB_VMB_STATUS_RESPONSE_T;


/* update_data_request */
#define VCB_VMB_ID_UPDATE_DATA_REQUEST (22)
typedef struct tagVCB_VMB_UPDATE_DATA_REQUEST
{
    /* 客户端的ID: 2、3 */
    uint8_t             client_id;
    /* 是否是第一个最后一个 0x01 第一个 0x02最后一个 0x0 中间消息 */
#define VCB_VMB_UPDATE_DATA_LAST_DATA   0x02
#define VCB_VMB_UPDATE_DATA_FIRST_DATA  0x01
#define VCB_VMB_UPDATE_DATA_OTHER       0x00
    uint8_t             boundary_flag;
    /* 消息内容的有效长度 */
    uint16_t            msg_len;
    /* 消息的序列号 */
    uint32_t            seq_id;
    /* 当前数据块的地址 */
    uint32_t            address;
    /* 程序数据 */
    uint8_t             app_data[128];
} VCB_VMB_UPDATE_DATA_REQUEST_T;


/* update_data_response */
#define VCB_VMB_ID_UPDATE_DATA_RESPONSE (23)
typedef struct tagVCB_VMB_UPDATE_DATA_RESPONSE
{
    /* 客户端的ID，CB的ID为2，ECC的ID为3 */
    uint8_t             client_id;
    /* 附加的状态信息，正常情况为0，非0为发生错误，代表错误码 */
#define VCB_VMB_UPDATE_STATUS_OK          0x00
#define VCB_VMB_UPDATE_STATUS_ADRESS_ERR  0x01
#define VCB_VMB_UPDATE_STATUS_FLASH_ERR   0x02
    uint32_t            status;
    /* 请求的序列号，如果状态正常，在收到序列号的基础上加1 */
    uint32_t            next_seq_id;
} VCB_VMB_UPDATE_DATA_RESPONSE_T;


/* vmb_monitoring_data_up_request */
#define VCB_VMB_ID_VMB_MONITORING_DATA_UP_REQUEST (30)
typedef struct tagVCB_VMB_VMB_MONITORING_DATA_UP_REQUEST
{
    /* 气源压力监控数据上送请求，不关闭，无需发送 */
#define VCB_VMB_VMB_MONITORING_DATA_UP_REQUEST_UPLOAD_ENABLE          0x01 // 上送使能（默认状态）
#define VCB_VMB_VMB_MONITORING_DATA_UP_REQUEST_UPLOAD_DISABLE         0x02 // 上送停止           
#define VCB_VMB_VMB_MONITORING_DATA_UP_REQUEST_ENERGIZE_SAFE_VALVE    0x04 // 激活安全阀
#define VCB_VMB_VMB_MONITORING_DATA_UP_REQUEST_DE_ENERGIZE_SAFE_VALVE 0x08 // 激活安全阀
    uint8_t             status;
} VCB_VMB_VMB_MONITORING_DATA_UP_REQUEST_T;


/* vmb_monitoring_data_up_response */
#define VCB_VMB_ID_VMB_MONITORING_DATA_UP_RESPONSE (31)
typedef struct tagVCB_VMB_VMB_MONITORING_DATA_UP_RESPONSE
{
    /* 每200ms发送一次上送数据 */
    /* vmb系统的状态 */
    uint32_t            status_bits;
    int32_t             air_supply_pressure;
    int32_t             o2_supply_pressure;
    int16_t             pht_pressure;
    int16_t             pht_temperature;
    int16_t             pht_humidity;
    /* IIC Error */
    uint32_t            exp_flow_error_count;
    uint32_t            exp_flow_reset_count;
    uint32_t            pht_error_count;
    uint32_t            pht_reset_count;
} VCB_VMB_VMB_MONITORING_DATA_UP_RESPONSE_T;


/* vmb_exp_pressure_up_request */
#define VCB_VMB_ID_VMB_EXP_PRESSURE_UP_REQUEST (32)
typedef struct tagVCB_VMB_VMB_EXP_PRESSURE_UP_REQUEST
{
    /* 压力监控数据上送请求，不关闭，无需发送 */
#define VCB_VMB_VMB_EXP_PRESSURE_UP_REQUEST_UPLOAD_ENABLE          0x01 // 上送使能（默认状态）
#define VCB_VMB_VMB_EXP_PRESSURE_UP_REQUEST_UPLOAD_DISABLE         0x02 // 上送停止           
    uint8_t             status;
} VCB_VMB_VMB_EXP_PRESSURE_UP_REQUEST_T;


/* vmb_exp_pressure_up_response */
#define VCB_VMB_ID_VMB_EXP_PRESSURE_UP_RESPONSE (33)
typedef struct tagVCB_VMB_VMB_EXP_PRESSURE_UP_RESPONSE
{
    /* 每2ms发送一次上送数据 */
    /* AD7689BCPZRL7 -> MPXV2010GP  分辨率: 16bit  0 kPa to 10 kPa 准确性：0.5 % */
    int32_t             exp_pressure;
    /* ml/min */
    int32_t             exp_flow_rate;
} VCB_VMB_VMB_EXP_PRESSURE_UP_RESPONSE_T;


/* vmb_test_data_request */
#define VCB_VMB_ID_VMB_TEST_DATA_REQUEST (34)
typedef struct tagVCB_VMB_VMB_TEST_DATA_REQUEST
{
    /* 测试数据上送请求，测试期间使用 */
    /* 有效消息的高字节必须为0xA5 */
#define VCB_VMB_VMB_TEST_DATA_REQUEST_STATUS_VALID_MASK     0xA500
    uint16_t            status;
} VCB_VMB_VMB_TEST_DATA_REQUEST_T;


/* vmb_test_data_response */
#define VCB_VMB_ID_VMB_TEST_DATA_RESPONSE (35)
typedef struct tagVCB_VMB_VMB_TEST_DATA_RESPONSE
{
    /* 必须收到请求才会发送 */
    uint8_t             id;
    uint8_t             data[32];
} VCB_VMB_VMB_TEST_DATA_RESPONSE_T;

// 对外依赖函数 
extern uint16_t VCB_VMB_msg_calc_crc(uint8_t* buf, int32_t len);
extern int VCB_VMB_msg_uart_send(uint8_t* buf, int32_t len);

// 注册接收消息的处理函数
extern int VCB_VMB_msg_recv_handler_register(VCB_VMB_MSG_HANDLER_T fun, uint32_t msg_id);
// 注册消息预处理函数 
extern void VCB_VMB_msg_preprocessing_hook_register(int (*fun)(uint8_t*, int));

// FUNCTION：消息处理函数 升级消息处理和用户自定义消息处理
extern int VCB_VMB_send_status_request(VCB_VMB_STATUS_REQUEST_T* msg); 
extern int VCB_VMB_send_status_response(VCB_VMB_STATUS_RESPONSE_T* msg); 
extern int VCB_VMB_send_update_data_request(VCB_VMB_UPDATE_DATA_REQUEST_T* msg); 
extern int VCB_VMB_send_update_data_response(VCB_VMB_UPDATE_DATA_RESPONSE_T* msg); 

extern int VCB_VMB_send_vmb_monitoring_data_up_request(VCB_VMB_VMB_MONITORING_DATA_UP_REQUEST_T* msg); 
extern int VCB_VMB_send_vmb_monitoring_data_up_response(VCB_VMB_VMB_MONITORING_DATA_UP_RESPONSE_T* msg); 
extern int VCB_VMB_send_vmb_exp_pressure_up_request(VCB_VMB_VMB_EXP_PRESSURE_UP_REQUEST_T* msg); 
extern int VCB_VMB_send_vmb_exp_pressure_up_response(VCB_VMB_VMB_EXP_PRESSURE_UP_RESPONSE_T* msg); 
extern int VCB_VMB_send_vmb_test_data_request(VCB_VMB_VMB_TEST_DATA_REQUEST_T* msg); 
extern int VCB_VMB_send_vmb_test_data_response(VCB_VMB_VMB_TEST_DATA_RESPONSE_T* msg); 
extern int VCB_VMB_receive(uint8_t *rawBuf, int len);

#ifdef __cplusplus 
}
#endif
#endif
