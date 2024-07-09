#include "main.h"
#include "cmsis_os.h"
#include "ad7689.h"


// Read ad7689 channel
int ad7689_read_raw(uint8_t ch, uint16_t *value)
{
    int ret = 0;
    uint16_t reg = 0;
    uint8_t txbuf[2] = {0};
    uint8_t rxbuf[2] = {0};

    reg = (1 << 13) | (7 << 10) | (ch << 7) | (0 << 6) | (1 << 3) | (0 << 1) | (0 << 0);

    reg <<= 2;

    txbuf[0] = (uint8_t)(reg >> 8);
    txbuf[1] = (uint8_t)(reg & 0xFF);

    HAL_GPIO_WritePin(ADC_SPI_CS_GPIO_Port, ADC_SPI_CS_Pin, GPIO_PIN_RESET);
//    delay(15);
    ret = HAL_SPI_TransmitReceive(&hspi1, txbuf, rxbuf, sizeof(txbuf), 20);
//    delay(15);
    HAL_GPIO_WritePin(ADC_SPI_CS_GPIO_Port, ADC_SPI_CS_Pin, GPIO_PIN_SET);
    *value = (rxbuf[0] << 8) | rxbuf[1];

    return ret;
}


uint16_t g_ad_raw_ad_value[E_AD_AD_CHNNEL_NUM_MAX][AD_RAW_DATA_BUFFER_COUNT] = {{0}};
uint8_t g_ad_value_index[E_AD_AD_CHNNEL_NUM_MAX] = {0};
uint32_t g_ad_read_err_count[E_AD_AD_CHNNEL_NUM_MAX] = {0};
void ad7689_scan_run()
{
    int ret = 0;
    int value_index = 0;
    static int ch = 0;

    value_index = (ch + E_AD_AD_CHNNEL_NUM_MAX - 2) % E_AD_AD_CHNNEL_NUM_MAX;

    ret = ad7689_read_raw(ch, &g_ad_raw_ad_value[value_index][g_ad_value_index[value_index]]);

    ++g_ad_value_index[value_index];
    if (g_ad_value_index[value_index] >= AD_RAW_DATA_BUFFER_COUNT)
    {
        g_ad_value_index[value_index] = 0;
    }
    if (ret != 0)
    {
        ++g_ad_read_err_count[ch];
    }

    ++ch;
    if (ch >= E_AD_AD_CHNNEL_NUM_MAX)
    {
        ch = 0;
    }

}


int ad_read_filter(int ch, uint16_t *fdata)
{
    uint32_t i = 0;
    uint32_t sum = 0;
    if (ch < E_AD_AD_CHNNEL_NUM_MAX)
    {
        for (i = 0; i < AD_RAW_DATA_BUFFER_COUNT; ++i)
        {
            sum += g_ad_raw_ad_value[ch][i];
        }
        *fdata = sum / AD_RAW_DATA_BUFFER_COUNT;

        return 0;
    }

    return -1;
}

// 输入端压力P-(INP LOOp PRES-1000mV)/80/2.5
// Vinsp = Vo * 10 / (10 + 5.23)
// Pinsp * 0.8 * Vs / 160 + 0.1 * Vs = Vo   // Vs=5V, Vinsp=adv, Pmax=160mbar
// Pinsp = [(adv * 4.096 / 65535) * (10 + 5.23) / 10 - 0.1 * 5] * (160 / (0.8 * 5))
void ad_get_insp_pressure(int32_t *insp_press, uint32_t *err_cnt)
{
    uint16_t adv = 0;

    ad_read_filter(E_AD_INP_LOOP_PRESS, &adv);

    *insp_press = ((float)(adv) * 4.096 / 65535 * (10 + 5.23) / 10 - 0.5) * 40 * 1000; // 单位:0.001mbar
    if (err_cnt != NULL)
    {
        *err_cnt = g_ad_read_err_count[E_AD_INP_LOOP_PRESS];
    }
}

#if 0
uint16_t ad7689_get_gnd()
{
    uint16_t adv = 0;

    ad_read_filter(GND, &adv);

    return adv;
}
uint16_t ad7689_get_air_o2_pressure(int ch)
{
    uint16_t adv = 0;

    ad_read_filter(ch, &adv);

    return ((float)(adv) / 65535 * 4.096 * 1.56 - 0.5) * 981 / 4;

}
uint16_t ad7689_get_air_pressure()
{
    return ad7689_get_air_o2_pressure(GND1);
}

uint16_t ad7689_get_o2_pressure()
{
    return ad7689_get_air_o2_pressure(GND2);
}


// 输入端压力P-(INP LOOp PRES-1000mV)/80/2.5
void ad7689_get_insp_pressure(uint16_t *insp_press, uint32_t *err_cnt)
{
    uint16_t adv = 0;

    ad_read_filter(INP_LOOP_PRESSURE, &adv);

    *insp_press = ((float)(adv) * 4.096 / 65535 * 15.23 / 10 - 0.5) * 40 + 1;
    if (err_cnt != NULL)
    {
        *err_cnt = g_ad_read_err_count[INP_LOOP_PRESSURE];
    }
}

uint16_t ad7689_get_press_calc(void)
{
    uint16_t adv = 0;
    float presscalc = 0;

    ad_read_filter(INP_LOOP_PRESSURE, &adv);

    presscalc = ((float)(adv) * 4.096 / 65535 * 15.23 / 10 - 0.5) * 40;

    if ((presscalc < 0) && (adv > 4500) && (adv < 5050))
    {
//        persszerocalc = presscalc;
    }
    return presscalc;
}

uint16_t ad7689_get_exp_pressure()
{
    uint16_t adv = 0;

    ad_read_filter(INP_LOOP_PRESSURE, &adv);

    return (((float)adv * 3.3 * 1000) / 65535 - 1000) / 80 / 2.5;
}

uint16_t ad7689_get_o2_valve_fb()
{
    uint16_t adv = 0;

    ad_read_filter(O2_VALVE_FB, &adv);

    return adv;
}

uint16_t ad7689_get_temp_fb()
{
    uint16_t adv = 0;

    ad_read_filter(Temp_FB, &adv);

    return adv;
}

uint16_t ad7689_get_air_valve_fb()
{
    uint16_t adv = 0;

    ad_read_filter(AIR_VALVE_FB, &adv);

    return adv;
}

uint16_t ad7689_get_vcm_valve_fb()
{
    uint16_t adv = 0;

    ad_read_filter(VCM_VALVE_FB, &adv);

    return adv;
}
#endif
