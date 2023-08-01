/*
 * Created by Kanyan Zheng on Mon Jul 03 2023
 *
 * Copyright (c) 2023 Zhejiang Lab. All rights reserved.
 */

#pragma once

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
     * Returns the totient value: phi of a number n.
     * 
     * @param n The input number.
     * @return Phi of n which is the number of integers m coprime to n such that 1 <= m <= n.
     */
    uint64_t GetTotient(const uint64_t n);

    /**
     * Finds roots of unity for given input.  Assumes the the input is a power of two.
     * 
     * @param m Number which is cyclotomic
     * @param modulo Modulo which is used to find generator.
     * @return A root of unity;
     */
    template<typename IntType>
    IntType RootOfUnity(uint32_t m, const IntType &modulo);

    // TODO: Understand and rewrite these helper functions below
    /**
 * Method to reverse bits of num and return an unsigned int, for all bits up to
 * an including the designated most significant bit.
 *
 * @param input an unsigned int
 * @param msb the most significant bit.  All larger bits are disregarded.
 *
 * @return an unsigned integer that represents the reversed bits.
 */

    // precomputed reverse of a byte

    inline static unsigned char reverse_byte(unsigned char x) {
        static const unsigned char table[] = {
                0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0, 0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0,
                0x70, 0xf0, 0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8, 0x18, 0x98, 0x58, 0xd8,
                0x38, 0xb8, 0x78, 0xf8, 0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4, 0x14, 0x94,
                0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4, 0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
                0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc, 0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2,
                0x62, 0xe2, 0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2, 0x0a, 0x8a, 0x4a, 0xca,
                0x2a, 0xaa, 0x6a, 0xea, 0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa, 0x06, 0x86,
                0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6, 0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
                0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee, 0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe,
                0x7e, 0xfe, 0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1, 0x11, 0x91, 0x51, 0xd1,
                0x31, 0xb1, 0x71, 0xf1, 0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9, 0x19, 0x99,
                0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9, 0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
                0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5, 0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad,
                0x6d, 0xed, 0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd, 0x03, 0x83, 0x43, 0xc3,
                0x23, 0xa3, 0x63, 0xe3, 0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3, 0x0b, 0x8b,
                0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb, 0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
                0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7, 0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7,
                0x77, 0xf7, 0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef, 0x1f, 0x9f, 0x5f, 0xdf,
                0x3f, 0xbf, 0x7f, 0xff,
        };
        return table[x];
    }

    static int shift_trick[] = {0, 7, 6, 5, 4, 3, 2, 1};

    /* Function to reverse bits of num */
    inline uint32_t ReverseBits(uint32_t num, uint32_t msb) {
        uint32_t msbb = (msb >> 3) + (msb & 0x7 ? 1 : 0);

        switch (msbb) {
            case 1:
                return (reverse_byte((num) &0xff) >> shift_trick[msb & 0x7]);

            case 2:
                return (reverse_byte((num) &0xff) << 8 | reverse_byte((num >> 8) & 0xff)) >>
                       shift_trick[msb & 0x7];

            case 3:
                return (reverse_byte((num) &0xff) << 16 | reverse_byte((num >> 8) & 0xff) << 8 |
                        reverse_byte((num >> 16) & 0xff)) >>
                       shift_trick[msb & 0x7];

            case 4:
                return (reverse_byte((num) &0xff) << 24 | reverse_byte((num >> 8) & 0xff) << 16 |
                        reverse_byte((num >> 16) & 0xff) << 8 | reverse_byte((num >> 24) & 0xff)) >>
                       shift_trick[msb & 0x7];
            default:
                return -1;
                // OPENFHE_THROW(math_error, "msbb value not handled:" +
                // std::to_string(msbb));
        }
    }

    inline uint32_t GetMSB(uint64_t x) {
        if (x == 0) return 0;

            // hardware instructions for finding MSB are used are used;
#if defined(_MSC_VER)
        // a wrapper for VC++
        unsigned long msb;// NOLINT
        _BitScanReverse64(&msb, x);
        return msb + 1;
#else
        // a wrapper for GCC
        return 64 - (sizeof(unsigned long) == 8 ? __builtin_clzl(x) : __builtin_clzll(x));// NOLINT
#endif
    }

    inline uint32_t GetMSB(uint32_t x) { return GetMSB((uint64_t) x); }

#if defined(HAVE_INT128)
    template<>
    inline usint GetMSB(unsigned __int128 x) {
        if (x == 0) return 0;

#if defined(_MSC_VER)
        static_assert(false, "MSVC doesn't support 128-bit integers");
#else
        return 128 - clz_u128(x);
#endif
    }
#endif

    /**
 * Get MSB of an unsigned 64 bit integer.
 *
 * @param x the input to find MSB of.
 *
 * @return the index of the MSB bit location.
 */
    inline uint32_t GetMSB64(uint64_t x) { return GetMSB(x); }
}// namespace zhejiangfhe