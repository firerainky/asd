//
// Created by 赵启明 on 2023/8/7.
//

#ifndef ZJ_FHE_LIB_FORMAT_BASE_H
#define ZJ_FHE_LIB_FORMAT_BASE_H

#include <cstdint>
#include <iosfwd>
#include <iostream>
#include <fmt/ostream.h>
#include <concepts>
namespace zhejiangfhe {
    class FormatBase {

        virtual const std::string ConvertToString() const;


        friend std::ostream &operator<<(std::ostream &os, const FormatBase &a) {
            return os << a.ConvertToString();
        }
    };
}

using namespace zhejiangfhe;
template <> struct fmt::formatter<FormatBase> : ostream_formatter {};

//template <class Derived, class Base>
//concept derived_from = std::is_base_of_v<Base, Derived> &&
//                       std::is_convertible_v<const volatile Derived*, const volatile Base*>;





//template<typename T, std::enable_if_t<std::is_base_of_v<FormatBase, T>, bool>>
//struct fmt::formatter<T> : ostream_formatter {};



//template<typename T, typename std::enable_if<std::is_base_of<FormatBase, T>::value, bool>::type>
//struct fmt::formatter<T> : ostream_formatter {};


#endif//ZJ_FHE_LIB_FORMAT_BASE_H
