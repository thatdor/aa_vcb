/*
 * VentSensor.cpp
 *
 *  Created on: Mar 27, 2024
 *      Author: dawei.zhou
 */
#include <cmath>
#include "cmsis_os.h"
#include "VMBProxy.h"
#include "VentSensor.h"
#include "../../common/utility/INOVFilter.h"
#include "../../Drivers/peripheral_drivers/sfm/sfm3003.h"
#include "../../Drivers/peripheral_drivers/ad/ad7689.h"
#include "../vcb_test/VCBTestBoundary.h"
#include "../../Drivers/peripheral_drivers/can/can.h"
VentSensor VentSensor::m_instance;

VentSensor::VentSensor()
{
    // TODO Auto-generated constructor stub
    m_sigma = 0.1;
    m_windsize = 10;
}

VentSensor::~VentSensor() {
    // TODO Auto-generated destructor stub
}

VentSensor& VentSensor::getInstanceRef()
{
    return m_instance;
}

int   VentSensor::initialize()
{
    int retries = 3;

    do
    {
        if (m_inspAirFlowSensorInitCode != 0)
        {
            // start insp air flow sensor
            m_inspAirFlowSensorInitCode = sfm3003_start_iic_air();
        }

        if (m_inspO2FlowSensorInitCode != 0)
        {
            // start insp o2 flow sensor
            m_inspO2FlowSensorInitCode = sfm3003_start_iic_o2();
        }

        if (m_inspAirFlowSensorInitCode || m_inspO2FlowSensorInitCode)
        {
            osDelay(50);
        }
        else
        {
            return 0;
        }
    }while(--retries > 0);

    return -1;
}

// sensor data from VMB
int32_t VentSensor::getExpPressure()
{
    return VMBProxy::getInstanceRef().getExpPressure();
}

int32_t VentSensor::getAirSupplyPressure()
{
    return VMBProxy::getInstanceRef().getAirSupplyPressure();
}

int32_t VentSensor::getO2SupplyPressure()
{
    return VMBProxy::getInstanceRef().getO2SupplyPressure();
}

int32_t VentSensor::getExpFlow()
{
    return VMBProxy::getInstanceRef().getExpFlow();
}


// sensor data from vcb
int32_t VentSensor::getInspPressure()
{
    return m_inspRawPressure;
}

int32_t VentSensor::getInspRawFlow(bool& valid)
{
    valid = m_inspAirFlowValid && m_inspO2FlowValid;
    return (m_inspAirRawFlow + m_inspO2RawFlow);
}

int32_t VentSensor::getAirRawFlow(bool& valid)
{
    valid = m_inspAirFlowValid;
    return m_inspAirRawFlow - m_inspAirFlowZeroDrift;
}

int32_t VentSensor::getO2RawFlow(bool& valid)
{
    valid = m_inspO2FlowValid;
    return m_inspO2RawFlow - m_inspO2FlowZeroDrift;
}

uint16_t VentSensor::getO2SigAD()
{
    return m_o2SigAD;
}
uint16_t VentSensor::getAirValveAD()
{
    return m_airValveAD;
}

uint16_t VentSensor::getO2ValveAD()
{
    return m_o2ValveAD;
}
uint16_t VentSensor::getVCMAD()
{
    return m_vcmAD;
}
uint16_t VentSensor::getPWMAD()
{
    return m_pwmAD;
}

