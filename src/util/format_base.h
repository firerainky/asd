//
// Created by 赵启明 on 2023/8/7.
//

#ifndef ZJ_FHE_LIB_FORMAT_BASE_H
#define ZJ_FHE_LIB_FORMAT_BASE_H

#include <cstdint>
#include <iosfwd>
#include <iostream>
#include <fmt/ostream.h>
namespace zhejiangfhe {
    class FormatBase {

    public:
        virtual const std::string ConvertToString() const {
            return "default str, please override this function!!!";
        };

        virtual ~FormatBase() = default;

        friend inline std::ostream &operator<<(std::ostream& os, const FormatBase &a) {
            return os << a.ConvertToString();
        }

    };


}

using namespace zhejiangfhe;
//template <> struct fmt::formatter<FormatBase> : ostream_formatter {};


template <typename T>
struct fmt::formatter<T, std::enable_if_t<std::is_base_of_v<FormatBase, T>, char>> : ostream_formatter {};


//template <typename T>
//struct fmt::formatter<T, std::enable_if_t<std::is_base_of<FormatBase, T>::value, char>> :
//    fmt::formatter<std::string> {
//    auto format(const FormatBase& a, format_context& ctx) const {
//        return fmt::formatter<std::string>::format(a.ConvertToString(), ctx);
//    }
//};



#endif//ZJ_FHE_LIB_FORMAT_BASE_H
