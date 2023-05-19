//
// Created by 赵启明 on 2023/5/22.
//

#ifndef ZJ_FHE_LIB_BIG_INTEGER_MODOP_H
#define ZJ_FHE_LIB_BIG_INTEGER_MODOP_H

#include "big_integer.h"
#include "modulus.h"

namespace zhejiangfhe {

    template<typename NativeInt>
    class BigIntegerMod : public BigInteger<NativeInt> {

    public:
        BigIntegerMod(const BigInteger<NativeInt> bigInteger): BigInteger<NativeInt>(bigInteger) {

        }
        BigIntegerMod(const std::string &strValue = ""): BigInteger<NativeInt>(strValue) {

        }

        BigIntegerMod(std::vector<NativeInt> vals, bool sign = false): BigInteger<NativeInt>(vals, sign) {

        }

        // Mod operations
        BigInteger<NativeInt> Mod(const Modulus<NativeInt> &modulus) const;
        const BigInteger<NativeInt> ModEq(const Modulus<NativeInt> &modulus);
        BigInteger<NativeInt> ModMul(const BigInteger<NativeInt> &another, const Modulus<NativeInt> &modulus) const;
        const BigInteger<NativeInt> ModMulEq(const BigInteger<NativeInt> &another, const Modulus<NativeInt> &modulus);
    };
}
#endif//ZJ_FHE_LIB_BIG_INTEGER_MODOP_H
