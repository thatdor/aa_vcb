/*
 * VMBTestBoudary.cpp
 *
 *  Created on: Mar 27, 2024
 *      Author: dawei.zhou
 */

#include <string.h>
#include "main.h"
#include "../system/ventilator/sensors/VentSensor.h"
#include "../system/msg_boundary/vcb_test/vcb_test_msg_fun.h"

#include "VCBTestBoundary.h"
#include "VCBTestMsgDefine.h"

VCBTestBoundary VCBTestBoundary::m_instance;

VCBTestBoundary::VCBTestBoundary() : m_enable(true)
{
    // TODO Auto-generated constructor stub
    vcb_test_uart_init();
}

VCBTestBoundary::~VCBTestBoundary()
{
    // TODO Auto-generated destructor stub
}

VCBTestBoundary &VCBTestBoundary::getInstanceRef()
{
    return m_instance;
}

void VCBTestBoundary::clockrun()
{
    static int index = 1;
    if (m_enable)
    {
        bulidTestMessage(index);
        // 发送数据
        vcb_test_uart_msg_send_cb_monitoring_data(m_msgBuf, VCB_TEST_MSG_BUF_LEN);
        index++;
        if (index > 3)
        {
            index = 1;
        }
    }
}
void VCBTestBoundary::bulidTestMessage(uint32_t index)
{
    VCB_TEST_VCB_FLOW_WAVE_DATA_T* wave = nullptr;
    int id = 0xF0 | index;
    static uint16_t seq_id = 0;
    if (id == VCB_TEST_MSG_T_ID_1)
    {
        VCB_TEST_MSG_TYPE_1_T* msg = (VCB_TEST_MSG_TYPE_1_T*)m_msgBuf;
        msg->id = VCB_TEST_MSG_T_ID_1;
        msg->seq_id = seq_id;
        msg->tick = HAL_GetTick();
        wave = &(msg->flowWave);

        // 填充其它字段
        VCB_TEST_VCB_WAVE_DA_DATA &da = msg->DAData;
        da.airValveDA = m_user_set.airPSV_DA_Set;
        da.o2ValveDA = m_user_set.o2PSV_DA_Set;
        da.VCM_PWM = m_user_set.tcPWM_Set;
        da.expVCMDA = m_user_set.VCM_DA_Set;

        // press data
        VCB_TEST_VCB_PRESS_DATA_T &press = msg->pressData;
        press.o2_sig = VentSensor::getInstanceRef().getO2SigAD();
        press.airAD = VentSensor::getInstanceRef().getAirValveAD();
        press.o2AD = VentSensor::getInstanceRef().getO2ValveAD();
        press.expVCMAD = VentSensor::getInstanceRef().getVCMAD();
        press.VCM_PWMAD = VentSensor::getInstanceRef().getPWMAD();
        press.airPressure = VentSensor::getInstanceRef().getAirSupplyPressure();
        press.o2Pressure = VentSensor::getInstanceRef().getO2SupplyPressure();

    }
    else if (id == VCB_TEST_MSG_T_ID_2)
    {
        VCB_TEST_MSG_TYPE_2_T* msg = (VCB_TEST_MSG_TYPE_2_T*)m_msgBuf;
        msg->id = VCB_TEST_MSG_T_ID_2;
        msg->seq_id = seq_id;
        msg->tick = HAL_GetTick();
        wave = &(msg->flowWave);

        // 填充其它字段
    }
    else if (id == VCB_TEST_MSG_T_ID_3)
    {
        VCB_TEST_MSG_TYPE_3_T* msg = (VCB_TEST_MSG_TYPE_3_T*)m_msgBuf;
        msg->id = VCB_TEST_MSG_T_ID_3;
        msg->seq_id = seq_id;
        msg->tick = HAL_GetTick();
        wave = &(msg->flowWave);

        // 填充其它字段
        VCB_TEST_VCB_IIC_ERROR_T &iic_error = msg->errData;
        iic_error.airFlowErrCount = VentSensor::getInstanceRef().getAirFlowErrorCount();
        iic_error.airFlowResetCount = VentSensor::getInstanceRef().getAirFlowResetCount();
        iic_error.o2FlowErrCount = VentSensor::getInstanceRef().getO2FlowErrorCount();
        iic_error.o2FlowResetCount = VentSensor::getInstanceRef().getO2FlowResetCount();
        iic_error.expFlowErrCount = VentSensor::getInstanceRef().getExpFlowErrorCount();
        iic_error.expFlowResetCount = VentSensor::getInstanceRef().getExpFlowResetCount();
        iic_error.phtErrCount = VentSensor::getInstanceRef().getPhtErrorCount();
        iic_error.phtResetCount = VentSensor::getInstanceRef().getPhtResetCount();
    }

    if (wave != nullptr)
    {
        ++seq_id;
        bool valid = false;
        wave->airFlow = VentSensor::getInstanceRef().getAirRawFlow(valid);
        wave->o2Flow  = VentSensor::getInstanceRef().getO2RawFlow(valid);
        wave->airFlowSet = m_user_set_fb.mfc.air_flow_set;
        wave->o2FlowSet  = m_user_set_fb.mfc.o2_flow_set;
        wave->expFlow    = VentSensor::getInstanceRef().getExpFlow();
        wave->inspPressure = VentSensor::getInstanceRef().getInspPressure();
        wave->expPressure  = VentSensor::getInstanceRef().getExpPressure();
        wave->expPressureSet = m_user_set_fb.VCM_DA_Set;
        // TO DEL
//        wave->inspPressureGauss = VentSensor::getInstanceRef().getInspPressGauss();
//        wave->inspPressureBessel = VentSensor::getInstanceRef().getInspPressBessel();
//        wave->inspPressureEMA = VentSensor::getInstanceRef().getInspPressEMA();
//        wave->inspPressureMA = VentSensor::getInstanceRef().getInspPressMA();
    }
}

// 处理收到的控制数据
void VCBTestBoundary::procConfigData(uint8_t *buf, uint32_t len)
{
    // 配置数据处理: 自定义结构体和PC保持一致
    // VCB_TEST_PC_SET_REQ_MSG_TYPE_1_T
    uint16_t id = *((uint16_t*)buf);
    if (id == VCB_TEST_PC_SET_REQ_MSG_T_ID_1)
    {
        VCB_TEST_PC_SET_REQ_MSG_TYPE_1_T* msg = (VCB_TEST_PC_SET_REQ_MSG_TYPE_1_T*)buf;

        enableMonitorDataTransmit((!!msg->flag) || (!!msg->mfc.flag));

        memcpy(&m_user_set, msg, sizeof(m_user_set));
        memcpy(&m_user_set_fb, msg, sizeof(m_user_set_fb));
    }
}

void VCBTestBoundary::enableMonitorDataTransmit(bool enable)
{
    m_enable = enable;
}

// 处理收到的数据流
void VCBTestBoundary::testRequestProcEntry()
{
    vcb_test_uart_msg_proc_task_entry();
}

