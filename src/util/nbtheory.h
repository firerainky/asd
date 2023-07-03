/*
 * Created by Kanyan Zheng on Mon Jul 03 2023
 *
 * Copyright (c) 2023 Zhejiang Lab. All rights reserved.
 */

#include <iostream>

namespace zhejiangfhe {

    /**
     * The Miller-Rabin Primality Test
     * @param p The candidate prime to test.
     * @param iterCount Number of iterations used for primality testing (default = 100.
     * @return False if evidence of non-primality is found. True is no evidence of non-primality is found.
     */
    template<typename IntType>
    bool IsPrime(const IntType &p, const uint32_t iterCount = 100);

    /**
     * Finds the first prime that is congruence to 1 modulo m.
     * @param nBits The number of bits needed to be in return value.
     * @param m The ring parameter.
     * @return The first prime satisfies the conditions.
     */
    template<typename IntType>
    IntType FirstPrime(uint64_t nBits, uint64_t m);
}// namespace zhejiangfhe