/*
 * vent_startup.cpp
 *
 *  Created on: Mar 25, 2024
 *      Author: dawei.zhou
 */
#include "main.h"
#include "cmsis_os.h"
#include "vent_startup.h"
//////////////////////////////////////////////////////////////

#include "../ventilator/actuator/FastCloseLoop.h"
#include "../ventilator/sensors/VentSensor.h"
#include "../ventilator/sensors/VMBProxy.h"
#include "../ventilator/vcb_test/VCBTestBoundary.h"



//////////////////////////////////////////////////////////////

typedef StaticTask_t osStaticThreadDef_t;

//////////////////////////////////////////////////////////////
osThreadId_t sys250HZTaskHandle;
uint32_t sys250HZTaskBuffer[ 128 ];
osStaticThreadDef_t sys250HZTaskControlBlock;
const osThreadAttr_t sys250HZTask_attributes = {
  .name = "sys250HZTask",
  .cb_mem = &sys250HZTaskControlBlock,
  .cb_size = sizeof(sys250HZTaskControlBlock),
  .stack_mem = &sys250HZTaskBuffer[0],
  .stack_size = sizeof(sys250HZTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal7,
};

void vent_sys250HZ_entry(void *argument)
{
    for(;;)
    {
        VCBTestBoundary::getInstanceRef().clockrun();
        osDelay(4);
    }
}
//////////////////////////////////////////////////////////////

osThreadId_t sys1000HZTaskHandle;
uint32_t sys1000HZTaskBuffer[ 128 ];
osStaticThreadDef_t sys1000HZTaskControlBlock;
const osThreadAttr_t sys1000HZTask_attributes = {
  .name = "sys1000HZTask",
  .cb_mem = &sys1000HZTaskControlBlock,
  .cb_size = sizeof(sys1000HZTaskControlBlock),
  .stack_mem = &sys1000HZTaskBuffer[0],
  .stack_size = sizeof(sys1000HZTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal7,
};

void vent_sys1000HZ_entry(void *argument)
{
    for(;;)
    {
//        VentSensor::getInstanceRef().clockrun();
//        FastCloseLoop::getInstanceRef().clockrun();
        osDelay(1);
    }
}
/////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////

osThreadId_t sysvcbTestTaskHandle;
uint32_t sysvcbTestTaskBuffer[ 128 ];
osStaticThreadDef_t sysvcbTestTaskControlBlock;
const osThreadAttr_t sysvcbTestTask_attributes = {
  .name = "sysvcbTestTask",
  .cb_mem = &sysvcbTestTaskControlBlock,
  .cb_size = sizeof(sysvcbTestTaskControlBlock),
  .stack_mem = &sysvcbTestTaskBuffer[0],
  .stack_size = sizeof(sysvcbTestTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};

void vent_pc_vcb_test_entry(void *argument)
{
    for(;;)
    {
        VCBTestBoundary::getInstanceRef().testRequestProcEntry();
        osDelay(1000);
    }
}
/////////////////////////////////////////////////////////////




void vent_startup(void *argument)
{
    osThreadId_t handle;

    VentSensor::getInstanceRef().initialize();

    VMBProxy::getInstanceRef().initialize();

    FastCloseLoop::getInstanceRef().initialize();

    handle = osThreadNew(vent_sys250HZ_entry, NULL, &sys250HZTask_attributes);

    handle = osThreadNew(vent_sys1000HZ_entry, NULL, &sys1000HZTask_attributes);
    // 处理接受到的pc端测试消息
    handle = osThreadNew(vent_pc_vcb_test_entry, NULL, &sysvcbTestTask_attributes);

    while(1)
    {
        static int i = 0;
        if (i >= 50)
        {
            HAL_GPIO_TogglePin(WORK_LED1_GPIO_Port, WORK_LED1_Pin);
            i = 0;
        }
        i++;
        osDelay(10);
    }
}
