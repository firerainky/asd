/*
 * Created by Kanyan Zheng on Tue Jul 18 2023
 *
 * Copyright (c) 2023 Zhejiang Lab. All rights reserved.
 */

#pragma once

#include "vector.h"
#include <map>

namespace zhejiangfhe {
    template<typename VecType>
    class Ntt {
        using IntType = typename VecType::Integer;

    public:
        static Ntt *getInstance() {
            if (!instance) {
                instance = new Ntt();
            }
            return instance;
        }

        /**
         * @description: In place perform number theory transform in the ring Z_q[x]/(x^d+1)
         *    where prime q = m * cycloOrder + 1, d = cycloOrder / 2 = phi(cycloOrder).
         *    Precompute roots table at first and it's a bit-reverse version.
         * @param vec Reverse order of coefficients in poly Z_q[x], and become evaluations at the end.
         * @param root 2d-th primitive root of unity in Z_q.
         * @param cycloOrder 2d, must be a power of two.
         */
        void NTForwardTransformBitReverseInPlace(VecType *vec,
                                                 const IntType &root,
                                                 const uint32_t cycloOrder);

        /**
         * @description: In place perform inverse number theory transform in the ring Z_q[x]/(x^d+1)
         *    where prime q = m * cycloOrder + 1, d = cycloOrder / 2 = phi(cycloOrder).
         *    Precompute roots table at first and it's a bit-reverse version.
         * @param vec Reverse order of coefficients in poly Z_q[x], and become evaluations at the end.
         * @param root 2d-th primitive root of unity in Z_q.
         * @param cycloOrder 2d, must be a power of two.
         */
        void NTInverseTransformBitReverseInPlace(VecType *vec,
                                                 const IntType &root,
                                                 const uint32_t cycloOrder);

    private:
        void precompute(const IntType &root,
                        const uint32_t cycloOrder,
                        const IntType &modulus);

        std::map<IntType, std::vector<IntType>>
                rootReverseMap;// [modulus : rootReverseVec]
        std::map<IntType, std::vector<IntType>>
                rootInverseReverseMap;// [modulus : rootInverseReverseVec]
        std::map<IntType, std::vector<IntType>>
                cycloOrderInverseMap;// [modulus : cycloOrderInverseVec]

        static Ntt *instance;
        Ntt(){};
    };
}// namespace zhejiangfhe