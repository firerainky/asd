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
    Vector<IntegerType>::Vector(const uint32_t length,
                                const Modulus<IntegerType> &modulus) {
        this->data.resize(length);
        for (uint32_t i = 0; i < length; i++) {
            this->data[i] = 0;
        }
        this->modulus = modulus;
        this->state = INITIALIZED;
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
    Vector<IntegerType>::Vector(const uint32_t length,
                                const IntegerType &modulus,
                                std::initializer_list<std::string> rhs) {
        this->data.resize(length);
        this->modulus = modulus;
        uint32_t len = rhs.size();
        for (uint32_t i = 0; i < length; i++) {
            if (i < len) {
                BigInteger<IntegerType> val =
                        BigInteger<IntegerType>(*(rhs.begin() + i));
                this->data[i] = util::Mod(val, this->modulus);
            } else {
                this->data[i] = 0;
            }
        }
        this->state = INITIALIZED;
    }

    template<typename IntegerType>
    Vector<IntegerType>::Vector(const uint32_t length,
                                const IntegerType &modulus,
                                std::initializer_list<uint64_t> rhs) {
        this->data.resize(length);
        this->modulus = modulus;
        uint32_t len = rhs.size();
        for (uint32_t i = 0; i < length; i++) {
            if (i < len) {
                BigInteger<IntegerType> val =
                        BigInteger<IntegerType>(*(rhs.begin() + i));
                this->data[i] = util::Mod(val, this->modulus);
            } else {
                this->data[i] = BigInteger<IntegerType>(0);
            }
        }
        this->state = INITIALIZED;
    }

    template<typename IntegerType>
    Vector<IntegerType>::Vector(const std::vector<std::string> &s,
                                const Modulus<IntegerType> &modulus) {
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
        if (this == &rhs)
            return true;
        if (data.size() != rhs.data.size() ||
            modulus.GetValue() != rhs.modulus.GetValue())
            return false;
        for (uint32_t i = 0; i < data.size(); ++i) {
            if (data[i] != rhs.data[i])
                return false;
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
            this->modulus = rhs.modulus;
            this->state = INITIALIZED;
        }
        return *this;
    }

    template<class IntegerType>
    const Vector<IntegerType> &Vector<IntegerType>::operator=(
            std::initializer_list<std::string> rhs) {
        size_t len = rhs.size();
        if (this->data.size() < len) {
            this->data.resize(len);
        }
        for (uint32_t i = 0; i < this->data.size(); i++) {// this loops over each entry
            if (i < len) {
                this->data[i] = *(rhs.begin() + i);
            } else {
                this->data[i] = 0;
            }
        }
        if (this->state == INITIALIZED) {
            this->Mod();
        }
        return *this;
    }

    template<class IntegerType>
    const Vector<IntegerType> &Vector<IntegerType>::operator=(
            std::initializer_list<uint64_t> rhs) {
        size_t len = rhs.size();
        if (this->data.size() < len) {
            this->data.resize(len);
        }
        for (uint32_t i = 0; i < this->data.size(); i++) {// this loops over each entry
            if (i < len) {
                this->data[i] = *(rhs.begin() + i);
            } else {
                this->data[i] = 0;
            }
        }
        if (this->state == INITIALIZED) {
            this->Mod();
        }
        return *this;
    }

    template<typename IntegerType>
    void Vector<IntegerType>::SetModulus(const Modulus<IntegerType> &value) {
        if (this->state == INITIALIZED) {
            ZJFHE_THROW(zhejiangfhe::ConfigException, "modulus already set");
        }
        this->modulus = value;
        this->Mod();
        this->state = INITIALIZED;
    }


    template<class IntegerType>
    void Vector<IntegerType>::SwitchModulus(const Modulus<IntegerType> &val) {
        BigInteger<IntegerType> newModulus = val.GetValue();
        BigInteger<IntegerType> oldModulus(this->modulus.GetValue());
        BigInteger<IntegerType> n;
        BigInteger<IntegerType> oldModulusByTwo(oldModulus >> 1);
        BigInteger<IntegerType> diff((oldModulus > newModulus)
                                             ? (oldModulus - newModulus)
                                             : (newModulus - oldModulus));
        for (uint32_t i = 0; i < this->data.size(); i++) {
            n = this->at(i);
            if (oldModulus < newModulus) {
                if (n > oldModulusByTwo) {
                    this->data[i] = util::ModAdd(n, diff, val);
                } else {
                    this->data[i] = util::Mod(n, val);
                }
            } else {
                if (n > oldModulusByTwo) {
                    this->at(i) = util::ModSub(n, diff, val);
                } else {
                    this->at(i) = util::Mod(n, val);
                }
            }
        }
        this->state = GARBAGE;
        this->SetModulus(val);
    }

    template<class IntegerType>
    const Modulus<IntegerType> &Vector<IntegerType>::GetModulus() const {
        return this->modulus;
    }

    template<class IntegerType>
    Vector<IntegerType> Vector<IntegerType>::ModAdd(
            const BigInteger<IntegerType> &b) const {
        Vector<IntegerType> ans(*this);
        for (uint32_t i = 0; i < this->data.size(); i++) {
            ans.data[i] = util::ModAdd(ans.data[i], b, ans.modulus);
        }
        return ans;
    }

    template<class IntegerType>
    const Vector<IntegerType> &Vector<IntegerType>::ModAddEq(
            const BigInteger<IntegerType> &b) {
        return *this = ModAdd(b);
    }

    template<class IntegerType>
    Vector<IntegerType> Vector<IntegerType>::ModAddAtIndex(
            uint32_t i,
            const BigInteger<IntegerType> &b) const {
        if (!this->IndexCheck(i)) {
            ZJFHE_THROW(MathException, "Vector index out of range");
        }
        Vector<IntegerType> ans(*this);
        ans.data[i] = util::ModAdd(ans.data[i], b, ans.modulus);
        return ans;
    }

    template<class IntegerType>
    const Vector<IntegerType> &Vector<IntegerType>::ModAddAtIndexEq(
            uint32_t i,
            const BigInteger<IntegerType> &b) {
        return *this = ModAddAtIndex(i, b);
    }

    template<class IntegerType>
    Vector<IntegerType> Vector<IntegerType>::ModAdd(
            const Vector<IntegerType> &b) const {
        if (this->modulus.GetValue() != b.modulus.GetValue()) {
            ZJFHE_THROW(MathException,
                        "Vector adding vectors of different modulus");
        } else if (this->data.size() != b.data.size()) {
            ZJFHE_THROW(MathException,
                        "Vector adding vectors of different lengths");
        }

        Vector<IntegerType> ans(*this);
        for (uint32_t i = 0; i < this->data.size(); i++) {
            ans.data[i] = util::ModAdd(ans.data[i], b.data[i], ans.modulus);
        }
        return ans;
    }

    template<class IntegerType>
    const Vector<IntegerType> &Vector<IntegerType>::ModAddEq(
            const Vector<IntegerType> &b) {
        return *this = ModAdd(b);
    }


    template<class IntegerType>
    Vector<IntegerType> Vector<IntegerType>::ModSub(
            const BigInteger<IntegerType> &b) const {
        Vector<IntegerType> ans(*this);
        for (uint32_t i = 0; i < this->data.size(); i++) {
            ans.data[i] = util::ModSub(ans.data[i], b, ans.modulus);
        }
        return ans;
    }

    template<class IntegerType>
    const Vector<IntegerType> &Vector<IntegerType>::ModSubEq(
            const BigInteger<IntegerType> &b) {
        return *this = ModSub(b);
    }

    template<class IntegerType>
    Vector<IntegerType> Vector<IntegerType>::ModSub(
            const Vector<IntegerType> &b) const {
        if (this->modulus.GetValue() != b.modulus.GetValue()) {
            ZJFHE_THROW(MathException,
                        "Vector subtract vectors of different modulus");
        } else if (this->data.size() != b.data.size()) {
            ZJFHE_THROW(MathException,
                        "Vector subtract vectors of different lengths");
        }

        Vector<IntegerType> ans(*this);
        for (uint32_t i = 0; i < this->data.size(); i++) {
            ans.data[i] = util::ModSub(ans.data[i], b.data[i], ans.modulus);
        }
        return ans;
    }

    template<class IntegerType>
    const Vector<IntegerType> &Vector<IntegerType>::ModSubEq(
            const Vector<IntegerType> &b) {
        return *this = ModSub(b);
    }


    template<class IntegerType>
    Vector<IntegerType> Vector<IntegerType>::ModMul(
            const BigInteger<IntegerType> &b) const {
        Vector<IntegerType> ans(*this);
        for (uint32_t i = 0; i < this->data.size(); i++) {
            ans.data[i] = util::ModMul(ans.data[i], b, ans.modulus);
        }
        return ans;
    }

    template<class IntegerType>
    const Vector<IntegerType> &Vector<IntegerType>::ModMulEq(
            const BigInteger<IntegerType> &b) {
        return *this = ModMul(b);
    }

    template<class IntegerType>
    Vector<IntegerType> Vector<IntegerType>::ModMul(
            const Vector<IntegerType> &b) const {
        if (this->modulus.GetValue() != b.modulus.GetValue()) {
            ZJFHE_THROW(MathException,
                        "Vector multiply vectors of different modulus");
        } else if (this->data.size() != b.data.size()) {
            ZJFHE_THROW(MathException,
                        "Vector multiply vectors of different lengths");
        }

        Vector<IntegerType> ans(*this);
        for (uint32_t i = 0; i < this->data.size(); i++) {
            ans.data[i] = util::ModMul(ans.data[i], b.data[i], ans.modulus);
        }
        return ans;
    }

    template<class IntegerType>
    const Vector<IntegerType> &Vector<IntegerType>::ModMulEq(
            const Vector<IntegerType> &b) {
        return *this = ModMul(b);
    }

    template<typename IntegerType>
    void Vector<IntegerType>::Mod() {
        for (uint32_t i = 0; i < this->data.size(); i++) {
            this->data[i] = util::Mod(this->data[i], this->modulus);
        }
    }

    template<class IntegerType>
    Vector<IntegerType> Vector<IntegerType>::ModExp(
            const BigInteger<IntegerType> &b) const {
        Vector<IntegerType> ans(*this);
        for (uint32_t i = 0; i < this->data.size(); i++) {
            ans.data[i] = util::ModExp(ans.data[i], b, this->modulus);
        }
        return ans;
    }

    template<class IntegerType>
    const Vector<IntegerType> &Vector<IntegerType>::ModExpEq(
            const BigInteger<IntegerType> &b) {
        return *this = ModExp(b);
    }

    template<class IntegerType>
    Vector<IntegerType> Vector<IntegerType>::ModInverse() const {
        Vector<IntegerType> ans(*this);
        for (uint32_t i = 0; i < this->data.size(); i++) {
            ans.data[i] = util::ModInverse(ans.data[i], this->modulus);
        }
        return ans;
    }

    template<class IntegerType>
    const Vector<IntegerType> &Vector<IntegerType>::ModInverseEq() {
        return *this = ModInverse();
    }

    template<typename IntegerType>
    Vector<IntegerType> Vector<IntegerType>::GetDigitAtIndexForBase(
            uint32_t index,
            uint32_t base) const {
        Vector ans(*this);
        for (uint32_t i = 0; i < data.size(); ++i) {
            ans.data[i] =
                    IntegerType(ans.data[i].GetDigitAtIndexForBase(index, base));
        }
        return ans;
    }

    template class zhejiangfhe::Vector<limbtype>;
}// namespace zhejiangfhe