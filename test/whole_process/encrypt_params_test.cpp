#include "encrypt_params.h"
#include <gtest/gtest.h>

namespace zhejiangfhe {
    TEST(encrypt_params, createAnEncryptParams) {
        zuint polyModulusDegree = 1024;
        zuint plainModulus = 109;
        zuvec coeffModulus = {1, 0, 2};

        EncryptParams(polyModulusDegree, plainModulus, coeffModulus);
    }
}// namespace zhejiangfhe