/*
 * INOVFilter.h
 *
 *  Created on: May 20, 2024
 *      Author: haiyang.zhu
 */

#ifndef SYSTEM_COMMON_UTILITY_INOVFilter_H_
#define SYSTEM_COMMON_UTILITY_INOVFilter_H_

#include <stdint.h>
#include <algorithm>
#include "INOLimit.h"

template <class T, uint32_t ARR_COUNT>
class INOVFilter
{
public:
    static void Append(T data[ARR_COUNT], T value);
    // gauss filter, sigma between [1, 10]
    static void generateGaussianKernel(float kernel[], int size, float sigma);
    static T gaussianFilter(const T data[ARR_COUNT], const float kernel[], int ksize, float sigma, int currentIndex);
    static void gaussianFilter(const T data[ARR_COUNT], const float kernel[], int ksize, float sigma, T filteredData[ARR_COUNT]);

    // bessel filter, sigma between (0, 1)
    static void generateBesselCoef(float b[], float a[], float sigma);
    static void besselFilter(const float b[], const float a[], int coefSize, const T data[ARR_COUNT], T filteredData[ARR_COUNT]);

    static T emotionalMovingAverageFilter(T value, T lastFiltedValue, float alpha);

    static void movingAverageFilter( T input[], T output[], int windowSize);

private:
    // float m_sigma;

};

template <class T, uint32_t ARR_COUNT>
inline void INOVFilter<T, ARR_COUNT>::Append(T data[ARR_COUNT], T value)
{
    for (uint32_t i = 0; i < ARR_COUNT - 1; ++i)
    {
        data[i] = data[i + 1];
    }
    data[ARR_COUNT - 1] = value;
}



template <class T, uint32_t ARR_COUNT>
void INOVFilter<T, ARR_COUNT>::generateGaussianKernel(float kernel[], int size, float sigma) {
    float sum = 0.0;
    int halfSize = size / 2;
    if (INOV::float_equal(sigma, 0.0))
    {
        return;
    }

    // 计算高斯核
    for (int i = 0; i < size; ++i) {
        float x = i - halfSize;
        kernel[i] = exp(-0.5 * (x * x) / (sigma * sigma));
        sum += kernel[i];
    }

    // 归一化高斯核
    for (int i = 0; i < size; ++i) {
        kernel[i] /= sum;
    }

    return;
}

template <class T, uint32_t ARR_COUNT>
T INOVFilter<T, ARR_COUNT>::gaussianFilter(const T data[ARR_COUNT], const float kernel[], int ksize, float sigma, int currentIndex)
{

    int halfSize = ksize / 2;
    int dataSize = ARR_COUNT;

    // 应用高斯滤波
    float sum = 0.0;
    for (int j = -halfSize; j <= halfSize; ++j) {
        int index = std::min(std::max(currentIndex + j, 0), dataSize - 1);
        sum += data[index] * kernel[j + halfSize];
    }

    return static_cast<T>(sum);
}

template <class T, uint32_t ARR_COUNT>
void INOVFilter<T, ARR_COUNT>::gaussianFilter(const T data[ARR_COUNT], const float kernel[], int ksize, float sigma, T filteredData[ARR_COUNT])
{
    for (int i = 0; i < ARR_COUNT; ++i)
    {
        filteredData[i] = gaussianFilter(data, kernel, ksize, sigma, i);
    }
    return;
}

