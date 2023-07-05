//
// Created by Zheng Kanyan on 2023/5/08.
//

#include "big_integer.h"
#include <cmath>
#include <limits>

namespace zhejiangfhe {

    template<typename NativeInt>
    BigInteger<NativeInt>::BigInteger(std::vector<NativeInt> vals, bool sign) {
        while (!vals.empty() && !vals.back()) {
            vals.pop_back();
        }
        if (vals.empty()) {
            value.push_back(0);
            return;
        }
        this->sign = sign;
        value = vals;
        m_MSB = (vals.size() - 1) * m_limbBitLength + m_GetMSBForLimb(vals.back());
        if (m_MSB == 0) {
            sign = false;
        }
    }

    template<typename NativeInt>
    BigInteger<NativeInt>::BigInteger(const std::string &strValue) {
        AssignVal(strValue);
    }

    template<typename NativeInt>
    BigInteger<NativeInt>::BigInteger(uint64_t val, bool signVal) {
        sign = signVal;
        if (std::is_same<uint64_t, NativeInt>::value) {
            value.push_back(val);
        } else if (std::is_same<uint32_t, NativeInt>::value) {
            if (val >= (1LL << 32)) {
                value.push_back(val & m_MaxLimb);
                value.push_back(val >> 32);
            } else {
                value.push_back(val);
            }
        }
        RefreshMSB();
    }

    template<typename NativeInt>
    void BigInteger<NativeInt>::AssignVal(const std::string &str) {

        std::string v = str;
        if (!str.empty() && str[0] == '-') {
            sign = true;
            v = str.substr(1);
        }
        v.erase(0, v.find_first_not_of('0'));
        v.erase(0, v.find_first_not_of(' '));
        if (v.empty()) {
            v = "0";// set to one zero
        }
        size_t arr_size = v.length();
        std::unique_ptr<u_int8_t[]> dec_arr = std::make_unique<u_int8_t[]>(arr_size);
        for (size_t i = 0; i < arr_size; i++)// store the string to decimal array
            dec_arr[i] = (uint8_t) stoi(v.substr(i, 1));

        // clear the current value of value;
        value.clear();

        size_t zero_ptr = 0;
        // index of highest non-zero number in decimal number
        // define  bit register array
        std::unique_ptr<u_int8_t[]> bit_arr = std::make_unique<u_int8_t[]>(m_limbBitLength);
        int cnt = m_limbBitLength - 1;
        // cnt is a pointer to the bit position in bit_arr, when bit_arr is complete it
        // is ready to be transfered to Value
        while (zero_ptr != arr_size) {
            bit_arr[cnt] = dec_arr[arr_size - 1] % 2;
            // start divide by 2 in the DecValue array
            for (size_t i = zero_ptr; i < arr_size - 1; i++) {
                dec_arr[i + 1] = (dec_arr[i] % 2) * 10 + dec_arr[i + 1];
                dec_arr[i] >>= 1;
            }
            dec_arr[arr_size - 1] >>= 1;
            // division ends here
            cnt--;
            if (cnt == -1) {// cnt = -1 indicates bit_arr is ready for transfer
                cnt = m_limbBitLength - 1;
                value.push_back(UintInBinaryToDecimal(bit_arr.get()));
            }
            if (dec_arr[zero_ptr] == 0) {
                zero_ptr++;// division makes Most significant digit zero, hence we increment
                           // zero_ptr to next value
            }
            if (zero_ptr == arr_size && dec_arr[arr_size - 1] == 0) {
                value.push_back(UintInBinaryToDecimal(bit_arr.get()));
                RefreshMSB();
            }
        }
    }

    template<typename NativeInt>
    uint8_t BigInteger<NativeInt>::GetBitAtIndex(uint32_t index) const {
        if (index > m_MSB) return 0;
        uint32_t i = MSB2NLimbs(index) - 1;
        // uint32_t i = index / m_limbBitLength;
        NativeInt x = value[i];
        uint32_t remain = index % m_limbBitLength == 0 ? m_limbBitLength : index % m_limbBitLength;
        NativeInt bMask = 1;
        bMask <<= (remain - 1);
        return x & bMask ? 1 : 0;
    }

