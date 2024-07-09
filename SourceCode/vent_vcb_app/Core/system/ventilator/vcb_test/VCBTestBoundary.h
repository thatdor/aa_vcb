/*
 * VMBTestBoudary.h
 *
 *  Created on: Mar 27, 2024
 *      Author: dawei.zhou
 */

#ifndef SYSTEM_VENTILATOR_ENGINE_VCBTESTBOUNDARY_H_
#define SYSTEM_VENTILATOR_ENGINE_VCBTESTBOUNDARY_H_
#include <stdint.h>
#include "VCBTestMsgDefine.h"

#define VCB_TEST_MSG_BUF_LEN  80

class VCBTestBoundary {
public:
    virtual ~VCBTestBoundary();
    static VCBTestBoundary& getInstanceRef();

    void clockrun();

    // 处理收到的配置消息
    void procConfigData(uint8_t* buf, uint32_t len);
    void enableMonitorDataTransmit(bool enable);
    void testRequestProcEntry();
    inline bool enable();

public:
    VCB_TEST_PC_SET_REQ_MSG_TYPE_1_T m_user_set;
    VCB_TEST_PC_SET_REQ_MSG_TYPE_1_T m_user_set_fb;
private:
    VCBTestBoundary();
    void bulidTestMessage(uint32_t index);
private:
    static VCBTestBoundary m_instance;

    uint8_t         m_msgBuf[VCB_TEST_MSG_BUF_LEN];
    bool            m_enable;
};


bool VCBTestBoundary::enable()
{
    return m_enable;
}

#endif /* SYSTEM_VENTILATOR_ENGINE_VCBTESTBOUNDARY_H_ */
