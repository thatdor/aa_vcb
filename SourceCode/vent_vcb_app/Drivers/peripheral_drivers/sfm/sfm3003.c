/*
 * sfm3003.c
 *
 *  Created on: Nov 23, 2023
 *      Author: weizhuang.kong
 */
#include "main.h"
#include "cmsis_os.h"
#include "sfm3003.h"


#define SFM_READ_STATUS    0
#define SFM3003_GET_I2C_HANDLE_PTR(value)  ((value) == SFM_AIR) ? (&hi2c3) : (&hi2c2)


const uint8_t sfm_crc8_table[] =
{
    0x00, 0x31, 0x62, 0x53, 0xc4, 0xf5, 0xa6, 0x97, 0xb9, 0x88, 0xdb, 0xea, 0x7d, 0x4c, 0x1f, 0x2e,
    0x43, 0x72, 0x21, 0x10, 0x87, 0xb6, 0xe5, 0xd4, 0xfa, 0xcb, 0x98, 0xa9, 0x3e, 0x0f, 0x5c, 0x6d,
    0x86, 0xb7, 0xe4, 0xd5, 0x42, 0x73, 0x20, 0x11, 0x3f, 0x0e, 0x5d, 0x6c, 0xfb, 0xca, 0x99, 0xa8,
    0xc5, 0xf4, 0xa7, 0x96, 0x01, 0x30, 0x63, 0x52, 0x7c, 0x4d, 0x1e, 0x2f, 0xb8, 0x89, 0xda, 0xeb,
    0x3d, 0x0c, 0x5f, 0x6e, 0xf9, 0xc8, 0x9b, 0xaa, 0x84, 0xb5, 0xe6, 0xd7, 0x40, 0x71, 0x22, 0x13,
    0x7e, 0x4f, 0x1c, 0x2d, 0xba, 0x8b, 0xd8, 0xe9, 0xc7, 0xf6, 0xa5, 0x94, 0x03, 0x32, 0x61, 0x50,
    0xbb, 0x8a, 0xd9, 0xe8, 0x7f, 0x4e, 0x1d, 0x2c, 0x02, 0x33, 0x60, 0x51, 0xc6, 0xf7, 0xa4, 0x95,
    0xf8, 0xc9, 0x9a, 0xab, 0x3c, 0x0d, 0x5e, 0x6f, 0x41, 0x70, 0x23, 0x12, 0x85, 0xb4, 0xe7, 0xd6,
    0x7a, 0x4b, 0x18, 0x29, 0xbe, 0x8f, 0xdc, 0xed, 0xc3, 0xf2, 0xa1, 0x90, 0x07, 0x36, 0x65, 0x54,
    0x39, 0x08, 0x5b, 0x6a, 0xfd, 0xcc, 0x9f, 0xae, 0x80, 0xb1, 0xe2, 0xd3, 0x44, 0x75, 0x26, 0x17,
    0xfc, 0xcd, 0x9e, 0xaf, 0x38, 0x09, 0x5a, 0x6b, 0x45, 0x74, 0x27, 0x16, 0x81, 0xb0, 0xe3, 0xd2,
    0xbf, 0x8e, 0xdd, 0xec, 0x7b, 0x4a, 0x19, 0x28, 0x06, 0x37, 0x64, 0x55, 0xc2, 0xf3, 0xa0, 0x91,
    0x47, 0x76, 0x25, 0x14, 0x83, 0xb2, 0xe1, 0xd0, 0xfe, 0xcf, 0x9c, 0xad, 0x3a, 0x0b, 0x58, 0x69,
    0x04, 0x35, 0x66, 0x57, 0xc0, 0xf1, 0xa2, 0x93, 0xbd, 0x8c, 0xdf, 0xee, 0x79, 0x48, 0x1b, 0x2a,
    0xc1, 0xf0, 0xa3, 0x92, 0x05, 0x34, 0x67, 0x56, 0x78, 0x49, 0x1a, 0x2b, 0xbc, 0x8d, 0xde, 0xef,
    0x82, 0xb3, 0xe0, 0xd1, 0x46, 0x77, 0x24, 0x15, 0x3b, 0x0a, 0x59, 0x68, 0xff, 0xce, 0x9d, 0xac
};


const int8_t g_sfm_init_step_duration[SFM_INIT_STEP_MAX + 1] = {0, 5, 20, 0, 30, 0, 15, 0, 0};

SFM_FM_CB_T  g_sfm_cb[SFM_MAX] = {0};

