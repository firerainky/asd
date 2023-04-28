//
// Created by 赵启明 on 2023/4/27.
//

#include "big_integer.h"

namespace zhejiangfhe {
    template<typename NativeInt>
    BigInteger<NativeInt>::BigInteger() {
        this->value.push_back(0);
    }

    template class zhejiangfhe::BigInteger<u_int32_t>;
}// namespace zhejiangfhe