//
// Created by 赵启明 on 2023/5/30.
//

#include "vector.h"
#include "modulus.h"
#include "big_integer.h"
#include "big_integer_modop.h"

namespace zhejiangfhe {


    template <typename IntegerType>
    Vector<IntegerType>::Vector(const uint32_t length) {
        this->data.resize(length);
        for (uint32_t i = 0; i < length; i++) {
            this->data[i] = 0;
        }
        this->modulus = Modulus<IntegerType>("");
    }

    template <typename IntegerType>
    Vector<IntegerType>::Vector(const uint32_t length, const Modulus<IntegerType>& modulus) {
        this->data.resize(length);
        for (uint32_t i = 0; i < length; i++) {
            this->data[i] = 0;
        }
        this->modulus = modulus;
        this->Mod();
    }

    template <typename IntegerType>
    Vector<IntegerType>::Vector(const uint32_t length, const IntegerType& modulus) {
        Modulus<IntegerType> newModulus = Modulus(modulus);
        this->data.resize(length);
        for (uint32_t i = 0; i < length; i++) {
            this->data[i] = 0;
        }
        this->modulus = newModulus;
        this->Mod();
    }

    template <typename IntegerType>
    Vector<IntegerType>::Vector(const uint32_t length, const std::string& modulus) {
        Modulus<IntegerType> newModulus(modulus);
        Vector(length, newModulus);
    }


    template <typename IntegerType>
    Vector<IntegerType>::Vector(const Vector& inVector) {
        size_t length = inVector.data.size();
        this->data.resize(length);
        for (size_t i = 0; i < length; i++) {
            this->data[i] = inVector.data[i];
        }
        this->modulus = inVector.modulus;
    }

    template <typename IntegerType>
    Vector<IntegerType>::Vector(Vector&& inVector) {
        this->data = std::move(inVector.data);
        this->modulus = std::move(inVector.modulus);
    }


    template <typename IntegerType>
    Vector<IntegerType>::Vector(const uint32_t length, const IntegerType& modulus,
                                     std::initializer_list<std::string> rhs) {
        this->data.resize(length);
        this->modulus = modulus;
        uint32_t len       = rhs.size();
        for (uint32_t i = 0; i < length; i++) {
            if (i < len) {
                BigInteger<IntegerType> val = BigInteger<IntegerType>(*(rhs.begin() + i));
                this->data[i] = util::Mod(val, this->modulus);
            } else {
                this->data[i] = 0;
            }
        }
    }

    template <typename IntegerType>
    Vector<IntegerType>::Vector(const uint32_t length, const IntegerType& modulus, std::initializer_list<uint64_t> rhs) {
        this->data.resize(length);
        this->modulus = modulus;
        uint32_t len       = rhs.size();
        for (uint32_t i = 0; i < length; i++) {
            if (i < len) {
                BigInteger<IntegerType> val = BigInteger<IntegerType>(*(rhs.begin() + i));
                this->data[i] = util::Mod(val, this->modulus);
            } else {
                this->data[i] = BigInteger<IntegerType>(0);
            }
        }
    }

    template <typename IntegerType>
    Vector<IntegerType>::Vector(const std::vector<std::string>& s, const Modulus<IntegerType>& modulus) {
        this->data.resize(s.size());
        this->modulus       = modulus;
        for (uint32_t i = 0; i < s.size(); i++) {
            BigInteger<IntegerType> val = (BigInteger<IntegerType>)s[i];
            this->data[i] = util::Mod(val, this->modulus);
        }
    }

    template <typename IntegerType>
    Vector<IntegerType>::Vector(const std::vector<std::string>& s, const IntegerType& modulus) {
        this->data.resize(s.size());
        this->modulus       = modulus;
        for (uint32_t i = 0; i < s.size(); i++) {
            BigInteger<IntegerType> val = (BigInteger<IntegerType>)s[i];
            this->data[i] = util::Mod(val, this->modulus);
        }
    }

    template <typename IntegerType>
    Vector<IntegerType>::Vector(const std::vector<std::string>& s, const std::string& modulus) {
        this->data.resize(s.size());
        this->modulus       = modulus;
        for (uint32_t i = 0; i < s.size(); i++) {
            BigInteger<IntegerType> val = (BigInteger<IntegerType>)s[i];
            this->data[i] = util::Mod(val, this->modulus);
        }
    }

    template <typename IntegerType>
    Vector<IntegerType>::~Vector() {
        this->data.clear();
    }
    
    template <class IntegerType>
    Vector<IntegerType>& Vector<IntegerType>::operator=(Vector<IntegerType>& rhs) {
        if (this != &rhs) {
            if (this->data.size() == rhs.data.size()) {
                for (uint32_t i = 0; i < this->data.size(); i++) {
                    this->data[i] = rhs.data[i];
                }
            } else {
                this->data.resize(rhs.data.size());
                for (uint32_t i = 0; i < this->data.size(); i++) {
                    this->data[i] = rhs.data[i];
                }
            }
            this->modulus = rhs.modulus;
        }
        return *this;
    }

    template <class IntegerType>
    Vector<IntegerType>& Vector<IntegerType>::operator=(Vector&& rhs) {
        if (this != &rhs) {
            this->data.swap(rhs.data);  // swap the two vector contents,
            if (rhs.data.size() > 0) {
                rhs.data.clear();
            }
            this->modulus       = rhs.modulus;
        }
        return *this;
    }

    template <typename IntegerType>
    void Vector<IntegerType>::Mod() {
        for (uint32_t i = 0; i < this->data.size(); i++) {
            this->data[i] = util::Mod(this->data[i], this->modulus);
        }
    }



    template class zhejiangfhe::Vector<uint32_t>;
    template class zhejiangfhe::Vector<uint64_t>;
}// namespace zhejiangfhe