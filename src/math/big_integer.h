//
// Created by 赵启明 on 2023/4/27.
//

#ifndef ZJ_FHE_LIB_BigInteger_H
#define ZJ_FHE_LIB_BigInteger_H

#include "../util/exception.h"
#include "integer_interface.h"
#include <cstdint>
#include <iosfwd>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace zhejiangfhe {

    template<typename NativeInt>
    class BigInteger;

    template<typename utype>
    struct SignedDoubleDataType {
        typedef void T;
    };


    template<>
    struct SignedDoubleDataType<uint16_t> {
        typedef int32_t T;
    };

    template<>
    struct SignedDoubleDataType<uint32_t> {
        typedef int64_t T;
    };

    template<>
    struct SignedDoubleDataType<uint64_t> {
        typedef __int128 T;
    };

    template<typename utype>
    struct DoubleDataType {
        typedef void T;
    };

    template<>
    struct DoubleDataType<uint16_t> {
        typedef uint32_t T;
    };

    template<>
    struct DoubleDataType<uint32_t> {
        typedef uint64_t T;
    };


    template<>
    struct DoubleDataType<uint64_t> {
        typedef unsigned __int128 T;
    };

    template<uint32_t N>
    struct Log2 {
        static const uint32_t value = 1 + Log2<N / 2>::value;
    };


    template<>
    struct Log2<2> {
        static const uint32_t value = 1;
    };

    template<typename NativeInt>
    class BigInteger : public IntegerInterface<BigInteger<NativeInt>> {

    public:
        // CONSTRUCTORS
        explicit BigInteger(const std::string &strValue = "");
        explicit BigInteger(const char *str) : BigInteger(std::string(str)){};

        BigInteger(uint64_t val, bool sign = false);

        // template<typename T>
        // BigInteger(T) = delete;

        BigInteger(std::vector<NativeInt> vals, bool sign = false);

        const std::string ConvertToString() const;

        NativeInt ConvertToLimb() const;

        template<typename T, typename std::enable_if<!std::is_same<T, const BigInteger>::value, bool>::type = true>
        const BigInteger &operator=(const T &val) {
            return (*this = BigInteger(val));
        }

        /**
         * Constructors from smaller basic types
         *
         * @param val is the initial integer represented as a basic integer type.
         */
        template<typename T,
                 typename std::enable_if<std::is_integral<T>::value && !std::is_same<T, char>::value && !std::is_same<T, uint64_t>::value,
                                         bool>::type = true>
        BigInteger(T val) {
            if (val < 0) {
                *this = BigInteger(static_cast<uint64_t>(-val), true);
            } else {
                *this = BigInteger(static_cast<uint64_t>(val));
            }
        }// NOLINT

        /**
         * @brief Compare the current BigInteger with another one
         * 
         * @param another is the BigInteger to be compared with. 
         * @return int -1 for strictly less than, 0 for equal to and 1 for strictly greater than conditions.
         */
        int Compare(const BigInteger<NativeInt> &another) const;

        int AbsoluteCompare(const BigInteger<NativeInt> &another) const;
        BigInteger<NativeInt> Abs() const {
            if (value.empty()) {
                return BigInteger<NativeInt>(0);
            } else {
                return BigInteger<NativeInt>(value, false);
            }
        }

        BigInteger<NativeInt> Add(const BigInteger<NativeInt> &num) const;
        const BigInteger<NativeInt> &AddEq(const BigInteger<NativeInt> &num);

        BigInteger<NativeInt> Sub(const BigInteger<NativeInt> &num) const;
        const BigInteger<NativeInt> &SubEq(const BigInteger<NativeInt> &num);
        BigInteger<NativeInt> operator-() {
            return BigInteger(this->value, !this->sign);
        }

        std::size_t length() const {
            return value.size();
        }
        std::vector<NativeInt> getValue() const { return value; }

        NativeInt getValueOfIndex(uint32_t index) const { return value[index]; }

        bool getSign() const { return sign; }

        BigInteger<NativeInt> Mul(const BigInteger<NativeInt> &b) const;

        const BigInteger<NativeInt> &MulEq(const BigInteger<NativeInt> &b);

        std::pair<BigInteger<NativeInt>, BigInteger<NativeInt>> DividedBy(const BigInteger<NativeInt> &b) const;

        const BigInteger<NativeInt> &DividedByEq(const BigInteger<NativeInt> &b) {
            return *this;
        }

        // Mod
        BigInteger<NativeInt> operator%(const BigInteger<NativeInt> &modulus) const;
        const BigInteger<NativeInt> &operator%=(const BigInteger<NativeInt> &modulus);

        BigInteger<NativeInt> ModMul(const BigInteger<NativeInt> &b, const BigInteger<NativeInt> &modulus) const;

        // Bit Operation
        BigInteger<NativeInt> And(const BigInteger<NativeInt>) const;
        const BigInteger<NativeInt> &AndEq(const BigInteger<NativeInt>);
        BigInteger<NativeInt> operator&(const BigInteger<NativeInt> &another) const {
            return And(another);
        }
        BigInteger<NativeInt> operator&=(const BigInteger<NativeInt> &another) {
            return *this = And(another);
        }

        BigInteger<NativeInt> Or(const BigInteger<NativeInt>) const;
        const BigInteger<NativeInt> &OrEq(const BigInteger<NativeInt>);
        BigInteger<NativeInt> operator|(const BigInteger<NativeInt> &another) const {
            return Or(another);
        }
        const BigInteger<NativeInt> operator|=(const BigInteger<NativeInt> &another) {
            return *this = Or(another);
        }

        BigInteger<NativeInt> Not() const;
        const BigInteger<NativeInt> &NotEq();
        BigInteger<NativeInt> operator~() const {
            return Not();
        }

        BigInteger<NativeInt> Xor(const BigInteger<NativeInt>) const;
        const BigInteger<NativeInt> &XorEq(const BigInteger<NativeInt>);
        BigInteger<NativeInt> operator^(const BigInteger<NativeInt> &another) const {
            return Xor(another);
        }
        const BigInteger<NativeInt> operator^=(const BigInteger<NativeInt> &another) {
            return *this = Xor(another);
        }

        BigInteger<NativeInt> Negate() const;
        const BigInteger<NativeInt> &NegateEq();

        BigInteger<NativeInt> LeftShift(uint16_t shift) const;
        BigInteger<NativeInt> operator<<(uint16_t shift) const {
            return LeftShift(shift);
        }
        const BigInteger<NativeInt> &operator<<=(uint16_t shift) {
            return *this = this->LeftShift(shift);
        }
        BigInteger<NativeInt> RightShift(uint16_t shift) const;
        BigInteger<NativeInt> operator>>(uint16_t shift) const {
            return RightShift(shift);
        }
        const BigInteger<NativeInt> &operator>>=(uint16_t shift) {
            return *this = this->RightShift(shift);
        }

        inline int nlz(NativeInt x) const {
            if (typeid(x) == typeid(uint64_t)) {
                return nlz64(x);
            } else if (typeid(x) == typeid(uint32_t)) {
                return nlz32(x);
            } else {
                ZJFHE_THROW(zhejiangfhe::TypeException, "not support native int");
            }
        }

        int nlz64(NativeInt x) const;

        int nlz32(NativeInt x) const;

        inline BigInteger Exp(uint32_t p) {
            if (p == 0) {
                return BigInteger(1);
            }
            if (p == 1) {
                return *this;
            }
            BigInteger tmp = Exp(p / 2);
            if (p % 2 == 0) {
                return tmp * tmp;
            } else {
                return tmp * tmp * (*this);
            }
        }

        uint32_t GetMSB() const {
            return m_MSB;
        }

        std::vector<NativeInt> GetValue() const {
            return value;
        }

        bool GetSign() {
            return sign;
        }

        /**
         * Gets the bit at the specified index.
         *
         * @param index is the index of the bit to get.
         * @return resulting bit.
         */
        uint8_t GetBitAtIndex(uint32_t index) const;

        /**
         * Get the number of digits using a specific base.
         * Warning: only power-of-2 bases are currently supported.
         * Example: for number 83, index 3 and base 4 we have:
         *
         *                         index:1,2,3,4
         * 83 --base 4 decomposition--> (3,0,1,1) --at index 3--> 1
         *
         * The return number is 1.
         *
         * @param index is the location to return value from in the specific base.
         * @param base is the base with which to determine length in.
         * @return the length of the representation in a specific base.
         */
        uint32_t GetDigitAtIndexForBase(uint32_t index, uint32_t base) const;

    protected:
        std::vector<NativeInt> value;
        bool sign = false;
        static const uint32_t m_limbBitLength;
        uint32_t m_MSB = 0;
        static const NativeInt m_MaxLimb;
        static const uint32_t m_log2LimbBitLength;
        typedef typename DoubleDataType<NativeInt>::T Dlimb_t;
        typedef typename SignedDoubleDataType<NativeInt>::T SDlimb_t;

    private:
        void AssignVal(const std::string &str);
        // BigInteger<NativeInt> &AssignObj(const BigInteger<NativeInt> &other);
        static uint32_t ceilIntByUInt(const NativeInt Number);
        void RefreshMSB();
        uint32_t m_GetMSBForLimb(NativeInt x) {
            uint64_t y = ((uint64_t) x);
            if (y == 0) {
                return 0;
            } else {
                return 64 - (sizeof(unsigned long) == 8 ? __builtin_clzl(y) : __builtin_clzll(y));
            }
        }
        void NormalizeLimbs(void);

        NativeInt UintInBinaryToDecimal(uint8_t *a);

        static uint8_t addWithCarry(NativeInt operand1, NativeInt operand2, uint8_t carry, NativeInt *result);
        static uint8_t subWithBorrow(NativeInt operand1, NativeInt operand2, uint8_t borrow, NativeInt *result);

        BigInteger<NativeInt> AddWithSameSign(const BigInteger<NativeInt> &num, bool sign = false) const;
        BigInteger<NativeInt> SubWithSameSign(const BigInteger<NativeInt> &num, bool sign = false) const;

        /**
         *
         * @param quotientIn 商
         * @param remainderIn 余数
         * @param uIn
         * @param v
         * @return
         */
        bool Divide(BigInteger &quotientIn, BigInteger &remainderIn, const BigInteger &uIn, const BigInteger &vIn) const;

        /**
         * @brief Karatsuba 算法计算 NativeInt * NativeInt, 结果为一个两倍于 NativeInt 长度的数
         *
         * (a * 2^n + b) * (c * 2^n + d) = ac*2^2n + (ad + bc)*2^n + bd
         * 这里 n = NativeInt 长度 / 2，下面的计算时由于进位的原因，做了多次平移。
         */
        static void MultiplyWithKaratsuba(NativeInt operand1, NativeInt operand2, NativeInt *resultTwo);

        /**
         * function to return the ceiling of the input number divided by
         * the number of bits in the limb data type.  DBC this is to
         * determine how many limbs are needed for an input bitsize.
         * @param number is the number to be divided.
         * @return the ceiling of Number/(bits in the limb data type)
         */
        static uint32_t MSB2NLimbs(const uint32_t number);
    };

    template<typename NativeInt>
    std::ostream &operator<<(std::ostream &os, const BigInteger<NativeInt> &a) {
        return os << a.ConvertToString() << ", MSB: " << a.GetMSB() << ", limb_size: " << sizeof(NativeInt) * 8;
    }

}// namespace zhejiangfhe

#endif//ZJ_FHE_LIB_BigInteger_H
