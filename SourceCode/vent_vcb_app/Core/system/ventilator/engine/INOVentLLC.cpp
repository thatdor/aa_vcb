/*
 * INOVentLLC.cpp
 *
 *  Created on: Mar 25, 2024
 *      Author: dawei.zhou
 */

#include "INOVentLLC.h"


INOVentLLC::INOVentLLC() : m_actuator(FastCloseLoop::getInstanceRef()) {
    // TODO Auto-generated constructor stub

}

INOVentLLC::~INOVentLLC() {
    // TODO Auto-generated destructor stub
}


void INOVentLLC::clockRun()
{
    switch(m_state)
    {
    case EXPIRATOR:
    case INSPIRATOR:
        doInspHoldControl();
        break;
    case HOLD:
        doInspHoldControl();
        break;
    default:
        break;
    }
}

void INOVentLLC::doInspHoldControl()
{
    int32_t    current_flow = 0;

    //int32_t pressure_err = m_targ_insp_pressure - m_raw_insp_pressure;

    // pressure_err * 0.1;
    // current_flow += pressure_err * gain;

    m_gasMixControl.setTotalFlow(current_flow);

    m_actuator.setTargetFlow(m_gasMixControl.getAirFlowSet(), m_gasMixControl.getO2FlowSet());
}


void INOVentLLC::doInspFlowControl()
{
    int32_t totalFlow = m_inspFlow;
    static int32_t delta_flow = 0;

    if (m_pressureCtlActive)
    {
        int32_t delta_press = m_inspPressureMax - m_inspPressure;

        if (INOV::ABS(delta_press) > 500)
        {
            delta_flow += (0.1 * delta_press);
            totalFlow += delta_flow;
        }
    }

    if (totalFlow <= (m_LLCParmeters.m_baseFlow + 50))
    {
        m_state = HOLD;
    }

    m_gasMixControl.setO2Percent(totalFlow, 21);

    m_actuator.setTargetFlow(m_gasMixControl.getAirFlowSet(), m_gasMixControl.getO2FlowSet());
}
