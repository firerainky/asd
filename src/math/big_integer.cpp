//
// Created by 赵启明 on 2023/4/27.
//

#include "big_integer.h"

namespace zhejiangfhe {
    template<typename NativeInt>
    BigInteger<NativeInt>::BigInteger() {
        this->value.push_back(0);
    }

    template<typename NativeInt>
    int BigInteger<NativeInt>::Compare(const BigInteger<NativeInt> &another) const {
        if (!sign && another.sign) {
            return 1;
        }
        if (sign && !another.sign) {
            return -1;
        }

        return AbsoluteCompare(another);
    }
    template<typename NativeInt>
    int BigInteger<NativeInt>::AbsoluteCompare(const BigInteger<NativeInt> &another) const {
        int absoluteCompare = 0;
        if (m_MSB < another.m_MSB) {
            absoluteCompare = -1;
        } else if (m_MSB > another.m_MSB) {
            absoluteCompare = 1;
        } else {
            for (int i = value.size() - 1; i >= 0; i--) {
                if (value[i] > another.value[i]) {
                    absoluteCompare = 1;
                    break;
                } else if (value[i] < another.value[i]) {
                    absoluteCompare = -1;
                    break;
                }
            }
        }

        return sign ? -absoluteCompare : absoluteCompare;
    }

    template<typename NativeInt>
    const std::string BigInteger<NativeInt>::ConvertToString() const {
        bool negative = sign;
        if (value.size() == 1) {
            if (value[0] == 0) {
                negative = false;
            }
            return std::string(negative ? "-" : "").append(std::to_string(value[0]));
        }

        std::vector<uint8_t> decimalArr;
        decimalArr.push_back(0);

        for (int i = value.size() - 1; i >= 0; --i) {
            int maxBitIdx = m_limbBitLength - 1;
            // if (i == m_value.size() - 1) {
            //   maxBitIdx = __builtin_ctzll(m_value[i]) + 1;
            // }
            for (int j = maxBitIdx; j >= 0; --j) {
                uint8_t carry = 0;
                for (int m = 0; m < decimalArr.size(); ++m) {
                    decimalArr[m] *= 2;
                    decimalArr[m] += carry;
                    carry = 0;
                    if (decimalArr[m] > 9) {
                        decimalArr[m] -= 10;
                        carry = 1;
                    }
                }

                if (carry == 1) {
                    decimalArr.push_back(1);
                    carry = 0;
                }

                uint64_t bMask = 1;
                for (int k = 0; k < j; ++k) {
                    bMask <<= 1;
                }
                if ((value[i] & bMask) != 0) {
                    decimalArr[0] += 1;
                }
            }
        }

        std::string printValue;
        for (int i = decimalArr.size() - 1; i >= 0; --i) {
            printValue.push_back(decimalArr[i] + '0');
        }

        // std::cout << printValue << "\n";
        return std::string(negative ? "-" : "").append(printValue);
    }

    template<typename NativeInt>
    NativeInt BigInteger<NativeInt>::ConvertToLimb() {
        if (sign) {
            return -value[0];
        } else {
            return value[0];
        }
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

    template class zhejiangfhe::BigInteger<u_int32_t>;
    template class zhejiangfhe::BigInteger<u_int64_t>;
}// namespace zhejiangfhe