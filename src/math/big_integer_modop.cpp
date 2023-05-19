//
// Created by Zheng Kanyan on 2023/5/17.
//
#include "big_integer.h"
#include "big_integer_modop.h"
#include "modulus.h"
#include <limits>

namespace zhejiangfhe {
    template<typename NativeInt>
    BigInteger<NativeInt> BigIntegerMod<NativeInt>::Mod(const Modulus<NativeInt> &modulus) const {
        BigInteger<NativeInt> modulusValue = modulus.GetValue();
        if (this->m_MSB < modulusValue.GetMSB()) {
            return BigInteger<NativeInt>(this->value, this->sign);
        }
        if (this->m_MSB == modulusValue.GetMSB() && *this < modulusValue) {
            return BigInteger<NativeInt>(this->value, this->sign);
        }

        // use simple masking operation if modulus is 2
        if (modulusValue.GetMSB() == 2 && modulusValue.GetValue()[0] == 2) {
            if (this->value[0] % 2 == 0) {
                return BigInteger<NativeInt>();
            } else {
                return BigInteger<NativeInt>("1");
            }
        }
        BigInteger<NativeInt> quotient;
        BigInteger<NativeInt> remainder;
        auto f = this->Divide(quotient, remainder, *this, modulusValue);
        return remainder;
    }

    template<typename NativeInt>
    const BigInteger<NativeInt> BigIntegerMod<NativeInt>::ModEq(const Modulus<NativeInt> &modulus) {
        return *this = this->Mod(modulus.GetValue());
    }

    template<typename NativeInt>
    BigInteger<NativeInt> BigIntegerMod<NativeInt>::ModMul(const BigInteger<NativeInt> &another, const Modulus<NativeInt> &modulus) const {
        BigIntegerMod<NativeInt> mulRet(this->Mul(another));
        return mulRet.Mod(modulus);
    }

    template<typename NativeInt>
    const BigInteger<NativeInt> BigIntegerMod<NativeInt>::ModMulEq(const BigInteger<NativeInt> &another, const Modulus<NativeInt> &modulus) {
        return *this = this->ModMul(another, modulus);
    }

    template class zhejiangfhe::BigIntegerMod<uint32_t>;
    template class zhejiangfhe::BigIntegerMod<uint64_t>;
}// namespace zhejiangfhe