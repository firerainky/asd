//
// Created by 赵启明 on 2023/4/27.
//

#ifndef ZJ_FHE_LIB_BigInteger_H
#define ZJ_FHE_LIB_BigInteger_H

#include "../util/exception.h"
#include "integer_interface.h"
#include <cstdint>
#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

namespace zhejiangfhe {


    template<typename NativeInt>
    class BigInteger : public IntegerInterface<BigInteger<NativeInt>> {

    public:
        // CONSTRUCTORS
        BigInteger();

        BigInteger(const std::string &strValue) {
            AssignVal(strValue);
        }


        BigInteger(const std::vector<NativeInt> vals) {
            value = vals;
        }

        std::size_t length() const {
            return value.size();
        }
        uint8_t addWithCarry(NativeInt operand1, NativeInt operand2, uint8_t carry, NativeInt *result) const {
            operand1 += operand2;
            *result = operand1 + carry;
            return (operand1 < operand2) || (~operand1 < carry);
        }
        uint8_t subWithBorrow(NativeInt operand1, NativeInt operand2, uint8_t borrow, NativeInt *result) const {
            auto diff = operand1 - operand2;
            *result = diff - (borrow != 0);
            return (diff > operand1) || (diff < borrow);
        }


        BigInteger<NativeInt> Add(const BigInteger<NativeInt> &num) const{
            if(num.sign==true){
                return Sub(num);
            }
            std::vector<NativeInt> resultVectors;

            uint8_t carry = 0;
            NativeInt currentLimb;
            int i = 0;
            while (i < length() && i < num.length()) {
                carry = addWithCarry(value[i], num.value[i], carry, &currentLimb);
                resultVectors.push_back(currentLimb);
                ++i;
            }
            while (i < length()) {
                carry = addWithCarry(value[i], 0, carry, &currentLimb);
                resultVectors.push_back(currentLimb);
                ++i;
            }
            while (i < num.length()) {
                carry = addWithCarry(0, num.value[i], carry, &currentLimb);
                resultVectors.push_back(currentLimb);
                ++i;
            }

            return BigInteger(resultVectors);
        }
        const BigInteger<NativeInt> &AddEq(const BigInteger<NativeInt> &b) {
            return *this;
        }
        BigInteger<NativeInt> Sub(const BigInteger<NativeInt> &num) const {
            if(num.sign==true){
                return Add(num);
            }
            std::vector<NativeInt> resultVectors;

            uint8_t borrow = 0;
            NativeInt currentLimb;

            for (int i = 0; i < length(); ++i) {
                if (i >= num.length()) {
                    borrow = subWithBorrow(value[i], 0, borrow, &currentLimb);
                    resultVectors.push_back(currentLimb);
                } else {
                    borrow = subWithBorrow(value[i], num.value[i], borrow, &currentLimb);
                    resultVectors.push_back(currentLimb);
                }
            }

            return BigInteger(resultVectors);
        }
        const BigInteger<NativeInt> &SubEq(const BigInteger<NativeInt> &b) {
            return *this;
        }
        BigInteger<NativeInt> Mul(const BigInteger<NativeInt> &b) const {
            return BigInteger("0");
        }
        const BigInteger<NativeInt> &MulEq(const BigInteger<NativeInt> &b) {
            return *this;
        }
        BigInteger<NativeInt> DividedBy(const BigInteger<NativeInt> &b) const {
            return BigInteger("0");
        }
        const BigInteger<NativeInt> &DividedByEq(const BigInteger<NativeInt> &b) {
            return *this;
        }

        const std::string ConvertToString();

        NativeInt ConvertToLimb();

        NativeInt UintInBinaryToDecimal(uint8_t *a) {
            NativeInt Val = 0;
            NativeInt one = 1;
            for (int i = m_limbBitLength - 1; i >= 0; i--) {
                Val += one * *(a + i);
                one <<= 1;
                *(a + i) = 0;
            }
            return Val;
        }

    protected:
        void AssignVal(const std::string &str) {

            std::string v = str;
            if (str[0] == '-') {
                sign = true;
                v = str.substr(1);
            }
            v.erase(0, v.find_first_not_of('0'));
            v.erase(0, v.find_first_not_of(' '));
            if (v.empty()) {
                v = "0";// set to one zero
            }
            size_t arr_size = v.length();
            std::unique_ptr<u_int8_t[]> dec_arr = std::make_unique<u_int8_t[]>(arr_size);
            for (size_t i = 0; i < arr_size; i++)// store the string to decimal array
                dec_arr[i] = (uint8_t) stoi(v.substr(i, 1));

            // clear the current value of value;
            value.clear();

            size_t zero_ptr = 0;
            // index of highest non-zero number in decimal number
            // define  bit register array
            std::unique_ptr<u_int8_t[]> bit_arr = std::make_unique<u_int8_t[]>(m_limbBitLength);
            int cnt = m_limbBitLength - 1;
            // cnt is a pointer to the bit position in bit_arr, when bit_arr is complete it
            // is ready to be transfered to Value
            while (zero_ptr != arr_size) {
                bit_arr[cnt] = dec_arr[arr_size - 1] % 2;
                // start divide by 2 in the DecValue array
                for (size_t i = zero_ptr; i < arr_size - 1; i++) {
                    dec_arr[i + 1] = (dec_arr[i] % 2) * 10 + dec_arr[i + 1];
                    dec_arr[i] >>= 1;
                }
                dec_arr[arr_size - 1] >>= 1;
                // division ends here
                cnt--;
                if (cnt == -1) {// cnt = -1 indicates bit_arr is ready for transfer
                    cnt = m_limbBitLength - 1;
                    value.push_back(UintInBinaryToDecimal(bit_arr.get()));
                }
                if (dec_arr[zero_ptr] == 0) {
                    zero_ptr++;// division makes Most significant digit zero, hence we increment
                               // zero_ptr to next value
                }
                if (zero_ptr == arr_size && dec_arr[arr_size - 1] == 0) {
                    value.push_back(UintInBinaryToDecimal(bit_arr.get()));// Value assignment
                }
            }
        }


    private:
        std::vector<NativeInt> value;
        bool sign = false;
        static const uint32_t m_limbBitLength;
    };


    template<typename NativeInt>
    const uint32_t BigInteger<NativeInt>::m_limbBitLength = sizeof(NativeInt) * 8;

}// namespace zhejiangfhe


#endif//ZJ_FHE_LIB_BigInteger_H
