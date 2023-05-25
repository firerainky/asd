//
// Created by 赵启明 on 2023/4/27.
//

#include "big_integer.h"

namespace zhejiangfhe {

    template<typename NativeInt>
    uint8_t BigInteger<NativeInt>::addWithCarry(NativeInt operand1, NativeInt operand2, uint8_t carry, NativeInt *result) {
        operand1 += operand2;
        *result = operand1 + carry;
        return (operand1 < operand2) || (~operand1 < carry);
    }

    template<typename NativeInt>
    uint8_t BigInteger<NativeInt>::subWithBorrow(NativeInt operand1, NativeInt operand2, uint8_t borrow, NativeInt *result) {
        auto diff = operand1 - operand2;
        *result = diff - (borrow != 0);
        return (diff > operand1) || (diff < borrow);
    }

    template<typename NativeInt>
    BigInteger<NativeInt> BigInteger<NativeInt>::AddWithSameSign(const BigInteger<NativeInt> &num, bool sign) const {
        std::vector<NativeInt> resultVectors;

        uint8_t carry = 0;
        NativeInt currentLimb;
        int i = 0;
        while (i < value.size() && i < num.value.size()) {
            carry = addWithCarry(value[i], num.value[i], carry, &currentLimb);
            resultVectors.push_back(currentLimb);
            ++i;
        }
        while (i < value.size()) {
            carry = addWithCarry(value[i], 0, carry, &currentLimb);
            resultVectors.push_back(currentLimb);
            ++i;
        }
        while (i < num.value.size()) {
            carry = addWithCarry(0, num.value[i], carry, &currentLimb);
            resultVectors.push_back(currentLimb);
            ++i;
        }

        return BigInteger(resultVectors, sign);
    }

    template<typename NativeInt>
    BigInteger<NativeInt> BigInteger<NativeInt>::SubWithSameSign(const BigInteger<NativeInt> &num, bool sign) const {

        std::vector<NativeInt> resultVectors;

        uint8_t borrow = 0;
        NativeInt currentLimb;

        for (int i = 0; i < value.size(); ++i) {
            if (i >= num.value.size()) {
                borrow = subWithBorrow(value[i], 0, borrow, &currentLimb);
                resultVectors.push_back(currentLimb);
            } else {
                borrow = subWithBorrow(value[i], num.value[i], borrow, &currentLimb);
                resultVectors.push_back(currentLimb);
            }
        }


        return BigInteger(resultVectors, sign);
    }

    template<typename NativeInt>
    BigInteger<NativeInt> BigInteger<NativeInt>::Add(const BigInteger<NativeInt> &num) const {
        int absoluteCompare = AbsoluteCompare(num);
        if (sign == false && num.sign == true) {
            if (AbsoluteCompare(num) == 0) {
                return BigInteger<NativeInt>();
            } else if (absoluteCompare > 0) {
                return SubWithSameSign(num);
            } else {
                return num.SubWithSameSign(*this, true);
            }
        }
        if (sign == true && num.sign == false) {
            if (AbsoluteCompare(num) == 0) {
                return BigInteger<NativeInt>();
            } else if (absoluteCompare > 0) {
                return SubWithSameSign(num, sign);
            } else {
                return num.SubWithSameSign(*this);
            }
        }
        return AddWithSameSign(num);
    }

    template<typename NativeInt>
    const BigInteger<NativeInt> &BigInteger<NativeInt>::AddEq(const BigInteger<NativeInt> &num) {
        int absoluteCompare = AbsoluteCompare(num);
        if (sign == false && num.sign == true) {
            if (AbsoluteCompare(num) == 0) {
                value.clear();
                value.push_back(0);
            } else if (absoluteCompare > 0) {
                AssignObj(SubWithSameSign(num));
            } else {
                AssignObj(num.SubWithSameSign(*this, true));
            }
        } else if (sign == true && num.sign == false) {
            if (AbsoluteCompare(num) == 0) {
                value.clear();
                value.push_back(0);
            } else if (absoluteCompare > 0) {
                AssignObj(SubWithSameSign(num, sign));
            } else {
                AssignObj(num.SubWithSameSign(*this));
            }
        } else {
            AssignObj(AddWithSameSign(num));
        }
        return *this;
    }

