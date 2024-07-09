/*
 * FastCloseLoop.cpp
 *
 *  Created on: Mar 27, 2024
 *      Author: dawei.zhou
 */

#include "FastCloseLoop.h"
#include "../system/ventilator/sensors/VentSensor.h"
#include "../system/ventilator/vcb_test/VCBTestBoundary.h"
#include "../../Drivers/peripheral_drivers/dac/dac.h"

#define FAST_LOOP_MFC_TEST_EN   1

FastCloseLoop FastCloseLoop::m_instance;


//piecewise function
// 2
// =0.8468*G14+24000
// =0.1394*G24+30000
const float  FCL_AIR_P_VAVLE_PARM[2][2] = {
        { 0.2116,   0.1331},  // K
        { 32448.04 , 34359.29}    // B
};
const int32_t FCL_AIR_P_VALVE_D_VALUE[1] = {
        24344
};
// 3
//=33.8*G6+18000
//=0.6567*G13+23000
//=0.158*G26+28000
const float  FCL_O2_P_VAVLE_PARM[2][3] = {
        {33.8,   0.6567,  0.158},
        {18000.0,  23000.0,   28000.0}
};
const int32_t FCL_O2_P_VALVE_D_VALUE[2] = {
        100, 10000
};

FastCloseLoop::FastCloseLoop() : m_ctl_mode(CTL_MODE_INVALID),
        m_max_air_flow(200000),
        m_max_o2_flow(200000)
{
}

FastCloseLoop::~FastCloseLoop()
{
    // TODO Auto-generated destructor stub
}

FastCloseLoop& FastCloseLoop::getInstanceRef()
{
    return m_instance;
}

void FastCloseLoop::initialize()
{
    m_air_PI.set_feedforward_parm(FCL_AIR_P_VAVLE_PARM[0], FCL_AIR_P_VAVLE_PARM[1], FCL_AIR_P_VALVE_D_VALUE);
    m_o2_PI.set_feedforward_parm(FCL_O2_P_VAVLE_PARM[0],   FCL_O2_P_VAVLE_PARM[1],  FCL_O2_P_VALVE_D_VALUE);
}

void FastCloseLoop::clockrun()
{
    bool     valid    = false;
    uint32_t output   = 0;

    // 读取实时数据
    m_raw_o2_flow = VentSensor::getInstanceRef().getO2RawFlow(valid);
    m_raw_air_flow = VentSensor::getInstanceRef().getAirRawFlow(valid);

    if (m_ctl_mode == CTL_MODE_INVALID)
    {
        m_air_set_flow = 0;
        m_o2_set_flow = 0;
        m_air_PI.reset();
        m_o2_PI.reset();
    }

    output = m_air_PI.run(m_air_set_flow, m_raw_air_flow);
    // 设置空气比例阀
    setAirPValve(output, m_air_set_flow);

    output = m_o2_PI.run(m_o2_set_flow, m_raw_o2_flow);
    // 设置氧气比例阀
    setO2PValve(output, m_o2_set_flow);

    // 设置音圈电机
    setVcmPValve(output, 0);
}

void FastCloseLoop::setTargetFlow(int32_t air_flow, int32_t o2_flow)
{
    m_air_set_flow = air_flow;
    m_o2_set_flow = o2_flow;
}

void FastCloseLoop::setAirTargetFlow(int32_t air_flow)
{
    m_air_set_flow = air_flow;
}

void FastCloseLoop::setO2TargetFlow(int32_t o2_flow)
{
    m_o2_set_flow = o2_flow;
}

// 将空气比例阀的反馈转为空气流量数据
// 这里处理前馈
void FastCloseLoop::setAirPValve(uint32_t output, int32_t set_flow)
{
    if (VCBTestBoundary::getInstanceRef().enable())
    {
        m_ctl_mode = CTL_MODE_FLOW;

        VCB_TEST_PC_SET_REQ_MSG_TYPE_1_T* set = &VCBTestBoundary::getInstanceRef().m_user_set;
        VCB_TEST_PC_SET_REQ_MSG_TYPE_1_T* set_fb = &VCBTestBoundary::getInstanceRef().m_user_set_fb;

        if (set->mfc.flag & VCB_PC_TEST_FLAG_MFC_AIR_FLOW_SET)
        {
#if FAST_LOOP_MFC_TEST_EN
            static int32_t cur_flow = 0;
            static int32_t step = 10000;
            static int32_t count = 0;
            if (set->mfc.air_flow_set > 400000)
            {
                ++count;
                if (count > 500)
                {
                    if (cur_flow < 0)
                    {
                        step = (set->mfc.air_flow_set - 400000);
                    }
                    else if (cur_flow > 180000)
                    {
                        step = (400000 - set->mfc.air_flow_set);
                    }
                    cur_flow += step;
                    setAirTargetFlow(cur_flow);
                    set_fb->mfc.air_flow_set = cur_flow;
                    count = 0;
                }
            }
            else if (set->mfc.air_flow_set > 300000)
            {
                ++count;
                if (count > 10)
                {
                    if (cur_flow < 0)
                    {
                        step = 100;
                    }
                    else if (cur_flow > 180000)
                    {
                        step = -100;
                    }
                    cur_flow += step;
                    setAirTargetFlow(cur_flow);
                    set_fb->mfc.air_flow_set = cur_flow;
                    count = 0;
                }
            }
            else
            {
                setAirTargetFlow(set->mfc.air_flow_set);
                set_fb->mfc.air_flow_set = set->mfc.air_flow_set;
            }
#endif
        }
        if (set->mfc.flag & VCB_PC_TEST_FLAG_MFC_AIR_PI_SET)
        {
            m_air_PI.set_parm(set->mfc.air_p, set->mfc.air_i, set->mfc.air_d);
        }

        if (set->flag & VCB_PC_TEST_FLAG_AIR_DA_SET)
        {
            dac_air_set(set->airPSV_DA_Set);
            return;
        }
    }
    if(set_flow == 0)
    {
        // 这里应该设置为一个小值，不是0
        dac_air_set(0);
    }
    else
    {
        uint16_t DA = 0;
        DA = (output < 22000) ? 22000 : output;
        DA = (output > 58000) ? 58000 : output;
        dac_air_set(DA);
    }
}

