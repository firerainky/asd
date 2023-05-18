//
// Created by Zheng Kanyan on 2023/5/17.
//
#include "big_integer.h"
#include <limits>

namespace zhejiangfhe {
    template<typename NativeInt>
    BigInteger<NativeInt> BigInteger<NativeInt>::Mod(const BigInteger &modulus) const {
        if (m_MSB < modulus.m_MSB) {
            return BigInteger(value, sign);
        }
        if (m_MSB == modulus.m_MSB && *this < modulus) {
            return BigInteger(value, sign);
        }

        // use simple masking operation if modulus is 2
        if (modulus.m_MSB == 2 && modulus.value[0] == 2) {
            if (value[0] % 2 == 0) {
                return BigInteger();
            } else {
                return BigInteger("1");
            }
        }
        BigInteger quotient;
        BigInteger remainder;
        auto f = Divide(quotient, remainder, *this, modulus);
        return remainder;
    }

    template<typename NativeInt>
    const BigInteger<NativeInt> BigInteger<NativeInt>::ModEq(const BigInteger &modulus) {
        return *this = this->Mod(modulus);
    }

    template<typename NativeInt>
    BigInteger<NativeInt> BigInteger<NativeInt>::ModMul(const BigInteger<NativeInt> &another, const BigInteger &modulus) const {
        auto mulRet = Mul(another);
        return mulRet.ModEq(modulus);
    }

    template<typename NativeInt>
    const BigInteger<NativeInt> BigInteger<NativeInt>::ModMulEq(const BigInteger<NativeInt> &another, const BigInteger &modulus) {
        return *this = this->ModMul(another, modulus);
    }

    template class zhejiangfhe::BigInteger<uint32_t>;
    template class zhejiangfhe::BigInteger<uint64_t>;
}// namespace zhejiangfhe