//
// Created by 赵启明 on 2023/5/22.
//

#ifndef ZJ_FHE_LIB_BIG_INTEGER_MODOP_H
#define ZJ_FHE_LIB_BIG_INTEGER_MODOP_H

#include "big_integer.h"
#include "modulus.h"

namespace zhejiangfhe {

    template<typename NativeInt>
    BigInteger<NativeInt> Mod(BigInteger<NativeInt> &operand, Modulus<NativeInt> &modulus) {
        return operand % modulus.GetValue();
    }

    template<typename NativeInt>
    BigInteger<NativeInt> ModIncrement(BigInteger<NativeInt> &operand, Modulus<NativeInt> &modulus) {
        BigInteger<NativeInt> a("1");
        return ModAdd(operand, a, modulus);
    }

    template<typename NativeInt>
    BigInteger<NativeInt> ModAdd(BigInteger<NativeInt> &a, BigInteger<NativeInt> &b, Modulus<NativeInt> &modulus) {

        if (a >= modulus.GetValue()) {
            a = Mod(a, modulus);
        }
        if (b >= modulus.GetValue()) {
            b = Mod(b, modulus);
        }
        a.AddEq(b);
        return Mod(a, modulus);
    }

    template<typename NativeInt>
    BigInteger<NativeInt> ModSub(BigInteger<NativeInt> &a, BigInteger<NativeInt> &b, Modulus<NativeInt> &modulus) {
        BigInteger<NativeInt> difference = a.Sub(b);
        BigInteger<NativeInt> modulusValue = modulus.GetValue();
        BigInteger<NativeInt> negativeModulus(modulusValue.GetValue(), true);
        if (difference == modulusValue) {
            return BigInteger<NativeInt>();
        } else if (difference < negativeModulus || difference > modulusValue) {
            return Mod(difference, modulus);
        } else {
            BigInteger<NativeInt> borrowTag(-static_cast<std::int64_t>(difference.GetSign()));
            return difference + modulusValue.And(borrowTag);
        }
    }


    template<typename NativeInt>
    BigInteger<NativeInt> ModMul(BigInteger<NativeInt> &operand, BigInteger<NativeInt> &another, Modulus<NativeInt> &modulus) {
        BigInteger<NativeInt> mulRes = operand.Mul(another);
        return Mod(mulRes, modulus);
    }

    template<typename NativeInt>
    BigInteger<NativeInt> ModExp(BigInteger<NativeInt> &operand, BigInteger<NativeInt> &exponent, Modulus<NativeInt> &modulus) {
        BigInteger<NativeInt> modulusValue = modulus.GetValue();
        BigInteger<NativeInt> mid = Mod(operand, modulus);
        BigInteger<NativeInt> product(1);
        BigInteger<NativeInt> exp(exponent);

        if (exponent == 0)
        {
            return 1;
        }

        if (exponent == 1 || operand == 1)
        {
            return operand;
        }
        while (true) {
            if (exp.getValueOfIndex(0) % 2 == 1) {
                product = product * mid;
            }
            if (product >= modulusValue) {
                product = Mod(product, modulus);
            }
            exp = exp.RightShift(1);
            if (exp == 0) {
                break;
            }
            mid = mid * mid;
            mid = Mod(mid, modulus);
        }
        return product;
    }
}
#endif//ZJ_FHE_LIB_BIG_INTEGER_MODOP_H