    template<typename NativeInt>
    BigInteger<NativeInt> BigInteger<NativeInt>::Sub(const BigInteger<NativeInt> &num) const {
        int absoluteCompare = AbsoluteCompare(num);
        if (sign == false && num.sign == true) {
            return AddWithSameSign(num, sign);
        }else if (sign == true && num.sign == false) {
            return AddWithSameSign(num, sign);
        }else{
            if (AbsoluteCompare(num) == 0) {
                return BigInteger<NativeInt>();
            } else if (absoluteCompare > 0) {
                return SubWithSameSign(num);
            } else {
                return num.SubWithSameSign(*this, true);
            }
        }
       
    }

    template<typename NativeInt>
    const BigInteger<NativeInt> &BigInteger<NativeInt>::SubEq(const BigInteger<NativeInt> &num) {
        int absoluteCompare = AbsoluteCompare(num);
        if (sign == false && num.sign == true) {
            AssignObj(AddWithSameSign(num, sign));
        } else if (sign == true && num.sign == false) {
            AssignObj(AddWithSameSign(num, sign));
        }else{
            if (AbsoluteCompare(num) == 0) {
                value.clear();
                value.push_back(0);
            } else if (absoluteCompare > 0) {
                AssignObj(SubWithSameSign(num));
            } else {
                AssignObj(num.SubWithSameSign(*this, true));
            }
        }
        return *this;
    }

    template<typename NativeInt>
    void BigInteger<NativeInt>::MultiplyWithKaratsuba(NativeInt operand1, NativeInt operand2, NativeInt *resultTwo) {
        NativeInt mask = 0x0;
        uint32_t halfLimbLength = m_limbBitLength / 2;
        for (int i = 0; i < halfLimbLength; ++i) {
            mask += (NativeInt) 1 << i;
        }

        NativeInt a = operand1 >> halfLimbLength;
        NativeInt b = operand1 & mask;
        NativeInt c = operand2 >> halfLimbLength;
        NativeInt d = operand2 & mask;

        NativeInt right = b * d;
        NativeInt middle;
        NativeInt left = a * c + (static_cast<NativeInt>(addWithCarry(a * d, b * c, 0, &middle)) << halfLimbLength);
        NativeInt temp_sum = (right >> halfLimbLength) + (middle & mask);

        resultTwo[1] = left + (middle >> halfLimbLength) + (temp_sum >> halfLimbLength);
        resultTwo[0] = (temp_sum << halfLimbLength) | (right & mask);
    }

    template<typename NativeInt>
    BigInteger<NativeInt> BigInteger<NativeInt>::Mul(const BigInteger<NativeInt> &b) const {

        std::vector<NativeInt> values;
        for (int i = 0; i < value.size(); ++i) {
            for (int j = 0; j < b.value.size(); ++j) {
                NativeInt temp_result[2];
                MultiplyWithKaratsuba(value[i], b.value[j], temp_result);
                uint8_t carry = 0;
                NativeInt sum;
                if (i + j + 1 > values.size()) {
                    values.push_back(temp_result[0]);
                } else {
                    carry = addWithCarry(temp_result[0], values[i + j], carry, &sum);
                    values[i + j] = sum;
                    temp_result[1] += carry;
                    carry = 0;
                }

                if (i + j + 2 > values.size()) {
                    values.push_back(temp_result[1]);
                } else {
                    carry = addWithCarry(temp_result[1], values[i + j + 1], carry, &sum);
                    values[i + j + 1] = sum;
                    uint8_t currentIdx = i + j + 2;
                    while (carry) {
                        if (currentIdx > values.size()) {
                            values.push_back(carry);
                        } else {
                            carry = addWithCarry(0, values[currentIdx], carry, &sum);
                            values[currentIdx] = sum;
                        }
                    }
                }
            }
        }

        return BigInteger(values, sign ^ b.sign);
    }

    template<typename NativeInt>
    const BigInteger<NativeInt> &BigInteger<NativeInt>::MulEq(const BigInteger<NativeInt> &b) {
        return *this = this->Mul(b);
    }


    template<typename NativeInt>
    std::pair<BigInteger<NativeInt>, BigInteger<NativeInt>> BigInteger<NativeInt>::DividedBy(const BigInteger<NativeInt> &denominator) const {

        bool finalSign = sign xor denominator.sign;

        BigInteger quotientIn(0, finalSign);
        BigInteger remainderIn(0, finalSign);
        Divide(quotientIn, remainderIn, *this, denominator);
        return std::make_pair(quotientIn, remainderIn);
    }

