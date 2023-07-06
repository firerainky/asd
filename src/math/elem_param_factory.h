/*
 * Created by Kanyan Zheng on Thu Jul 06 2023
 *
 * Copyright (c) 2023 Zhejiang Lab. All rights reserved.
 */

#include "int_lattice_param.h"
#include "nbtheory.h"

namespace zhejiangfhe {
    class ElemParamFactory {
    public:
        template<typename P>
        static std::shared_ptr<P> GenElemParams(uint32_t m, uint32_t bits) {
            typename P::Integer q = FirstPrime<typename P::Integer>(bits, m);
            typename P::Integer ru = RootOfUnity<typename P::Integer>(m, q);
            return std::make_shared<P>(m, q, ru);
        }
    };
}// namespace zhejiangfhe