
// #include "basic_math.h"
// #include <MathFunctions/big_integer.h>
// #include "basic_math.h"
#include "big_integer.h"
// #include "internal/basic_math.h"
#include "native_integer.h"
#include <iosfwd>
#include <iostream>

using namespace zhejiangfhe;
int main() {
    std::cout << "Hello, World!" << std::endl;
    NativeInteger<uint32_t> testIntA("123");
    NativeInteger<uint32_t> testIntB("1");
    NativeInteger ans = testIntA.Add(testIntB);
    std::cout << testIntA.ConvertToString().c_str() << std::endl;
    std::cout << ans.ConvertToString().c_str() << std::endl;


    BigInteger<uint32_t> testBigIntA("52");
    std::cout << testBigIntA.ConvertToString().c_str() << std::endl;

    uint64_t result[2];
    basic_math::MultiplyWithKaratsuba<uint64_t>(0, 0, result);
    return 0;
}
