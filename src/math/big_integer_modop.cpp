//
// Created by Zheng Kanyan on 2023/5/17.
//
#include "big_integer_modop.h"
#include "big_integer.h"
#include "modulus.h"
#include <limits>

namespace zhejiangfhe {
    template<typename NativeInt>
    BigInteger<NativeInt> BigIntegerMod<NativeInt>::Mod(const Modulus<NativeInt> &modulus) const {
        BigInteger<NativeInt> modulusValue = modulus.GetValue();
        if (this->m_MSB < modulusValue.GetMSB() || this->m_MSB == modulusValue.GetMSB() && this->AbsoluteCompare(modulusValue) < 0) {
            if (this->sign) {
                return *this + modulusValue;
            } else {
                return BigInteger<NativeInt>(this->value, this->sign);
            }
        }

        if (this->m_MSB == modulusValue.GetMSB() && this->AbsoluteCompare(modulusValue) == 0) {
            return BigInteger<NativeInt>();
        }

        // use simple masking operation if modulus is 2
        if (modulusValue.GetMSB() == 2 && modulusValue.GetValue()[0] == 2) {
            if (this->value[0] % 2 == 0) {
                return BigInteger<NativeInt>();
            } else {
                return BigInteger<NativeInt>("1");
            }
        }
        auto f = this->DividedBy(modulusValue);
        if (this->sign) {
            return f.second + modulusValue;
        } else {
            return f.second;
        }
    }

    template<typename NativeInt>
    BigInteger<NativeInt> BigIntegerMod<NativeInt>::ModBarrett(NativeInt modulus) {
        // Precomputation
        BigInteger<NativeInt> power(std::vector<NativeInt>({0, 0, 1}));
        BigInteger<NativeInt> quotient;
        BigInteger<NativeInt> remainder;
        this->Divide(quotient, remainder, power, modulus);
        BigInteger bigA = *this * quotient;
        bigA = bigA.RightShift(2 * this->m_limbBitLength);
        bigA *= modulus;
        bigA = this->Sub(bigA);
        if (bigA > modulus) {
            bigA -= modulus;
        }
        return bigA;
    }

    template<typename NativeInt>
    const BigInteger<NativeInt> BigIntegerMod<NativeInt>::ModEq(const Modulus<NativeInt> &modulus) {
        return *this = this->Mod(modulus.GetValue());
    }
    template<typename NativeInt>
    BigInteger<NativeInt> BigIntegerMod<NativeInt>::ModIncrement(const Modulus<NativeInt> &modulus) const {
         BigInteger<NativeInt> a("1");
        return this->ModAdd(a, modulus);
    }

    template<typename NativeInt>
    BigInteger<NativeInt> BigIntegerMod<NativeInt>::ModAdd(const BigIntegerMod<NativeInt> &b, const Modulus<NativeInt> &modulus) const {
        BigIntegerMod<NativeInt> a(this->value, this->sign);
        BigIntegerMod<NativeInt> b_op(b.getValue(), b.getSign());
        if (*this >= modulus.GetValue()) {
            a.ModEq(modulus);
        }
        if (b >= modulus.GetValue()) {
            b_op.ModEq(modulus);
        }
        a.AddEq(b_op);
        return a.ModEq(modulus);
    }

    template<typename NativeInt>
    const BigInteger<NativeInt> &BigIntegerMod<NativeInt>::ModAddEq(const BigIntegerMod<NativeInt> &b, const Modulus<NativeInt> &modulus) {
         BigIntegerMod<NativeInt> b_op(b.getValue(), b.getSign());
        if (*this >= modulus.GetValue()) {
            this->ModEq(modulus);
        }
        if (b >= modulus.GetValue()) {
            b_op.ModEq(modulus);
        }
        this->AddEq(b_op);
        this->ModEq(modulus);
        return *this;
    }


    template<typename NativeInt>
    BigInteger<NativeInt> BigIntegerMod<NativeInt>::ModSub(const BigInteger<NativeInt> &another, const Modulus<NativeInt> &modulus) const {
        BigInteger difference = this->Sub(another);
        BigInteger<NativeInt> modulusValue = modulus.GetValue();
        BigInteger negativeModulus(modulusValue.GetValue(), true);
        if (difference == modulusValue) {
            return BigInteger<NativeInt>();
        } else if (difference < negativeModulus || difference > modulusValue) {
            BigIntegerMod<NativeInt> diff(difference);
            return diff.Mod(modulusValue);
        } else {
            BigInteger<NativeInt> borrowTag(-static_cast<std::int64_t>(difference.GetSign()));
            return difference + modulusValue.And(borrowTag);
        }
    }

    template<typename NativeInt>
    const BigInteger<NativeInt> BigIntegerMod<NativeInt>::ModSubEq(const BigInteger<NativeInt> &another, const Modulus<NativeInt> &modulus) {
        return *this = this->ModSub(another, modulus);
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