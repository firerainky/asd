//
// Created by 赵启明 on 2023/6/26.
//

#ifndef ZJ_FHE_LIB_NUMTHUTIL_H
#define ZJ_FHE_LIB_NUMTHUTIL_H
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
    }
};


#endif//ZJ_FHE_LIB_NUMTHUTIL_H
