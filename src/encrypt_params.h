#pragma once

#include <iostream>
#include <vector>

namespace zhejiangfhe {
    using zuint = uint64_t;
    using zuvec = std::vector<zuint>;

    class EncryptParams {
    public:
        EncryptParams(zuint polyModulusDegree, zuint plainModulus, zuvec &coeffModulus) : polyModulusDegree(polyModulusDegree), plainModulus(plainModulus), coeffModulus(coeffModulus){};

    private:
        zuint polyModulusDegree;
        zuint plainModulus;
        zuvec coeffModulus;
    };
}// namespace zhejiangfhe