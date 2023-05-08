//
// Created by 赵启明 on 2023/4/27.
//

#ifndef ZJ_FHE_LIB_INTEGER_INTERFACE_H
#define ZJ_FHE_LIB_INTEGER_INTERFACE_H

#include <iosfwd>
#include <iostream>

namespace zhejiangfhe {
    template<typename T>
    class IntegerInterface {

        /**
         *
         * @param b
         * @return
         */
        T Add(const T &b);


        const T &AddEq(const T &b);


        inline friend T operator+(const T &a, const T &b) {
            return a.Add(b);
        }
        inline friend const T &operator+=(T &a, const T &b) {
            return a.AddEq(b);
        }


        T Sub(const T &b) const;

        const T &SubEq(const T &b);


        T Mul(const T &b) const;


        const T &MulEq(const T &b);

        inline friend T operator*(const T &a, const T &b) {
            return a.Mul(b);
        }
        inline friend const T &operator*=(T &a, const T &b) {
            return a.MulEq(b);
        }

        T DividedBy(const T &b) const;


        const T &DividedByEq(const T &b);


        inline friend T operator/(const T &a, const T &b) {
            return a.DividedBy(b);
        }
        inline friend const T &operator/=(T &a, const T &b) {
            return a.DividedByEq(b);
        }


        inline T Exp(uint32_t p) {
            if (p == 0) {
                return 1;
            }
            if (p == 1) {
                return this;
            }
            T tmp = Exp(p / 2);
            if (p % 2 == 0) {
                return tmp * tmp;
            } else {
                return tmp * tmp * (*this);
            }
        }
    };
}// namespace zhejiangfhe

#endif//ZJ_FHE_LIB_INTEGER_INTERFACE_H
