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

        BigInteger(const std::string &strValue = "");

        BigInteger(std::vector<NativeInt> vals, bool sign = false);

        const std::string ConvertToString() const;

        NativeInt ConvertToLimb() const;

        BigInteger(NativeInt val, bool sign = false) {
            value.push_back(val);
            m_MSB = m_GetMSBForLimb(val);
        }


        template<typename T, typename std::enable_if<!std::is_same<T, const BigInteger>::value, bool>::type = true>
        const BigInteger &operator=(const T &val) {
            return (*this = BigInteger(val));
        }

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

        std::size_t length() const {
            return value.size();
        }

        BigInteger<NativeInt> Mul(const BigInteger<NativeInt> &b) const;

        const BigInteger<NativeInt> &MulEq(const BigInteger<NativeInt> &b);

        BigInteger<NativeInt> DividedBy(const BigInteger<NativeInt> &b) const;

        const BigInteger<NativeInt> &DividedByEq(const BigInteger<NativeInt> &b) {
            return *this;
        }

        // Bit Operation
        BigInteger<NativeInt> And(const BigInteger<NativeInt>) const;
        const BigInteger<NativeInt> &AndEq(const BigInteger<NativeInt>);
        BigInteger<NativeInt> operator&(const BigInteger<NativeInt> &another) const {
            return And(another);
        }

        BigInteger<NativeInt> Or(const BigInteger<NativeInt>) const;
        const BigInteger<NativeInt> &OrEq(const BigInteger<NativeInt>);
        BigInteger<NativeInt> operator|(const BigInteger<NativeInt> &another) const {
            return Or(another);
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

        BigInteger<NativeInt> Negate() const;
        const BigInteger<NativeInt> &NegateEq();

        BigInteger<NativeInt> LeftShift(uint16_t shift) const;
        BigInteger<NativeInt> RightShift(uint16_t shift) const;
        /**
         *
         * @param quotientIn 商
         * @param remainderIn 余数
         * @param uIn
         * @param v
         * @return
         */
        bool Divide(BigInteger &quotientIn, BigInteger &remainderIn, const BigInteger &uIn, const BigInteger &vIn) const;

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

        // Mod operations
        BigInteger<NativeInt> Mod(const BigInteger &modulus) const;
        const BigInteger<NativeInt> ModEq(const BigInteger &modulus);
        BigInteger<NativeInt> ModMul(const BigInteger &another, const BigInteger &modulus) const;

    protected:
    private:
        std::vector<NativeInt> value;
        bool sign = false;
        static const uint32_t m_limbBitLength;
        uint32_t m_MSB = 0;
        static const NativeInt m_MaxLimb;
        static const uint32_t m_log2LimbBitLength;
        typedef typename DoubleDataType<NativeInt>::T Dlimb_t;
        typedef typename SignedDoubleDataType<NativeInt>::T SDlimb_t;

        void AssignVal(const std::string &str);
        BigInteger<NativeInt> &AssignObj(const BigInteger<NativeInt> &other);
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


}// namespace zhejiangfhe


#endif//ZJ_FHE_LIB_BigInteger_H
