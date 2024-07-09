/*
 * StableValueChecker.h
 *
 *  Created on: Mar 25, 2024
 *      Author: dawei.zhou
 */

#ifndef SYSTEM_COMMON_UTILITY_STABLEVALUECHECKER_H_
#define SYSTEM_COMMON_UTILITY_STABLEVALUECHECKER_H_
#include <stdint.h>

template <class T, uint32_t ARR_COUNT>
class StableValueChecker {
public:
	StableValueChecker(T minValid, T maxValid, T stabilityFactor);
	~StableValueChecker();

	bool isStable() const;
	void add(T value);
	void reset();

private:
	StableValueChecker(const StableValueChecker< T,ARR_COUNT> &right);
	StableValueChecker<T, ARR_COUNT> & operator=(const StableValueChecker<T, ARR_COUNT> &right);
	void updateMinMax(T newValue);
	void checkStability();

private:
	uint32_t m_points_count;
	T        m_values[ARR_COUNT];
	bool     m_isStable;
	const T  MIN;
	const T  MAX;
	const T  STABILITY;
	T        m_minV;
	T        m_maxV;
};

template <class T, uint32_t ARR_COUNT>
inline StableValueChecker<T, ARR_COUNT>::StableValueChecker(const StableValueChecker< T,ARR_COUNT> &right)
	: m_points_count(0),
	  m_isStable(false),
	  MIN(right.MIN),
	  MAX(right.MAX),
	  STABILITY(right.STABILITY),
	  m_minV(0),
	  m_maxV(0)
{

}

template <class T, uint32_t ARR_COUNT>
inline StableValueChecker<T, ARR_COUNT>::StableValueChecker(T minValid, T maxValid, T stabilityFactor)
: m_points_count(0),
  m_isStable(false),
  MIN(minValid),
  MAX(maxValid),
  STABILITY(stabilityFactor),
  m_minV(0),
  m_maxV(0)
{

}

template <class T, uint32_t ARR_COUNT>
inline StableValueChecker<T, ARR_COUNT>::~StableValueChecker()
{

}

template <class T, uint32_t ARR_COUNT>
inline StableValueChecker<T, ARR_COUNT> &StableValueChecker<T, ARR_COUNT>::operator=(const StableValueChecker<T, ARR_COUNT>& right)
{
	return *this;
}

template <class T, uint32_t ARR_COUNT>
inline bool StableValueChecker<T, ARR_COUNT>::isStable() const
{
	return m_isStable;
}

template <class T, uint32_t ARR_COUNT>
inline void StableValueChecker<T, ARR_COUNT>::add(T newValue)
{
	if ((newValue < MIN) || (newValue > MAX))
	{
		reset();
		return;
	}
	uint32_t index = static_cast<uint32_t>(m_points_count % ARR_COUNT);

	m_values[index] = newValue;

	if (m_points_count == 0)
	{
		m_minV = m_maxV = newValue;
	}
	else if (m_points_count > ARR_COUNT)
	{
		m_minV = m_maxV = m_values[0];
		for (int i = 1; i < ARR_COUNT; ++i)
		{
			updateMinMax(m_values[i]);
		}
	}
	else
	{
		updateMinMax(newValue);
	}
	++m_points_count;

	checkStability();
}

template <class T, uint32_t ARR_COUNT>
inline void StableValueChecker<T, ARR_COUNT>::reset()
{
	m_points_count = 0;
	m_isStable = false;
}

template <class T, uint32_t ARR_COUNT>
inline void StableValueChecker<T, ARR_COUNT>::updateMinMax(T newValue)
{
	if (newValue > m_maxV)
	{
		m_maxV = newValue;
	}
	else if (newValue < m_minV)
	{
		m_minV = newValue;
	}
}

template <class T, uint32_t ARR_COUNT>
inline void StableValueChecker<T, ARR_COUNT>::checkStability()
{
	bool isStable = false;

	if ((m_points_count >= ARR_COUNT) && ((m_maxV - m_minV) <= STABILITY))
	{
		isStable = true;
	}
	m_isStable = isStable;
}


#endif /* SYSTEM_COMMON_UTILITY_STABLEVALUECHECKER_H_ */
