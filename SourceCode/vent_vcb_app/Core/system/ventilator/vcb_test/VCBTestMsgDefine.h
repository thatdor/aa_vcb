/*
 * VCBTestMsgDefine.h
 *
 *  Created on: Apr 17, 2024
 *      Author: dawei.zhou
 */

#ifndef SYSTEM_VENTILATOR_VCB_TEST_VCBTESTMSGDEFINE_H_
#define SYSTEM_VENTILATOR_VCB_TEST_VCBTESTMSGDEFINE_H_

#ifdef __cplusplus
extern "C" {
#endif

// CB ====> PC 消息定义
// 32B
typedef struct tagVCB_TEST_VCB_FLOW_WAVE_DATA
{
    // Air流量，O2流量，exFlow，inPressure，exPressure,airPressure, O2Pressure
    int32_t  airFlow;
    int32_t  o2Flow;
    int32_t  airFlowSet;  // 空气MFC的目标流量
    int32_t  o2FlowSet;   // 氧气MFC的目标流量
    int32_t  expFlow;     // 呼气流量
    int32_t  inspPressure;
    int16_t  inspPressureSet;
    int32_t  expPressure;
    int16_t  expPressureSet;
    // TO DEL +8B
    // int16_t inspPressureGauss;
    // int16_t inspPressureBessel;
    // int16_t inspPressureEMA;
    // int16_t inspPressureMA;
}VCB_TEST_VCB_FLOW_WAVE_DATA_T;

// 8B
typedef struct tagVCB_TEST_VCB_WAVE_DA_DATA
{
    uint16_t expVCMDA;
    uint16_t airValveDA;
    uint16_t o2ValveDA;
    uint16_t VCM_PWM;
}VCB_TEST_VCB_WAVE_DA_DATA;

// 16B
typedef struct tagVCB_TEST_VCB_VOLT_INFO
{
    uint16_t VCB_12V_VAL;
    uint16_t VCB_7V_VAL;
    uint16_t VCB_3V3_VAL;
    uint16_t VCB_5V_VAL;
    uint16_t VMB_12V_VAL;
    uint16_t VMB_7V_VAL;
    uint16_t VMB_3V3_VAL;
    uint16_t VMB_5V_VAL;
}VCB_TEST_VCB_VOLT_INFO_T;

// 16B
typedef struct tagVCB_TEST_VCB_PRESS_DATA
{
    uint16_t  o2_sig;
    uint16_t  valve_flag;
    uint16_t  airAD;
    uint16_t  o2AD;
    uint16_t  airPressure;
    uint16_t  o2Pressure;
    uint16_t  expVCMAD;
    uint16_t  VCM_PWMAD;
}VCB_TEST_VCB_PRESS_DATA_T;

// 32B
typedef struct tagVCB_TEST_VCB_IIC_ERROR
{
    uint32_t  airFlowErrCount;
    uint32_t  airFlowResetCount;
    uint32_t  o2FlowErrCount;
    uint32_t  o2FlowResetCount;
    uint32_t  expFlowErrCount;
    uint32_t  expFlowResetCount;
    uint32_t  phtErrCount;
    uint32_t  phtResetCount;
} VCB_TEST_VCB_IIC_ERROR_T;

#define VCB_TEST_MSG_T_ID_1  0xF1
typedef struct tagVCB_TEST_MSG_TYPE_1_T
{
    uint16_t id;                             // 0xF1
    uint16_t seq_id;                         // 序列号
    uint32_t tick;                           // 16B

    VCB_TEST_VCB_FLOW_WAVE_DATA_T flowWave;  // 32B;
    VCB_TEST_VCB_WAVE_DA_DATA     DAData;    // 8B;
    VCB_TEST_VCB_PRESS_DATA_T     pressData; // 16B;  // 72B

}VCB_TEST_MSG_TYPE_1_T;

#define VCB_TEST_MSG_T_ID_2  0xF2
typedef struct tagVCB_TEST_MSG_TYPE_2_T
{
    uint16_t id;                             // 0xF1
    uint16_t seq_id;                         // 序列号
    uint32_t tick;                           // 16B

    VCB_TEST_VCB_FLOW_WAVE_DATA_T flowWave;  // 32B;
    VCB_TEST_VCB_VOLT_INFO_T      voltValue; // 16B;
    VCB_TEST_VCB_VOLT_INFO_T      voltDA;    // 16B; // 80B
}VCB_TEST_MSG_TYPE_2_T;

#define VCB_TEST_MSG_T_ID_3  0xF3
typedef struct tagVCB_TEST_MSG_TYPE_3_T
{
    uint16_t id;                             // 0xF3
    uint16_t seq_id;                         // 序列号
    uint32_t tick;                           // 16B
    VCB_TEST_VCB_FLOW_WAVE_DATA_T flowWave;  // 32B;
    VCB_TEST_VCB_IIC_ERROR_T errData;        // 32B  // 80B
}VCB_TEST_MSG_TYPE_3_T;

//////////////////////////////////////////////////////////
// PC ==========> CB 消息定义

typedef struct tagVCB_TEST_PC_SET_MFC_TEST
{
#define VCB_PC_TEST_FLAG_MFC_AIR_FLOW_SET 0x01
#define VCB_PC_TEST_FLAG_MFC_O2_FLOW_SET  0x02
#define VCB_PC_TEST_FLAG_MFC_AIR_PI_SET   0x04
#define VCB_PC_TEST_FLAG_MFC_O2_PI_SET    0x08
    uint32_t flag;
    int32_t air_flow_set;
    int32_t o2_flow_set;
    float air_p;
    float air_i;
    float air_d;
    float o2_p;
    float o2_i;
    float o2_d;
}VCB_TEST_PC_SET_MFC_TEST;

//typedef struct tagVCB_TEST_PC_SET_DA_TEST
//{
//
//}VCB_TEST_PC_SET_DA_TEST;

#define VCB_TEST_PC_SET_REQ_MSG_T_ID_1  0xE1
typedef struct tagVCB_TEST_PC_SET_REQ_MSG_TYPE_1_T
{
    uint16_t id;                             // 0xE1
    uint16_t seq_id;                         // 序列号
#define VCB_PC_TEST_FLAG_AIR_DA_SET   0x01
#define VCB_PC_TEST_FLAG_O2_DA_SET    0x02
#define VCB_PC_TEST_FLAG_TC_PWM_SET   0x04
#define VCB_PC_TEST_FLAG_VCM_DA_SET   0x08
    uint32_t flag;
    uint16_t  airPSV_DA_Set;                 //  Air比例阀
    uint16_t  o2PSV_DA_Set;                  // O2比例阀
    uint16_t  tcPWM_Set;                     // TC PWM
    uint16_t  VCM_DA_Set;                    // VCM

    VCB_TEST_PC_SET_MFC_TEST mfc;
}VCB_TEST_PC_SET_REQ_MSG_TYPE_1_T;

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_VENTILATOR_VCB_TEST_VCBTESTMSGDEFINE_H_ */