#define SFM_CHECK_TIME_AND_ENTER(cb, interval, next_step) \
    if (cb->cur_phase_remaining <= 0) \
    { \
        cb->phase = next_step; \
        cb->cur_phase_remaining = g_sfm_init_step_duration[next_step]; \
    } else \
    { \
        cb->cur_phase_remaining -= interval; \
    }

uint8_t sfm_iic_crc8_cal(uint8_t *ptr, uint32_t len)
{
    uint8_t crc = 0xff;

    while (len--)
    {
        crc = sfm_crc8_table[crc ^ *ptr++];
    }
    return (crc);
}


/*
 physical = (sensor_output - offset) * multiple / scale_factor
 multiple倍数，如流量sensor_output单位为slm，需要*1000转换成ml/min
 */
#define sfm_iic_get_physical_value_by_scale_factor(sensor_output, scale, offset, multiple) \
        (int32_t)((sensor_output - offset) * multiple / scale)
//int32_t sfm_iic_get_physical_value_by_scale_factor(int32_t sensor_output, int32_t scale, int32_t offset, int32_t multiple)
//{
//    return (int32_t)((int64_t)(sensor_output - offset) * multiple / scale);
//}


int sfm_iic_read_measurement(I2C_HandleTypeDef *hi2c, uint16_t dev_addr, SFM_FM_CB_T* cb)
{
#if SFM_READ_STATUS
    uint8_t rbuf[9] = {0};
#else
    uint8_t rbuf[6] = {0};
#endif
    uint32_t timeout = 5;


    HAL_StatusTypeDef ret = HAL_I2C_Master_Receive(hi2c, dev_addr, rbuf, sizeof(rbuf), timeout);
    if (ret != HAL_OK)
    {
        return ret;
    }
    int16_t flow = (int16_t)(rbuf[0] << 8 | rbuf[1]);
    if (rbuf[2] != sfm_iic_crc8_cal(&rbuf[0], 2))
    {
        return -1;
    }
    cb->raw_flow = flow;

    int32_t temp = (int16_t)(rbuf[3] << 8 | rbuf[4]);
    if (rbuf[5] != sfm_iic_crc8_cal(&rbuf[3], 2))
    {
        return -1;
    }
    cb->raw_temp = temp;

#if SFM_READ_STATUS
    if (rbuf[8] != sfm_iic_crc8_cal(&rbuf[6], 2))
    {
        return -1;
    }
    cb->status = rbuf[6] << 8 | rbuf[7];
#endif
    return 0;
}


int sfm3003_get_flow_temp(SFM3003_SELECT value, int32_t *flow_result, int32_t *temp_result)
{
    SFM_FM_CB_T* cb = &g_sfm_cb[value];
    I2C_HandleTypeDef *hi2c = SFM3003_GET_I2C_HANDLE_PTR(value);
    HAL_StatusTypeDef ret = sfm_iic_read_measurement(hi2c, SFM3003_IIC_ADDRESS, cb);
    if (ret != HAL_OK)
    {
        return ret;
    }
    // flow: scale factor: 170 1/slm, offset: -24576
    // temperature: scale factor: 200, offset: 0
    // return (int32_t)((int64_t)(sensor_output - offset) * multiple / scale);
    // int32_t  (int32_t sensor_output, int32_t scale, int32_t offset, int32_t multiple)
    *flow_result = sfm_iic_get_physical_value_by_scale_factor(cb->raw_flow, cb->sacle, cb->offset, 1000);
    *temp_result = sfm_iic_get_physical_value_by_scale_factor(cb->raw_temp, 200, 0, 100);

    return ret;
}


