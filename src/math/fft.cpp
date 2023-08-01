//
// Created by 赵启明 on 2023/7/14.
//

#pragma
#include "fft.h"
#include <complex>
#include <math.h>
#include <memory>

namespace zhejiangfhe {

    template<typename IntType>
    void FFT<IntType>::fft_recursion(int length, std::complex<double> *vector, int inverseFlag) {
        if (length == 1) { return; }
        int half = length >> 1;

        std::complex<double> vectorEven[half];

        std::complex<double> vectorOdd[half];

        for (int i = 0; i < length; i += 2) {
            IntType index = i >> 1;
            vectorEven[index] = vector[i];
            vectorOdd[index] = vector[i + 1];
        }
        fft_recursion(half, vectorEven, inverseFlag);
        fft_recursion(half, vectorOdd, inverseFlag);

        std::complex<double> w = {cos(2 * pi / length), inverseFlag * sin(2 * pi / length)};
        std::complex<double> p = {1, 0};
        for (int i = 0; i < half; i++, p = p * w) {
            vector[i] = vectorEven[i] + p * vectorOdd[i];
            vector[i + half] = vectorEven[i] - p * vectorOdd[i];
        }
    }


    template<typename IntType>
    std::unique_ptr<IntType[]> FFT<IntType>::multiply_recursion(std::vector<IntType> &numA,
                                                                std::vector<IntType> &numB) {

        int lengthA = numA.size();
        int lengthB = numB.size();
        int length = 1;
        while (length < lengthA + lengthB) { length <<= 1; }

        std::unique_ptr<std::complex<double>[]> vectorA(new std::complex<double>[length]);
        std::unique_ptr<std::complex<double>[]> vectorB(new std::complex<double>[length]);

        for (int i = 0; i < lengthA; i++) { vectorA[i] = std::complex<double>(numA[i], 0); }

        for (int i = 0; i < lengthB; i++) { vectorB[i] = std::complex<double>(numB[i], 0); }

        fft_recursion(length, vectorA.get(), 1);
        fft_recursion(length, vectorB.get(), 1);

        std::unique_ptr<std::complex<double>[]> vectorC(new std::complex<double>[length]);
        for (int i = 0; i < length; i++) { vectorC[i] = vectorA[i] * vectorB[i]; }

        fft_recursion(length, vectorC.get(), -1);

        int resultLength = lengthA + lengthB - 1;
        std::unique_ptr<IntType[]> result(new IntType[resultLength]);
        for (int i = 0; i < resultLength; i++) { result[i] = round(vectorC[i].real() / length); }

        return result;
    }


    template<typename IntType>
    void FFT<IntType>::fft_iteration(std::vector<int> &index,
                                     std::vector<std::complex<double>> &vector, int inverseFlag) {

        int length = vector.size();
        for (int i = 0; i < length; i++) {
            if (i < index[i]) { std::swap(vector[i], vector[index[i]]); }
        }

        std::vector<std::complex<double>> cacheW(length / 2);
        std::complex<double> p = {1, 0};
        for (int i = 0; i < length / 2; i++) {
            cacheW[i] = {cos(2 * pi * i / length), inverseFlag * sin(2 * pi * i / length)};
        }
        for (int i = 1; i < length; i <<= 1) {

            int seg = i << 1;
            int tableStep = length / seg;
            for (int j = 0; j < length; j += seg) {

                for (int k = 0, index = 0; k < i; k++, index += tableStep) {
                    p = cacheW[index];
                    std::complex<double> x = vector[j + k];
                    std::complex<double> y = p * vector[j + k + i];
                    vector[j + k] = x + y;
                    vector[j + k + i] = x - y;
                    //                    printf( "vector[%d]:{%f, %f} vector[%d]: {%f, %f} \n",  j+k, vector[j+k].real(), vector[j+k].imag(), j+k+i, vector[j+k+i].real(), vector[j+k+i].imag());
                }
            }
        }
    }


    template<typename IntType>
    void FFT<IntType>::rearrange(int bitSize, std::vector<int> &index) {
        for (int i = 0; i < index.size(); i++) {
            index[i] = (index[i >> 1] >> 1) | ((i & 1) << (bitSize - 1));
        }
    }


    template<typename IntType>
    std::unique_ptr<IntType[]> FFT<IntType>::multiply_iteration(std::vector<IntType> &numA,
                                                                std::vector<IntType> &numB) {

        int lengthA = numA.size();
        int lengthB = numB.size();
        int length = 1;
        int bitSize = 0;
        while (length < lengthA + lengthB) {
            length <<= 1;
            bitSize++;
        }

        std::vector<int> index(length);
        rearrange(bitSize, index);

        std::vector<std::complex<double>> vectorA(length);
        std::vector<std::complex<double>> vectorB(length);


        for (int i = 0; i < lengthA; i++) { vectorA[i] = std::complex<double>(numA[i], 0); }

        for (int i = 0; i < lengthB; i++) { vectorB[i] = std::complex<double>(numB[i], 0); }

        fft_iteration(index, vectorA, 1);
        fft_iteration(index, vectorB, 1);

        std::vector<std::complex<double>> vectorC(length);
        for (int i = 0; i < length; i++) { vectorC[i] = vectorA[i] * vectorB[i]; }

        fft_iteration(index, vectorC, -1);

        int resultLength = lengthA + lengthB - 1;
        std::unique_ptr<IntType[]> result(new IntType[resultLength]);
        for (int i = 0; i < resultLength; i++) { result[i] = round(vectorC[i].real() / length); }

        return result;
    }


    template<typename IntType>
    std::unique_ptr<std::complex<double>[]>
    FFT<IntType>::fft_forward_transform(std::vector<IntType> &vector) {
        int length = 1;
        int bitSize = 0;
        int size = vector.size();
        while (length < size) {
            length <<= 1;
            bitSize++;
        }

        std::vector<int> index(length);
        rearrange(bitSize, index);

        std::vector<std::complex<double>> vectorA(length);
        for (int i = 0; i < size; i++) { vectorA[i] = std::complex<double>(vector[i], 0); }
        fft_iteration(index, vectorA, 1);

        std::unique_ptr<std::complex<double>[]> result(new std::complex<double>[length]);

        for (int i = 0; i < length; i++) { result[i] = vectorA[i]; }
        return result;
    }

    template<typename IntType>
    std::unique_ptr<IntType[]> FFT<IntType>::fft_inverse_transform(int size,
                                                                   std::complex<double> *vectorA) {
        int length = 1;
        int bitSize = 0;
        while (length < size) {
            length <<= 1;
            bitSize++;
        }

        std::vector<int> index(length);
        rearrange(bitSize, index);

        std::vector<std::complex<double>> vector(length);
        for (int i = 0; i < length; i++) { vector[i] = vectorA[i]; }

        fft_iteration(index, vector, -1);

        std::unique_ptr<IntType[]> result(new IntType[size]);
        for (int i = 0; i < size; i++) result[i] = ceil(vector[i].real() / length);
        return result;
    }


    template class zhejiangfhe::FFT<limbtype>;

}// namespace zhejiangfhe