void VentSensor::filterPressure(uint16_t newPress)
{
    constexpr uint32_t arrCount = 30;
    int vcm = VCBTestBoundary::getInstanceRef().m_user_set.VCM_DA_Set;
    int pwm = VCBTestBoundary::getInstanceRef().m_user_set.tcPWM_Set;

    // 临时用vcm与pwm来调节滤波参数
    if (vcm > 0 && vcm < 10) {
        m_sigma = (float)vcm * 0.1;
    }
    if (pwm > 0 && pwm < 50) {
        m_windsize = pwm;
    }
#if 0
    if (0) {   // gauss filter
        static float gaussCache[arrCount] = {0};
        float sigma = m_sigma * 10.0;
        uint32_t gaussKsize = (2 * static_cast<uint32_t>(std::round(3 * sigma)) + 1) | 1; // 根据sigma确定核大小, | 1操作以保证ksize为奇数
        uint32_t gaussIndex = std::max(0ul, arrCount - 1 - gaussKsize / 2);
        static float gaussKernel[arrCount] = {0};  // 静态变量, 仅ksize有效
        INOVFilter<float, arrCount>::generateGaussianKernel(gaussKernel, gaussKsize, sigma);
        INOVFilter<float, arrCount>::Append(gaussCache, newPress);
        m_inspPressGauss = INOVFilter<float, arrCount>::gaussianFilter(gaussCache, gaussKernel, gaussKsize, sigma, gaussIndex);
    }

    if (0) {   // bessel filter
        static float besselCache[arrCount] = {0};
        static float besselFilt[arrCount] = {0};
        static float b[6] = {0.0};
        static float a[6] = {0.0};
        INOVFilter<float, arrCount>::generateBesselCoef(b, a, m_sigma);
        INOVFilter<float, arrCount>::Append(besselCache, newPress);
        INOVFilter<float, arrCount>::besselFilter(b, a, 6, besselCache, besselFilt);
        m_inspPressBessel = besselFilt[arrCount - 1];
    }

    // ema
    if (0) {
        m_inspPressEMA = INOVFilter<uint16_t, arrCount>::emotionalMovingAverageFilter(newPress, m_inspPressEMA, m_sigma);
    }

    // MA
    if (0) {
        static float maCache[arrCount] = {0};
        static float maFilt[arrCount] = {0};
        INOVFilter<float, arrCount>::Append(maCache, newPress);
        INOVFilter<float, arrCount>::movingAverageFilter(maCache, maFilt, m_windsize);
        m_inspPressMA = maFilt[arrCount - 1];
    }

#endif
}
uint32_t VentSensor::getAirFlowErrorCount()
{
    return m_inspAirFlowErrorCount;
}
uint32_t VentSensor::getAirFlowResetCount()
{
    return m_inspAirFlowResetCount;
}
uint32_t VentSensor::getO2FlowErrorCount()
{
    return m_inspO2FlowErrorCount;
}
uint32_t VentSensor::getO2FlowResetCount()
{
    return m_inspO2FlowResetCount;
}
uint32_t VentSensor::getExpFlowErrorCount()
{
    return VMBProxy::getInstanceRef().getExpFlowErrorCount();
}

uint32_t VentSensor::getExpFlowResetCount()
{
    return VMBProxy::getInstanceRef().getExpFlowResetCount();
}

uint32_t VentSensor::getPhtErrorCount()
{
    return VMBProxy::getInstanceRef().getPhtErrorCount();
}

uint32_t VentSensor::getPhtResetCount()
{
    return VMBProxy::getInstanceRef().getPhtResetCount();
}

void VentSensor::clockrun()
{
    int ret = 0;
    static int air_flow_err = 0;
    static int o2_flow_err = 0;
    int32_t flow_result = 0;
    int32_t temp_result = 0;

    ad7689_scan_run();  // 1
    ret = sfm3003_read(SFM_AIR, &flow_result, &temp_result);
    if (ret == 0)
    {
        m_inspAirRawFlow = flow_result;
        m_inspAirRawTemp = temp_result;
        m_inspAirFlowValid = true;
    }
    else
    {
        ++air_flow_err;
        if (air_flow_err > 4)
        {
            m_inspAirFlowValid = false;
        }
    }
    ad7689_scan_run();  // 2

    sfm3003_get_error_count(SFM_AIR, &m_inspAirFlowErrorCount, &m_inspAirFlowResetCount);

    ad7689_scan_run();  // 3
    ret = sfm3003_read(SFM_O2, &flow_result, &temp_result);
    if (ret == 0)
    {
        m_inspO2RawFlow = flow_result;
        m_inspO2RawTemp = temp_result;
        m_inspO2FlowValid = true;
    }
    else
    {
        ++o2_flow_err;
        if (o2_flow_err > 4)
        {
            m_inspO2FlowValid = false;
        }
    }
    ad7689_scan_run();  // 4
    sfm3003_get_error_count(SFM_O2, &m_inspO2FlowErrorCount, &m_inspO2FlowResetCount);

    ad7689_scan_run();  // 5
    // 更新读取AD
    ad_read_filter(E_AD_O2_SIG_AD, &m_o2SigAD);
    ad_read_filter(E_AD_PSV1_FB, &m_o2ValveAD);
    ad_read_filter(E_AD_PSV2_FB, &m_airValveAD);
    ad_read_filter(E_AD_VCM_FB, &m_vcmAD);
    ad_read_filter(E_AD_INP_LOOP_PRESS, &m_inspPressureAD);
    ad7689_scan_run();  // 6
    ad_get_insp_pressure(&m_inspRawPressure, &m_inspPressureErrorCount);
    ad7689_scan_run();  // 7
    filterPressure(m_inspRawPressure);

#ifdef DEBUG_CAN
    static int i = 0;
    if (++i % 50 == 0)
    {
        can_test();
        i = 0;
    }
#endif

}
