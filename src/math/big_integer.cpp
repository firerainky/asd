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
    const std::string BigInteger<NativeInt>::ConvertToString() {
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

    template class zhejiangfhe::BigInteger<u_int32_t>;
}// namespace zhejiangfhe