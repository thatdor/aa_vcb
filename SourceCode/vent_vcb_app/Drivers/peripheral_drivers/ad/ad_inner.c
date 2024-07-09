/*
 * ad_internal.c
 *
 *  Created on: Apr 17, 2024
 *      Author: dawei.zhou
 */
#include "main.h"
#include "cmsis_os.h"
#include "ad_inner.h"


uint32_t g_ad_inner_ch_value[AD_INNER_CH_MAX] = {0};
uint32_t g_ad_inner_ch_value_index = 0;

void ad_inner_init()
{
    HAL_ADC_Start_IT(&hadc1);
}

//__weak
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1)
    {
        g_ad_inner_ch_value[g_ad_inner_ch_value_index] = HAL_ADC_GetValue(hadc);
        ++g_ad_inner_ch_value_index;
        if (g_ad_inner_ch_value_index >= AD_INNER_CH_MAX)
        {
            g_ad_inner_ch_value_index = 0;
        }
    }

}
