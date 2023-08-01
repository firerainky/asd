//
// Created by 赵启明 on 2023/7/14.
//

#ifndef ZJ_FHE_LIB_FFT_H
#define ZJ_FHE_LIB_FFT_H

#include "vector.h"
#include <cmath>
#include <complex>
#include <iostream>
#include <vector>

namespace zhejiangfhe {

    template<typename IntType>
    class FFT {

    public:
        /**
          * 多项式乘法，fft实现  递归版本
          * @param vectorA 多项式A系数表示
          * @param vectorB 多项式B系数表示
          * @return
          */
        std::unique_ptr<IntType[]> multiply_recursion(std::vector<IntType> &vectorA,
                                                      std::vector<IntType> &vectorB);


        /**
          * 多项式乘法，fft实现 迭代版本
          * @param vectorA 多项式A系数表示
          * @param vectorB 多项式B系数表示
          * @return
          */
        std::unique_ptr<IntType[]> multiply_iteration(std::vector<IntType> &vectorA,
                                                      std::vector<IntType> &vectorB);


        /**
         * fft 正变换
         * @param length
         * @param vector
         */
        std::unique_ptr<std::complex<double>[]> fft_forward_transform(std::vector<IntType> &vector);


        /**
         * fft 逆变换
         * @param length
         * @param vector
         */
        std::unique_ptr<IntType[]> fft_inverse_transform(int size, std::complex<double> *vectorA);

    private:
        double pi = acos(-1.0);

        /**
         * fft 迭代版本
         * @param index
         * @param length
         * @param vector
         * @param inverseFlag
         */
        void fft_iteration(std::vector<int> &index, std::vector<std::complex<double>> &vector,
                           int inverseFlag);


        /**
         * fft 递归版本
         * @param length
         * @param vector 多项式向量表示
         * @param inverseFlag -1表示FFT逆变换
         */
        void fft_recursion(int length, std::complex<double> *vector, int inverseFlag);


        /**
         * 蝴蝶变换
         * @param length
         * @param bitSize
         * @param index
         */
        void rearrange(int bitSize, std::vector<int> &index);
    };
}// namespace zhejiangfhe

#endif//ZJ_FHE_LIB_FFT_H
