/*
 * VMBProxy.cpp
 *
 *  Created on: Mar 27, 2024
 *      Author: dawei.zhou
 */
#include "VMBProxy.h"
#include "../system/msg_boundary/vcb_vmb/vcb_vmb_msg_fun.h"

VMBProxy VMBProxy::m_instance;

VMBProxy::VMBProxy() {
    // TODO Auto-generated constructor stub
}

VMBProxy::~VMBProxy() {
    // TODO Auto-generated destructor stub
}

VMBProxy& VMBProxy::getInstanceRef()
{
    return m_instance;
}

int  VMBProxy::initialize()
{
    vcb_vmb_uart_init();
}

void VMBProxy::setStatusBits(int32_t bits)
{
    m_status_bits = bits;
}
void VMBProxy::setExpPressure(int32_t press)
{
    m_exp_pressure = press;
}
void VMBProxy::setAirSupplyPressure(int32_t press)
{
    m_air_supply_pressure = press;
}
void VMBProxy::setO2SupplyPressure(int32_t press)
{
    m_o2_supply_pressure = press;
}
void VMBProxy::setExpFlow(int32_t flow)
{
    m_exp_flow = flow;
}

void VMBProxy::setExpFlowErrorCount(uint32_t count)
{
    m_exp_flow_error_count = count;
}

void VMBProxy::setExpFlowResetCount(uint32_t count)
{
    m_exp_flow_reset_count = count;
}

void VMBProxy::setPhtErrorCount(uint32_t count)
{
    m_pht_error_count = count;
}

void VMBProxy::setPhtResetCount(uint32_t count)
{
    m_pht_reset_count = count;
}

int32_t VMBProxy::getExpPressure()
{
    return m_exp_pressure;
}
int32_t VMBProxy::getAirSupplyPressure()
{
    return m_air_supply_pressure;
}
int32_t VMBProxy::getO2SupplyPressure()
{
    return m_o2_supply_pressure;
}
int32_t VMBProxy::getExpFlow()
{
    return m_exp_flow;
}

uint32_t VMBProxy::getExpFlowErrorCount()
{
    return m_exp_flow_error_count;
}

uint32_t VMBProxy::getExpFlowResetCount()
{
    return m_exp_flow_reset_count;
}

uint32_t VMBProxy::getPhtErrorCount()
{
    return m_pht_error_count;
}

uint32_t VMBProxy::getPhtResetCount()
{
    return m_pht_reset_count;
}
