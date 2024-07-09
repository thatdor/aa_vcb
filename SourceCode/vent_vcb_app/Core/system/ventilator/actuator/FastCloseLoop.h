/*
 * FastCloseLoop.h
 *
 *  Created on: Mar 27, 2024
 *      Author: dawei.zhou
 */

#ifndef SYSTEM_VENTILATOR_ACTUATOR_FASTCLOSELOOP_H_
#define SYSTEM_VENTILATOR_ACTUATOR_FASTCLOSELOOP_H_
#include <stdint.h>
#include "../system/common/utility/INOLimit.h"

template <uint32_t SEGMENT_COUNT>
class PI
{
public:
    PI(float p = 0.01, float i = 0.01, float d = 0.01, int32_t min_err = 16) :
        m_kp(p),
        m_ki(i),
        m_kd(d),
        m_min_err(min_err),
        m_last_err(0),
        m_ierror(0),
        m_derror(0)
    {
    }

    // 限制输出大于0
    uint32_t run(int32_t set, int32_t cur)
    {
        int32_t ret = 0;
        int32_t error = set - cur;
        if ((error > (0 - m_min_err)) && (error < m_min_err))
        {
            error = 0;
        }
        m_ierror += error;
        if (set == 0)
        {
            m_ierror = 0;
        }

        m_derror = cur - m_last_err;
        m_ff = calc_feedforward(set);
        ret = (m_kp * error + m_ki * m_ierror + m_kd * m_derror) + m_ff;

        ret = (ret < 0) ? 0 : ret;
        m_last_err = cur;

        return ret;
    }

    // 测试算法使用
    void set_parm(float p, float i, float d = 0.01f)
    {
        if ((!INOV::float_equal(m_kp, p)) || (!INOV::float_equal(m_ki, i)) || (!INOV::float_equal(m_kd, d)))
        {
            m_kp = p;
            m_ki = i;
            m_kd = d;
            m_ierror = 0;
        }
    }

    void set_feedforward_parm(const float k[], const float b[], const int32_t d_value[]);

    void reset()
    {
        m_ierror = 0;
    }


private:
    uint32_t calc_feedforward(int32_t set);

private:
    float    m_kp;
    float    m_ki;
    float    m_kd;
    int32_t  m_min_err;
    int32_t  m_last_err;
    int32_t  m_ierror;
    int32_t  m_derror;
    uint32_t m_ff{0};

    // 分段函数
    int32_t  m_devide_value[SEGMENT_COUNT-1];
    float    m_k[SEGMENT_COUNT];
    float    m_b[SEGMENT_COUNT];
};

template <uint32_t SEGMENT_COUNT>
inline uint32_t PI<SEGMENT_COUNT>::calc_feedforward(int32_t set)
{
    float ff = 0;
    uint32_t i = 0;

    for (i = 0; i < (SEGMENT_COUNT - 1); ++i)
    {
        if (set < m_devide_value[i])
        {
            ff = m_k[i] * set + m_b[i];
            break;
        }
    }
    if (i == (SEGMENT_COUNT - 1))
    {
        ff = m_k[i] * set + m_b[i];
    }

    if (ff < 0)
    {
        return 0;
    }
    else
    {
        return (uint32_t)ff;
    }
}

template <uint32_t SEGMENT_COUNT>
inline void PI<SEGMENT_COUNT>::set_feedforward_parm(const float k[], const float b[], const int32_t d_value[])
{
    uint32_t i = 0;
    for (i = 0; i < (SEGMENT_COUNT-1); ++i)
    {
        m_devide_value[i] = d_value[i];
        m_k[i] = k[i];
        m_b[i] = b[i];
    }
    m_k[i] = k[i];
    m_b[i] = b[i];
}

class FastCloseLoop {
public:
    typedef enum CTL_MODE
    {
        CTL_MODE_FLOW,
        CTL_MODE_INVALID
    }CTL_MODE_E;

    virtual ~FastCloseLoop();
    static FastCloseLoop& getInstanceRef();
    void initialize();

    void clockrun();

    void setTargetFlow(int32_t air_flow, int32_t o2_flow);
    void setAirTargetFlow(int32_t air_flow);
    void setO2TargetFlow(int32_t o2_flow);
    void enableController(bool enable);
private:
    FastCloseLoop();
    int32_t  convertToFlow(float output);
    void     setAirPValve(uint32_t output, int32_t set_flow);
    void     setO2PValve(uint32_t output, int32_t set_flow);
    void     setVcmPValve(uint32_t output, int32_t set_flow);

private:
    static FastCloseLoop m_instance;
    CTL_MODE_E m_ctl_mode;
    int32_t    m_target_flow;
    int32_t    m_target_pressure;

    // 当前的吸入流量
    int32_t    m_o2_set_flow;
    int32_t    m_air_set_flow;

    // 系统实时的空气压力流量和压力
    int32_t    m_raw_o2_flow;
    int32_t    m_raw_air_flow;

    // PI相关控制数据
    // 需要考虑是否限制流量或压力
    PI<2>      m_air_PI;
    PI<3>      m_o2_PI;

    const int32_t m_max_air_flow;
    const int32_t m_max_o2_flow;
};

#endif /* SYSTEM_VENTILATOR_ACTUATOR_FASTCLOSELOOP_H_ */
