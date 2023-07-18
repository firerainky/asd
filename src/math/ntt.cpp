/*
 * Created by Kanyan Zheng on Tue Jul 18 2023
 *
 * Copyright (c) 2023 Zhejiang Lab. All rights reserved.
 */

#include "ntt.h"
#include "nbtheory.h"

namespace zhejiangfhe {

    template<typename VecType>
    void Ntt<VecType>::NumberTheoryTransformBitReverseInPlace(VecType *vec, const IntType &root, const uint32_t cycloOrder) {
        if (root == 1 || root == 0) return;

        // TODO: Add power of two assertion for cyclo order?

        uint32_t halfOrder = cycloOrder >> 1;
        if (vec->GetLength() != halfOrder) {
            ZJFHE_THROW(MathException, "Element size must be equal to half order.");
        }

        // Precompute
        IntType modulus = vec->GetModulus().GetValue();
        std::vector<typename VecType::Integer> rootReverseVec = precompute(root, cycloOrder, modulus);

        // NTT
        uint32_t n = halfOrder;
        uint32_t i, m, j1, j2, indexOmega, indexLo, indexHi;
        IntType omega, omegaFactor, loVal, hiVal, zero(0);

        uint32_t t = (n >> 1);
        uint32_t logt1 = GetMSB64(t);
        for (m = 1; m < n; m <<= 1) {
            for (i = 0; i < m; ++i) {
                j1 = i << logt1;
                j2 = j1 + t;
                indexOmega = m + i;
                omega = rootReverseVec[indexOmega];
                for (indexLo = j1; indexLo < j2; ++indexLo) {
                    indexHi = indexLo + t;
                    loVal = (*vec)[indexLo];
                    omegaFactor = (*vec)[indexHi];
                    omegaFactor = omegaFactor * omega % modulus;

                    hiVal = loVal + omegaFactor;
                    if (hiVal >= modulus) {
                        hiVal -= modulus;
                    }

                    if (loVal < omegaFactor) {
                        loVal += modulus;
                    }
                    loVal -= omegaFactor;

                    (*vec)[indexLo] = hiVal;
                    (*vec)[indexHi] = loVal;
                }
            }
            t >>= 1;
            logt1--;
        }
    }

    template<typename VecType>
    std::vector<typename VecType::Integer> Ntt<VecType>::precompute(const IntType &root, const uint32_t cycloOrder, const IntType &modulus) {
        uint32_t halfOrder = (cycloOrder >> 1);

        std::vector<IntType> rootReverseVec(halfOrder);
        IntType x = 1;
        uint32_t msb = GetMSB64(halfOrder - 1);
        for (size_t i = 0; i < halfOrder; ++i) {
            uint32_t iinv = ReverseBits(i, msb);
            rootReverseVec[iinv] = x;
            x = x * root % modulus;
        }

        return rootReverseVec;
    }

    template class zhejiangfhe::Ntt<Vector<limbtype>>;
}// namespace zhejiangfhe