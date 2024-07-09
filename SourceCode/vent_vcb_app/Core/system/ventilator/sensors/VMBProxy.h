/*
 * VMBProxy.h
 *
 *  Created on: Mar 27, 2024
 *      Author: dawei.zhou
 */

#ifndef SYSTEM_VENTILATOR_SENSORS_VMBPROXY_H_
#define SYSTEM_VENTILATOR_SENSORS_VMBPROXY_H_
#include <stdint.h>

class VMBProxy {
public:
    VMBProxy();
    virtual ~VMBProxy();
    static VMBProxy& getInstanceRef();
    int  initialize();
    void setStatusBits(int32_t bits);
    void setExpPressure(int32_t press);
    void setAirSupplyPressure(int32_t press);
    void setO2SupplyPressure(int32_t press);
    void setExpFlow(int32_t flow);
    void setExpFlowErrorCount(uint32_t count);
    void setExpFlowResetCount(uint32_t count);
    void setPhtErrorCount(uint32_t count);
    void setPhtResetCount(uint32_t count);
    int32_t getExpPressure();
    int32_t getAirSupplyPressure();
    int32_t getO2SupplyPressure();
    int32_t getExpFlow();

    uint32_t getExpFlowErrorCount();
    uint32_t getExpFlowResetCount();
    uint32_t getPhtErrorCount();
    uint32_t getPhtResetCount();
private:
    static VMBProxy     m_instance;
    uint32_t            m_status_bits;
    int32_t             m_exp_pressure;
    int32_t             m_air_supply_pressure;
    int32_t             m_o2_supply_pressure;
    int32_t             m_exp_flow;

    // IIC error count
    uint32_t            m_exp_flow_error_count;
    uint32_t            m_exp_flow_reset_count;
    uint32_t            m_pht_error_count;
    uint32_t            m_pht_reset_count;
};

#endif /* SYSTEM_VENTILATOR_SENSORS_VMBPROXY_H_ */
