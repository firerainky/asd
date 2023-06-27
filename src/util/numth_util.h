//
// Created by 赵启明 on 2023/6/26.
//

#ifndef ZJ_FHE_LIB_NUMTHUTIL_H
#define ZJ_FHE_LIB_NUMTHUTIL_H
#include "exception.h"
#include <cstdint>
#include <iosfwd>
#include <set>

namespace zhejiangfhe {
    namespace util {

        template <typename IntType>
        void PrimeFactorize(IntType n, std::set<IntType>& primeFactors);

        template <typename IntType>
        const IntType PollardRhoFactorization(const IntType& n);

        template <typename IntType>
        bool MillerRabinPrimalityTest(const IntType& p, const uint32_t niter = 100);

        /**
         * 计算n的欧拉函数值
         *
         * @param &n the input number.
         * @return phi of n which is the number of integers m coprime to n such that 1 <= m <=
         * n.
         */
        uint64_t GetTotient(const uint64_t n);


        inline int nlz64(uint64_t x) {
            int n;
            if (x == 0) {
                return (64);
            }
            n = 0;
            if (x <= 0x00000000FFFFFFFF) {
                n = n + 32;
                x = x << 32;
            }
            if (x <= 0x0000FFFFFFFFFFFF) {
                n = n + 16;
                x = x << 16;
            }
            if (x <= 0x00FFFFFFFFFFFFFF) {
                n = n + 8;
                x = x << 8;
            }
            if (x <= 0x0FFFFFFFFFFFFFFF) {
                n = n + 4;
                x = x << 4;
            }
            if (x <= 0x3FFFFFFFFFFFFFFF) {
                n = n + 2;
                x = x << 2;
            }
            if (x <= 0x7FFFFFFFFFFFFFFF) {
                n = n + 1;
            }
            return n;
        }

        inline int nlz32(uint32_t x) {
            int n;

            if (x == 0) {
                return (32);
            }
            n = 0;
            if (x <= 0x0000FFFF) {
                n = n + 16;
                x = x << 16;
            }
            if (x <= 0x00FFFFFF) {
                n = n + 8;
                x = x << 8;
            }
            if (x <= 0x0FFFFFFF) {
                n = n + 4;
                x = x << 4;
            }
            if (x <= 0x3FFFFFFF) {
                n = n + 2;
                x = x << 2;
            }
            if (x <= 0x7FFFFFFF) {
                n = n + 1;
            }
            return n;
        }


        template<typename NativeInt>
        inline int nlz(NativeInt x) {
            if (typeid(x) == typeid(uint64_t)) {
                return nlz64(x);
            } else if (typeid(x) == typeid(uint32_t)) {
                return nlz32(x);
            } else {
                ZJFHE_THROW(zhejiangfhe::TypeException, "not support native int type");
            }
        }
    }
};


#endif//ZJ_FHE_LIB_NUMTHUTIL_H
