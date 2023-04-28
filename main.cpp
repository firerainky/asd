
#include "big_integer.h"
#include "native_integer.h"
#include <iosfwd>
#include <iostream>

using namespace zhejiangfhe;
int main() {
    std::cout << "Hello, World!" << std::endl;
    NativeInteger<u_int32_t> testIntA("123");
    NativeInteger<u_int32_t> testIntB("1");
    NativeInteger ans = testIntA.Add(testIntB);
    std::cout << testIntA.ConvertToString().c_str() << std::endl;
    std::cout << ans.ConvertToString().c_str() << std::endl;


    BigInteger<u_int32_t> testBigIntA("123456789123");
    std::cout << testBigIntA.ConvertToString().c_str() << std::endl;

    return 0;
}
