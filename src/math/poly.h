//
// Created by 赵启明 on 2023/6/26.
//

#ifndef ZJ_FHE_LIB_POLY_H
#define ZJ_FHE_LIB_POLY_H

#include "big_integer_modop.h"
#include "exception.h"
#include "int_lattice_param.h"
#include "logger.h"
#include "modulus.h"
#include "vector.h"
#include <cstdint>
#include <iosfwd>

namespace zhejiangfhe {
    enum Format { EVALUATION = 0, COEFFICIENT = 1 };

    template<typename VecType>
    class Poly : public FormatBase {

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

        Poly &operator=(const Poly &rhs);

        Poly &operator=(Poly &&rhs) noexcept;

        Poly &operator=(std::initializer_list<std::string> rhs);

        /**
         * @param rhs The list of coefficients to set for the poly.
         * @return The resulting poly.
         */
        Poly &operator=(std::initializer_list<uint64_t> rhs);

        Poly &operator=(std::vector<int64_t> rhs);


        Poly &operator=(std::vector<int32_t> rhs);

        Poly &operator=(uint64_t val);

        bool operator==(const Poly &rhs) const {
            if (GetFormat() != rhs.GetFormat()) { return false; }
            if (params->GetRootOfUnity() != rhs.params->GetRootOfUnity()) { return false; }
            if (GetValue() != rhs.GetValue()) { return false; }
            return true;
        }

        bool operator!=(const Poly &rhs) const { return !(*this == rhs); }

        void SetValue(const VecType &value, Format format);

        void SetValueToZero();

        void SetValuesToMax();

        bool IsEmpty() const;

        const VecType &GetValue() const;

        uint32_t GetLength() const;

        const std::shared_ptr<Params> GetParams() const { return params; }

        const Format GetFormat() const { return format; }

        void SetFormat(const Format format) {
            if (GetFormat() != format) { SwitchFormat(); }
        }

        /**
         * @description: Convert representation from coefficient format to evaluation format or vice versa.
         *      Using FFT or inverse FFT algorithm.
         */
        void SwitchFormat();

        Integer &at(uint32_t i);
        const Integer &at(uint32_t i) const;

        Integer &operator[](uint32_t i);
        const Integer &operator[](uint32_t i) const;

        /**
         * @description: Performs an addition operation on polynomials.
         * @param rhs The polynomial to add with.
         * @return The result polynomial.
         */
        Poly Add(const Poly &rhs) const;

        /**
         * @description: Performs an addition operation in place.
         * @param rhs The polynomial to add with.
         * @return Self poly after addition.
         */
        Poly AddEq(const Poly &rhs);

        /**
         * @description: Performs a subtraction operation on polynomials.
         * @param rhs The polynomial to sub with.
         * @return The result polynomial.
         */
        Poly Sub(const Poly &rhs) const;

        /**
         * @description: Performs an subtraction operation in place.
         * @param rhs The polynomial to sub with.
         * @return Self poly after subtraction.
         */
        Poly SubEq(const Poly &rhs);

        /**
         * @brief 多项式乘标量
         * @param &value is the scalar to multiply with.
         * @return is the result of the multiplication.
         */
        Poly MultiplyScalar(const Integer &value) const;

        /**
         * @brief 原地进行多项式乘标量
         * @param &value is the scalar to multiply with.
         * @return is the result of the multiplication.
         */
        Poly MultiplyScalarEq(const Integer &value) const;


        /**
         * @brief 点值模式下，进行多项式点值乘法
         * @param &element is the element to multiply with.
         * @return is the result of the multiplication.
         */
        Poly MultiplyForEvaluation(const Poly &element) const;

        /**
         * @description: 点值模式下，原地进行多项式点值乘法
         * @param rhs The polynomial to multiply with.
         * @return
         */
        Poly MultiplyForEvaluationEq(const Poly &element);


        /**
         *
         * @return
         */

        const std::string ConvertToString() const {
            return (format == Format::EVALUATION ? "EVAL, " : "COEF") + this->GetValue().ConvertToString();
        }

        /**
         * @brief 系数模式下，进行多项式乘法
         * @param &element is the element to multiply with.
         * @return is the result of the multiplication.
         */
        Poly MultiplyPoly(const Poly &element);


        /**
         * @brief 系数模式下，原地进行多项式乘法
         * @param &element is the element to multiply with.
         * @return is the result of the multiplication.
         */
        Poly MultiplyPolyEq(const Poly &element);


    protected:
        std::unique_ptr<VecType> value;
        Format format;

        std::shared_ptr<Params> params;


    private:
        /**
         * @brief 系数模式下，进行多项式乘法
         * @param &element is the element to multiply with.
         * @return is the result of the multiplication.
         */
        Poly DoMultiplyPoly(const Poly &element);
    };
}// namespace zhejiangfhe


#endif//ZJ_FHE_LIB_POLY_H
