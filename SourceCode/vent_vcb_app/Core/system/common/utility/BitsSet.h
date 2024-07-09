/*
 * BitsSet.h
 *
 *  Created on: Apr 17, 2024
 *      Author: dawei.zhou
 */

#ifndef SYSTEM_COMMON_UTILITY_BITSSET_H_
#define SYSTEM_COMMON_UTILITY_BITSSET_H_

template <typename T, int W_BITS, int BITS_COUNT>
class BitsSet
{
public:
    BitsSet() {};
    virtual ~BitsSet() {};

    void set(int index);
    void clear(int index);
    bool test(int index);
    void getBuf(T* buf, int buflen);
private:
    int wordIndex(int index);
    int bitIndex(int index);
private:

    const int m_words_count{(BITS_COUNT + (W_BITS - 1))/W_BITS};
    T         m_bits[(BITS_COUNT + (W_BITS - 1))/W_BITS];
};

template <typename T, int W_BITS, int BITS_COUNT>
inline int BitsSet<T, W_BITS, BITS_COUNT>::wordIndex(int index)
{
    return ((index)/W_BITS);
}

template <typename T, int W_BITS, int BITS_COUNT>
inline int BitsSet<T, W_BITS, BITS_COUNT>::bitIndex(int index)
{
    return (index % W_BITS);
}

template <typename T, int W_BITS, int BITS_COUNT>
inline void BitsSet<T, W_BITS, BITS_COUNT>::set(int index)
{
    m_bits[wordIndex(index)] |= (1 << bitIndex(index));
}

template <typename T, int W_BITS, int BITS_COUNT>
void BitsSet<T, W_BITS, BITS_COUNT>::clear(int index)
{
    m_bits[wordIndex(index)] &= ~(1 << bitIndex(index));
}

template <typename T, int W_BITS, int BITS_COUNT>
bool BitsSet<T, W_BITS, BITS_COUNT>::test(int index)
{
    return !!(m_bits[wordIndex(index)] & (1 << bitIndex(index)));
}

template <typename T, int W_BITS, int BITS_COUNT>
void BitsSet<T, W_BITS, BITS_COUNT>::getBuf(T* buf, int buflen)
{
    int len = (m_words_count < buflen) ? m_words_count : buflen;
    for (int i = 0; i < len; ++i)
    {
        buf[i] = m_bits[i];
    }
}

#endif /* SYSTEM_COMMON_UTILITY_BITSSET_H_ */
