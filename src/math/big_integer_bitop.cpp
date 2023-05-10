//
// Created by 赵启明 on 2023/5/9.
//

#include "big_integer.h"

namespace zhejiangfhe {

    template<typename NativeInt>
    BigInteger<NativeInt> BigInteger<NativeInt>::And(const BigInteger<NativeInt> another) const {
        std::vector<NativeInt> vals;
        for (auto i = 0; i < value.size() && i < another.value.size(); ++i) {
            vals.push_back(value[i] & another.value[i]);
        }

        return BigInteger(vals, !(!sign & !another.sign));
    }

    template<typename NativeInt>
    const BigInteger<NativeInt> &BigInteger<NativeInt>::AndEq(const BigInteger<NativeInt> another) {
        return *this = this->And(another);
    }

    template<typename NativeInt>
    BigInteger<NativeInt> BigInteger<NativeInt>::Or(const BigInteger<NativeInt> another) const {
        std::vector<NativeInt> vals;
        for (auto i = 0; (i < value.size() || i < another.value.size()); ++i) {
            if (i >= value.size()) {
                vals.push_back(another.value[i]);
            } else if (i >= another.value.size()) {
                vals.push_back(value[i]);
            } else {
                vals.push_back(value[i] | another.value[i]);
            }
        }

        return BigInteger(vals, sign | another.sign);
    }

    template<typename NativeInt>
    const BigInteger<NativeInt> &BigInteger<NativeInt>::OrEq(const BigInteger<NativeInt> another) {
        return *this = this->Or(another);
    }

    template<typename NativeInt>
    BigInteger<NativeInt> BigInteger<NativeInt>::Not() const {
        std::vector<NativeInt> vals;
        for (auto val: value) {
            vals.push_back(~val);
        }
        return BigInteger(vals, sign);
    }

    template<typename NativeInt>
    const BigInteger<NativeInt> &BigInteger<NativeInt>::NotEq() {
        return *this = this->Not();
    }

    template<typename NativeInt>
    BigInteger<NativeInt> BigInteger<NativeInt>::Xor(const BigInteger<NativeInt> another) const {
        std::vector<NativeInt> vals;
        for (auto i = 0; (i < value.size() || i < another.value.size()); ++i) {
            if (i >= value.size()) {
                vals.push_back(another.value[i]);
            } else if (i >= another.value.size()) {
                vals.push_back(value[i]);
            } else {
                vals.push_back(value[i] ^ another.value[i]);
            }
        }

        return BigInteger(vals, sign ^ another.sign);
    }

    template<typename NativeInt>
    const BigInteger<NativeInt> &BigInteger<NativeInt>::XorEq(const BigInteger<NativeInt> another) {
        return *this = this->Xor(another);
    }

    template<typename NativeInt>
    BigInteger<NativeInt> BigInteger<NativeInt>::Negate() const {
        std::vector<NativeInt> vals;
        auto carry = 0;
        for (auto val: value) {
            NativeInt temp_val;
            carry = addWithCarry(~val, 1, carry, &temp_val);
            vals.push_back(temp_val);
        }
        if (carry) {
            vals.push_back(carry);
        }
        return BigInteger(vals, sign);
    }

    template<typename NativeInt>
    const BigInteger<NativeInt> &BigInteger<NativeInt>::NegateEq() {
        return *this = this->Negate();
    }

    template<typename NativeInt>
    BigInteger<NativeInt> BigInteger<NativeInt>::LeftShift(uint16_t shift) const {
        if (this->m_MSB == 0) {
            return BigInteger(0);
        }
        BigInteger ans(*this);
        // compute the number of whole limb shifts
        uint32_t shiftByLimb = shift >> m_log2LimbBitLength;
        // ans.value.reserve(shiftByLimb+this->value.size());
        // compute the remaining number of bits to shift
        NativeInt remainingShift = (shift & (m_limbBitLength - 1));
        // first shift by the # remainingShift bits
        if (remainingShift != 0) {
            size_t i;
            for (i = ceilIntByUInt(m_MSB) - 1; i > 0; i--) {
                ans.value[i] = (ans.value[i] << remainingShift) | ans.value[i - 1] >> (m_limbBitLength - remainingShift);
            }
            ans.value[0] = ans.value[0] << remainingShift;
        }
        if (shiftByLimb != 0) {
            uint32_t currentSize = ans.value.size();
            ans.value.resize(currentSize + shiftByLimb);// allocate more storage
            for (int i = currentSize - 1; i >= 0; i--) {// shift limbs required # of indicies
                ans.value[i + shiftByLimb] = ans.value[i];
            }
            for (int i = shiftByLimb - 1; i >= 0; i--) {
                ans.value[i] = 0;
            }
        }
        ans.m_MSB += remainingShift + shiftByLimb * m_limbBitLength;
        return ans;
    }

    template<typename NativeInt>
    BigInteger<NativeInt> BigInteger<NativeInt>::RightShift(uint16_t shift) const {
        if (this->m_MSB == 0 || this->m_MSB <= shift) {
            return BigInteger(0);
        }

        BigInteger ans(*this);
        uint16_t shiftByLimb = shift >> m_log2LimbBitLength;
        NativeInt remainingShift = (shift & (m_limbBitLength - 1));
        NativeInt negativeShift = m_limbBitLength - remainingShift;
        if (shiftByLimb != 0) {
            for (auto i = shiftByLimb; i < ans.value.size(); ++i) {
                ans.value[i - shiftByLimb] = ans.value[i];
            }
            for (uint32_t i = 0; i < shiftByLimb; ++i) {
                ans.value.pop_back();
            }
        }

        // remainderShift bit shifts
        if (remainingShift != 0) {
            for (uint32_t i = 0; i < ans.value.size() - 1; i++) {
                ans.value[i] = (ans.value[i] >> remainingShift) | ans.value[i + 1] << negativeShift;
            }
            ans.value[ans.value.size() - 1] = ans.value[ans.value.size() - 1] >> remainingShift;
        }
        ans.RefreshMSB();
        return ans;
    }


    template class zhejiangfhe::BigInteger<uint32_t>;
    template class zhejiangfhe::BigInteger<uint64_t>;

}// namespace zhejiangfhe
