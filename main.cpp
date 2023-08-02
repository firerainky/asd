
#include "big_integer.h"
#include "big_integer_modop.h"
#include "modulus.h"
#include "native_integer.h"
#include <iosfwd>
#include <iostream>
#include "logger.h"

using namespace zhejiangfhe;
int main() {
    ZJDebug("Hello, World!");
    NativeInteger<uint32_t> testIntA("123");
    NativeInteger<uint32_t> testIntB("1");
    NativeInteger ans = testIntA.Add(testIntB);
    ZJDebug("{}", testIntA.ConvertToString().c_str());
    ZJDebug("{}", ans.ConvertToString().c_str());

    // Uncomment below 2 lines you will see memory error if address sanitizer is enabled.
    // int *x = new int[42];
    // x[100] = 5;// Boom!

    BInt testBigIntA("52");
    ZJDebug("{}", testBigIntA.ConvertToString().c_str());

    BMod modulus(BInt("3"));
    BInt operand("4");
    ZJDebug("{}", util::Mod(operand, modulus).ConvertToString());
    BInt another("2");
    ZJInfo("{}", util::ModMul(operand, another, modulus).ConvertToString());

    return 0;
}
