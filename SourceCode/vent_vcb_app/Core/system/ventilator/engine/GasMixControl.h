/*
 * GasMixControl.h
 *
 *  Created on: Apr 2, 2024
 *      Author: dawei.zhou
 */

#ifndef SYSTEM_VENTILATOR_ENGINE_GASMIXCONTROL_H_
#define SYSTEM_VENTILATOR_ENGINE_GASMIXCONTROL_H_

#include <stdint.h>
// 通过目标浓度计算空氧流量
class GasMixControl {
public:
    GasMixControl();
    virtual ~GasMixControl();

    void    setO2Percent(int32_t totalFlow, int32_t userSetOxygentPercent);
    void    setTotalFlow(int32_t totalFlow);
    int32_t getAirFlowSet();
    int32_t getO2FlowSet();
    int16_t getO2Percent();

private:
    int32_t m_airFlowSet;
    int32_t m_o2FlowSet;
    int32_t m_o2Percent;
};

inline int32_t GasMixControl::getAirFlowSet()
{
    return m_airFlowSet;
}

inline int32_t GasMixControl::getO2FlowSet()
{
    return m_o2FlowSet;
}

inline int16_t GasMixControl::getO2Percent()
{
    return static_cast<int16_t>(m_o2Percent / 100);
}

#endif /* SYSTEM_VENTILATOR_ENGINE_GASMIXCONTROL_H_ */
