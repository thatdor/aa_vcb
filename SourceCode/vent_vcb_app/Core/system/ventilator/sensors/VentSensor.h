/*
 * VentSensor.h
 *
 *  Created on: Mar 27, 2024
 *      Author: dawei.zhou
 */

#ifndef SYSTEM_VENTILATOR_SENSORS_VENTSENSOR_H_
#define SYSTEM_VENTILATOR_SENSORS_VENTSENSOR_H_
#include <stdint.h>

class VentSensor
{
public:
    VentSensor();
    virtual ~VentSensor();
    static VentSensor& getInstanceRef();
    int     initialize();
    int32_t getExpPressure();
    int32_t getAirSupplyPressure();
    int32_t getO2SupplyPressure();
    int32_t getInspPressure();
    int32_t getExpFlow();
    int32_t getInspRawFlow(bool& valid);
    int32_t getAirRawFlow(bool& valid);
    int32_t getO2RawFlow(bool& valid);
    uint16_t getO2SigAD();
    uint16_t getAirValveAD();
    uint16_t getO2ValveAD();
    uint16_t getVCMAD();
    uint16_t getPWMAD();
    uint16_t getInspPressureAD() { return m_inspPressureAD; }
    void    clockrun();
    void filterPressure(uint16_t newPress);
    uint16_t getInspPressGauss() { return m_inspPressGauss; }
    uint16_t getInspPressBessel() { return m_inspPressBessel; }
    uint16_t getInspPressEMA() { return m_inspPressEMA; }
    uint16_t getInspPressMA() { return m_inspPressMA; }

    // error count
    uint32_t getAirFlowErrorCount();
    uint32_t getAirFlowResetCount();
    uint32_t getO2FlowErrorCount();
    uint32_t getO2FlowResetCount();
    uint32_t getExpFlowErrorCount();
    uint32_t getExpFlowResetCount();
    uint32_t getPhtErrorCount();
    uint32_t getPhtResetCount();
private:
    bool    m_inspAirFlowValid;
    bool    m_inspO2FlowValid;
    int32_t m_inspRawPressure;
    int32_t m_inspAirRawFlow;
    int32_t m_inspAirRawTemp;
    int32_t m_inspO2RawFlow;
    int32_t m_inspO2RawTemp;

    uint16_t m_o2SigAD;
    uint16_t m_airValveAD;
    uint16_t m_o2ValveAD;
    uint16_t m_vcmAD;
    uint16_t m_pwmAD;
    uint16_t m_inspPressureAD;

    int     m_inspAirFlowSensorInitCode{-1};
    int     m_inspO2FlowSensorInitCode{-1};

    uint32_t m_inspAirFlowErrorCount;
    uint32_t m_inspAirFlowResetCount;
    uint32_t m_inspO2FlowErrorCount;
    uint32_t m_inspO2FlowResetCount;
    uint32_t m_inspPressureErrorCount;

    // temp code to test filter
    uint16_t m_inspPressGauss;
    uint16_t m_inspPressBessel;
    uint16_t m_inspPressEMA;
    uint16_t m_inspPressMA;
    float m_sigma;
    float m_windsize;

    uint16_t m_inspAirFlowZeroDrift{29};
    uint16_t m_inspO2FlowZeroDrift{11};

    static VentSensor m_instance;
};

#endif /* SYSTEM_VENTILATOR_SENSORS_VENTSENSOR_H_ */
