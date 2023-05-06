//
// Created by 赵启明 on 2023/4/27.
//

#ifndef ZJ_FHE_LIB_BigInteger_H
#define ZJ_FHE_LIB_BigInteger_H

#include "../util/exception.h"
#include "integer_interface.h"
#include <cstdint>
#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

namespace zhejiangfhe {


    template<typename NativeInt>
    class BigInteger : public IntegerInterface<BigInteger<NativeInt>> {

    public:
        // CONSTRUCTORS
        BigInteger();

        BigInteger(const std::string &strValue) {
            AssignVal(strValue);
        }

        BigInteger(std::vector<NativeInt> vals, bool sign = false) {
            // std::cout << vals.back() << "\n";
            while (!vals.empty() && vals.back() == 0) {
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
        BigInteger<NativeInt> &AssignObj(const BigInteger<NativeInt> &other) {
            if (this != &other) {// 避免自我赋值
                std::vector<NativeInt> resultVectors;
                for (auto it = other.value.begin(); it != other.value.end(); ++it) {
                    resultVectors.push_back(*it);
                }
                while (!resultVectors.empty() && resultVectors.back() == 0) {
                    resultVectors.pop_back();
                }
                if (other.value.empty()) {
                    value.push_back(0);
                    return *this;
                }
                this->sign = sign;
                for (auto it = resultVectors.begin(); it != resultVectors.end(); ++it) {
                    value.push_back(*it);
                }
                m_MSB = (resultVectors.size() - 1) * m_limbBitLength + m_GetMSBForLimb(resultVectors.back());
                if (m_MSB == 0) {
                    sign = false;
                }
            }
            return *this;
        }


        /**
         * @brief Compare the current BigInteger with another one
         * 
         * @param another is the BigInteger to be compared with. 
         * @return int -1 for strictly less than, 0 for equal to and 1 for strictly greater than conditions.
         */
        int
        Compare(const BigInteger<NativeInt> &another) const;

        int AbsoluteCompare(const BigInteger<NativeInt> &another) const;

        std::size_t length() const {
            return value.size();
        }
        uint8_t addWithCarry(NativeInt operand1, NativeInt operand2, uint8_t carry, NativeInt *result) const {
            operand1 += operand2;
            *result = operand1 + carry;
            return (operand1 < operand2) || (~operand1 < carry);
        }
        uint8_t subWithBorrow(NativeInt operand1, NativeInt operand2, uint8_t borrow, NativeInt *result) const {
            auto diff = operand1 - operand2;
            *result = diff - (borrow != 0);
            return (diff > operand1) || (diff < borrow);
        }

        BigInteger<NativeInt> AddWithoutSign(const BigInteger<NativeInt> &num, bool sign = false) const {
            std::vector<NativeInt> resultVectors;

            uint8_t carry = 0;
            NativeInt currentLimb;
            int i = 0;
            while (i < length() && i < num.length()) {
                carry = addWithCarry(value[i], num.value[i], carry, &currentLimb);
                resultVectors.push_back(currentLimb);
                ++i;
            }
            while (i < length()) {
                carry = addWithCarry(value[i], 0, carry, &currentLimb);
                resultVectors.push_back(currentLimb);
                ++i;
            }
            while (i < num.length()) {
                carry = addWithCarry(0, num.value[i], carry, &currentLimb);
                resultVectors.push_back(currentLimb);
                ++i;
            }

            return BigInteger(resultVectors, sign);
        }
        BigInteger<NativeInt> SubWithoutSign(const BigInteger<NativeInt> &num, bool sign = false) const {

            std::vector<NativeInt> resultVectors;

            uint8_t borrow = 0;
            NativeInt currentLimb;

            for (int i = 0; i < length(); ++i) {
                if (i >= num.length()) {
                    borrow = subWithBorrow(value[i], 0, borrow, &currentLimb);
                    resultVectors.push_back(currentLimb);
                } else {
                    borrow = subWithBorrow(value[i], num.value[i], borrow, &currentLimb);
                    resultVectors.push_back(currentLimb);
                }
            }


            return BigInteger(resultVectors, sign);
        }


        BigInteger<NativeInt> Add(const BigInteger<NativeInt> &num) {
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

        const BigInteger<NativeInt> &AddEq(const BigInteger<NativeInt> &num) {
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
        BigInteger<NativeInt> Sub(const BigInteger<NativeInt> &num) {
            if (sign == false && num.sign == true) {
                return AddWithoutSign(num,sign);
            }
            if (sign == true && num.sign == false) {
                return AddWithoutSign(num,sign);
            }
            return SubWithoutSign(num);
        }
        const BigInteger<NativeInt> &SubEq(const BigInteger<NativeInt> &num) {
            if (sign == false && num.sign == true) {
                AssignObj(AddWithoutSign(num,sign));
            }
            if (sign == true && num.sign == false) {
                AssignObj(AddWithoutSign(num,sign));
            }
            AssignObj(SubWithoutSign(num));
            return *this;
        }

        BigInteger<NativeInt> Mul(const BigInteger<NativeInt> &b) const;
         const BigInteger<NativeInt> &MulEq(const BigInteger<NativeInt> &b);

        BigInteger<NativeInt> DividedBy(const BigInteger<NativeInt> &b) const {
            return BigInteger("0");
        }
        const BigInteger<NativeInt> &DividedByEq(const BigInteger<NativeInt> &b) {
            return *this;
        }

        const std::string ConvertToString() const;

        NativeInt ConvertToLimb();

        NativeInt UintInBinaryToDecimal(uint8_t *a) {
            NativeInt Val = 0;
            NativeInt one = 1;
            for (int i = m_limbBitLength - 1; i >= 0; i--) {
                Val += one * *(a + i);
                one <<= 1;
                *(a + i) = 0;
            }
            return Val;
        }

        /**
         * @brief Karatsuba 算法计算 uint64_t * uint64_t, 结果为一个 128 位的数
         *
         * (a * 2^n + b) * (c * 2^n + d) = ac*2^2n + (ad + bc)*2^n + bd
         * 这里 n = 32，下面的计算时由于进位的原因，做了多次平移。
         */
        inline void MultiplyWithKaratsuba(NativeInt operand1, NativeInt operand2, NativeInt *resultTwo) const {
            NativeInt a = operand1 >> (m_limbBitLength / 2);
            NativeInt b = operand1 & 0x0000FFFF;
            NativeInt c = operand2 >> (m_limbBitLength / 2);
            NativeInt d = operand2 & 0x0000FFFF;

            NativeInt right = b * d;
            NativeInt middle;
            NativeInt left = a * c + (static_cast<NativeInt>(addWithCarry(a * d, b * c, 0, &middle)) << (m_limbBitLength / 2));
            NativeInt temp_sum = (right >> (m_limbBitLength / 2)) + (middle & 0x0000FFFF);

            resultTwo[1] = left + (middle >> (m_limbBitLength / 2)) + (temp_sum >> (m_limbBitLength / 2));
            resultTwo[0] = (temp_sum << (m_limbBitLength / 2)) | (right & 0x0000FFFF);
        }

    protected:
        void AssignVal(const std::string &str) {

            std::string v = str;
            if (str[0] == '-') {
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
                    m_GetMSB();
                }
            }
        }


    private:
        std::vector<NativeInt> value;
        bool sign = false;
        static const uint32_t m_limbBitLength;
        uint32_t m_MSB = 0;

        void m_GetMSB() {
            m_MSB = (value.size() - 1) * m_limbBitLength + m_GetMSBForLimb(value.back());
        }
        uint32_t m_GetMSBForLimb(NativeInt x) {
            uint64_t y = ((uint64_t) x);
            return 63 - (sizeof(unsigned long) == 8 ? __builtin_clzl(y) : __builtin_clzll(y));
        }
    };


    template<typename NativeInt>
    const uint32_t BigInteger<NativeInt>::m_limbBitLength = sizeof(NativeInt) * 8;

}// namespace zhejiangfhe


#endif//ZJ_FHE_LIB_BigInteger_H
