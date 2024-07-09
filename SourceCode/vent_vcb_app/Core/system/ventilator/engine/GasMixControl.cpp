/*
 * GasMixControl.cpp
 *
 *  Created on: Apr 2, 2024
 *      Author: dawei.zhou
 */

#include "GasMixControl.h"
#include "../system/common/utility/INOLimit.h"

#define GAS_MIX_CONTROL_O2_PERCENT_MAXMUM  100000
#define GAS_MIX_CONTROL_O2_PERCENT_MINIMUM  20900

GasMixControl::GasMixControl() :
   m_airFlowSet(0),
   m_o2FlowSet(0),
   m_o2Percent(GAS_MIX_CONTROL_O2_PERCENT_MINIMUM){

}

GasMixControl::~GasMixControl() {
    // TODO Auto-generated destructor stub
}

// userSetOxygentPercent: 20.9% -->20900
void GasMixControl::setO2Percent(int32_t totalFlow, int32_t userSetOxygentPercent)
{
    userSetOxygentPercent = INOV::limitMin<int32_t>(userSetOxygentPercent, GAS_MIX_CONTROL_O2_PERCENT_MINIMUM);
    userSetOxygentPercent = INOV::limitMax<int32_t>(userSetOxygentPercent, GAS_MIX_CONTROL_O2_PERCENT_MAXMUM);

    if (userSetOxygentPercent == GAS_MIX_CONTROL_O2_PERCENT_MINIMUM)
    {
        m_airFlowSet = totalFlow;
        m_o2FlowSet  = 0;
    }
    else if (userSetOxygentPercent == GAS_MIX_CONTROL_O2_PERCENT_MAXMUM)
    {
        m_airFlowSet = 0;
        m_o2FlowSet  = totalFlow;
    }
    else
    {
        m_o2FlowSet = (totalFlow * (userSetOxygentPercent - GAS_MIX_CONTROL_O2_PERCENT_MINIMUM))
                 / (GAS_MIX_CONTROL_O2_PERCENT_MAXMUM - GAS_MIX_CONTROL_O2_PERCENT_MINIMUM);
        m_airFlowSet = totalFlow - m_o2FlowSet;
    }

    m_o2Percent = userSetOxygentPercent;
}

void GasMixControl::setTotalFlow(int32_t totalFlow)
{
    setO2Percent(totalFlow, GAS_MIX_CONTROL_O2_PERCENT_MINIMUM);
}
