//
// Created by 赵启明 on 2023/7/14.
//
#include "big_integer_test.h"
#include "fft.h"
#include "vector.h"


namespace zhejiangfhe {
    // 创建继承自 Test 的 test fixture
    template<typename T>
    class FFTtest : public testing::Test {};


    TYPED_TEST_SUITE(FFTtest, LimbTypes);

    TYPED_TEST(FFTtest, multiply) {

        using FFT = zhejiangfhe::FFT<TypeParam>;

        TypeParam v1[2] = {1, 2};
        TypeParam v2[3] = {1, 2, 1};
        TypeParam v3[6] = {8, 8, 2, 4, 5, 5};
        TypeParam v4[6] = {1, 7, 4, 0, 9, 4};

        TypeParam mulResult1[4] = {1, 4, 5, 2};
        TypeParam mulResult2[11] = {8, 64, 90, 50, 113, 160, 105, 64, 61, 65, 20};


        FFT fft;
        std::unique_ptr<TypeParam[]> result1 = fft.multiply_iteration(2, v1, 3, v2);
        for (int i=0; i<4; i++) {
            EXPECT_EQ(result1.get()[i], mulResult1[i]);
        }


        std::unique_ptr<TypeParam[]> result2 = fft.multiply_iteration(6, v3, 6, v4);

        for (int i=0; i<11; i++) {
            EXPECT_EQ(result2.get()[i], mulResult2[i]);
        }


        std::unique_ptr<std::complex<double>> fft_forward = fft.fft_forward_transform(6, v3);
        std::unique_ptr<TypeParam[]> origin_vec = fft.fft_inverse_transform(6, fft_forward.get());

        for (int i=0; i<6; i++) {
            EXPECT_EQ(v3[i], origin_vec[i]);
        }

    }

}