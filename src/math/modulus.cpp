//
// Created by 赵启明 on 2023/5/19.
//

#include "modulus.h"

namespace zhejiangfhe {

    template<typename NativeInt>
    const BigInteger<NativeInt> Modulus<NativeInt>::numerator = Modulus<NativeInt>::ValueInit::InitB();

    template<typename NativeInt>
    void Modulus<NativeInt>::set_value(BigInteger<NativeInt> input) {
        if (input == 0UL) {
            value = 0;
            const_ratio = BigInteger<NativeInt>("0");

        } else {
            value = input;
            const_ratio = numerator.DividedBy(input).first;
        }
    }


    template<typename NativeInt>
    const Modulus<NativeInt>& Modulus<NativeInt>::operator=(const Modulus<NativeInt>& val) {
        if (this != &val) {
            this->set_value(val.value);
        }
        return *this;
    }


    template<typename NativeInt>
    const Modulus<NativeInt>& Modulus<NativeInt>::operator=(Modulus<NativeInt>&& val) {
        if (this != &val) {
            this->set_value(val.value);
        }
        return *this;
    }

    template class zhejiangfhe::Modulus<uint32_t>;
    template class zhejiangfhe::Modulus<uint64_t>;
}// namespace zhejiangfhe