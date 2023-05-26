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
        BigInteger<NativeInt> modulusValue = modulus.GetValue();
        if (operand.GetMSB() < modulusValue.GetMSB() || operand.GetMSB() == modulusValue.GetMSB() && operand.AbsoluteCompare(modulusValue) < 0) {
            if (operand.getSign()) {
                return operand + modulusValue;
            } else {
                return BigInteger<NativeInt>(operand.GetValue(), operand.getSign());
            }
        }

        if (operand.GetMSB() == modulusValue.GetMSB() && operand.AbsoluteCompare(modulusValue) == 0) {
            return BigInteger<NativeInt>();
        }

        // use simple masking operation if modulus is 2
        if (modulusValue.GetMSB() == 2 && modulusValue.GetValue()[0] == 2) {
            if (operand.GetValue()[0] % 2 == 0) {
                return BigInteger<NativeInt>();
            } else {
                return BigInteger<NativeInt>("1");
            }
        }
        auto f = operand.DividedBy(modulusValue);
        if (operand.getSign()) {
            return f.second + modulusValue;
        } else {
            return f.second;
        }
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
}
#endif//ZJ_FHE_LIB_BIG_INTEGER_MODOP_H
