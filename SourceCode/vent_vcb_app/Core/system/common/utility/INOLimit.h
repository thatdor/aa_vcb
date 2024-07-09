/*
 * INOLimit.h
 *
 *  Created on: Apr 3, 2024
 *      Author: dawei.zhou
 */

#ifndef SYSTEM_COMMON_UTILITY_INOLIMIT_H_
#define SYSTEM_COMMON_UTILITY_INOLIMIT_H_

#include <stdint.h>
#include <algorithm>

namespace INOV
{
    template <class T>
    T limitMax(T value, T maxValue)
    {
        return ((value < maxValue) ? value : maxValue);
    }

    template <class T>
    T limitMin(T value, T minValue)
    {
         return ((value < minValue) ? minValue : value);
    }

    template<int SCALE_FACTOR = 1000000>
    bool float_equal(float a, float b)
    {
        return (std::abs((a - b) * SCALE_FACTOR) < 1);
    }

    template <class T>
    T ABS(T value)
    {
        if (value < 0) return (0-value);
        else return value;
    }

}


#endif /* SYSTEM_COMMON_UTILITY_INOLIMIT_H_ */
