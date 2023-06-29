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
    enum Format { EVALUATION = 0, COEFFICIENT = 1 };

    template<typename VecType>
    class Poly {

    public:
        using Integer = typename VecType::Integer;
        using Params  = IntLatticeParam<Integer>;

        Poly();

        Poly(const std::shared_ptr<Params> params, Format format = Format::EVALUATION,
                 bool initializeElementToZero = false);

        Poly(const Poly& element, std::shared_ptr<Params> param);

        const Poly& operator=(const Poly& rhs);

        const Poly& operator=(Poly&& rhs);

        const Poly& operator=(std::initializer_list<uint64_t> rhs);

        const Poly& operator=(std::vector<int64_t> rhs);

        const Poly& operator=(std::initializer_list<std::string> rhs);

        const Poly& operator=(std::vector<int32_t> rhs);

        const Poly& operator=(uint64_t val);

        void SetValue(const VecType& value, Format format);

        void SetValueToZero();

        void SetValuesToMax();

        bool IsEmpty() const;


        uint32_t GetLength() const;

        const Params GetParams() const {
            return params;
        }

        const Format GetFormat() const {
            return format;
        }

        Integer& at(uint32_t i);
        const Integer& at(uint32_t i) const;


        Integer& operator[](uint32_t i);
        const Integer& operator[](uint32_t i) const;


    protected:
        VecType value;
        Format format;

        std::shared_ptr<Params> params;
    };
}


#endif//ZJ_FHE_LIB_POLY_H
