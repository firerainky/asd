
#include "basic_arithmatic.h"
#include "big_integer.h"
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


    BigInteger<uint64_t> aaaa("222222");

    BigInteger<uint64_t> bbbb("222222");
    aaaa.DividedBy(bbbb);

    std::cout << zhejiangfhe::basic_arithmatic::nlz(1) << std::endl;

    return 0;
}
