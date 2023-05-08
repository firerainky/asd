#ifndef ZJ_FHE_LIB_BasicMath_H
#define ZJ_FHE_LIB_BasicMath_H

#include <iostream>

namespace basic_math {

    template<typename NativeInt>
    uint8_t addWithCarry(NativeInt operand1, NativeInt operand2, uint8_t carry, NativeInt *result) {
        operand1 += operand2;
        *result = operand1 + carry;
        return (operand1 < operand2) || (~operand1 < carry);
    }

    /**
     * @brief Karatsuba 算法计算 NativeInt * NativeInt, 结果为一个两倍于 NativeInt 长度的数
     *
     * (a * 2^n + b) * (c * 2^n + d) = ac*2^2n + (ad + bc)*2^n + bd
     * 这里 n = NativeInt 长度 / 2，下面的计算时由于进位的原因，做了多次平移。
     */
    template<typename NativeInt>
    inline void MultiplyWithKaratsuba(NativeInt operand1, NativeInt operand2, NativeInt *resultTwo) {
        NativeInt mask = 0x0;
        size_t halfLimbLength = sizeof(NativeInt) * 8 / 2;
        for (int i = 0; i < halfLimbLength; ++i) {
            mask += (NativeInt) 1 << i;
        }

        NativeInt a = operand1 >> halfLimbLength;
        NativeInt b = operand1 & mask;
        NativeInt c = operand2 >> halfLimbLength;
        NativeInt d = operand2 & mask;

        NativeInt right = b * d;
        NativeInt middle;
        NativeInt left = a * c + (static_cast<NativeInt>(addWithCarry(a * d, b * c, 0, &middle)) << halfLimbLength);
        NativeInt temp_sum = (right >> halfLimbLength) + (middle & mask);

        resultTwo[1] = left + (middle >> halfLimbLength) + (temp_sum >> halfLimbLength);
        resultTwo[0] = (temp_sum << halfLimbLength) | (right & mask);
    }
}// namespace basic_math

#endif