    template<typename NativeInt>
    uint32_t BigInteger<NativeInt>::MSB2NLimbs(const uint32_t number) {
        static uint32_t mask = m_limbBitLength - 1;

        if (!number) return 1;
        if ((number & mask) != 0) {
            return (number >> m_log2LimbBitLength) + 1;
        } else {
            return number >> m_log2LimbBitLength;
        }
    }

    template<typename NativeInt>
    uint32_t BigInteger<NativeInt>::GetDigitAtIndexForBase(uint32_t index, uint32_t base) const {
        uint32_t digitLength = ceil(log2(base));
        uint32_t digit = 0;
        uint32_t newIndex = 1 + (index - 1) * digitLength;
        for (uint32_t i = 1; i < base; i = i * 2) {
            digit += GetBitAtIndex(newIndex) * i;
            newIndex++;
        }

        return digit;
    }

    template<typename NativeInt>
    int BigInteger<NativeInt>::Compare(const BigInteger<NativeInt> &another) const {
        if (!sign && another.sign) {
            return 1;
        }
        if (sign && !another.sign) {
            return -1;
        }
        int absoluteCompare = AbsoluteCompare(another);
        return sign ? -absoluteCompare : absoluteCompare;
    }

    template<typename NativeInt>
    int BigInteger<NativeInt>::AbsoluteCompare(const BigInteger<NativeInt> &another) const {
        if (m_MSB < another.m_MSB) {
            return -1;
        } else if (m_MSB > another.m_MSB) {
            return 1;
        } else {
            for (int i = value.size() - 1; i >= 0; i--) {
                if (value[i] > another.value[i]) {
                    return 1;
                } else if (value[i] < another.value[i]) {
                    return -1;
                }
            }
            return 0;
        }
    }

    template<typename NativeInt>
    NativeInt BigInteger<NativeInt>::ConvertToLimb() const {
        if (sign) {
            return -value[0];
        } else {
            return value[0];
        }
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

        return std::string(negative ? "-" : "").append(printValue);
    }

    template<typename NativeInt>
    NativeInt BigInteger<NativeInt>::UintInBinaryToDecimal(uint8_t *a) {
        NativeInt Val = 0;
        NativeInt one = 1;
        for (int i = m_limbBitLength - 1; i >= 0; i--) {
            Val += one * *(a + i);
            one <<= 1;
            *(a + i) = 0;
        }
        return Val;
    }

    template<typename NativeInt>
    uint32_t BigInteger<NativeInt>::ceilIntByUInt(const NativeInt Number) {
        // mask to perform bitwise AND
        static NativeInt mask = m_limbBitLength - 1;

        if (!Number) {
            return 1;
        }

        if ((Number & mask) != 0) {
            return (Number >> m_log2LimbBitLength) + 1;
        } else {
            return Number >> m_log2LimbBitLength;
        }
    }
    template<typename NativeInt>
    void BigInteger<NativeInt>::NormalizeLimbs(void) {

        for (uint32_t i = this->value.size() - 1; i >= 1; i--) {
            if (!this->value.back()) {
                this->value.pop_back();
            } else {
                break;
            }
        }

        RefreshMSB();
    }

    template<typename NativeInt>
    void BigInteger<NativeInt>::RefreshMSB() {
        m_MSB = (value.size() - 1) * m_limbBitLength + m_GetMSBForLimb(value.back());
    }

    template<typename NativeInt>
    const uint32_t BigInteger<NativeInt>::m_limbBitLength = sizeof(NativeInt) * 8;

    template<typename NativeInt>
    const NativeInt BigInteger<NativeInt>::m_MaxLimb = std::numeric_limits<NativeInt>::max();

    template<typename NativeInt>
    const uint32_t BigInteger<NativeInt>::m_log2LimbBitLength = Log2<m_limbBitLength>::value;

    template class zhejiangfhe::BigInteger<limbtype>;
}// namespace zhejiangfhe