// 将空气比例阀的反馈转为氧气流量数据
// 这里处理前馈
void FastCloseLoop::setO2PValve(uint32_t output, int32_t set_flow)
{
    if (VCBTestBoundary::getInstanceRef().enable())
    {
        m_ctl_mode = CTL_MODE_FLOW;

        VCB_TEST_PC_SET_REQ_MSG_TYPE_1_T* set = &VCBTestBoundary::getInstanceRef().m_user_set;
        VCB_TEST_PC_SET_REQ_MSG_TYPE_1_T* set_fb = &VCBTestBoundary::getInstanceRef().m_user_set_fb;
        if (set->mfc.flag & VCB_PC_TEST_FLAG_MFC_O2_FLOW_SET)
        {
#if FAST_LOOP_MFC_TEST_EN
            static int32_t cur_flow = 0;
            static int32_t step = 100;
            static int32_t count = 0;
            if (set->mfc.o2_flow_set > 400000)
            {
                ++count;
                if (count > 1000)
                {
                    if (cur_flow < 0)
                    {
                        step = (set->mfc.o2_flow_set - 400000);
                    }
                    else if (cur_flow > 180000)
                    {
                        step = (400000 - set->mfc.o2_flow_set);
                    }
                    cur_flow += step;
                    setO2TargetFlow(cur_flow);
                    set_fb->mfc.o2_flow_set = cur_flow;
                    count = 0;
                }
            }
            else if (set->mfc.o2_flow_set > 300000)
            {
                ++count;
                if (count > 10)
                {
                    if (cur_flow < 0)
                    {
                        step = 100;
                    }
                    else if (cur_flow > 180000)
                    {
                        step = -100;
                    }
                    cur_flow += step;
                    setO2TargetFlow(cur_flow);
                    set_fb->mfc.o2_flow_set = cur_flow;
                    count = 0;
                }
            }
            else
            {
                setO2TargetFlow(set->mfc.o2_flow_set);
                set_fb->mfc.o2_flow_set = set->mfc.o2_flow_set;
            }
#endif
        }
        if (set->mfc.flag & VCB_PC_TEST_FLAG_MFC_O2_PI_SET)
        {
            m_o2_PI.set_parm(set->mfc.o2_p, set->mfc.o2_i, set->mfc.o2_d);
        }
        if (set->flag & VCB_PC_TEST_FLAG_O2_DA_SET)
        {
            dac_o2_set(set->o2PSV_DA_Set);
            return;
        }
    }

    if(set_flow == 0)
    {
        dac_o2_set(0);
    }
    else
    {
        uint16_t DA = 0;
        DA = (output < 18000) ? 18000 : (uint16_t)output;
        DA = (output > 55000) ? 55000  : output;
        dac_o2_set(DA);
    }
}


void FastCloseLoop::setVcmPValve(uint32_t output, int32_t set_flow)
{
    if (VCBTestBoundary::getInstanceRef().enable())
    {
        m_ctl_mode = CTL_MODE_FLOW;
        VCB_TEST_PC_SET_REQ_MSG_TYPE_1_T* set = &VCBTestBoundary::getInstanceRef().m_user_set;
        VCB_TEST_PC_SET_REQ_MSG_TYPE_1_T* set_fb = &VCBTestBoundary::getInstanceRef().m_user_set_fb;
        if (set->flag & VCB_PC_TEST_FLAG_VCM_DA_SET)
        {
#if FAST_LOOP_MFC_TEST_EN
            static int cur_vcm = 0;
            static int step = 1000;
            static int count = 0;
            static int total_count = 5000;
            if (set->VCM_DA_Set >= 40000)
            {
                ++count;
                if (count > total_count)
                {
                    if (cur_vcm <= 0)
                    {
                        step = set->VCM_DA_Set - 40000;
                        total_count = 8000;
                    }
                    else if (cur_vcm >= 25000)
                    {
                        step = 40000 - set->VCM_DA_Set;
                        total_count = 2000;
                    }
                    cur_vcm += step;
                    dac_vcm_set(cur_vcm);
                    set_fb->VCM_DA_Set = cur_vcm;
                    count = 0;
                }
                return;
            }
#endif
            dac_vcm_set(set->VCM_DA_Set);
            return;
        }
    }
    return;
}
