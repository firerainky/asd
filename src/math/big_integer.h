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

namespace zhejiangfhe {
    template<typename NativeInt>
    class BigInteger : public IntegerInterface<BigInteger<NativeInt>> {

    public:
        // CONSTRUCTORS
        BigInteger();

        BigInteger(const std::string &strValue) {
            AssignVal(strValue);
        }

        BigInteger<NativeInt> Add(const BigInteger<NativeInt> &b) {
            return BigInteger("0");
        }
        const BigInteger<NativeInt> &AddEq(const BigInteger<NativeInt> &b) {
            return BigInteger("0");
        }
        BigInteger<NativeInt> Sub(const BigInteger<NativeInt> &b) const {
            return BigInteger("0");
        }
        const BigInteger<NativeInt> &SubEq(const BigInteger<NativeInt> &b) {
            return BigInteger("0");
        }
        BigInteger<NativeInt> Mul(const BigInteger<NativeInt> &b) const {
            return BigInteger("0");
        }
        const BigInteger<NativeInt> &MulEq(const BigInteger<NativeInt> &b) {
            return BigInteger("0");
        }
        BigInteger<NativeInt> DividedBy(const BigInteger<NativeInt> &b) const {
            return BigInteger("0");
        }
        const BigInteger<NativeInt> &DividedByEq(const BigInteger<NativeInt> &b) {
            return BigInteger("0");
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
            // Todo: eliminate m_limbBitLength, make dynamic instead
            // strip off leading zeros from the input string
            v.erase(0, v.find_first_not_of('0'));
            // strip off leading spaces from the input string
            v.erase(0, v.find_first_not_of(' '));
            if (v.size() == 0) {
                // caustic case of input string being all zeros
                v = "0";// set to one zero
            }

            size_t arrSize = v.length();
            // todo smartpointer
            uint8_t *DecValue = new uint8_t[arrSize];// array of decimal values

            for (size_t i = 0; i < arrSize; i++)// store the string to decimal array
                DecValue[i] = (uint8_t) stoi(v.substr(i, 1));

            // clear the current value of m_value;
            value.clear();

            size_t zptr = 0;
            // index of highest non-zero number in decimal number
            // define  bit register array
            uint8_t *bitArr = new uint8_t[m_limbBitLength]();// todo smartpointer

            int cnt = m_limbBitLength - 1;
            // cnt is a pointer to the bit position in bitArr, when bitArr is compelete it
            // is ready to be transfered to Value
            while (zptr != arrSize) {
                bitArr[cnt] = DecValue[arrSize - 1] % 2;
                // start divide by 2 in the DecValue array
                for (size_t i = zptr; i < arrSize - 1; i++) {
                    DecValue[i + 1] = (DecValue[i] % 2) * 10 + DecValue[i + 1];
                    DecValue[i] >>= 1;
                }
                DecValue[arrSize - 1] >>= 1;
                // division ends here
                cnt--;
                if (cnt == -1) {// cnt = -1 indicates bitArr is ready for transfer
                    cnt = m_limbBitLength - 1;
                    value.push_back(UintInBinaryToDecimal(bitArr));
                }
                if (DecValue[zptr] == 0) {
                    zptr++; // division makes Most significant digit zero, hence we increment
                            // zptr to next value
                }
                if (zptr == arrSize && DecValue[arrSize - 1] == 0) {
                    value.push_back(UintInBinaryToDecimal(bitArr));// Value assignment
                }
            }

            delete[] bitArr;
            delete[] DecValue;// deallocate memory
        }


    private:
        std::vector<NativeInt> value;
        bool sign;
        static const uint32_t m_limbBitLength;
    };


    template<typename NativeInt>
    const uint32_t BigInteger<NativeInt>::m_limbBitLength = sizeof(NativeInt) * 8;

}// namespace zhejiangfhe


#endif//ZJ_FHE_LIB_BigInteger_H
