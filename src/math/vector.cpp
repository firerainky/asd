//
// Created by 赵启明 on 2023/5/30.
//

#include "vector.h"
#include "big_integer.h"
#include "big_integer_modop.h"
#include "modulus.h"

namespace zhejiangfhe {


    template<typename IntegerType>
    Vector<IntegerType>::Vector(const uint32_t length) {
        this->data.resize(length);
        for (uint32_t i = 0; i < length; i++) {
            this->data[i] = 0;
        }
        this->modulus = Modulus<IntegerType>("");
        this->state = GARBAGE;
    }

    template<typename IntegerType>
    Vector<IntegerType>::Vector(const uint32_t length, const Modulus<IntegerType> &modulus) {
        this->data.resize(length);
        for (uint32_t i = 0; i < length; i++) {
            this->data[i] = 0;
        }
        this->modulus = modulus;
        this->state = INITIALIZED;
    }

    template<typename IntegerType>
    Vector<IntegerType>::Vector(const uint32_t length, const IntegerType &modulus) {
        Modulus<IntegerType> newModulus = Modulus(modulus);
        this->data.resize(length);
        for (uint32_t i = 0; i < length; i++) {
            this->data[i] = 0;
        }
        this->modulus = newModulus;
        this->Mod();
        this->state = INITIALIZED;
    }

    template<typename IntegerType>
    Vector<IntegerType>::Vector(const uint32_t length, const std::string &modulus) {
        Modulus<IntegerType> newModulus(modulus);
        Vector(length, newModulus);
    }

    template<typename IntegerType>
    Vector<IntegerType>::Vector(const Vector &inVector) {
        size_t length = inVector.data.size();
        this->data.resize(length);
        for (size_t i = 0; i < length; i++) {
            this->data[i] = inVector.data[i];
        }
        this->modulus = inVector.modulus;
        this->state = INITIALIZED;
    }

    template<typename IntegerType>
    Vector<IntegerType>::Vector(Vector &&inVector) {
        this->data = std::move(inVector.data);
        this->modulus = std::move(inVector.modulus);
        this->state = INITIALIZED;
    }

    template<typename IntegerType>
    Vector<IntegerType>::Vector(const uint32_t length, const IntegerType &modulus,
                                std::initializer_list<std::string> rhs) {
        this->data.resize(length);
        this->modulus = modulus;
        uint32_t len = rhs.size();
        for (uint32_t i = 0; i < length; i++) {
            if (i < len) {
                BigInteger<IntegerType> val = BigInteger<IntegerType>(*(rhs.begin() + i));
                this->data[i] = util::Mod(val, this->modulus);
            } else {
                this->data[i] = 0;
            }
        }
        this->state = INITIALIZED;
    }

    template<typename IntegerType>
    Vector<IntegerType>::Vector(const uint32_t length, const IntegerType &modulus, std::initializer_list<uint64_t> rhs) {
        this->data.resize(length);
        this->modulus = modulus;
        uint32_t len = rhs.size();
        for (uint32_t i = 0; i < length; i++) {
            if (i < len) {
                BigInteger<IntegerType> val = BigInteger<IntegerType>(*(rhs.begin() + i));
                this->data[i] = util::Mod(val, this->modulus);
            } else {
                this->data[i] = BigInteger<IntegerType>(0);
            }
        }
        this->state = INITIALIZED;
    }

    template<typename IntegerType>
    Vector<IntegerType>::Vector(const std::vector<std::string> &s, const Modulus<IntegerType> &modulus) {
        this->data.resize(s.size());
        this->modulus = modulus;
        for (uint32_t i = 0; i < s.size(); i++) {
            BigInteger<IntegerType> val = (BigInteger<IntegerType>) s[i];
            this->data[i] = util::Mod(val, this->modulus);
        }
        this->state = INITIALIZED;
    }

    template<typename IntegerType>
    Vector<IntegerType>::Vector(const std::vector<std::string> &s, const IntegerType &modulus) {
        this->data.resize(s.size());
        this->modulus = modulus;
        for (uint32_t i = 0; i < s.size(); i++) {
            BigInteger<IntegerType> val = (BigInteger<IntegerType>) s[i];
            this->data[i] = util::Mod(val, this->modulus);
        }
        this->state = INITIALIZED;
    }

    template<typename IntegerType>
    Vector<IntegerType>::Vector(const std::vector<std::string> &s, const std::string &modulus) {
        this->data.resize(s.size());
        this->modulus = modulus;
        for (uint32_t i = 0; i < s.size(); i++) {
            BigInteger<IntegerType> val = (BigInteger<IntegerType>) s[i];
            this->data[i] = util::Mod(val, this->modulus);
        }
        this->state = INITIALIZED;
    }

    template<typename IntegerType>
    Vector<IntegerType>::~Vector() {
        this->data.clear();
    }

    template<typename IntegerType>
    bool Vector<IntegerType>::operator==(const Vector &rhs) const {
        if (data.size() != rhs.data.size() || modulus.GetValue() != rhs.modulus.GetValue()) return false;
        for (uint32_t i = 0; i < data.size(); ++i) {
            if (data[i] != rhs.data[i]) return false;
        }
        return true;
    }

    template<typename IntegerType>
    bool Vector<IntegerType>::operator!=(const Vector &rhs) const {
        return !(*this == rhs);
    }

    template<class IntegerType>
    Vector<IntegerType> &Vector<IntegerType>::operator=(Vector<IntegerType> &rhs) {
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
            this->state = INITIALIZED;
        }
        return *this;
    }

    template<class IntegerType>
    Vector<IntegerType> &Vector<IntegerType>::operator=(Vector &&rhs) {
        if (this != &rhs) {
            this->data.swap(rhs.data);// swap the two vector contents,
            if (rhs.data.size() > 0) {
                rhs.data.clear();
            }
            this->modulus       = rhs.modulus;
            this->state = INITIALIZED;
        }
        return *this;
    }


    template<typename IntegerType>
    void Vector<IntegerType>::SetModulus(const Modulus<IntegerType>& value) {
        if (this->state == INITIALIZED) {
            ZJFHE_THROW(zhejiangfhe::ConfigException, "modulus already set");
        }
        this->modulus = value;
        this->Mod();
        this->state = INITIALIZED;
    }


    template <class IntegerType>
    void Vector<IntegerType>::SwitchModulus(const Modulus<IntegerType>& val) {
        BigInteger<IntegerType> newModulus = val.GetValue();
        BigInteger<IntegerType> oldModulus(this->modulus.GetValue());
        BigInteger<IntegerType> n;
        BigInteger<IntegerType> oldModulusByTwo(oldModulus >> 1);
        BigInteger<IntegerType> diff((oldModulus > newModulus) ? (oldModulus - newModulus) : (newModulus - oldModulus));
        for (uint32_t i = 0; i < this->data.size(); i++) {
            n = this->at(i);
            if (oldModulus < newModulus) {
                if (n > oldModulusByTwo) {
                    this->data[i] = util::ModAdd(n, diff, val);
                }
                else {
                    this->data[i] = util::Mod(n, val);
                }
            }
            else {
                if (n > oldModulusByTwo) {
                    this->at(i) = util::ModSub(n, diff, val);
                }
                else {
                    this->at(i) = util::Mod(n, val);
                }
            }
        }
        this->state = GARBAGE;
        this->SetModulus(val);
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