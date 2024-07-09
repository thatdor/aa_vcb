/*
 * LLCparameters.h
 *
 *  Created on: Mar 25, 2024
 *      Author: dawei.zhou
 */

#ifndef SYSTEM_VENTILATOR_ENGINE_LLCPARAMETERS_H_
#define SYSTEM_VENTILATOR_ENGINE_LLCPARAMETERS_H_
#include <stdint.h>
#include "../../common/enum/enum.h"


class LLCparameters {
public:
    LLCparameters();
    virtual ~LLCparameters();

    int32_t m_inspiratory;               // 第一阶段时间，吸气时间
    int32_t m_inspiratoryPause;          // 吸气暂停时间，吸气阀关闭，呼气阀设置最高的压力（仅在VC使用），结束时测量平台压力。
    int32_t m_inspiratoryPauseTrigger;   // 吸气屏气，与上一阶段相似，吸气阀关闭，呼气阀密封。

    int32_t m_expiratory;                // 呼气阶段时间
    int32_t m_expiratoryTrigger;         // 触发窗口，此阶段的产生的触发将导致新的机械循环
    int32_t m_expiratoryPauseTrigger;    // 最后阶段的时间，有创：回路密封，吸气阀关闭，NIV：吸气阀最小

    int32_t m_maxTidalVolume;           // 潮气量

    int32_t m_minFlow;                  // 引擎允许的最小目标流量。体积补偿的下限
    int32_t m_initialFlow;              // 引擎第一次检测到模式转换时的目标流量
    int32_t m_maxFlow;                  // 引擎允许的最大目标流量。体积补偿的上限

    int32_t m_maxExprFlow;              // 引擎允许的最大流量值
    PATIENT_TYPE_E m_patientType;       // 成人、儿童和新生儿

    int32_t m_triggerFlow;               // 吸气流量触发阈值
    int32_t m_triggerPressure;           // 吸气压力触发阈值

    int32_t m_minPressure;               // 引擎允许的最小吸气
    int32_t m_initialPressure;           // 设置更改时候引擎使用的吸气目标压力
    int32_t m_maxPressure;               // 引擎允许的最大吸气

    int32_t m_inspiratoryPressureSupport; // PS阶段吸气支持压力，0不支持
    int32_t m_expiratoryPressureSupport;  // PS阶段PEEP的压力水平，0不支持
    int32_t m_pressureSupportTimeout;     // 压力支持的最长时间
    int32_t m_supportTermination;         // 检测到触发，
    int32_t m_peep;                       // 呼气支持压力
    int32_t m_baseFlow;                   // 基础流量
    int32_t m_flowRiseTime;               // VC期间流量上升时间
    int32_t m_pressureRiseTime;           // 呼气阀到目标压力的时间
    int32_t m_pressureSupportRiseTime;    // 压力支持的上升时间
    int32_t m_plimit;                     // 用户设置定义的最大压力。引擎仅使用它来确定吸气暂停和保持压力目标
    int32_t m_inspPressureRamp;           // 呼气阀如何到目标值
    int32_t m_flowPattern;                // 吸气阶段如何控制流量
    int32_t m_ventOptions;                //
};

#endif /* SYSTEM_VENTILATOR_ENGINE_LLCPARAMETERS_H_ */
