/*
 * Created by Kanyan Zheng on Mon Jul 03 2023
 *
 * Copyright (c) 2023 Zhejiang Lab. All rights reserved.
 */

#include "nbtheory.h"
#include "big_integer.h"
#include "big_integer_modop.h"
#include "debug.h"
#include <random>

namespace zhejiangfhe {

    /**
     * @description: Generate a random big integer between 0 and uper bound.
     * @param upperBound The upper bound of the generated random integer.
     * @return Randomly generated big integer.
     * TODO: Currently using system RNG generator which can not generate big integer, need better one.
     */
    template<typename IntType>
    static IntType RNG(const IntType &upperBound) {
        // Create a random number generator object
        std::random_device rd;
        std::mt19937 generator(rd());

        // Define the range of random numbers
        uint64_t max = upperBound.ConvertToLimb();

        // Create a distribution object to generate numbers in the desired range
        std::uniform_int_distribution<uint64_t> distribution(0, max);

        // Generate a random number
        uint64_t randomNumber = distribution(generator);

        return IntType(randomNumber);
    }

    /* 
    TODO: Understands the Miller rabin primality test algorithm, then normalize parameter comments
      A witness function used for the Miller-Rabin Primality test.
      Inputs: a is a randomly generated witness between 2 and p-1,
      p is the number to be tested for primality,
      s and d satisfy p-1 = ((2^s) * d), d is odd.
      Output: true if p is composite,
      false if p is likely prime
    */
    template<typename IntType>
    static bool WitnessFunction(const IntType &a,
                                const IntType &d,
                                uint32_t s,
                                const IntType &p) {
        IntType mod = util::ModExp(a, d, BMod(p));
        bool prevMod = false;
        for (uint32_t i = 1; i < s + 1; i++) {
            if (mod != IntType(1) && mod != p - IntType(1)) {
                prevMod = true;
            } else {
                prevMod = false;
            }
            mod = mod * mod % p;
            if (mod == IntType(1) && prevMod) {
                return true;
            }
        }
        return (mod != IntType(1));
    }

    /**
     * A helper function to RootOfUnity function. This finds a generator for a given
     * prime q. Input: BigInteger q which is a prime. Output: A generator of prime q
     */
    template<typename IntType>
    static IntType FindGenerator(const IntType &q) {
        ZJ_DEBUG_FLAG(false);

        std::set<IntType> primeFactors;

        IntType qm1 = q - IntType(1);
        IntType qm2 = q - IntType(2);
        PrimeFactorize<IntType>(qm1, primeFactors);

        ZJ_DEBUG("Find Generator(" << q << ")，calling prime factorization.");
        ZJ_DEBUG("Prime factors of " << qm1);
        for (auto &v: primeFactors) {
            ZJ_DEBUG(v << " ");
        }

        bool generatorFound = false;
        IntType gen;
        while (!generatorFound) {
            uint32_t count = 0;
            gen = RNG(qm2) + IntType(1);

            ZJ_DEBUG("Generator " << gen);

            for (auto it = primeFactors.begin(); it != primeFactors.end(); ++it) {
                IntType t = qm1 / (*it);
                ZJ_DEBUG(qm1 << " / " << *it << " "
                             << util::ModExp(gen, t, BMod(q)));
                if (util::ModExp(gen, t, BMod(q)) == IntType(1))
                    break;
                else
                    count++;
            }
            if (count == primeFactors.size())
                generatorFound = true;
        }
        return gen;
    }

    template<typename IntType>
    bool IsPrime(const IntType &p, const uint32_t iterCount) {
        if (p < IntType(2) || p == IntType(2) || p == IntType(3) || p == IntType(5))
            return true;
        if (p % 2 == IntType(0))
            return false;

        IntType d = p - IntType(1);
        uint32_t s = 0;
        while (d % 2 == IntType(0)) {
            d = d.DividedByEq(IntType(2));
            s++;
        }
        bool composite = true;
        for (uint32_t i = 0; i < iterCount; ++i) {
            IntType a = RNG(p - IntType(3)) + IntType(2);
            composite = (WitnessFunction(a, d, s, p));
            if (composite)
                break;
        }
        return (!composite);
    }

    // TODO: Consider integer overflow?
    template<typename IntType>
    IntType FirstPrime(uint64_t nBits, uint64_t m) {
        IntType r = util::ModExp(IntType(2), IntType(nBits - 1), BMod(IntType(m)));
        IntType qNew = (IntType(1) << nBits);
        IntType qNew2 = (r > IntType(0)) ? (qNew + (IntType(m) - r) + IntType(1))
                                         : (qNew + IntType(1));// if r == 0
        qNew = qNew2;

        while (!IsPrime(qNew)) {
            qNew2 = qNew + IntType(m);
            qNew = qNew2;
        }

        return qNew;
    }

