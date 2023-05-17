//
// Created by Zheng Kanyan on 2023/5/17.
//
#include "big_integer.h"
#include <limits>

namespace zhejiangfhe {
    template<typename NativeInt>
    BigInteger<NativeInt> BigInteger<NativeInt>::ModMul(const BigInteger<NativeInt> &another, const BigInteger &modulus) const {
        return BigInteger();
    }

    template class zhejiangfhe::BigInteger<uint32_t>;
    template class zhejiangfhe::BigInteger<uint64_t>;
}// namespace zhejiangfhe