/*
 * Created by Kanyan Zheng on Mon Jul 03 2023
 *
 * Copyright (c) 2023 Zhejiang Lab. All rights reserved.
 */

#include "nbtheory.h"
#include "big_integer.h"
#include "big_integer_modop.h"
#include <random>

namespace zhejiangfhe {

    /**
     * @description: Generate a random big integer between 0 and uper bound.
     * @param upperBound The upper bound of the generated random integer.
     * @return Randomly generated big integer.
     * TODO: Currently using system RNG generator which can not generate big integer, need better one.
     */
    template<typename IntType>
    static IntType RNG(const IntType &upperBound) {
        // Create a random number generator object
        std::random_device rd;
        std::mt19937 generator(rd());

        // Define the range of random numbers
        uint64_t max = upperBound.ConvertToLimb();

        // Create a distribution object to generate numbers in the desired range
        std::uniform_int_distribution<uint64_t> distribution(0, max);

        // Generate a random number
        uint64_t randomNumber = distribution(generator);

        return IntType(randomNumber);
    }

    /* 
    TODO: Understands the Miller rabin primality test algorithm, then normalize parameter comments
      A witness function used for the Miller-Rabin Primality test.
      Inputs: a is a randomly generated witness between 2 and p-1,
      p is the number to be tested for primality,
      s and d satisfy p-1 = ((2^s) * d), d is odd.
      Output: true if p is composite,
      false if p is likely prime
    */
    template<typename IntType>
    static bool WitnessFunction(const IntType &a, const IntType &d, uint32_t s, const IntType &p) {
        IntType mod = util::ModExp(a, d, BMod(p));
        bool prevMod = false;
        for (uint32_t i = 1; i < s + 1; i++) {
            if (mod != IntType(1) && mod != p - IntType(1)) {
                prevMod = true;
            } else {
                prevMod = false;
            }
            mod = mod * mod % p;
            if (mod == IntType(1) && prevMod) {
                return true;
            }
        }
        return (mod != IntType(1));
    }

    template<typename IntType>
    bool IsPrime(const IntType &p, const uint32_t iterCount) {
        if (p < IntType(2) || p == IntType(2) || p == IntType(3) || p == IntType(5))
            return true;
        if (p % 2 == IntType(0))
            return false;

        IntType d = p - IntType(1);
        uint32_t s = 0;
        while (d % 2 == IntType(0)) {
            d = d.DividedByEq(IntType(2));
            s++;
        }
        bool composite = true;
        for (uint32_t i = 0; i < iterCount; ++i) {
            IntType a = RNG(p - IntType(3)) + IntType(2);
            composite = (WitnessFunction(a, d, s, p));
            if (composite)
                break;
        }
        return (!composite);
    }

    // TODO: Consider integer overflow?
    template<typename IntType>
    IntType FirstPrime(uint64_t nBits, uint64_t m) {
        IntType r = util::ModExp(IntType(2), IntType(nBits - 1), BMod(IntType(m)));
        IntType qNew = (IntType(1) << nBits);
        IntType qNew2 = (r > IntType(0)) ? (qNew + (IntType(m) - r) + IntType(1)) : (qNew + IntType(1));// if r == 0
        qNew = qNew2;

        while (!IsPrime(qNew)) {
            qNew2 = qNew + IntType(m);
            qNew = qNew2;
        }

        return qNew;
    }

    template bool IsPrime(const BInt &p, const uint32_t iterCount);
    template BInt FirstPrime(uint64_t nBits, uint64_t m);
}// namespace zhejiangfhe