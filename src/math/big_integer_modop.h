//
// Created by 赵启明 on 2023/5/22.
//

#ifndef ZJ_FHE_LIB_BIG_INTEGER_MODOP_H
#define ZJ_FHE_LIB_BIG_INTEGER_MODOP_H

#include "big_integer.h"
#include "modulus.h"

namespace zhejiangfhe {

    namespace util {

        template<typename NativeInt>
        BigInteger<NativeInt> Mod(const BigInteger<NativeInt> &operand, const Modulus<NativeInt> &modulus) {
            return operand % modulus.GetValue();
        }

        template<typename NativeInt>
        BigInteger<NativeInt> ModBarrett(const BigInteger<NativeInt> &operand, const Modulus<NativeInt> &modulus) {
            BigInteger<NativeInt> estimated = operand * (modulus.GetConstRatio());
            estimated >>= 128;
            estimated *= modulus.GetValue();
            BigInteger<NativeInt> ret = operand - estimated;
            return ret < modulus.GetValue() ? ret : ret - modulus.GetValue();
        }

        inline uint64_t ModBarrett64(uint64_t &operand, const Modulus<uint64_t> &modulus) {
            uint64_t modulusValue = modulus.GetValue().getValueOfIndex(0);
            uint64_t ration1 = modulus.GetConstRatio().getValueOfIndex(1);
            __int128 estimated = static_cast<__int128>(operand) * static_cast<__int128>(ration1);
            estimated >>= 64;

            estimated *= modulusValue;
            uint64_t ret = operand - estimated;
            return ret < modulusValue ? ret : ret - modulusValue;
        }

        template<typename NativeInt>
        BigInteger<NativeInt> ModAdd(const BigInteger<NativeInt> &operand1, const BigInteger<NativeInt> &operand2, const Modulus<NativeInt> &modulus) {
            BigInteger<NativeInt> a(operand1);
            BigInteger<NativeInt> b(operand2);
            if (a >= modulus.GetValue()) {
                a = Mod(a, modulus);
            }
            if (b >= modulus.GetValue()) {
                b = Mod(b, modulus);
            }
            a.AddEq(b);
            return Mod(a, modulus);
        }

        template<typename NativeInt>
        BigInteger<NativeInt> ModIncrement(const BigInteger<NativeInt> &operand, const Modulus<NativeInt> &modulus) {
            BigInteger<NativeInt> a("1");
            return ModAdd(operand, a, modulus);
        }


        template<typename NativeInt>
        BigInteger<NativeInt> ModSub(const BigInteger<NativeInt> &a, const BigInteger<NativeInt> &b, const Modulus<NativeInt> &modulus) {
            BigInteger<NativeInt> difference = a.Sub(b);
            BigInteger<NativeInt> modulusValue = modulus.GetValue();
            BigInteger<NativeInt> negativeModulus(modulusValue.GetValue(), true);
            if (difference == modulusValue) {
                return BigInteger<NativeInt>();
            } else if (difference < negativeModulus || difference > modulusValue) {
                return Mod(difference, modulus);
            } else {
                if (difference.GetSign()) {// negative
                    return modulusValue + difference;
                } else {
                    return difference;
                }
            }
        }

        template<typename NativeInt>
        BigInteger<NativeInt> ModMul(const BigInteger<NativeInt> &operand, const BigInteger<NativeInt> &another, const Modulus<NativeInt> &modulus) {
            BigInteger<NativeInt> mulRes = operand.Mul(another);
            return Mod(mulRes, modulus);
        }

        // template<typename NativeInt>
        // BigInteger<NativeInt> ModMul2(BigInteger<NativeInt> &operand, BigInteger<NativeInt> &another, BigInteger<NativeInt> &modulus) {
        //     size_t nSize = operand.GetValue().size();
        //     size_t bSize = another.GetValue().size();
        //     BigInteger<NativeInt> tmpans;
        //     ans.m_value.reserve(nSize + bSize);
        //     tmpans.m_value.reserve(nSize + bSize);

        //     for (size_t i = 0; i < bSize; ++i) {
        //         tmpans.m_value.clear();// make sure there are no limbs to start.
        //         Dlimb_t limbb(b.m_value[i]);
        //         Dlimb_t temp = 0;
        //         limb_t ofl = 0;
        //         usint ix = 0;
        //         while (ix < i) {
        //             tmpans.m_value.push_back(0);// equivalent of << shift
        //             ++ix;
        //         }

        //         for (auto itr: a.m_value) {
        //             temp = ((Dlimb_t) itr * (Dlimb_t) limbb) + ofl;
        //             tmpans.m_value.push_back((limb_t) temp);
        //             ofl = temp >> a.m_limbBitLength;
        //         }
        //         // check if there is any final overflow
        //         if (ofl) {
        //             tmpans.m_value.push_back(ofl);
        //         }
        //         tmpans.m_state = INITIALIZED;
        //         tmpans.SetMSB();
        //         ans += tmpans;
        //         ans = ans.Mod(modulus);
        //     }
        //     return ans;
        // }

        template<typename NativeInt>
        BigInteger<NativeInt> ModExp(const BigInteger<NativeInt> &operand, const BigInteger<NativeInt> &exponent, const Modulus<NativeInt> &modulus) {
            BigInteger<NativeInt> modulusValue = modulus.GetValue();
            BigInteger<NativeInt> mid = Mod(operand, modulus);
            BigInteger<NativeInt> product(1);
            BigInteger<NativeInt> exp(exponent);

            if (exponent == 0) {
                return 1;
            }

            if (operand == 1) {
                return operand;
            }
            if (exponent == 1) {
                return Mod(operand, modulus);
            }
            while (true) {
                if (exp.getValueOfIndex(0) % 2 == 1) {
                    product = product * mid;
                }
                if (product >= modulusValue) {
                    product = Mod(product, modulus);
                }
                exp = exp.RightShift(1);
                if (exp == 0) {
                    break;
                }
                mid = mid * mid;
                mid = Mod(mid, modulus);
            }
            return product;
        }
    }// namespace util
}// namespace zhejiangfhe
#endif//ZJ_FHE_LIB_BIG_INTEGER_MODOP_H
