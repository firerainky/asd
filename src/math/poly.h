//
// Created by 赵启明 on 2023/6/26.
//

#ifndef ZJ_FHE_LIB_POLY_H
#define ZJ_FHE_LIB_POLY_H

#include "big_integer_modop.h"
#include "exception.h"
#include "int_lattice_param.h"
#include "modulus.h"
#include "vector.h"
#include <cstdint>
#include <iosfwd>

namespace zhejiangfhe {
    enum Format { EVALUATION = 0,
                  COEFFICIENT = 1 };

    template<typename VecType>
    class Poly {

    public:
        using Integer = typename VecType::Integer;
        using Params = IntLatticeParam<Integer>;

        Poly();

        Poly(const std::shared_ptr<Params> params, Format format = Format::EVALUATION,
             bool initializeElementToZero = false);

        /**
         * @description: Copy constructor.
         * @param element The copied poly.
         */
        Poly(const Poly &element);

        const Poly &operator=(const Poly &rhs);

        const Poly &operator=(Poly &&rhs);

        const Poly &operator=(std::initializer_list<std::string> rhs);

        /**
         * @param rhs The list of coefficients to set for the poly.
         * @return The resulting poly.
         */
        const Poly &operator=(std::initializer_list<uint64_t> rhs);

        const Poly &operator=(std::vector<int64_t> rhs);


        const Poly &operator=(std::vector<int32_t> rhs);

        const Poly &operator=(uint64_t val);

        bool operator==(const Poly &rhs) const {
            if (GetFormat() != rhs.GetFormat()) { return false; }
            // if (params->GetRootOfUnity() != rhs.params->GetRootOfUnity()) {
            //     return false;
            // }
            return true;
        }

        void SetValue(const VecType &value, Format format);

        void SetValueToZero();

        void SetValuesToMax();

        bool IsEmpty() const;

        const VecType &GetValue() const;

        uint32_t GetLength() const;

        const std::shared_ptr<Params> GetParams() const {
            return params;
        }

        const Format GetFormat() const {
            return format;
        }

        Integer &at(uint32_t i);
        const Integer &at(uint32_t i) const;


        Integer &operator[](uint32_t i);
        const Integer &operator[](uint32_t i) const;


    protected:
        std::unique_ptr<VecType> value;
        Format format;

        std::shared_ptr<Params> params;
    };
}// namespace zhejiangfhe


#endif//ZJ_FHE_LIB_POLY_H
