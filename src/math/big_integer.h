//
// Created by 赵启明 on 2023/4/27.
//

#ifndef ZJ_FHE_LIB_BigInteger_H
#define ZJ_FHE_LIB_BigInteger_H

#include "../util/exception.h"
#include "integer_interface.h"
#include <cstdint>
#include <iosfwd>
#include <string>
#include <vector>
#include <memory>

namespace zhejiangfhe {
    template<typename NativeInt>
    class BigInteger : public IntegerInterface<BigInteger<NativeInt>> {

    public:
        // CONSTRUCTORS
        BigInteger();

        BigInteger(const std::string &strValue) {
            AssignVal(strValue);
        }

        /**
         * @brief Compare the current BigInteger with another one
         * 
         * @param another is the BigInteger to be compared with. 
         * @return int -1 for strictly less than, 0 for equal to and 1 for strictly greater than conditions.
         */
        int Compare(const BigInteger<NativeInt> &another) const;

        BigInteger<NativeInt> Add(const BigInteger<NativeInt> &b) {
            return BigInteger("0");
        }
        const BigInteger<NativeInt> &AddEq(const BigInteger<NativeInt> &b) {
            return *this;
        }
        BigInteger<NativeInt> Sub(const BigInteger<NativeInt> &b) const {
            return BigInteger("0");
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

            // clear the current value of m_value;
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
                    zero_ptr++; // division makes Most significant digit zero, hence we increment
                            // zero_ptr to next value
                }
                if (zero_ptr == arr_size && dec_arr[arr_size - 1] == 0) {
                    NativeInt lastValue = UintInBinaryToDecimal(bit_arr.get());
                    m_MSB = value.size() * m_limbBitLength;
                    if (lastValue > 0) {
                        value.push_back(lastValue);// Value assignment
                        m_MSB += m_limbBitLength - cnt;
                    }
                }
            }
        }


    private:
        std::vector<NativeInt> value;
        bool sign = false;
        static const uint32_t m_limbBitLength;
        uint32_t m_MSB = 0;
    };


    template<typename NativeInt>
    const uint32_t BigInteger<NativeInt>::m_limbBitLength = sizeof(NativeInt) * 8;

}// namespace zhejiangfhe


#endif//ZJ_FHE_LIB_BigInteger_H
