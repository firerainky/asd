#ifndef ZJ_FHE_LIB_BasicArithmatic_H
#define ZJ_FHE_LIB_BasicArithmatic_H

#pragma once

#include <iostream>
#include <vector>

#define BInt_32

#ifdef BInt_32
using LimbType = uint32_t;
using DoubleLimbType = uint64_t;
using SignedDoubleLimbType = int64_t;
#else
using LimbType = uint64_t;
using DoubleLimbType = unsigned __int128;
using SignedDoubleLimbType = __int128;
#endif

namespace zhejiangfhe {

    const auto LimbSize = sizeof(LimbType) * 8;

    namespace basic_arithmatic {

        int nlz(LimbType x);

        bool divide(const std::vector<LimbType> &numerator, const std::vector<LimbType> &denominator, const std::vector<LimbType> &quotient, const std::vector<LimbType> &remainder);
        // using Dlimb_t = typename DoubleDataType<NativeInt>::T;
        // using SDlimb_t = typename SignedDoubleDataType<NativeInt>::T;

        // auto numeratorSize = numerator.size();
        // auto denominatorSize = denominator.size();

        // const NativeInt m_MaxLimb = std::numeric_limits<NativeInt>::max();
        // const Dlimb_t ffs = (Dlimb_t) m_MaxLimb;
        // const Dlimb_t b = (Dlimb_t) m_MaxLimb + 1;
        // quotient.resize(numeratorSize - denominatorSize + 1);

        // SDlimb_t k;
        // int j;

        // if (numeratorSize < denominatorSize || denominatorSize <= 0 || denominator[denominatorSize - 1] == 0) {
        //     return false;
        // }

        // if (denominatorSize == 1) {
        //     k = 0;
        //     for (j = numeratorSize - 1; j >= 0; --j) {
        //         quotient[j] = (k * b + u[j]) / denominator[0];
        //         k = (k * b + numerator[j]) - quotient[j] * denominator[0];
        //     }
        //     if (remainder.size() != 0) {
        //         remainder[0] = k;
        //     }
        //     return true;
        // }
        // };
    }// namespace basic_arithmatic

}// namespace zhejiangfhe

#endif