template <class T, uint32_t ARR_COUNT>
void INOVFilter<T, ARR_COUNT>::generateBesselCoef(float b[], float a[], float sigma)
{
    static float besselCoef[][6] = {
        {0}, {0},
        // 0.1
        { 5.526481775061669e-05, 0.00027632408875308343, 0.0005526481775061669, 0.0005526481775061669, 0.00027632408875308343, 5.526481775061669e-05 },
        { 1.0, -3.8510736171866276, 5.998560773842058, -4.7195001286055325, 1.8740092824903631, -0.30022783637224093 },
        // 0.2
        { 0.001118651209262189, 0.005593256046310945, 0.01118651209262189, 0.01118651209262189, 0.005593256046310945, 0.001118651209262189 },
        { 1.0, -2.793670664859824, 3.304276138324569, -2.043902191448633, 0.6559835362332858, -0.0868899795530069 },
        // 0.30000000000000004
        { 0.005809337550633008, 0.029046687753165042, 0.058093375506330085, 0.058093375506330085, 0.029046687753165042, 0.005809337550633008 },
        { 1.0, -1.8109761651613576, 1.5929436087158804, -0.7793357748419085, 0.20656287190447326, -0.023295738996831364 },
        // 0.4
        { 0.017937505600887343, 0.08968752800443672, 0.17937505600887343, 0.17937505600887343, 0.08968752800443672, 0.017937505600887343 },
        { 1.0, -0.8806775415466561, 0.6484930733734532, -0.24451679905571289, 0.05618249915295434, -0.0054810526956437155 },
        // 0.5
        { 0.042716740253909875, 0.21358370126954937, 0.42716740253909874, 0.42716740253909874, 0.21358370126954937, 0.042716740253909875 },
        { 1.0, 0.020779156836258375, 0.3489054872642596, -0.02010046109542645, 0.0180302008608563, -0.0006786957408327351 },
        // 0.6
        { 0.08804240270658811, 0.4402120135329406, 0.8804240270658812, 0.8804240270658812, 0.4402120135329406, 0.08804240270658811 },
        { 1.0, 0.91723191674366, 0.6514938821806134, 0.20313358506806134, 0.042363252820684164, 0.0031342497977988088 },
        // 0.7000000000000001
        { 0.16719443505111595, 0.8359721752555798, 1.6719443505111595, 1.6719443505111595, 0.8359721752555798, 0.16719443505111595 },
        { 1.0, 1.83346336638717, 1.58833157610956, 0.7316790810338332, 0.17864885340099199, 0.018099044704152762 },
        // 0.8
        { 0.30445651137715674, 1.5222825568857836, 3.0445651137715672, 3.0445651137715672, 1.5222825568857836, 0.30445651137715674 },
        { 1.0, 2.796724758343955, 3.2760035207580773, 1.9794753873534996, 0.6129417523502062, 0.07746294526327119 },
        // 0.9
        { 0.5477598592569628, 2.7387992962848138, 5.4775985925696276, 5.4775985925696276, 2.7387992962848138, 0.5477598592569628 },
        { 1.0, 3.8389583629773743, 5.945022510425791, 4.636929254019652, 1.820008757493331, 0.28739661130665356 },
    };
    if (sigma < 0.1 || sigma > 0.9) {
        return;
    }
    for (int i = 0; i < 6; ++i)
    {
        int index = static_cast<int>(sigma * 10);
        b[i] = besselCoef[index * 2][i];
        a[i] = besselCoef[index * 2 + 1][i];
    }
}

// b a 可通过scipy.signal.bessel计算出来, a.size() == b.size() = coefSize = N + 1, N为滤波器阶数
template <class T, uint32_t ARR_COUNT>
void INOVFilter<T, ARR_COUNT>::besselFilter(const float b[], const float a[], int coefSize, const T data[ARR_COUNT], T filteredData[ARR_COUNT])
{
    // Implement direct form II transposed (DF-II T) structure for filtering
    for (int n = 0; n < ARR_COUNT; ++n)
    {
        // Initialize variables for accumulator and feedforward terms
        float accumulator = 0;
        float feedforward = 0;

        // Calculate feedforward term using past input samples
        for (int i = 0; i < coefSize; ++i)
        {
            if (n - i >= 0)
            {
                feedforward += b[i] * data[n - i];
            }
        }

        // Calculate accumulator term using past output samples and current input sample
        for (int i = 1; i < coefSize; ++i)
        {
            if (n - i >= 0)
            {
                accumulator += a[i] * filteredData[n - i];
            }
        }

        // Calculate current output sample
        filteredData[n] = feedforward - accumulator;
    }

    return;
}

// EMA 指数加权移动平均滤波
template <class T, uint32_t ARR_COUNT>
T INOVFilter<T, ARR_COUNT>::emotionalMovingAverageFilter(T value, T lastFiltedValue, float alpha)
{
    return static_cast<T>((1 - alpha) * lastFiltedValue + alpha * value);
}

// 移动平均滤波函数
template <class T, uint32_t ARR_COUNT>
void INOVFilter<T, ARR_COUNT>::movingAverageFilter( T input[], T output[], int windowSize)
{
    if (windowSize <= 0 || windowSize > ARR_COUNT) {
        return;
    }
    float sum = 0.0f;

    // 初始化窗口的第一个平均值
    for (int i = 0; i < windowSize; ++i) {
        sum += input[i];
    }
    output[0] = static_cast<T>(sum / windowSize);

    // 滑动窗口并计算每个位置的平均值
    for (size_t i = 1; i < ARR_COUNT - windowSize + 1; ++i) {
        sum = sum - input[i - 1] + input[i + windowSize - 1]; // 移除窗口左侧的值，加上右侧的值
        output[i] = static_cast<T>(sum / windowSize);
    }

    return;
}

#endif /* SYSTEM_COMMON_UTILITY_INOVFilter_H_ */
