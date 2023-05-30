//
// Created by 赵启明 on 2023/5/30.
//

#ifndef ZJ_FHE_LIB_VECTOR_H
#define ZJ_FHE_LIB_VECTOR_H

#include "exception.h"
#include <cstdint>
#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

namespace zhejiangfhe {
    template <class IntegerType>
    class Vector {

    public:
        Vector();

        explicit Vector(uint32_t length);

        Vector(uint32_t length, const IntegerType& modulus);

        Vector(const Vector& bigVector);

        Vector(Vector&& bigVector);

        Vector(uint32_t length, const IntegerType& modulus, std::initializer_list<std::string> rhs);

        Vector(uint32_t length, const IntegerType& modulus, std::initializer_list<uint64_t> rhs);

        virtual ~Vector();


        const Vector& operator=(const Vector& rhs);

        Vector& operator=(Vector&& rhs);

        const Vector& operator=(std::initializer_list<std::string> rhs);

        const Vector& operator=(std::initializer_list<uint64_t> rhs);

        IntegerType& at(size_t i) {
            if (!this->IndexCheck(i)) {
                ZJFHE_THROW(MathException, "Vector index out of range");
            }
            return this->data[i];
        }

        const IntegerType& at(size_t i) const {
            if (!this->IndexCheck(i)) {
                ZJFHE_THROW(MathException, "Vector index out of range");
            }
            return this->data[i];
        }

        IntegerType& operator[](size_t idx) {
            return (this->data[idx]);
        }

        const IntegerType& operator[](size_t idx) const {
            return (this->data[idx]);
        }

        void SetModulus(const IntegerType& value);

        void SwitchModulus(const IntegerType& value);

        const IntegerType& GetModulus() const;

        Vector Mod(const IntegerType& modulus) const;

        const Vector& ModEq(const IntegerType& modulus);

        Vector ModAdd(const IntegerType& b) const;

        const Vector& ModAddEq(const IntegerType& b);

        Vector ModAddAtIndex(uint32_t i, const IntegerType& b) const;

        const Vector& ModAddAtIndexEq(uint32_t i, const IntegerType& b);

        Vector ModAdd(const Vector& b) const;

        const Vector& ModAddEq(const Vector& b);

        Vector ModSub(const IntegerType& b) const;

        const Vector& ModSubEq(const IntegerType& b);

        Vector ModSub(const Vector& b) const;

        const Vector& ModSubEq(const Vector& b);

        Vector ModMul(const IntegerType& b) const;

        const Vector& ModMulEq(const IntegerType& b);

        Vector ModMul(const Vector& b) const;

        const Vector& ModMulEq(const Vector& b);

        Vector ModExp(const IntegerType& b) const;

        const Vector& ModExpEq(const IntegerType& b);

        Vector ModInverse() const;

        const Vector& ModInverseEq();

        Vector Multiply(const Vector& b) const;

        const Vector MultiplyEq(const Vector& b);

        Vector MultiplyAndRound(const IntegerType& p, const IntegerType& q) const;

        const Vector& MultiplyAndRoundEq(const IntegerType& p, const IntegerType& q);

        Vector DivideAndRound(const IntegerType& q) const;

        const Vector& DivideAndRoundEq(const IntegerType& q);

        Vector GetDigitAtIndexForBase(uint32_t index, uint32_t base) const;

    private:

        std::vector<IntegerType> data;
        IntegerType modulus = 0;

        bool IndexCheck(size_t length) const {
            if (length > this->data.size()) {
                return false;
            }
            return true;
        }

    };

}// namespace zhejiangfhe

#endif//ZJ_FHE_LIB_VECTOR_H
