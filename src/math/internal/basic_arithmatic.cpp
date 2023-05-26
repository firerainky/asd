#include "basic_arithmatic.h"
#include "../../util/exception.h"
#include <limits>
namespace zhejiangfhe {
    namespace basic_arithmatic {

        int nlz64(LimbType x) {
            int n;
            if (x == 0) {
                return (64);
            }
            n = 0;
            if (x <= 0x00000000FFFFFFFF) {
                n = n + 32;
                x = x << 32;
            }
            if (x <= 0x0000FFFFFFFFFFFF) {
                n = n + 16;
                x = x << 16;
            }
            if (x <= 0x00FFFFFFFFFFFFFF) {
                n = n + 8;
                x = x << 8;
            }
            if (x <= 0x0FFFFFFFFFFFFFFF) {
                n = n + 4;
                x = x << 4;
            }
            if (x <= 0x3FFFFFFFFFFFFFFF) {
                n = n + 2;
                x = x << 2;
            }
            if (x <= 0x7FFFFFFFFFFFFFFF) {
                n = n + 1;
            }
            return n;
        }

        int nlz32(LimbType x) {
            int n;

            if (x == 0) {
                return (32);
            }
            n = 0;
            if (x <= 0x0000FFFF) {
                n = n + 16;
                x = x << 16;
            }
            if (x <= 0x00FFFFFF) {
                n = n + 8;
                x = x << 8;
            }
            if (x <= 0x0FFFFFFF) {
                n = n + 4;
                x = x << 4;
            }
            if (x <= 0x3FFFFFFF) {
                n = n + 2;
                x = x << 2;
            }
            if (x <= 0x7FFFFFFF) {
                n = n + 1;
            }
            return n;
        }

        int nlz(LimbType x) {
            if (typeid(x) == typeid(uint64_t)) {
                return nlz64(x);
            } else if (typeid(x) == typeid(uint32_t)) {
                return nlz32(x);
            } else {
                ZJFHE_THROW(zhejiangfhe::TypeException, "not support native int");
            }
        }

        bool divide(const std::vector<LimbType> &numerator, const std::vector<LimbType> &denominator, std::vector<LimbType> &quotient, std::vector<LimbType> &remainder) {

            auto m = numerator.size();
            auto n = denominator.size();
            quotient.resize(m - n + 1);

            const LimbType maxLimb = std::numeric_limits<LimbType>::max();
            const DoubleLimbType ffs = (DoubleLimbType) maxLimb;
            const DoubleLimbType b = (DoubleLimbType) maxLimb + 1;

            DoubleLimbType qhat;
            DoubleLimbType rhat;
            DoubleLimbType product;
            SignedDoubleLimbType t, k;
            int s, i, j;

            if (m < n || n <= 0 || denominator[n - 1] == 0) {
                std::cout << "Error in Divide m, n, v[n-1] " << m << ", " << n << ", " << denominator[n - 1] << std::endl;
                return false;// Return if invalid param.
            }

            if (n == 1) {                                                 // Take care of
                k = 0;                                                    // the case of a
                for (j = m - 1; j >= 0; j--) {                            // single-digit
                    quotient[j] = (k * b + numerator[j]) / denominator[0];// divisor here.
                    k = (k * b + denominator[j]) - quotient[j] * denominator[0];
                }
                if (remainder.size() != 0) {
                    remainder[0] = k;
                }
                return true;
            }

            s = nlz(denominator[n - 1]);

            std::vector<LimbType> vn(n);
            for (i = n - 1; i > 0; i--) {
                vn[i] = (denominator[i] << s) | denominator[i - 1] >> (LimbSize - s);
            }
            vn[0] = denominator[0] << s;

            std::vector<LimbType> un(m + 1);

            un[m] = numerator[m - 1] >> (LimbSize - s);
            for (i = m - 1; i > 0; i--) {
                un[i] = (numerator[i] << s) | (numerator[i - 1] >> (LimbSize - s));
            }
            un[0] = numerator[0] << s;

            // Main loop
            for (j = m - n; j >= 0; j--) {
                // Compute estimate qhat of q[j].
                qhat = (un[j + n] * b + un[j + n - 1]) / vn[n - 1];
                rhat = (un[j + n] * b + un[j + n - 1]) - qhat * vn[n - 1];
                while (qhat >= b || qhat * vn[n - 2] > b * rhat + un[j + n - 2]) {
                    qhat = qhat - 1;
                    rhat = rhat + vn[n - 1];
                    if (rhat >= b) {
                        break;
                    }
                }

                // Multiply and subtract.
                k = 0;
                for (i = 0; i < n; i++) {
                    product = qhat * vn[i];
                    t = un[i + j] - k - (product & ffs);
                    un[i + j] = t;
                    k = (product >> LimbSize) - (t >> LimbSize);
                }
                t = un[j + n] - k;
                un[j + n] = t;


                // Store quotient digit.
                quotient[j] = qhat;
                if (t < 0) {                      // If we subtracted too
                    quotient[j] = quotient[j] - 1;// much, add back.
                    k = 0;
                    for (i = 0; i < n; i++) {
                        t = (DoubleLimbType) un[i + j] + vn[i] + k;
                        un[i + j] = t;
                        k = t >> LimbSize;
                    }
                    un[j + n] = un[j + n] + k;
                }
            }

            // store remainder
            remainder.resize(n);
            for (i = 0; i < n - 1; i++) {
                remainder[i] = (un[i] >> s) | un[i + 1] << (LimbSize - s);
            }
            remainder[n - 1] = un[n - 1] >> s;
            return true;
        };
    }// namespace basic_arithmatic

}// namespace zhejiangfhe