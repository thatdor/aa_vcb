/*
 * valve.c
 *
 *  Created on: Apr 15, 2024
 *      Author: dawei.zhou
 */
#include "main.h"
#include "cmsis_os.h"
#include "valve.h"




void valve_set(VALVE_E valve, int en)
{

    GPIO_TypeDef* GPIOx = NULL;
    uint16_t GPIO_Pin;
    switch(valve)
    {
    case VALVE_NO:
        GPIOx = Valve_NO_GPIO_Port;
        GPIO_Pin = Valve_NO_Pin;
        break;
    case VALVE_MIX:
        GPIOx = Valve_MIX_GPIO_Port;
        GPIO_Pin = Valve_MIX_Pin;
        break;
    case VALVE_SOL_INP:
        GPIOx = Valve_SOL_INP_GPIO_Port;
        GPIO_Pin = Valve_SOL_INP_Pin;
        break;
    case VALVE_SAFE:
        GPIOx = Valve_Safe_GPIO_Port;
        GPIO_Pin = Valve_Safe_Pin;
        break;
    default:
        return;
    }

    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, (en) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

int8_t valve_get_fb(VALVE_E valve)
{
    GPIO_TypeDef* GPIOx = NULL;
    uint16_t GPIO_Pin;
    switch(valve)
    {
    case VALVE_NO:
        GPIOx = FB_Valve_NO_GPIO_Port;
        GPIO_Pin = FB_Valve_NO_Pin;
        break;
    case VALVE_MIX:
        GPIOx = FB_Valve_MIX_GPIO_Port;
        GPIO_Pin = FB_Valve_MIX_Pin;
        break;
    case VALVE_SOL_INP:
        GPIOx = FB_Valve_SOL_INP_GPIO_Port;
        GPIO_Pin = FB_Valve_SOL_INP_Pin;
        break;
    case VALVE_SAFE:
        GPIOx = FB_Valve_Safe_GPIO_Port;
        GPIO_Pin = FB_Valve_Safe_Pin;
        break;
    default:
        return 0;
    }
    return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
}
