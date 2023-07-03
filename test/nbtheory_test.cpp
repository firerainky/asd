/*
 * Created by Kanyan Zheng on Mon Jul 03 2023
 *
 * Copyright (c) 2023 Zhejiang Lab. All rights reserved.
 */

#include "big_integer.h"
#include "nbtheory.h"
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
        uint64_t m, nBits;
        {
            m = 2048;
            nBits = 30;
            BInt expectedResult("1073750017");
            BInt computedResult = FirstPrime<BInt>(nBits, m);
            EXPECT_EQ(computedResult, expectedResult) << "Compute first prime with small integer failed";
        }

        {
            m = 4096;
            nBits = 49;
            BInt expectedResult("562949953548289");
            BInt computedResult = FirstPrime<BInt>(nBits, m);
            EXPECT_EQ(computedResult, expectedResult) << "Compute first prime with big integer failed";
        }
    }
}// namespace zhejiangfhe