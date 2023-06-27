//
// Created by 赵启明 on 2023/6/26.
//

#ifndef ZJ_FHE_LIB_INT_LATTICE_PARAM_H
#define ZJ_FHE_LIB_INT_LATTICE_PARAM_H
#include "exception.h"
#include "big_integer.h"
#include <cstdint>
#include <iosfwd>


template <typename IntegerType>
class IntLatticeParam {

    IntLatticeParam(uint32_t order, const IntegerType& ctModulus, const IntegerType& rUnity = IntegerType(0),
               const IntegerType& bigCtModulus = IntegerType(0), const IntegerType& bigRUnity = IntegerType(0)) {
        cyclotomicOrder      = order;
//        ringDimension        = GetTotient(order);
        isPowerOfTwo         = ringDimension == cyclotomicOrder / 2;
        ciphertextModulus    = ctModulus;
        rootOfUnity          = rUnity;
        bigCiphertextModulus = bigCtModulus;
        bigRootOfUnity       = bigRUnity;
    }


protected:
    uint32_t cyclotomicOrder;
    uint32_t ringDimension;  // True iff the Ring Dimension is a power of 2.

    bool isPowerOfTwo;
    IntegerType ciphertextModulus;
    IntegerType rootOfUnity;
    IntegerType bigCiphertextModulus;  // Used for only some applications.
    IntegerType bigRootOfUnity;        // Used for only some applications.

};
#endif//ZJ_FHE_LIB_INT_LATTICE_PARAM_H
