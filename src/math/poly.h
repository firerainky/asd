//
// Created by 赵启明 on 2023/6/26.
//

#ifndef ZJ_FHE_LIB_POLY_H
#define ZJ_FHE_LIB_POLY_H

#include "exception.h"
#include "modulus.h"
#include "int_lattice_param.h"
#include "big_integer_modop.h"
#include "vector.h"
#include <cstdint>
#include <iosfwd>

namespace zhejiangfhe {
    template<typename NativeInt>
    class poly {

    public:
        poly();

    protected:
        IntLatticeParam<NativeInt> param;
        Vector<NativeInt> value;
    };
}


#endif//ZJ_FHE_LIB_POLY_H