void sfm3003_enable(SFM3003_SELECT valve, int enable)
{
    if (valve == SFM_AIR)
    {
        HAL_GPIO_WritePin(AIR_FLOW_EN_GPIO_Port, AIR_FLOW_EN_Pin, (enable) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
    else if (valve == SFM_O2)
    {
        HAL_GPIO_WritePin(O2_FLOW_EN_GPIO_Port, O2_FLOW_EN_Pin, (enable) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}

// For the SFM3003 series, the flow unit is a fixed value 0x0148 and corresponds to slm:
// standard liter per minute at 20°C and 1013mbar pressure.
int sfm3003_start_continuous_measurement(I2C_HandleTypeDef *i2c, uint16_t address, uint8_t type, SFM_FM_CB_T* cb)
{
    HAL_StatusTypeDef ret = 0;
    uint8_t crc;
    uint8_t cmd[5];
    uint8_t rbuf[9] = {0};

    cmd[0] = (SFM3003_IIC_READ_FACTOR >> 8) & 0xff;
    cmd[1] = (SFM3003_IIC_READ_FACTOR) & 0xff;

    if (type == SFM_AIR)
    {
        cmd[2] = (SFM3003_IIC_START_AIR_FLOW_CMD >> 8) & 0xFF;
        cmd[3] =  SFM3003_IIC_START_AIR_FLOW_CMD & 0xFF;
    }
    else
    {
        cmd[2] = (SFM3003_IIC_START_O2_FLOW_CMD >> 8) & 0xFF;
        cmd[3] = SFM3003_IIC_START_O2_FLOW_CMD & 0xFF;
    }

    crc = sfm_iic_crc8_cal(&cmd[2], 2);
    cmd[4] = crc;

    ret = HAL_I2C_Master_Transmit(i2c, address, cmd, sizeof(cmd), 5);
    if (ret == 0)
    {
        osDelay(1);
        ret = HAL_I2C_Master_Receive(i2c, address, rbuf, sizeof(rbuf), 5);
        if (ret == HAL_OK)
        {
            crc = sfm_iic_crc8_cal(&rbuf[0], 2);
            if (crc != rbuf[2])
            {
                return -3;
            }
            cb->sacle = (int16_t)(rbuf[0] << 8 | rbuf[1]);
            crc = sfm_iic_crc8_cal(&rbuf[3], 2);
            if (crc != rbuf[5])
            {
                return -4;
            }
            cb->offset = (int16_t)((rbuf[3] << 8 | rbuf[4]) & 0xffff);
            cb->unit   = rbuf[6] << 8 | rbuf[7];

            // 避免除零
            if (cb->sacle == 0)
            {
                cb->sacle = 1;
            }

            // write measeure cmd
            ret = HAL_I2C_Master_Transmit(i2c, address, &cmd[2], 2, 5);
        }
        else
        {
            return (ret + 0xF);
        }
    }
    return ret;
}


int sfm3003_read_product_info(I2C_HandleTypeDef* i2c, uint16_t address, uint8_t pn[4], uint8_t sn[8])
{
    int i = 0;
    int ret = 0;
    uint8_t crc;
    uint8_t cmd[2];
    uint8_t rbuf[18];

    cmd[0] = (SFM3003_IIC_READ_SN_CMD >> 8) & 0xff;
    cmd[1] = (SFM3003_IIC_READ_SN_CMD) & 0xff;

    ret = HAL_I2C_Master_Transmit(i2c, address, cmd, sizeof(cmd), 5);
    if (ret == 0)
    {
        ret = HAL_I2C_Master_Receive(i2c, address, rbuf, sizeof(rbuf), 5);
        if (0 == ret)
        {
            do
            {
                crc = sfm_iic_crc8_cal(&rbuf[i], 2);
                if (crc != rbuf[i+2])
                {
                    return -5;
                }
                i += 3;
            } while(i < 16);
            pn[0] = rbuf[0];
            pn[1] = rbuf[1];
            pn[2] = rbuf[3];
            pn[3] = rbuf[4];

            sn[0] = rbuf[6];
            sn[1] = rbuf[7];
            sn[2] = rbuf[9];
            sn[3] = rbuf[10];
            sn[4] = rbuf[12];
            sn[5] = rbuf[13];
            sn[6] = rbuf[15];
            sn[7] = rbuf[16];
        }
    }
    return ret;
}


void sfm3003_do_initialization(SFM_FM_CB_T* cb, SFM3003_SELECT value, int interval)
{
    uint8_t cmd[2] = {0};
    HAL_StatusTypeDef ret;
    I2C_HandleTypeDef *hi2c = SFM3003_GET_I2C_HANDLE_PTR(value);

    switch(cb->phase)
    {
    case SFM_INIT_STEP_START_INIT:
        SFM_CHECK_TIME_AND_ENTER(cb, interval, SFM_INIT_STEP_OFF);
    case SFM_INIT_STEP_OFF:
        // OFF
        sfm3003_enable(value, 1);
        SFM_CHECK_TIME_AND_ENTER(cb, interval, SFM_INIT_STEP_ON);
        break;
    case SFM_INIT_STEP_ON:
        // ON
        sfm3003_enable(value, 0);
        SFM_CHECK_TIME_AND_ENTER(cb, interval, SFM_INIT_STEP_RESET);
        //osDelay(20);   // 典型时间2ms
        break;
    case SFM_INIT_STEP_RESET:
        // RESET
        // stop read
        cmd[0] = (SFM3003_IIC_STOP_READ >> 8) & 0xFF;
        cmd[1] =  SFM3003_IIC_STOP_READ & 0xFF;
        ret = HAL_I2C_Master_Transmit(hi2c, SFM3003_IIC_ADDRESS, cmd, sizeof(cmd), 5);

        // soft reset
        cmd[0] = (SFM3003_IIC_SOFT_RESET >> 8) & 0xFF;
        cmd[1] = SFM3003_IIC_SOFT_RESET & 0xFF;
        ret = HAL_I2C_Master_Transmit(hi2c, SFM3003_IIC_ADDRESS, cmd, sizeof(cmd), 10);
        SFM_CHECK_TIME_AND_ENTER(cb, interval, SFM_INIT_STEP_RESTING);
        break;
    case SFM_INIT_STEP_RESTING:
        //osDelay(30);
        SFM_CHECK_TIME_AND_ENTER(cb, interval, SFM_INIT_STEP_START_READ);
        break;
    case SFM_INIT_STEP_START_READ:
        // read product id
        sfm3003_read_product_info(hi2c, SFM3003_IIC_ADDRESS, cb->pn, cb->sn);

        // read factor/offset
        // start
        // After the start measurement command is sent:
        // - the first measurement result will be available after 12ms
        // - small accuracy deviations (few % of reading) can occur during the first 30ms (including the 12ms)
        ret = sfm3003_start_continuous_measurement(hi2c, SFM3003_IIC_ADDRESS, value, cb);
        if (ret != 0)
        {
            ++cb->init_err_count;
            cb->phase = SFM_INIT_STEP_ERR;
            cb->init_code = ret;
            break;
        }
        else
        {
            cb->init_code = 0;
            SFM_CHECK_TIME_AND_ENTER(cb, interval, SFM_INIT_STEP_STARTING);
        }
        break;
    case SFM_INIT_STEP_STARTING:
        // 持续12ms
        SFM_CHECK_TIME_AND_ENTER(cb, interval, SFM_INIT_STEP_READY);
        break;
    default:
        break;
    }
}


int sfm3003_init(SFM3003_SELECT value)
{
    SFM_FM_CB_T* cb = &g_sfm_cb[value];
    cb->phase = SFM_INIT_STEP_START_INIT;

    do
    {
        if ((cb->phase == SFM_INIT_STEP_ERR) ||
                (cb->phase == SFM_INIT_STEP_READY))
        {
            return cb->init_code;
        }
        else
        {
            sfm3003_do_initialization(cb, value, SFM3003_IIC_UPDATE_INTERVAL);
        }
    } while(1);
}

int sfm3003_start_iic_air()
{
    return sfm3003_init(SFM_AIR);
}

int sfm3003_start_iic_o2()
{
    return sfm3003_init(SFM_O2);
}

void sfm3003_get_error_count(SFM3003_SELECT value, uint32_t *err_count, uint32_t *reset_count)
{
    *err_count = g_sfm_cb[value].read_error_count;
    *reset_count = g_sfm_cb[value].reset_count;
}

int sfm3003_read(SFM3003_SELECT value, int32_t *flow_result, int32_t *temp_result)
{
    int ret = 0;
    static uint32_t err_c = 0;
    SFM_FM_CB_T* cb = &g_sfm_cb[value];

    if (cb->phase == SFM_INIT_STEP_READY)
    {
        ret = sfm3003_get_flow_temp(value, flow_result, temp_result);
        if (ret != 0)
        {
            ++err_c;
            ++cb->read_error_count;
            // 发生连续错误，设置重置
            if (err_c > 20)
            {
                cb->phase = SFM_INIT_STEP_START_INIT;
                cb->init_try_count = 3;
                err_c = 0;
                ++cb->reset_count;
            }
            if (err_c > cb->c_errors_max)
            {
                cb->c_errors_max = err_c;
            }
        }
        else
        {
            err_c = 0;
            cb->abn_duration = 0;
        }
        ++cb->read_count;
    }
    else if (cb->phase == SFM_INIT_STEP_ERR)
    {
        if (cb->init_try_count > 0)
        {
            cb->phase = SFM_INIT_STEP_START_INIT;
            cb->init_try_count--;
        }
        cb->abn_duration += SFM3003_IIC_UPDATE_INTERVAL;
    }
    else
    {
        sfm3003_do_initialization(cb, value, SFM3003_IIC_UPDATE_INTERVAL);
        cb->abn_duration += SFM3003_IIC_UPDATE_INTERVAL;
    }

    return ret;
}
