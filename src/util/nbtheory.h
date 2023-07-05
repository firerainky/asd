/*
 * Created by Kanyan Zheng on Mon Jul 03 2023
 *
 * Copyright (c) 2023 Zhejiang Lab. All rights reserved.
 */

#include <iostream>
#include <set>
#include <vector>

namespace zhejiangfhe {

    /**
     * The Miller-Rabin Primality Test
     * 
     * @param p The candidate prime to test.
     * @param iterCount Number of iterations used for primality testing (default = 100.
     * @return False if evidence of non-primality is found. True is no evidence of non-primality is found.
     */
    template<typename IntType>
    bool IsPrime(const IntType &p, const uint32_t iterCount = 100);

    /**
     * Return greatest common divisor of two big integers.
     * 
     * @param a One integer to find greatest common divisor of.
     * @param b Another integer to find greatest common divisor of. 
     * @return The greatest common divisor of a and b.
     */
    template<typename IntType>
    IntType GCD(const IntType &a, const IntType &b);

    /**
     * Recursively factorizes to find the distinct primefactors of a number.
     * Side effect: the value of number is destroyed.
     * 
     * @param n The value to factorize. [note the value of n is destroyed]
     * @param primeFactors Set of factors found.
     */
    template<typename IntType>
    void PrimeFactorize(IntType n, std::set<IntType> &primeFactors);

    /**
     * Finds the first prime that is congruence to 1 modulo m.
     * 
     * @param nBits The number of bits needed to be in return value.
     * @param m The ring parameter.
     * @return The first prime satisfies the conditions.
     */
    template<typename IntType>
    IntType FirstPrime(uint64_t nBits, uint64_t m);

    /**
     * Get the residue classes in Z*_n
     * 
     * @param n The input number.
     * @return Vector of residue classes x under Z_n such that gcd(x, n) == 1.
     */
    template<typename IntType>
    std::vector<IntType> GetTotientList(const IntType &n);

    /**
     * Finds roots of unity for given input.  Assumes the the input is a power of two.
     * 
     * @param m Number which is cyclotomic
     * @param modulo Modulo which is used to find generator.
     * @return A root of unity;
     */
    template<typename IntType>
    IntType RootOfUnity(uint32_t m, const IntType &modulo);
}// namespace zhejiangfhe