    template<typename NativeInt>
    bool BigInteger<NativeInt>::Divide(BigInteger &quotientIn, BigInteger &remainderIn, const BigInteger &uIn, const BigInteger &vIn) const {

        std::vector<NativeInt> &quotient = quotientIn.value;
        std::vector<NativeInt> &remainder = remainderIn.value;
        const std::vector<NativeInt> &u = (uIn.value);
        const std::vector<NativeInt> &v = (vIn.value);

        int m = u.size();
        int n = v.size();
        quotient.resize(m - n + 1);

        Dlimb_t qhat;   // Estimated quotient digit.
        Dlimb_t rhat;   // remainder
        Dlimb_t product;// Product of two digits.
        SDlimb_t t, k;
        int s, i, j;

        const Dlimb_t ffs = (Dlimb_t) m_MaxLimb;  // Number  (2**64)-1.
        const Dlimb_t b = (Dlimb_t) m_MaxLimb + 1;// Number base (2**64).


        if (m < n || n <= 0 || v[n - 1] == 0) {
            std::cout << "Error in Divide m, n, v[n-1] " << m << ", " << n << ", " << v[n - 1] << std::endl;
            return false;// Return if invalid param.
        }

        if (n == 1) {                               // Take care of
            k = 0;                                  // the case of a
            for (j = m - 1; j >= 0; j--) {          // single-digit
                quotient[j] = (k * b + u[j]) / v[0];// divisor here.
                k = (k * b + u[j]) - quotient[j] * v[0];
            }
            if (remainder.size() != 0) {
                remainder[0] = k;
            }
            remainderIn.NormalizeLimbs();
            return true;
        }

        s = nlz(v[n - 1]);

        std::vector<NativeInt> vn(n);
        for (i = n - 1; i > 0; i--) {
            vn[i] = (v[i] << s) | v[i - 1] >> (m_limbBitLength - s);
        }
        vn[0] = v[0] << s;

        std::vector<NativeInt> un(m + 1);

        un[m] = u[m - 1] >> (m_limbBitLength - s);
        for (i = m - 1; i > 0; i--) {
            un[i] = (u[i] << s) | (u[i - 1] >> (m_limbBitLength - s));
        }
        un[0] = u[0] << s;

        // Main loop
        for (j = m - n; j >= 0; j--) {
            // Compute estimate qhat of q[j].
            qhat = (un[j + n] * b + un[j + n - 1]) / vn[n - 1];
            rhat = (un[j + n] * b + un[j + n - 1]) - qhat * vn[n - 1];
            while (qhat >= b || qhat * vn[n - 2] > b * rhat + un[j + n - 2]) {
                qhat = qhat - 1;
                rhat = rhat + vn[n - 1];
                if (rhat >= b) {
                    break;
                }
            }

            // Multiply and subtract.
            k = 0;
            for (i = 0; i < n; i++) {
                product = qhat * vn[i];
                t = un[i + j] - k - (product & ffs);
                un[i + j] = t;
                k = (product >> m_limbBitLength) - (t >> m_limbBitLength);
            }
            t = un[j + n] - k;
            un[j + n] = t;


            // Store quotient digit.
            quotient[j] = qhat;
            if (t < 0) {                      // If we subtracted too
                quotient[j] = quotient[j] - 1;// much, add back.
                k = 0;
                for (i = 0; i < n; i++) {
                    t = (Dlimb_t) un[i + j] + vn[i] + k;
                    un[i + j] = t;
                    k = t >> m_limbBitLength;
                }
                un[j + n] = un[j + n] + k;
            }
        }

        // store remainder
        remainder.resize(n);
        for (i = 0; i < n - 1; i++) {
            remainder[i] = (un[i] >> s) | un[i + 1] << (m_limbBitLength - s);
        }
        remainder[n - 1] = un[n - 1] >> s;
        remainderIn.NormalizeLimbs();
        return true;
    }

    template class zhejiangfhe::BigInteger<uint32_t>;
    template class zhejiangfhe::BigInteger<uint64_t>;
}// namespace zhejiangfhe