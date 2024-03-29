//
// Created by 赵启明 on 2023/6/26.
//

#include "numth_util.h"
#include <cstdint>
#include <iosfwd>
#include <set>
#include <vector>

namespace zhejiangfhe {
    namespace util {

        //        template <typename IntType>
        //        void PrimeFactorize(IntType n, std::set<IntType>& primeFactors) {
        //
        //            if (n == IntType(0) || n == IntType(1))
        //                return;
        //            if (MillerRabinPrimalityTest(n)) {
        //                primeFactors.insert(n);
        //                return;
        //            }
        //
        //            IntType divisor(PollardRhoFactorization(n));
        //
        //            PrimeFactorize(divisor, primeFactors);
        //
        //            n /= divisor;
        //
        //            PrimeFactorize(n, primeFactors);
        //        }
        //
        //        template <typename IntType>
        //        static IntType RNG(const IntType& modulus) {
        //            // static parameters for the 32-bit unsigned integers used for multiprecision
        //            // random number generation
        //            static const uint32_t chunk_min   = 0;
        //            static const uint32_t chunk_width = std::numeric_limits<uint32_t>::digits;
        //            static const uint32_t chunk_max   = std::numeric_limits<uint32_t>::max();
        //
        //            static std::uniform_int_distribution<uint32_t> distribution(chunk_min, chunk_max);
        //
        //            // Update values that depend on modulus.
        //            uint32_t modulusWidth = modulus.GetMSB();
        //            // Get the number of chunks in the modulus
        //            // 1 is subtracted to make sure the last chunk is fully used by the modulus
        //            uint32_t chunksPerValue = (modulusWidth - 1) / chunk_width;
        //
        //            // result is initialized to 0
        //            IntType result;
        //
        //            // temp is used for intermediate multiprecision computations
        //            IntType temp;
        //
        //            // stores current random number generated by built-in C++ 11 uniform generator
        //            // (used for 32-bit unsigned integers)
        //            uint32_t value;
        //
        //            do {
        //                result = 0;
        //                // Generate random uint32_t "limbs" of the BigInteger
        //                for (uint32_t i = 0; i < chunksPerValue; i++) {
        //                    // Generate an unsigned long integer
        //                    value = distribution(PseudoRandomNumberGenerator::GetPRNG());
        //                    // converts value into IntType
        //                    temp = value;
        //                    // Move it to the appropriate chunk of the big integer
        //                    temp <<= i * chunk_width;
        //                    // Add it to the current big integer storing the result
        //                    result += temp;
        //                }
        //
        //                // work with the remainder - after all 32-bit chunks were processed
        //                temp = modulus >> chunksPerValue * chunk_width;
        //
        //                // Generate a uniform number for the remainder
        //                // If not 1, i.e., the modulus is either 1 or a power of 2*CHUNK_WIDTH
        //                if (temp.GetMSB() != 1) {
        //                    uint32_t bound = temp.ConvertToInt();
        //
        //                    // default generator for the most significant chunk of the multiprecision
        //                    // number
        //                    std::uniform_int_distribution<uint32_t> distribution2 =
        //                            std::uniform_int_distribution<uint32_t>(chunk_min, bound);
        //
        //                    value = distribution2(PseudoRandomNumberGenerator::GetPRNG());
        //                    // converts value into IntType
        //                    temp = value;
        //                    // Move it to the appropriate chunk of the big integer
        //                    temp <<= chunksPerValue * chunk_width;
        //                    // Add it to the current big integer storing the result
        //                    result += temp;
        //                }
        //            } while (result > modulus);  // deals with the rare scenario when the bits in
        //                                       // the most significant chunk are the same
        //            // and the bits in the following chunk of the result are larger than in the
        //            // modulus
        //            return result;
        //        }
        //
        //
        //        template <typename IntType>
        //        bool MillerRabinPrimalityTest(const IntType& p, const uint32_t niter) {
        //
        //            if (p < IntType(2) || ((p != IntType(2)) && (p.Mod(2) == IntType(0))))
        //                return false;
        //            if (p == IntType(2) || p == IntType(3) || p == IntType(5))
        //                return true;
        //
        //            IntType d = p - IntType(1);
        //            uint32_t s   = 0;
        //            while (d.Mod(2) == IntType(0)) {
        //                d.DividedByEq(2);
        //                s++;
        //            }
        //            bool composite = true;
        //            for (uint32_t i = 0; i < niter; i++) {
        //
        //                IntType a = RNG(p - IntType(3)).ModAdd(2, p);
        //
        //                composite = (WitnessFunction(a, d, s, p));
        //                if (composite)
        //                    break;
        //            }
        //            return (!composite);
        //        }
        //
        //
        //        template <typename IntType>
        //        const IntType PollardRhoFactorization(const IntType& n) {
        //            IntType divisor(1);
        //
        //            IntType c(RNG(n));
        //            IntType x(RNG(n));
        //            IntType xx(x);
        //
        //            // check divisibility by 2
        //            if (n.Mod(2) == IntType(0))
        //                return IntType(2);
        //
        //            // Precompute the Barrett mu parameter
        //            IntType mu = n.ComputeMu();
        //
        //            do {
        //                x  = x.ModMul(x, n, mu).ModAdd(c, n, mu);
        //                xx = xx.ModMul(xx, n, mu).ModAdd(c, n, mu);
        //                xx = xx.ModMul(xx, n, mu).ModAdd(c, n, mu);
        //
        //                divisor = GreatestCommonDivisor((x > xx) ? x - xx : xx - x, n);
        //
        //            } while (divisor == IntType(1));
        //
        //            return divisor;
        //        }
        //
        //
        //        template <typename IntType>
        //        static bool WitnessFunction(const IntType& a, const IntType& d, uint32_t s, const IntType& p) {
        //            IntType mod = a.ModExp(d, p);
        //            bool prevMod = false;
        //            for (uint32_t i = 1; i < s + 1; i++) {
        //                if (mod != IntType(1) && mod != p - IntType(1)) {
        //                    prevMod = true;
        //                }
        //                else {
        //                    prevMod = false;
        //                }
        //                mod.ModMulFastEq(mod, p);
        //                if (mod == IntType(1) && prevMod) {
        //                    return true;
        //                }
        //            }
        //            return (mod != IntType(1));
        //        }
        //
        //
        //
        //        uint64_t GetTotient(uint64_t n) {
        //            std::set<uint64_t> factors;
        //            uint64_t enn(n);
        //            PrimeFactorize(enn, factors);
        //
        //            uint64_t primeProd(1);
        //            uint64_t numerator(1);
        //            for (auto& r : factors) {
        //                numerator = numerator * (r - 1);
        //                primeProd = primeProd * r;
        //            }
        //
        //            primeProd = (enn / primeProd) * numerator;
        //            return primeProd;
        //        }

    }
}// namespace zhejiangfhe