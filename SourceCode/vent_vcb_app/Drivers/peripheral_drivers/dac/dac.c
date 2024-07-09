/*
 * dac.c
 *
 *  Created on: Apr 15, 2024
 *      Author: dawei.zhou
 */
#include "main.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "dac.h"

// DAC8551  The DAC8551 is a small, low-power, voltage output,
//          16-bit digital-to-analog

/*O2 MFC ctrl*/
int dac_o2_set(uint16_t dac)
{
    int ret = 0;
    uint8_t buf[3] = {0};

    buf[0] = 0x00;
    buf[1] = (dac >> 8) & 0xff;
    buf[2] = dac & 0xff;

    HAL_GPIO_WritePin(O2_SPI4_CS_GPIO_Port, O2_SPI4_CS_Pin, GPIO_PIN_RESET);
    ret = HAL_SPI_Transmit(&hspi4, buf, sizeof(buf), 20);
    HAL_GPIO_WritePin(O2_SPI4_CS_GPIO_Port, O2_SPI4_CS_Pin, GPIO_PIN_SET);

    return ret;
}

/*AIR MFC ctrl*/
int dac_air_set(uint16_t dac)
{
    int ret = 0;
    uint8_t buf[3]={0};

    buf[0] = 0x00;
    buf[1] = (dac >> 8) & 0xff;
    buf[2] = dac & 0xff;

    HAL_GPIO_WritePin(AIR_SPI5_CS_GPIO_Port, AIR_SPI5_CS_Pin, GPIO_PIN_RESET);
    ret = HAL_SPI_Transmit(&hspi5, buf, sizeof(buf),20);
    HAL_GPIO_WritePin(AIR_SPI5_CS_GPIO_Port, AIR_SPI5_CS_Pin, GPIO_PIN_SET);

    return ret;
}

/*AIR MFC ctrl*/
int dac_vcm_set(uint16_t dac)
{
    int ret = 0;
    uint8_t buf[3]={0};

    buf[0] = 0x00;
    buf[1] = (dac >> 8) & 0xff;
    buf[2] = dac & 0xff;

    HAL_GPIO_WritePin(VCM_SPI2_CS_GPIO_Port, VCM_SPI2_CS_Pin, GPIO_PIN_RESET);
    ret = HAL_SPI_Transmit(&hspi2,buf,sizeof(buf), 20);
    HAL_GPIO_WritePin(VCM_SPI2_CS_GPIO_Port, VCM_SPI2_CS_Pin, GPIO_PIN_SET);

    return ret;
}
