//
// Created by 赵启明 on 2023/5/19.
//

#ifndef ZJ_FHE_LIB_MODULUS_H
#define ZJ_FHE_LIB_MODULUS_H

#include "big_integer.h"

namespace zhejiangfhe {

    template<typename NativeInt>
    class Modulus {

    public:
        Modulus(BigInteger<NativeInt> value)
        {
            set_value(value);
        }

        inline const BigInteger<NativeInt> GetConstRatio(){
            return const_ratio;
        };

        inline const BigInteger<NativeInt> GetValue() const {
            return value;
        };

    private:
        BigInteger<NativeInt> value;
        bool is_prime = false; //todo add numth_util.h
        BigInteger<NativeInt> const_ratio;

        static const BigInteger<NativeInt> numerator;

        void set_value(BigInteger<NativeInt> value);


        class ValueInit {
        public:
            static BigInteger<NativeInt> InitB() {
                BigInteger<NativeInt> numerator(1);
                return numerator.LeftShift(128);
            }
        };

    };

}// namespace zhejiangfhe

#endif//ZJ_FHE_LIB_MODULUS_H
