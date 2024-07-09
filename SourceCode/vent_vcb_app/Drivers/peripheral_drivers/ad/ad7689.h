/*
 * ad7689.h
 *
 *  Created on: Nov 28, 2023
 *      Author: weizhuang.kong
 */

#ifndef PERIPHERAL_DRIVERS_AD7689_AD7689_H_
#define PERIPHERAL_DRIVERS_AD7689_AD7689_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"


enum AD7689_CHANNEL
{
    E_AD_GND,
    E_AD_GND1,
    E_AD_O2_SIG_AD,
    E_AD_PSV1_FB,         // O2 比例阀反馈
    E_AD_Temp_FB,         // NTC
    E_AD_PSV2_FB,         // Air比例阀反馈
    E_AD_VCM_FB,          // 音圈电机
    E_AD_INP_LOOP_PRESS,  // 吸气回路压力传感器
    E_AD_AD_CHNNEL_NUM_MAX
};

#define AD_RAW_DATA_BUFFER_COUNT 3

int ad_read_filter(int ch, uint16_t *fdata);
void ad7689_scan_run();
void ad_get_insp_pressure(int32_t *insp_press, uint32_t *err_cnt);
uint16_t ad7689_get_temp_fb();

#ifdef __cplusplus
}
#endif

#endif /* PERIPHERAL_DRIVERS_AD7689_AD7689_H_ */
