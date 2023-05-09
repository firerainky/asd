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

        BigInteger(const std::string &strValue);

        BigInteger(std::vector<NativeInt> vals, bool sign = false);

        const std::string ConvertToString() const;

        NativeInt ConvertToLimb() const;

        /**
         * @brief Compare the current BigInteger with another one
         * 
         * @param another is the BigInteger to be compared with. 
         * @return int -1 for strictly less than, 0 for equal to and 1 for strictly greater than conditions.
         */
        int Compare(const BigInteger<NativeInt> &another) const;

        BigInteger<NativeInt> Add(const BigInteger<NativeInt> &num);
        const BigInteger<NativeInt> &AddEq(const BigInteger<NativeInt> &num);

        BigInteger<NativeInt> Sub(const BigInteger<NativeInt> &num);
        const BigInteger<NativeInt> &SubEq(const BigInteger<NativeInt> &num);

        BigInteger<NativeInt> Mul(const BigInteger<NativeInt> &b) const;
        const BigInteger<NativeInt> &MulEq(const BigInteger<NativeInt> &b);

        BigInteger<NativeInt> DividedBy(const BigInteger<NativeInt> &b) const {
            return BigInteger("0");
        }
        const BigInteger<NativeInt> &DividedByEq(const BigInteger<NativeInt> &b) {
            return *this;
        }

    private:
        std::vector<NativeInt> value;
        bool sign = false;
        static const uint32_t m_limbBitLength;
        uint32_t m_MSB = 0;

        void AssignVal(const std::string &str);
        BigInteger<NativeInt> &AssignObj(const BigInteger<NativeInt> &other);

        void m_GetMSB() {
            m_MSB = (value.size() - 1) * m_limbBitLength + m_GetMSBForLimb(value.back());
        }
        uint32_t m_GetMSBForLimb(NativeInt x) {
            uint64_t y = ((uint64_t) x);
            if (y == 0) {
                return 0;
            } else {
                return 64 - (sizeof(unsigned long) == 8 ? __builtin_clzl(y) : __builtin_clzll(y));
            }
        }

        int AbsoluteCompare(const BigInteger<NativeInt> &another) const;

        NativeInt UintInBinaryToDecimal(uint8_t *a);

        static uint8_t addWithCarry(NativeInt operand1, NativeInt operand2, uint8_t carry, NativeInt *result);
        static uint8_t subWithBorrow(NativeInt operand1, NativeInt operand2, uint8_t borrow, NativeInt *result);

        BigInteger<NativeInt> AddWithoutSign(const BigInteger<NativeInt> &num, bool sign = false) const;
        BigInteger<NativeInt> SubWithoutSign(const BigInteger<NativeInt> &num, bool sign = false) const;

        /**
         * @brief Karatsuba 算法计算 NativeInt * NativeInt, 结果为一个两倍于 NativeInt 长度的数
         *
         * (a * 2^n + b) * (c * 2^n + d) = ac*2^2n + (ad + bc)*2^n + bd
         * 这里 n = NativeInt 长度 / 2，下面的计算时由于进位的原因，做了多次平移。
         */
        static void MultiplyWithKaratsuba(NativeInt operand1, NativeInt operand2, NativeInt *resultTwo);
    };


    template<typename NativeInt>
    const uint32_t BigInteger<NativeInt>::m_limbBitLength = sizeof(NativeInt) * 8;

}// namespace zhejiangfhe


#endif//ZJ_FHE_LIB_BigInteger_H
