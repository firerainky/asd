//
// Created by 赵启明 on 2023/5/30.
//

#ifndef ZJ_FHE_LIB_VECTOR_H
#define ZJ_FHE_LIB_VECTOR_H

#include "exception.h"
#include "modulus.h"
#include <cstdint>
#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

namespace zhejiangfhe {

    enum State { INITIALIZED,
                 GARBAGE };

    template<typename IntegerType>
    class Vector {

    public:
        typedef BigInteger<IntegerType> Integer;

        Vector();

        Vector(uint32_t length);

        Vector(uint32_t length, const Modulus<IntegerType> &modulus);

        Vector(uint32_t length, const IntegerType &modulus) : Vector(length, Modulus<IntegerType>(modulus)){};

        Vector(const uint32_t length, const std::string &modulus) : Vector(length, Modulus<IntegerType>(modulus)){};

        Vector(const Vector &bigVector);

        Vector(Vector &&bigVector);

        Vector(uint32_t length, const IntegerType &modulus, std::initializer_list<std::string> rhs);

        Vector(uint32_t length, const IntegerType &modulus, std::initializer_list<uint64_t> rhs);

        explicit Vector(const std::vector<std::string> &s, const Modulus<IntegerType> &modulus);

        explicit Vector(const std::vector<std::string> &s, const IntegerType &modulus) : Vector(s, Modulus<IntegerType>(modulus)){};

        explicit Vector(const std::vector<std::string> &s, const std::string &modulus) : Vector(s, Modulus<IntegerType>(modulus)){};

        const Vector &operator=(std::initializer_list<std::string> rhs);
        const Vector &operator=(std::initializer_list<uint64_t> rhs);

        virtual ~Vector();

        bool operator==(const Vector &rhs) const;
        bool operator!=(const Vector &rhs) const;

        Vector &operator=(Vector &rhs);

        Vector &operator=(Vector &&rhs);

        Integer* ConvertToIntList() {
            Integer *result = new Integer[GetLength()];
            for (int i=0; i<GetLength(); i++) {
                result[i] = at(i);
            }
            return result;
        }

        BigInteger<IntegerType> &at(size_t i) {
            if (!this->IndexCheck(i)) {
                ZJFHE_THROW(MathException, "Vector index out of range");
            }
            return this->data[i];
        }

        const BigInteger<IntegerType> &at(size_t i) const {
            if (!this->IndexCheck(i)) {
                ZJFHE_THROW(MathException, "Vector index out of range");
            }
            return this->data[i];
        }

        BigInteger<IntegerType> &operator[](size_t idx) {
            return (this->data[idx]);
        }

        const BigInteger<IntegerType> &operator[](size_t idx) const {
            return (this->data[idx]);
        }

        size_t GetLength() const {
            return data.size();
        }

        void SetModulus(const Modulus<IntegerType> &value);

        void SwitchModulus(const Modulus<IntegerType> &value);

        const Modulus<IntegerType> &GetModulus() const;

        Vector ModAdd(const BigInteger<IntegerType> &b) const;

        const Vector &ModAddEq(const BigInteger<IntegerType> &b);

        Vector ModAddAtIndex(uint32_t i, const BigInteger<IntegerType> &b) const;

        const Vector &ModAddAtIndexEq(uint32_t i, const BigInteger<IntegerType> &b);

        Vector ModAdd(const Vector &b) const;

        const Vector &ModAddEq(const Vector &b);

        Vector ModSub(const BigInteger<IntegerType> &b) const;

        const Vector &ModSubEq(const BigInteger<IntegerType> &b);

        Vector ModSub(const Vector &b) const;

        const Vector &ModSubEq(const Vector &b);

        Vector ModMul(const BigInteger<IntegerType> &b) const;

        const Vector &ModMulEq(const BigInteger<IntegerType> &b);

        Vector ModMul(const Vector &b) const;

        const Vector &ModMulEq(const Vector &b);

        Vector ModExp(const BigInteger<IntegerType> &b) const;

        const Vector &ModExpEq(const BigInteger<IntegerType> &b);

        Vector ModInverse() const;

        const Vector &ModInverseEq();

        Vector Multiply(const Vector &b) const;

        const Vector MultiplyEq(const Vector &b);

        Vector MultiplyAndRound(const BigInteger<IntegerType> &p, const BigInteger<IntegerType> &q) const;

        const Vector &MultiplyAndRoundEq(const BigInteger<IntegerType> &p, const BigInteger<IntegerType> &q);

        Vector DivideAndRound(const BigInteger<IntegerType> &q) const;

        const Vector &DivideAndRoundEq(const BigInteger<IntegerType> &q);

        /**
         * Digit vector at a specific index for all entries for a given number base.
         * Example: for vector (83, 1, 45), index 3 and base 4 we have:
         *
         *                           index:1,2,3,4
         * |83|                           |3,0,1,1|                 |1|
         * |1 | --base 4 decomposition--> |1,0,0,0| --at index 3--> |0|
         * |45|                           |1,3,2,0|                 |2|
         *
         * The return vector is (1,0,2)
         *
         * @param index is the index to return the digit from in all entries.
         * @param base is the base to use for the operation.
         * @return is the digit at a specific index for all entries for a given number
         * base
         */
        Vector<IntegerType> GetDigitAtIndexForBase(uint32_t index, uint32_t base) const;

        friend std::ostream &operator<<(std::ostream &os, const Vector &vec) {
            auto len = vec.data.size();
            os << "[";
            for (size_t i = 0; i < len; ++i) {
                os << vec.data[i] << ((i == (len - 1)) ? "]" : " ");
            }
            os << " modulus: " << vec.modulus.GetValue();
            return os;
        }

    private:
        std::vector<BigInteger<IntegerType>> data;
        Modulus<IntegerType> modulus;
        State state;

        bool IndexCheck(size_t index) const {
            return index < this->data.size();
        }

        void Mod();
    };

}// namespace zhejiangfhe

#endif//ZJ_FHE_LIB_VECTOR_H
