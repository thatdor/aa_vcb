/*
 * INOVentLLC.h
 *
 *  Created on: Mar 25, 2024
 *      Author: dawei.zhou
 */

#ifndef SYSTEM_VENTILATOR_ENGINE_INOVENTLLC_H_
#define SYSTEM_VENTILATOR_ENGINE_INOVENTLLC_H_

#include "VentLLCInterface.h"
#include "../system/ventilator/actuator/FastCloseLoop.h"
#include "../system/ventilator/engine/GasMixControl.h"

class INOVentLLC : public VentLLCInterface
{
public:
    INOVentLLC();
    virtual ~INOVentLLC();

    void clockRun() override;

    enum STATE
    {
        EXPIRATOR,
        INSPIRATOR,
        HOLD,
        MAX
    };

private:
    void doInspControl();
    void doInspHoldControl();
    void doExiratoryControl();

    void doInspFlowControl();

private:
    // 当前的状态
    int32_t        m_inspPressure;
    int32_t        m_inspFlow;

    int32_t        m_inspPressureMax;  // 吸入压力的最大值

    bool           m_pressureCtlActive;

    FastCloseLoop& m_actuator;
    GasMixControl  m_gasMixControl;
    LLCparameters  m_LLCParmeters;
    enum STATE     m_state;
};

#endif /* SYSTEM_VENTILATOR_ENGINE_INOVENTLLC_H_ */
