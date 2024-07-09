/*
 * sfm3003.h
 *
 *  Created on: Nov 23, 2023
 *      Author: weizhuang.kong
 */

#ifndef PERIPHERAL_DRIVERS_SFM_SFM3003_H_
#define PERIPHERAL_DRIVERS_SFM_SFM3003_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define SFM3003_IIC_ADDRESS                (0x28 << 1)
#define SFM3003_IIC_SOFT_RESET              0x0006
#define SFM3003_IIC_START_AIR_FLOW_CMD      0x3608
#define SFM3003_IIC_START_O2_FLOW_CMD       0x3603
#define SFM3003_IIC_CFG_AVG_CMD             0x366A
#define SFM3200_IIC_START_FLOW_CMD          0x1000
#define SFM3200_IIC_START_TEMP_CMD          0x1001
#define SFM3003_IIC_STOP_READ               0x3FF9
#define SFM3003_IIC_READ_PARM               0x3001
#define SFM3003_IIC_READ_SN_CMD             0xE102 // Read product identifier
#define SFM3003_IIC_READ_FACTOR             0x3661


#define SFM3003_IIC_UPDATE_INTERVAL         1  // 默认的调用时间间隔

typedef enum enSFM_INIT_STEP
{
    SFM_INIT_STEP_START_INIT,      // 开始
    SFM_INIT_STEP_OFF,        // 断电（5ms)
    SFM_INIT_STEP_ON,         // 上电（20ms)
    SFM_INIT_STEP_RESET,      // 重启 (0)
    SFM_INIT_STEP_RESTING,    // 复位等待 (60ms)
    SFM_INIT_STEP_START_READ, // 设置读取 (0ms)
    SFM_INIT_STEP_STARTING,   // 12ms后浓度可以使用(15ms)
    SFM_INIT_STEP_READY,
    SFM_INIT_STEP_ERR,
    SFM_INIT_STEP_MAX
}SFM_INIT_STEP_E;


typedef struct tagSFM_FM_CB_T
{
    SFM_INIT_STEP_E phase;
    int8_t          cur_phase_remaining;
    int             init_code;
    int             init_try_count;

    // 最近一次读到的值
    int32_t         raw_flow;
    int32_t         raw_temp;
    uint16_t        status;

    uint8_t         pn[4];  // 0x04020110
    uint8_t         sn[8];
    int16_t         sacle;
    int16_t         offset;
    uint16_t        unit;  // 328
    uint32_t        init_err_count;
    uint32_t        read_count;
    uint32_t        read_error_count;
    uint32_t        c_errors_max;   // 发生连续错误的最大次
    uint32_t        abn_duration;   // 异常状态的持续时长
    uint32_t        reset_count;    // 传感器重置次数
}SFM_FM_CB_T;


typedef enum enSFM3003_SELECT
{
    SFM_AIR,
    SFM_O2,
    SFM_MAX
} SFM3003_SELECT;


int sfm3003_start_iic_air();
int sfm3003_start_iic_o2();
int sfm3003_read(SFM3003_SELECT value, int32_t *flow_result, int32_t *temp_result);
void sfm3003_enable(SFM3003_SELECT valve, int enable);
void sfm3003_get_error_count(SFM3003_SELECT value, uint32_t *err_count, uint32_t *reset_count);

#ifdef __cplusplus
}
#endif

#endif /* PERIPHERAL_DRIVERS_SFM_SFM3003_H_ */
