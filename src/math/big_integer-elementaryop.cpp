//
// Created by 赵启明 on 2023/4/27.
//

#include "big_integer.h"

namespace zhejiangfhe {

    template<typename NativeInt>
    uint8_t BigInteger<NativeInt>::addWithCarry(NativeInt operand1, NativeInt operand2, uint8_t carry, NativeInt *result) {
        operand1 += operand2;
        *result = operand1 + carry;
        return (operand1 < operand2) || (~operand1 < carry);
    }

    template<typename NativeInt>
    uint8_t BigInteger<NativeInt>::subWithBorrow(NativeInt operand1, NativeInt operand2, uint8_t borrow, NativeInt *result) {
        auto diff = operand1 - operand2;
        *result = diff - (borrow != 0);
        return (diff > operand1) || (diff < borrow);
    }

    template<typename NativeInt>
    BigInteger<NativeInt> BigInteger<NativeInt>::AddWithoutSign(const BigInteger<NativeInt> &num, bool sign) const {
        std::vector<NativeInt> resultVectors;

        uint8_t carry = 0;
        NativeInt currentLimb;
        int i = 0;
        while (i < value.size() && i < num.value.size()) {
            carry = addWithCarry(value[i], num.value[i], carry, &currentLimb);
            resultVectors.push_back(currentLimb);
            ++i;
        }
        while (i < value.size()) {
            carry = addWithCarry(value[i], 0, carry, &currentLimb);
            resultVectors.push_back(currentLimb);
            ++i;
        }
        while (i < num.value.size()) {
            carry = addWithCarry(0, num.value[i], carry, &currentLimb);
            resultVectors.push_back(currentLimb);
            ++i;
        }

        return BigInteger(resultVectors, sign);
    }

    template<typename NativeInt>
    BigInteger<NativeInt> BigInteger<NativeInt>::SubWithoutSign(const BigInteger<NativeInt> &num, bool sign) const {

        std::vector<NativeInt> resultVectors;

        uint8_t borrow = 0;
        NativeInt currentLimb;

        for (int i = 0; i < value.size(); ++i) {
            if (i >= num.value.size()) {
                borrow = subWithBorrow(value[i], 0, borrow, &currentLimb);
                resultVectors.push_back(currentLimb);
            } else {
                borrow = subWithBorrow(value[i], num.value[i], borrow, &currentLimb);
                resultVectors.push_back(currentLimb);
            }
        }


        return BigInteger(resultVectors, sign);
    }

    template<typename NativeInt>
    BigInteger<NativeInt> BigInteger<NativeInt>::Add(const BigInteger<NativeInt> &num) {
        int absoluteCompare = AbsoluteCompare(num);
        if (sign == false && num.sign == true) {
            if (AbsoluteCompare(num) == 0) {
                return BigInteger<NativeInt>();
            } else if (absoluteCompare > 0) {
                return SubWithoutSign(num);
            } else {
                return num.SubWithoutSign(*this, true);
            }
        }
        if (sign == true && num.sign == false) {
            if (AbsoluteCompare(num) == 0) {
                return BigInteger<NativeInt>();
            } else if (absoluteCompare > 0) {
                return SubWithoutSign(num, sign);
            } else {
                return num.SubWithoutSign(*this);
            }
        }
        return AddWithoutSign(num);
    }

    template<typename NativeInt>
    const BigInteger<NativeInt> &BigInteger<NativeInt>::AddEq(const BigInteger<NativeInt> &num) {
        int absoluteCompare = AbsoluteCompare(num);
        if (sign == false && num.sign == true) {
            if (AbsoluteCompare(num) == 0) {
                value.clear();
                value.push_back(0);
            } else if (absoluteCompare > 0) {
                AssignObj(SubWithoutSign(num));
            } else {
                AssignObj(num.SubWithoutSign(*this, true));
            }
        } else if (sign == true && num.sign == false) {
            if (AbsoluteCompare(num) == 0) {
                value.clear();
                value.push_back(0);
            } else if (absoluteCompare > 0) {
                AssignObj(SubWithoutSign(num, sign));
            } else {
                AssignObj(num.SubWithoutSign(*this));
            }
        } else {
            AssignObj(SubWithoutSign(num));
        }
        return *this;
    }

    template<typename NativeInt>
    BigInteger<NativeInt> BigInteger<NativeInt>::Sub(const BigInteger<NativeInt> &num) {
        if (sign == false && num.sign == true) {
            return AddWithoutSign(num, sign);
        }
        if (sign == true && num.sign == false) {
            return AddWithoutSign(num, sign);
        }
        return SubWithoutSign(num);
    }

    template<typename NativeInt>
    const BigInteger<NativeInt> &BigInteger<NativeInt>::SubEq(const BigInteger<NativeInt> &num) {
        if (sign == false && num.sign == true) {
            AssignObj(AddWithoutSign(num, sign));
        }
        if (sign == true && num.sign == false) {
            AssignObj(AddWithoutSign(num, sign));
        }
        AssignObj(SubWithoutSign(num));
        return *this;
    }

    template<typename NativeInt>
    void BigInteger<NativeInt>::MultiplyWithKaratsuba(NativeInt operand1, NativeInt operand2, NativeInt *resultTwo) {
        NativeInt mask = 0x0;
        uint32_t halfLimbLength = m_limbBitLength / 2;
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

    template<typename NativeInt>
    BigInteger<NativeInt> BigInteger<NativeInt>::Mul(const BigInteger<NativeInt> &b) const {

        std::vector<NativeInt> values;
        for (int i = 0; i < value.size(); ++i) {
            for (int j = 0; j < b.value.size(); ++j) {
                NativeInt temp_result[2];
                MultiplyWithKaratsuba(value[i], b.value[j], temp_result);
                uint8_t carry = 0;
                NativeInt sum;
                if (i + j + 1 > values.size()) {
                    values.push_back(temp_result[0]);
                } else {
                    carry = addWithCarry(temp_result[0], values[i + j], carry, &sum);
                    values[i + j] = sum;
                    temp_result[1] += carry;
                    carry = 0;
                }

                if (i + j + 2 > values.size()) {
                    values.push_back(temp_result[1]);
                } else {
                    carry = addWithCarry(temp_result[1], values[i + j + 1], carry, &sum);
                    values[i + j + 1] = sum;
                    uint8_t currentIdx = i + j + 2;
                    while (carry) {
                        if (currentIdx > values.size()) {
                            values.push_back(carry);
                        } else {
                            carry = addWithCarry(0, values[currentIdx], carry, &sum);
                            values[currentIdx] = sum;
                        }
                    }
                }
            }
        }

        return BigInteger(values, sign ^ b.sign);
    }

    template<typename NativeInt>
    const BigInteger<NativeInt> &BigInteger<NativeInt>::MulEq(const BigInteger<NativeInt> &b) {
        return *this = this->Mul(b);
    }

    template class zhejiangfhe::BigInteger<uint32_t>;
    template class zhejiangfhe::BigInteger<uint64_t>;
}// namespace zhejiangfhe