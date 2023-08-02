/*
 * Created by Kanyan Zheng on Tue Jul 18 2023
 *
 * Copyright (c) 2023 Zhejiang Lab. All rights reserved.
 */

#include "ntt.h"
#include "big_integer_modop.h"
#include "debug.h"
#include "nbtheory.h"

namespace zhejiangfhe {

    template<typename VecType>
    void Ntt<VecType>::NTForwardTransformBitReverseInPlace(VecType *vec, const IntType &root,
                                                           const uint32_t cycloOrder) {
        ZJ_DEBUG_FLAG(false);
        if (root == 1 || root == 0) return;

        // TODO: Add power of two assertion for cyclo order?

        uint32_t halfOrder = cycloOrder >> 1;
        if (vec->GetLength() != halfOrder) {
            ZJFHE_THROW(MathException, "Element size must be equal to half order.");
        }

        // Precompute
        IntType modulus = vec->GetModulus().GetValue();
        auto mapSearch = rootReverseMap.find(modulus);
        if (mapSearch == rootReverseMap.end() || mapSearch->second.size() != halfOrder) {
            precompute(root, cycloOrder, modulus);
            ZJ_DEBUG("Precccccompute!!");
        }
        std::vector<IntType> rootReverseVec = rootReverseMap[modulus];

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
                    if (hiVal >= modulus) { hiVal -= modulus; }

                    if (loVal < omegaFactor) { loVal += modulus; }
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
    void Ntt<VecType>::NTInverseTransformBitReverseInPlace(VecType *vec, const IntType &root,
                                                           const uint32_t cycloOrder) {
        ZJ_DEBUG_FLAG(false);
        if (root == 1 || root == 0) return;

        // TODO: Add power of two assertion for cyclo order?

        uint32_t halfOrder = cycloOrder >> 1;
        if (vec->GetLength() != halfOrder) {
            ZJFHE_THROW(MathException, "Element size must be equal to half order.");
        }

        // Precompute
        IntType modulus = vec->GetModulus().GetValue();
        auto mapSearch = rootInverseReverseMap.find(modulus);
        if (mapSearch == rootInverseReverseMap.end() || mapSearch->second.size() != halfOrder) {
            precompute(root, cycloOrder, modulus);
            ZJ_DEBUG("Precccccompute!!");
        }
        std::vector<IntType> rootInverseReverseVec = rootInverseReverseMap[modulus];

        // Inverse NTT
        uint32_t n = halfOrder;
        uint32_t i, m, j1, j2, indexOmega, indexLo, indexHi;
        IntType omega, omegaFactor, loVal, hiVal, zero(0);

        uint32_t t = 1;
        uint32_t logt1 = 1;
        for (m = (n >> 1); m >= 1; m >>= 1) {
            for (i = 0; i < m; ++i) {
                j1 = i << logt1;
                j2 = j1 + t;
                indexOmega = m + i;
                omega = rootInverseReverseVec[indexOmega];

                for (indexLo = j1; indexLo < j2; ++indexLo) {
                    indexHi = indexLo + t;

                    loVal = (*vec)[indexLo];
                    hiVal = (*vec)[indexHi];

                    omegaFactor = loVal;
                    if (omegaFactor < hiVal) { omegaFactor += modulus; }
                    omegaFactor -= hiVal;

                    loVal += hiVal;
                    if (loVal >= modulus) { loVal -= modulus; }
                    omegaFactor = omegaFactor * omega % modulus;

                    (*vec)[indexLo] = loVal;
                    (*vec)[indexHi] = omegaFactor;
                }
            }
            t <<= 1;
            logt1++;
        }

        uint32_t msb = GetMSB64(halfOrder - 1);
        IntType cycloOrderInv = cycloOrderInverseMap[modulus][msb];
        for (size_t i = 0; i < n; ++i) { (*vec)[i] = (*vec)[i] * cycloOrderInv % modulus; }
    }

    template<typename VecType>
    void Ntt<VecType>::precompute(const IntType &root, const uint32_t cycloOrder,
                                  const IntType &modulus) {
        uint32_t halfOrder = (cycloOrder >> 1);
        IntType rootInverse = util::ModInverse(root, Modulus(modulus));

        std::vector<IntType> rootReverseVec(halfOrder);
        std::vector<IntType> rootInverseReverseVec(halfOrder);
        IntType x = 1, xinv = 1;
        uint32_t msb = GetMSB64(halfOrder - 1);

        for (size_t i = 0; i < halfOrder; ++i) {
            uint32_t iinv = ReverseBits(i, msb);
            rootReverseVec[iinv] = x;
            rootInverseReverseVec[iinv] = xinv;
            x = x * root % modulus;
            xinv = xinv * rootInverse % modulus;
        }
        rootReverseMap[modulus] = rootReverseVec;
        rootInverseReverseMap[modulus] = rootInverseReverseVec;

        std::vector<IntType> cycloOrderInverseVec(msb + 1);
        for (size_t i = 0; i < msb + 1; ++i) {
            IntType coInv = util::ModInverse(IntType(1 << i), Modulus(modulus));
            cycloOrderInverseVec[i] = coInv;
        }
        cycloOrderInverseMap[modulus] = cycloOrderInverseVec;
    }

    template class zhejiangfhe::Ntt<Vector<limbtype>>;
}// namespace zhejiangfhe