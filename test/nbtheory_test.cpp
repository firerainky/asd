/*
 * Created by Kanyan Zheng on Mon Jul 03 2023
 *
 * Copyright (c) 2023 Zhejiang Lab. All rights reserved.
 */

#include "big_integer.h"
#include "big_integer_modop.h"
#include "debug.h"
#include "nbtheory.h"
#include "logger.h"
#include <gtest/gtest.h>

namespace zhejiangfhe {
    TEST(NbTheoryTest, IsPrime) {
        BInt a = 2;
        EXPECT_TRUE(IsPrime(a)) << "Expected a(" << a << ") be a prime.";

        a = 4;
        EXPECT_FALSE(IsPrime(a)) << "Expected a(" << a << ") be not a prime.";

        a = 24469;
        EXPECT_TRUE(IsPrime(a)) << "Expected small integer a(" << a << ") is a prime.";

        a = 10403;
        EXPECT_FALSE(IsPrime(a)) << "Expected small integer a(" << a << ") is a composite.";

        a = 952229140957;
        EXPECT_TRUE(IsPrime(a)) << "Expected big integer a(" << a << ") is a prime.";

        a = 952229140959;
        EXPECT_FALSE(IsPrime(a)) << "Expected big integer a(" << a << ") is a composite.";
    }

    TEST(NbTheoryTest, FirstPrime) {
        uint32_t m, nBits;
        {
            m = 2048;
            nBits = 30;
            BInt expectedResult("1073750017");
            BInt computedResult = FirstPrime<BInt>(nBits, m);
            EXPECT_EQ(computedResult, expectedResult)
                    << "Compute first prime with small integer failed";
        }

        {
            m = 4096;
            nBits = 49;
            BInt expectedResult("562949953548289");
            BInt computedResult = FirstPrime<BInt>(nBits, m);
            EXPECT_EQ(computedResult, expectedResult)
                    << "Compute first prime with big integer failed";
        }
    }

    TEST(NbTheoryTest, GCD) {
        {
            BInt a = 10403, b = 103;
            BInt computedResult = 103;
            BInt expectedResult = GCD(a, b);
            EXPECT_EQ(computedResult, expectedResult) << "Compute GCD for small integers failed.";
        }
        {
            BInt a("883035439563027"), b("3042269397984931");
            BInt computedResult = GCD(a, b);
            BInt expectedResult = 1;
            EXPECT_EQ(expectedResult, computedResult) << "Compute GCD for big integers failed.";
        }
    }

    TEST(NbTheoryTest, PrimeFactorize) {
        BInt composite = 53093040;
        std::set<BInt> expectedFactors({2, 3, 5, 7, 11, 13, 17});
        std::set<BInt> computedFactors;
        PrimeFactorize(composite, computedFactors);

        ASSERT_EQ(computedFactors.size(), expectedFactors.size())
                << "Factorize an composite failed.";

        for (auto it = computedFactors.begin(); it != computedFactors.end(); ++it) {
            EXPECT_TRUE(expectedFactors.find(*it) != expectedFactors.end())
                    << "Factorize an composite error.";
        }
    }

    TEST(NbTheoryTest, RootOfUnity) {
        {
            ZJ_DEBUG_FLAG(false);
            uint32_t m = 4096;
            uint32_t nBits = 33;

            BInt primeModulus = FirstPrime<BInt>(nBits, m);
            BInt primitiveRootOfUnity = RootOfUnity(m, primeModulus);

            BInt powerm = util::ModExp(primitiveRootOfUnity, BInt(m), BMod(primeModulus));
            ZJDebug("First Prime: {}, Root of Unity: {}", primeModulus, primitiveRootOfUnity);
            EXPECT_EQ(powerm, 1);

            BInt powermBy2 = util::ModExp(primitiveRootOfUnity, BInt(m / 2), BMod(primeModulus));
            EXPECT_NE(powermBy2, 1);

            BInt powermBy4 = util::ModExp(primitiveRootOfUnity, BInt(m / 4), BMod(primeModulus));
            EXPECT_NE(powermBy4, 1);
        }
        {
            ZJ_DEBUG_FLAG(false);
            uint32_t m = 32768 * 2;
            uint32_t nBits = 59;
            BInt primeModulus = FirstPrime<BInt>(nBits, m);
            BInt primitiveRootOfUnity = RootOfUnity(m, primeModulus);

            BInt powerm = util::ModExp(primitiveRootOfUnity, BInt(m), BMod(primeModulus));
            ZJDebug("First Prime: {}, Root of Unity: {}", primeModulus, primitiveRootOfUnity);
            EXPECT_EQ(powerm, 1);

            BInt powermBy2 = util::ModExp(primitiveRootOfUnity, BInt(m / 2), BMod(primeModulus));
            EXPECT_NE(powermBy2, 1);

            BInt powermBy4 = util::ModExp(primitiveRootOfUnity, BInt(m / 4), BMod(primeModulus));
            EXPECT_NE(powermBy4, 1);
        }
        {
            uint32_t m = 16;
            uint32_t nBits = 22;
            BInt primeModulus = FirstPrime<BInt>(nBits, m);
            BInt primitiveRootOfUnity = RootOfUnity(m, primeModulus);
            EXPECT_EQ(primeModulus, 4194353);
            EXPECT_EQ(primitiveRootOfUnity, 132170);
        }
    }
}// namespace zhejiangfhe