    template<typename IntType>
    IntType GCD(const IntType &a, const IntType &b) {
        IntType m_a = a;
        IntType m_b = b;
        IntType m_t;

        while (m_b != IntType(0)) {
            m_t = m_b;
            m_b = m_a % m_b;
            m_a = m_t;
        }
        return m_a;
    }

    template<typename IntType>
    void PrimeFactorize(IntType n, std::set<IntType> &primeFactors) {

        if (n == IntType(0) || n == IntType(1))
            return;

        if (IsPrime(n)) {
            primeFactors.insert(n);
            return;
        }

        IntType divisor(PollardRho(n));
        PrimeFactorize(divisor, primeFactors);

        n /= divisor;
        PrimeFactorize(n, primeFactors);
    }

    template<typename IntType>
    const IntType PollardRho(const IntType &n) {
        IntType divisor(1);

        IntType c = RNG(n);
        IntType x = RNG(n);
        IntType xx(x);

        // check divisibility by 2
        if (n % 2 == IntType(0))
            return IntType(2);

        do {
            x = (x * x + c) % n;
            xx = (xx * xx + c) % n;
            xx = (xx * xx + c) % n;

            divisor = GCD((x > xx) ? x - xx : xx - x, n);
        } while (divisor == IntType(1));

        return divisor;
    }

    template<typename IntType>
    std::vector<IntType> GetTotientList(const IntType &n) {
        std::vector<IntType> result;
        IntType one(1);
        for (IntType i = IntType(1); i < n; i = i + IntType(1)) {
            if (GCD(i, n) == one) {
                result.push_back(i);
            }
        }
        return result;
    }

    // TODO: Replace with an native integer version in nbtheory2?
    uint64_t GetTotient(const uint64_t n) {
        std::set<BInt> factors;
        BInt enn(n);
        PrimeFactorize(enn, factors);

        BInt primeProd(1);
        BInt numerator(1);

        for (auto &r: factors) {
            numerator = numerator * (r - 1);
            primeProd = primeProd * r;
        }

        return ((enn / primeProd) * numerator).ConvertToLimb();
    }

    template<typename IntType>
    IntType RootOfUnity(uint32_t m, const IntType &modulo) {
        ZJ_DEBUG_FLAG(false);

        IntType M(m);
        if ((modulo - IntType(1)) % m != IntType(0)) {
            // TODO: Throw an exception?
            return BInt();
        }
        IntType result;
        IntType gen = FindGenerator(modulo);
        IntType mid = (modulo - IntType(1)).DividedByEq(M);
        ZJ_DEBUG("mid = " << mid);
        result = util::ModExp(gen, mid, BMod(modulo));
        if (result == IntType(1)) {
            result = RootOfUnity(m, modulo);
        }
        ZJ_DEBUG("result = " << result);

        /**
          * At this point, result contains a primitive root of unity. However,
          * we want to return the minimum root of unity, to avoid different
          * crypto contexts having different roots of unity for the same
          * cyclotomic order and moduli. Therefore, we are going to cycle over
          * all primitive roots of unity and select the smallest one (minRU).
          *
          * To cycle over all primitive roots of unity, we raise the root of
          * unity in result to all the powers that are co-prime to the
          * cyclotomic order. In power-of-two cyclotomics, this will be the
          * set of all odd powers, but here we use a more general routine
          * to support arbitrary cyclotomics.
          *
          */
        IntType x = result % modulo;
        IntType minRU(x);
        IntType curPowIdx(1);
        std::vector<IntType> coprimes = GetTotientList<IntType>(m);
        for (uint32_t i = 0; i < coprimes.size(); i++) {
            auto nextPowIdx = coprimes[i];
            IntType diffPow(nextPowIdx - curPowIdx);
            for (IntType j(0); j < diffPow; j += IntType(1)) {
                x = (x * result) % modulo;
            }
            if (x < minRU && x != IntType(1)) {
                minRU = x;
            }
            curPowIdx = nextPowIdx;
        }
        return minRU;
    }

    template bool IsPrime(const BInt &p, const uint32_t iterCount);
    template BInt FirstPrime(uint64_t nBits, uint64_t m);
    template const BInt PollardRho(const BInt &n);
    template BInt GCD(const BInt &a, const BInt &b);
    template void PrimeFactorize(BInt n, std::set<BInt> &primeFactors);
    template BInt RootOfUnity(uint32_t m, const BInt &modulo);
    template std::vector<BInt> GetTotientList(const BInt &n);
}// namespace zhejiangfhe