//
// Created by 赵启明 on 2023/6/26.
//

#ifndef ZJ_FHE_LIB_INT_LATTICE_PARAM_H
#define ZJ_FHE_LIB_INT_LATTICE_PARAM_H
#include "exception.h"
#include "big_integer.h"
#include "numth_util.h"
#include <cstdint>
#include <iosfwd>

namespace zhejiangfhe {
    template<typename IntegerType>
    class IntLatticeParam {

    public:
        IntLatticeParam(uint32_t order, const IntegerType &ctModulus, const IntegerType &rUnity = IntegerType(0),
                        const IntegerType &bigCtModulus = IntegerType(0), const IntegerType &bigRUnity = IntegerType(0)) {
            cyclotomicOrder = order;
            ringDimension = order / 2; // todo ???
//            ringDimension = util::GetTotient(order);
            isPowerOfTwo = ringDimension == cyclotomicOrder / 2;
            ciphertextModulus = ctModulus;
            rootOfUnity = rUnity;
            bigCiphertextModulus = bigCtModulus;
            bigRootOfUnity = bigRUnity;
        }

        IntLatticeParam(const IntLatticeParam& rhs) {
            cyclotomicOrder      = rhs.cyclotomicOrder;
            ringDimension        = rhs.ringDimension;
            isPowerOfTwo         = rhs.isPowerOfTwo;
            ciphertextModulus    = rhs.ciphertextModulus;
            rootOfUnity          = rhs.rootOfUnity;
            bigCiphertextModulus = rhs.bigCiphertextModulus;
            bigRootOfUnity       = rhs.bigRootOfUnity;
        }

        IntLatticeParam(const IntLatticeParam&& rhs) {
            cyclotomicOrder      = rhs.cyclotomicOrder;
            ringDimension        = rhs.ringDimension;
            isPowerOfTwo         = rhs.isPowerOfTwo;
            ciphertextModulus    = std::move(rhs.ciphertextModulus);
            rootOfUnity          = std::move(rhs.rootOfUnity);
            bigCiphertextModulus = std::move(rhs.bigCiphertextModulus);
            bigRootOfUnity       = std::move(rhs.bigRootOfUnity);
        }


        const IntLatticeParam& operator=(const IntLatticeParam& rhs) {
            cyclotomicOrder      = rhs.cyclotomicOrder;
            ringDimension        = rhs.ringDimension;
            isPowerOfTwo         = rhs.isPowerOfTwo;
            ciphertextModulus    = rhs.ciphertextModulus;
            rootOfUnity          = rhs.rootOfUnity;
            bigCiphertextModulus = rhs.bigCiphertextModulus;
            bigRootOfUnity       = rhs.bigRootOfUnity;
            return *this;
        }

        uint32_t GetCyclotomicOrder() const {
            return cyclotomicOrder;
        }

        uint32_t GetRingDimension() const {
            return ringDimension;
        }

        bool OrderIsPowerOfTwo() const {
            return isPowerOfTwo;
        }

        const IntegerType& GetModulus() const {
            return ciphertextModulus;
        }

        const IntegerType& GetBigModulus() const {
            return bigCiphertextModulus;
        }

        const IntegerType& GetRootOfUnity() const {
            return rootOfUnity;
        }

        const IntegerType& GetBigRootOfUnity() const {
            return bigRootOfUnity;
        }


    protected:
        /**
         * 分圆多项式的阶？
         */
        uint32_t cyclotomicOrder;

        /**
         * 环的尺寸
         */
        uint32_t ringDimension;

        /**
         * ringDimension 是否为2的幂
         */
        bool isPowerOfTwo;

        /**
         * 模数
         */
        IntegerType ciphertextModulus;


        /**
         * 单位根
         */
        IntegerType rootOfUnity;

        /**
         * the big ciphertext modulus used for bit packing operations
         */
        IntegerType bigCiphertextModulus;


        /**
         * the big root of unity used for bit packing operations
         */
        IntegerType bigRootOfUnity;
    };
}
#endif//ZJ_FHE_LIB_INT_LATTICE_PARAM_H
